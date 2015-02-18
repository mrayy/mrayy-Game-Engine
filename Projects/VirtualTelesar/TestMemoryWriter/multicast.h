#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#ifndef _INC_MULTICAST_H
#define _INC_MULTICAST_H


/*--------------------------------------------------------------------------*/
// Multicast Routins 
//
// Written by charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/


#define MYWSOCK_VERSION	2.3

#define MCASTADDR	"239.255.40.99"		// address of the multicast group.
#define MCASTPORT	1211				// multicast port number. 
#define M_BUFSIZE	1024				// receive data buffer size. 

// netsh firewall show config

//224.0.0.0		| 224.0.0.255		| Reserved for special Ågwell-knownÅh multicast addresses.
//224.0.1.0		| 238.255.255.255	| Globally-scoped (Internet-wide) multicast addresses.
//239.0.0.0		| 239.255.255.255	| Administratively-scoped (local) multicast addresses.

/*--------------------------------------------------------------------------*/

// hosts
#define TELESAR_SERVER	"192.168.10.32"
#define TELESAR_MASTER	"192.168.10.40"
#define TELESAR_BACKUP	"192.168.10.116"
#define LOOPBACK		"127.0.0.1"		// for testing only

// comm ports
#define BUFFLEN			500				// buffer length
#define GLOVEDATA_PORT	50000	
#define PA10DATA_PORT	20000	
#define SATTUSDATA_PORT	20001
#define NCORD_PORT		26574

// define UDP Receiver also as UDP Server and UDP Sender also as UDPClient
#define UDPReceiver	UDPServer
#define UDPSender	UDPClient


#include <winsock2.h>			// link 32bit/64bit ws2_32.lib
#include <process.h>
#include <string>



// ---------------------- TCP/UDP Class Definitions -----------------------------------//

class TCPServer{
	private:
		WSADATA wsaData;
		SOCKET sock0;
		struct sockaddr_in addr;
		struct sockaddr_in client;
		int len;
		SOCKET sock;
	public:
		TCPServer(int portnum, int maxcon);
		~TCPServer();
		int sendstr(char *str);
		int recvstr(char *buf, int buflen);
};


class TCPClient{
	private:
		WSADATA wsaData;
		struct sockaddr_in server;
		SOCKET sock;
	public:
		TCPClient(int portnum, char *ipaddr);
		~TCPClient();
		int sendstr(char *str);
		int recvstr(char *buf, int buflen);
};


class UDPServer{
	private:
		WSAData wsaData;
		SOCKET sock;
		struct sockaddr_in addr;
		int buffcount,thisbuffcount; 
		bool threadflag;
		std::string buffer; 
		HANDLE hTh;
	public:
		UDPServer(int portnum);
		~UDPServer();
		int recvstr(char *buf,int buflen);
		int readBuffer(char *buf, int buflen);
		int recvBuffer();
		std::string readBuffer();
		int startRecving();
		int stopRecving();
		unsigned recvloop();
		static unsigned __stdcall threadfunc(void *pArg);
};


class UDPClient{
	private:
		WSAData wsaData;
		SOCKET sock;
		struct sockaddr_in addr;
		std::string buffer; 
		int buffcount;
		bool threadflag;
		HANDLE hTh,hEv,hEvQt;
	public:
		UDPClient(int portnum, char *ipaddr);
		~UDPClient();
		int sendstr(char* str); 
		int sendBuffer();
		int setBuffer(const std::string str);
		int startSending();
 		int stopSending();
		unsigned sendloop();
		static unsigned __stdcall threadfunc(void *pArg);
};



// ---------------------- MULTICAST Class Definitions -----------------------------------//

class MCASTClient{		//sender
	private:
		WSAData wsaData;
		SOCKET sock, sockM;
		struct sockaddr_in addr;
		std::string buffer; 
		int buffcount;
		bool threadflag;
		HANDLE hTh,hEv,hEvQt;
	public:
		MCASTClient(int portnum, char *ipaddr);
		~MCASTClient();
		int sendstr(char* str); 
		int sendBuffer();
		int setBuffer(const std::string str);
		int startSending();
 		int stopSending();
		unsigned sendloop();
		static unsigned __stdcall threadfunc(void *pArg);
};


class MCASTServer{		//receiver
	private:
		WSAData wsaData;
		SOCKET sock, sockM;
		struct sockaddr_in addr, remote_addr;
		int buffcount,thisbuffcount; 
		int len;
		bool threadflag;
		std::string buffer; 
		HANDLE hTh;
	public:
		struct sockaddr_in from_addr;
		MCASTServer(int portnum, char *ipaddr);
		~MCASTServer();
		int recvstr(char *buf,int buflen);
		int readBuffer(char *buf, int buflen);
		int recvBuffer();
		std::string readBuffer();
		int startRecving();
		int stopRecving();
		unsigned recvloop();
		static unsigned __stdcall threadfunc(void *pArg);
};



#endif


