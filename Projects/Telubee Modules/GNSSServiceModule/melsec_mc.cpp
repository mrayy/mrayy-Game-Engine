
/**
*	NEDO-Obayashi - Data Acquisition Daemon (DAQ)
*	Charith Fernando. (charith@inmojo.com)
*	Nichiha USA, Inc.
*
*	MELSEC Communication Protocol (MC Protocol) Driver Implementation
*	Copyright (c) 2015-2016 Charith Fernando/TachiLab.org.
*
*	2015.11.04.
*
*	This driver implements "MC Protocol" communications as described
*	in Mitsubishi "MELSEC-Q/L MELSEC Communication Protocol Reference
*	Manual", in the following manuals (English & Japanese)...
*	[en] SH(NA)-080008-Q - http://www.meau.com/
*	[jp] SH-08003-V      - http://www.mitsubishielectric.co.jp/
*
*	* Protocol Type 3E (binary) QnA compatible framing
*
**/

#include "StdAfx.h"
#include "plc_config.h"



// ---------------------- PLC MC Protocol Driver function implementation -----------------------------------//

MCClient::MCClient(int portnum, char *ipaddr){
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	server.sin_family = AF_INET;
	server.sin_port = htons(portnum);
	server.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	int ret = connect(sock, (struct sockaddr *)&server, sizeof(server));

	if (ret != 0)
		printf("Melsec PLC Connection Error.. \r\n");
	else
		printf("Melsec PLC Connected Succssfully at %s : %d \r\n", ipaddr, portnum);
}

MCClient::~MCClient(){
	closesocket(sock);
	WSACleanup();
}



char* MCClient::errmsg(unsigned short errnum) {
	char* msgptr = NULL;

	for (int ez = 0; ez < 1024; ez++) {
		if (!errmap[ez].errval && !errmap[ez].errdesc[0]) break;
		if (errmap[ez].errval == errnum) {
			msgptr = errmap[ez].errdesc;
			return msgptr;
		}
	}

	return NULL;
}


unsigned char MCClient::devcode(char devs[3]) {
	unsigned char devptr = NULL;

	for (int ez = 0; ez < 1024; ez++) {
		if (!regmap[ez].bval && !regmap[ez].cval[0]) break;
		if (strcmp(regmap[ez].cval, devs) == 0){
			devptr = regmap[ez].bval;
			return devptr;
		}
	}

	return NULL;
}


void* MCClient::set_header_3e(MELSEC_MC_3E_REQ* ahdr) {

	ahdr->subheader[0] = MC_3E_REQ_SIG;
	ahdr->subheader[1] = 0x00;
	ahdr->network_no = MC_DEFAULT_NETWORK;
	ahdr->pc_no = MC_DEFAULT_PC;
	ahdr->request_dest_io = MC_DEFAULT_MODULE_IO;
	ahdr->request_dest_sta = MC_DEFAULT_MODULE_STATION;
	ahdr->monitor_timer = 0x0010;

	return ahdr;
}



