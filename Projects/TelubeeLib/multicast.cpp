#include <winsock2.h> // for socker communication
#include <ws2tcpip.h> 
#include <sys/types.h>
#include "multicast.h"

#include <stdio.h>

long i=0;



// ---------------------- TCPServer function implementation -----------------------------------//

TCPServer::TCPServer(int portnum, int maxcon){
	WSAStartup(MAKEWORD(2,0), &wsaData);
	// �\�P�b�g�̍쐬
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	listen(sock0, maxcon);
	// TCP�N���C�A���g����̐ڑ��v�����󂯕t����
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

}

TCPServer::~TCPServer(){
	closesocket(sock);
	// winsock2�̏I������
	WSACleanup();
}

int TCPServer::sendstr(char *str){
	int ret = send(sock, str, (int)strlen(str), 0);
	// TCP�Z�b�V�����̏I��
	return ret;
}

int TCPServer::recvstr(char *buf, int buflen){
	memset(buf, 0, buflen);
	int n = recv(sock, buf, buflen, 0);
	return n;
}



// ---------------------- TCPClient function implementation -----------------------------------//

TCPClient::TCPClient(int portnum, char *ipaddr){
	// winsock2�̏�����
	WSAStartup(MAKEWORD(2,0), &wsaData);

	// �ڑ���w��p�\���̂̏���
	server.sin_family = AF_INET;
	server.sin_port = htons(portnum);
	server.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// �T�[�o�ɐڑ�
	connect(sock, (struct sockaddr *)&server, sizeof(server));

}

TCPClient::~TCPClient(){
	closesocket(sock);
	// winsock2�̏I������
	WSACleanup();
}

int TCPClient::recvstr(char *buf, int buflen){
	memset(buf, 0, buflen);
	int n = recv(sock, buf, buflen, 0);
	return n;
}

int TCPClient::sendstr(char *str){
	int ret = send(sock, str, (int)strlen(str), 0);
	// TCP�Z�b�V�����̏I��
	return ret;
}


// ---------------------- UDPServer function implementation -----------------------------------//


UDPServer::UDPServer(int portnum){
	WSAStartup(MAKEWORD(2,0), &wsaData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	buffcount = 0; thisbuffcount = 0;
	threadflag = false;
}

UDPServer::~UDPServer(){
	// �X���b�h���N�����Ă�����~�߂�
	stopRecving();
	CloseHandle(hTh);
	// �\�P�b�g�N���[�Y
	closesocket(sock);
	WSACleanup();
}

//�P���̎�M�֐�
int UDPServer::recvstr(char *buf,int buflen){//�P���̎�M
	memset(buf,0,buflen);
	int ret = recv(sock, buf, buflen, 0);
	return ret;
}

//��M���ē����o�b�t�@���X�V����
int UDPServer::recvBuffer(){// ��M���ē����o�b�t�@�Ɋi�[
	char buf[BUFFLEN];
	memset(buf,0,BUFFLEN);
	int ret = recv(sock,buf,BUFFLEN,0);
	buffer = buf;
	buffcount++;
	return ret;
}

//�����o�b�t�@��ǂ�
int UDPServer::readBuffer(char *buf, int buflen){//�����o�b�t�@����ǂ�
	// �����𖞂����Ȃ����̂��R��o��
	if(buf == NULL) return 0;	// �^����ꂽbuf��NULL�̏ꍇ
	if(buflen < (int)buffer.size()+1) return 0;	// �^����ꂽ�̈�T�C�Y������Ȃ��ꍇ
	//�R�s�[
	strcpy_s(buf,buflen,buffer.c_str());
	thisbuffcount = buffcount;
	return buffcount;
}

//�����o�b�t�@��ǂށA�ȒP��std::string�ԋp�^�C�v
std::string UDPServer::readBuffer(){
	return buffer;
}

// �ʐM�p�X���b�h
// �X���b�h�J�n
int UDPServer::startRecving(){
	if(threadflag) return 0; //�X���b�h�N���ς݂Ȃ̂ŃG���[�Ԃ�
	threadflag = true;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPServer::threadfunc),this,0,&threadID);
	return 1;
}

