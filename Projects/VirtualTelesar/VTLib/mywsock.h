/*================================================
* Winsock2���x�[�X�ɂ����ȈՒʐM�p���C�u����
*                      by Fukamachi Soichiro
*                      Version1.2 : 2007/7/3
================================================*/


// ���d�C���N���[�h�h�~
#ifndef MYWSOCK_
#define MYWSOCK_

#define MYWSOCK_VERSION	2.3

// UDP��Server��Client���AReceiver��Sender�Ƃ������O�ł��g����悤�ɂ���
#define UDPReceiver	UDPServer
#define UDPSender	UDPClient
#define BUFFLEN		500	//�o�b�t�@��

// ��������{��
#include <winsock2.h>	//ws2_32.lib�������N����
#include <process.h>
#include <string>

//============================================================
// TCP�T�[�o�p�N���X
//--------------------
// �錾
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
//--------------------
// ��`
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
//============================================================
// TCP�N���C�A���g�p�N���X
//--------------------
// �錾
class TCPClient{
private:
	// Socket�p�ϐ�
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
public:
	TCPClient(int portnum, char *ipaddr);
	~TCPClient();
	int sendstr(char *str);
	int recvstr(char *buf, int buflen);
};
//--------------------
// ��`
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
//============================================================
// UDP�T�[�o�p�N���X
class UDPServer{
private:
	// �\�P�b�g�ʐM�p�ϐ�
	WSAData wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	// ��M�p�ϐ�
	int buffcount,thisbuffcount; //�o�b�t�@���ǂݎ����ƃJ�E���g�A�b�v
	bool threadflag;
	std::string buffer; //�����o�b�t�@
	//�X���b�h����ϐ�
	HANDLE hTh;
public:
	//�ϐ�
	UDPServer(int portnum);
	~UDPServer();
	//�֐�
	int recvstr(char *buf,int buflen);// ��񂾂��ǂ�
	int readBuffer(char *buf, int buflen);// �o�b�t�@���當�����ǂݎ��
	int recvBuffer();// �󂯎���ăN���X���̃o�b�t�@�Ɋi�[
	std::string readBuffer();
	// �X���b�h�p�֐�
	int startRecving();
	int stopRecving();
	unsigned recvloop();
	static unsigned __stdcall threadfunc(void *pArg);
};
//--------------------
// ��`
// �R���X�g���N�^�A�f�X�g���N�^
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
	strcpy(buf,buffer.c_str());
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
//
//============================================================
// UDP�N���C�A���g�p�N���X
//--------------------
// �錾
class UDPClient{
private:
	// �\�P�b�g�ʐM�p�ϐ�
	WSAData wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	// ���̑��ϐ�
	std::string buffer; //�o�b�t�@
	int buffcount;	//�o�b�t�@�ԍ�
	bool threadflag;
	// �X���b�h����p�ϐ�
	HANDLE hTh,hEv,hEvQt;
public:
	// ���N�^
	UDPClient(int portnum,const char *ipaddr);
	~UDPClient();
	// ���̂ق��֐�
	int sendstr(char* str); //�P���ő��M����֐�
	int sendBuffer();	//�o�b�t�@�̓��e�𑗐M����
	int setBuffer(const std::string str); //���M�p�o�b�t�@��o�^
	// �}���`�X���b�h�p�֐�
	int startSending();	// �o�b�t�@���e�̘A�����M���J�n����
 	int stopSending();	// �o�b�t�@���e�̘A�����M���~����
	static unsigned __stdcall threadfunc(void *pArg);
	unsigned sendloop();
};
//--------------------
// ��`
// �R���X�g���N�^�A�f�X�g���N�^
UDPClient::UDPClient(int portnum, const char *ipaddr){
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
}
UDPClient::~UDPClient(){
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
	DWORD retth = WaitForSingleObject(hTh,0); //�X���b�h�͊J�n���Ă��邩�H
	if(retth == WAIT_TIMEOUT) return 0;//�X���b�h�N���ς݂Ȃ̂ŃG���[�Ԃ�
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPClient::threadfunc),this,0,&threadID);
	return 1;
}
int UDPClient::stopSending(){
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
		if(rethd == WAIT_OBJECT_0) break; //hEvQt���V�O�i����ԂɂȂ����Ȃ�I��
		sendBuffer();
	}
	return 1;
}
//�{�������܂�
#endif //MYWSOCK