int MCClient::batch_read(char* devname, char destination, int head_dev, mc_buff *outbuf, unsigned short length) {

	MELSEC_MC_3E_REQ request_header;
	MELSEC_MC_BATCHRW read_req;
	MELSEC_MC_3E_ACK resp_header;
	unsigned char tx_buf[128];		// header(21) + data buff(64) = for xyrix it's 85
	unsigned char rx_buf[4096];
	unsigned short qcontent_sz = 12; // Q content size is always 12 bytes for Tx
	int tx_sz = MC_3E_HEADER_SZ + qcontent_sz; // transmit data length
	int rx_sz = 0;
	unsigned short rez_datalen = 0;
	unsigned short rez_wordlen = 0;
	unsigned short cdatabuf[1024];


	// Setup header with defaults
	set_header_3e(&request_header);

	// Set command
	request_header.command = 0x0401;		// Batch Read
	read_req.subcommand = 0x0000;			// (read request) Response data type = Word [0000]
	read_req.dev_type = devcode(devname);	// Device Type
	request_header.data_length = qcontent_sz;	// content size + data length
	memcpy(read_req.head_device, &head_dev, 3);	// Copy head device number (24-bit integer)
	read_req.num_points = length;  			// Specify number of points


	// Copy data to transmit buffer
	memcpy(tx_buf, &request_header, sizeof(request_header));
	memmove(tx_buf + 7, tx_buf + 8, 6);
	memcpy(tx_buf + sizeof(request_header)-1, &read_req, sizeof(read_req));

	// Tx
	if ((send(sock, (char *)tx_buf, tx_sz, 0)) < tx_sz) {
		printf("data send error!\n");
		return 0;
	}

	// Rx
	if ((rx_sz = recv(sock, (char *)rx_buf, 4096, 0)) <= 0) {
		printf("no ack received!\n");
		return 0;
	}

	//printf("got %i bytes!\n", rx_sz);
	memcpy(&resp_header, rx_buf, sizeof(resp_header));


	if (resp_header.complete_code == 0x0000) {
		rez_datalen = resp_header.data_length - 2; // subtract 2 from data length to account for response code (word)
		rez_wordlen = rez_datalen / 2;
		//printf("char data len = %u bytes (%i words).\n", rez_datalen, rez_wordlen);
		if (outbuf > 0) {
			//memcpy(&cdatabuf, rx_buf + sizeof(resp_header), rez_datalen);
			//memcpy(outbuf, rx_buf + sizeof(resp_header), rez_datalen);
			if (destination == SELECT_XYRIS)
				memcpy(&outbuf->xyris, rx_buf+1 + sizeof(resp_header), rez_datalen);
			else if (destination == SELECT_TORSO)
				memcpy(&outbuf->torso, rx_buf+1 + sizeof(resp_header), rez_datalen);
			else if (destination == SELECT_YBM)
				memcpy(&outbuf->ybm, rx_buf+1 + sizeof(resp_header), rez_datalen);
			else if (destination == SELECT_INTERLOCK)
				memcpy(&outbuf->interlock, rx_buf + 1 + sizeof(resp_header), rez_datalen);
			else if (destination == SELECT_COMMON)
				memcpy(&outbuf->common, rx_buf + 1 + sizeof(resp_header), rez_datalen);

			return rez_wordlen;
		}
		else {
			printf("invalid outbuf pointer!\n");
		}
	}
	else {
		printf("abnormal completion. [%04hX] %s\n", errmsg(resp_header.complete_code));
		printf("abnormal response: [0x%04hX] %s", errmsg(resp_header.complete_code));
		return -1;
	}

	return -1;
}



int MCClient::batch_write(char* devname, char destination, int head_dev, mc_buff* inbuf, unsigned short length) {

	MELSEC_MC_3E_REQ request_header;
	MELSEC_MC_BATCHRW read_req;
	MELSEC_MC_3E_ACK resp_header;
	unsigned char tx_buf[128];		// header(21) + data buff(64) = for xyrix it's 85
	unsigned char rx_buf[4096];
	unsigned short qcontent_sz = 12; // Q content size is always 12 bytes for Tx
	int tx_sz = MC_3E_HEADER_SZ + length * 2; // transmit data length
	int rx_sz = 0;
	unsigned short rez_datalen = 0;
	unsigned short rez_wordlen = 0;
	unsigned short cdatabuf[1024];

	// Setup header with defaults
	set_header_3e(&request_header);

	// Set command
	request_header.command = 0x1401;	// Batch Write
	read_req.subcommand = 0x0000;			// (read request) Response data type = Word [0000]
	read_req.dev_type = devcode(devname);	// Device Type
	request_header.data_length = qcontent_sz + length * 2;	// content size + data length
	memcpy(read_req.head_device, &head_dev, 3);	// Copy head device number (24-bit integer)
	read_req.num_points = length;  			// Specify number of points

	// Copy data to transmit buffer
	memcpy(tx_buf, &request_header, sizeof(request_header));
	memmove(tx_buf + 7, tx_buf + 8, 6);
	memcpy(tx_buf + sizeof(request_header)-1, &read_req, sizeof(read_req));

	// appending xyris data to tx_buff
	if (destination == SELECT_XYRIS)
		memcpy(tx_buf + 21, &inbuf->xyris, sizeof(inbuf->xyris));
	else if (destination == SELECT_TORSO)
		memcpy(tx_buf + 21, &inbuf->torso, sizeof(inbuf->torso));
	else if (destination == SELECT_YBM)
		memcpy(tx_buf + 21, &inbuf->ybm, sizeof(inbuf->ybm));
	else if (destination == SELECT_INTERLOCK)
		memcpy(tx_buf + 21, &inbuf->interlock, sizeof(inbuf->interlock));
	else if (destination == SELECT_COMMON)
		memcpy(tx_buf + 21, &inbuf->common, sizeof(inbuf->common));

	// Tx
	if ((send(sock, (char *)tx_buf, tx_sz, 0)) < tx_sz) {
		//printf("data send error!\n");
		return 0;
	}

	// Rx
	if ((rx_sz = recv(sock, (char *)rx_buf, 4096, 0)) <= 0) {
	//	printf("no ack received!\n");
		return 0;
	}

	memcpy(&resp_header, rx_buf, sizeof(resp_header));

	if (resp_header.complete_code == 0x0000) {
		//printf("write %i bytes complete!\n", tx_sz);
	}

	return -1;
}