int UDPServer::stopRecving(){
	if(!threadflag) return 0;	// ���łɏI���ς݂ł���Ζ���
	else threadflag = false;	// �����łȂ���ΏI���t���O��������
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}

// �X���b�h�p���[�v
unsigned __stdcall UDPServer::threadfunc(void *pArg){
	return ((UDPServer*)pArg)->recvloop();
}

unsigned UDPServer::recvloop(){
	while(1){
		recvBuffer();
		if(!threadflag) break;	// �X���b�h�N���t���O���I�t�ɂ���Ă�����I��
	}
	return 1;
}


// ---------------------- UDPClient function implementation -----------------------------------//

UDPClient::UDPClient()
{
	stopped=true;
}
UDPClient::~UDPClient()
{
	Close();
}

bool UDPClient::Connect(int portnum, const char *ipaddr){
	// �\�P�b�g������
	WSAStartup(MAKEWORD(2,0), &wsaData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// ���M�p�ϐ��̏�����
	buffer = "";
	buffcount = 0;
	threadflag = false;
	// �X���b�h����p�ϐ�������
	hEv = CreateEvent(NULL,true,false,NULL); //���M�^�C�~���O�����p�C�x���g�n���h��
	hEvQt = CreateEvent(NULL,true,false,NULL); //���M�X���b�h�I���ʒm�p�C�x���g
	return true;
}

void UDPClient::Close(){
	// �X���b�h�I������
	stopSending();
	CloseHandle(hTh);
	CloseHandle(hEv);
	CloseHandle(hEvQt);
	// �\�P�b�g�������
	closesocket(sock);
	WSACleanup();
}

// �P���ŕ�����𑗐M����
int UDPClient::sendstr(char *str){
	int ret = sendto(sock, str, (int)strlen(str), 0, (struct sockaddr *)&addr, sizeof(addr));
	return ret;
}

// ���M�p�o�b�t�@���Z�b�g����
int UDPClient::setBuffer(const std::string str){
	buffer = str;
	buffcount++;
	PulseEvent(hEv);
	return 1;
}

// �o�b�t�@���e�𑗐M
int UDPClient::sendBuffer(){
	static int buffcountnow=buffcount;
	if(buffcountnow==buffcount) return 0;
	int ret = sendto(sock, buffer.c_str(), (int)buffer.size(), 0, (struct sockaddr *)&addr, (int)sizeof(addr));
	return ret;
}

// �ʐM�p�X���b�h
// �X���b�h�J�n
int UDPClient::startSending(){
	stopped=false;
	DWORD retth = WaitForSingleObject(hTh,0); //�X���b�h�͊J�n���Ă��邩�H
	if(retth == WAIT_TIMEOUT) return 0;//�X���b�h�N���ς݂Ȃ̂ŃG���[�Ԃ�
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPClient::threadfunc),this,0,&threadID);
	return 1;
}

int UDPClient::stopSending(){
	stopped=true;
	DWORD retth = WaitForSingleObject(hTh,0); //�X���b�h�͊J�n���Ă��邩�H
	if(retth == WAIT_OBJECT_0) return 0;//�X���b�h�������Ă��Ȃ���Ζ���
	SetEvent(hEvQt);	//�X���b�h�I���w�߃C�x���g
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}

// �X���b�h�p���[�v
unsigned __stdcall UDPClient::threadfunc(void *pArg){
	return ((UDPClient*)pArg)->sendloop();
}

unsigned UDPClient::sendloop(){
	HANDLE hs[2] = {hEvQt,hEv};
	DWORD rethd;
	while(1){
		rethd = WaitForMultipleObjects(2,hs,false,INFINITE);
		if(rethd == WAIT_OBJECT_0 || stopped) break; //hEvQt���V�O�i����ԂɂȂ����Ȃ�I��
		sendBuffer();
	}
	return 1;
}





// ---------------------- MCAST Client function implementation -----------------------------------//


MCASTClient::MCASTClient(int portnum, char *ipaddr){
	WSAStartup(MAKEWORD(2,0), &wsaData);
	sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	buffer = "";
	buffcount = 0;
	sockM = WSAJoinLeaf(sock, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL, JL_BOTH); 
	
	threadflag = false;
	hEv = CreateEvent(NULL,true,false,NULL); 
	hEvQt = CreateEvent(NULL,true,false,NULL); 
}

MCASTClient::~MCASTClient(){
	stopSending();
	CloseHandle(hTh);
	CloseHandle(hEv);
	CloseHandle(hEvQt);
	closesocket(sock);
	closesocket(sockM);
	WSACleanup();
}

int MCASTClient::sendstr(char *str){
	int ret = sendto(sockM, str, (int)strlen(str), 0, (struct sockaddr *)&addr, sizeof(addr));
	return ret;
}

int MCASTClient::setBuffer(const std::string str){
	buffer = str;
	buffcount++;
	PulseEvent(hEv);
	return 1;
}

int MCASTClient::sendBuffer(){
	static int buffcountnow=buffcount;
	if(buffcountnow==buffcount) return 0;
	int ret = sendto(sockM, buffer.c_str(), (int)buffer.size(), 0, (struct sockaddr *)&addr, (int)sizeof(addr));
	return ret;
}

int MCASTClient::startSending(){
	DWORD retth = WaitForSingleObject(hTh,0); 
	if(retth == WAIT_TIMEOUT) return 0;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(MCASTClient::threadfunc),this,0,&threadID);
	return 1;
}

int MCASTClient::stopSending(){
	DWORD retth = WaitForSingleObject(hTh,0); 
	if(retth == WAIT_OBJECT_0) return 0;
	SetEvent(hEvQt);
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}


unsigned __stdcall MCASTClient::threadfunc(void *pArg){
	return ((MCASTClient*)pArg)->sendloop();
}

unsigned MCASTClient::sendloop(){
	HANDLE hs[2] = {hEvQt,hEv};
	DWORD rethd;
	while(1){
		rethd = WaitForMultipleObjects(2,hs,false,INFINITE);
		if(rethd == WAIT_OBJECT_0) break; 
		sendBuffer();
	}
	return 1;
}



// ---------------------- MCAST Server function implementation -----------------------------------//

MCASTServer::MCASTServer(int portnum, char *ipaddr){
	len = sizeof(struct sockaddr_in); 
	WSAStartup(MAKEWORD(2,2), &wsaData);
	sock = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&addr, sizeof(addr));

	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(portnum); 
	remote_addr.sin_addr.s_addr = inet_addr(ipaddr); 

	sockM =	WSAJoinLeaf(sock, (SOCKADDR*)&remote_addr, sizeof(remote_addr), NULL, NULL, NULL, NULL, JL_BOTH);

	buffcount = 0; thisbuffcount = 0;
	threadflag = false;
}

MCASTServer::~MCASTServer(){
	stopRecving();
	CloseHandle(hTh);
	closesocket(sock);
	WSACleanup();
}


int MCASTServer::recvstr(char *buf,int buflen){
	memset(buf,0,buflen);
	int ret = recvfrom(sock, buf, buflen, 0, (struct sockaddr*)&from_addr, &len);
	return ret;
}


int MCASTServer::recvBuffer(){
	char buf[M_BUFSIZE];
	memset(buf,0,M_BUFSIZE);

	int ret = recvfrom(sock, buf, M_BUFSIZE, 0, (struct sockaddr*)&from_addr, &len); 
	buf[ret] = '\0'; 

	buffer = buf;
	buffcount++;

	return ret;

}


int MCASTServer::readBuffer(char *buf, int buflen){
	if(buf == NULL) return 0;
	if(buflen < (int)buffer.size()+1) return 0;	
	strcpy_s(buf, buflen, buffer.c_str());
	thisbuffcount = buffcount;

	return buffcount;
}


std::string MCASTServer::readBuffer(){
	return buffer;
}

int MCASTServer::startRecving(){
	if(threadflag) return 0; 
	threadflag = true;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(MCASTServer::threadfunc),this,0,&threadID);
	return 1;
}

int MCASTServer::stopRecving(){
	if(!threadflag) return 0;
	else threadflag = false;
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}

unsigned __stdcall MCASTServer::threadfunc(void *pArg){
	return ((MCASTServer*)pArg)->recvloop();
}

unsigned MCASTServer::recvloop(){
	while(1){
		recvBuffer();
		if(!threadflag) break;
	}
	return 1;
}