// Error code to Error message mapping
MELSEC_ERRMAP errmap[] = {
	{ 0x4000, "Serial communication checksum error" },
	{ 0x4001, "Unsupported request" },
	{ 0x4002, "Unsupported request" },
	{ 0x4003, "Global request cannot be performed" },
	{ 0x4004, "Access denied. System Protection is in effect" },
	{ 0x4005, "Data size too large" },
	{ 0x4006, "Initial communication failed" },
	{ 0x4008, "CPU module is busy (buffer full)" },
	{ 0x4010, "Cannot execute while CPU is in RUN Mode" },
	{ 0x4013, "Cannot execute while CPU is in RUN Mode" },

	{ 0x4021, "Specified drive memory does not exist or read error" },
	{ 0x4022, "Specified file name or number does not exist" },
	{ 0x4023, "Specified file name and file number do not match" },
	{ 0x4024, "Access denied. Insufficient user permissions" },
	{ 0x4025, "File is busy" },
	{ 0x4026, "Password, keyword, or password-32 must be set before access" },
	{ 0x4027, "Specified range is greater than file size" },
	{ 0x4028, "File already exists" },
	{ 0x4029, "Insufficient space to allocate file memory" },
	{ 0x402A, "Specified file is corrupt/abnormal" },
	{ 0x402B, "Unable to execute request in specified memory location" },
	{ 0x402C, "Operation failed, device is busy. Try again later" },

	{ 0x4030, "Specified device type unsupported or device number exceeds CPU limits for extended registers" },
	{ 0x4031, "Specified device number is out of range" },
	{ 0x4032, "Invalid device type specification or unsupported device type for requested operation" },
	{ 0x4033, "Write access denied. Device is reserved for system use only" },
	{ 0x4034, "Execution failed. CPU is in STOP Mode" },

	{ 0x4040, "Function not supported by target module" },
	{ 0x4041, "Specified address is not within the target module's memory range" },
	{ 0x4042, "Failed to access target module" },
	{ 0x4043, "Incorrect I/O address spec for target module" },
	{ 0x4044, "Failed to access target module due to bus or hardware error" },

	{ 0x4170, "Specified password is incorrect" },
	{ 0x4171, "Password is required before communication can proceed" },
	{ 0x4174, "Invalid request during password unlock attempt" },
	{ 0x4176, "Direct connection failed" },
	{ 0x4178, "Access denied. An FTP operation is currently active" },
	{ 0x4180, "System error due to module, CPU, or power supply hardware failure" },
	{ 0x4181, "Downstream transmission failed" },
	{ 0x4182, "Downstream communication timed out" },
	{ 0x4183, "Downstream connection lost" },
	{ 0x4184, "Communication buffer full" },
	{ 0x4185, "Lost connection to remote device" },

	{ NULL, NULL }
};


// Mapping of register types from ASCII to binary codes used by MC Protocol
MELSEC_REGMAP regmap[] = {
	{ 0x91, "SM" },
	{ 0xA9, "SD" },
	{ 0x9C, "X" },
	{ 0x9D, "Y" },
	{ 0x90, "M" },
	{ 0x92, "L" },
	{ 0x93, "F" },
	{ 0x94, "V" },
	{ 0xA0, "B" },
	{ 0xA8, "D" },
	{ 0xB4, "W" },
	{ 0xC1, "TS" },
	{ 0xC0, "TC" },
	{ 0xC7, "SS" },
	{ 0xC6, "SC" },
	{ 0xC8, "SN" },
	{ 0xC4, "CS" },
	{ 0xC3, "CC" },
	{ 0xC5, "CN" },
	{ 0xA1, "SB" },
	{ 0xB5, "SW" },
	{ 0x98, "S" },
	{ 0xA2, "DX" },
	{ 0xA3, "DY" },
	{ 0xCC, "Z" },
	{ 0xAF, "R" },
	{ 0xB0, "ZR" },
	{ NULL, NULL }
};