
#include "stdafx.h"
#include "Socket.h"

// ---------------------- UDPServer function implementation -----------------------------------//


UDPServer::UDPServer(int portnum){
	WSAStartup(MAKEWORD(2, 0), &wsaData);
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
int UDPServer::recvstr(char *buf, int buflen){//�P���̎�M
	memset(buf, 0, buflen);
	int ret = recv(sock, buf, buflen, 0);
	return ret;
}

//��M���ē����o�b�t�@���X�V����
int UDPServer::recvBuffer(){// ��M���ē����o�b�t�@�Ɋi�[
	char buf[BUFFLEN];
	memset(buf, 0, BUFFLEN);
	int ret = recv(sock, buf, BUFFLEN, 0);
	buffer = buf;
	buffcount++;
	return ret;
}

//�����o�b�t�@��ǂ�
int UDPServer::readBuffer(char *buf, int buflen){//�����o�b�t�@����ǂ�
	// �����𖞂����Ȃ����̂��R��o��
	if (buf == NULL) return 0;	// �^����ꂽbuf��NULL�̏ꍇ
	if (buflen < (int)buffer.size() + 1) return 0;	// �^����ꂽ�̈�T�C�Y������Ȃ��ꍇ
	//�R�s�[
	strcpy(buf, buffer.c_str());
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
	if (threadflag) return 0; //�X���b�h�N���ς݂Ȃ̂ŃG���[�Ԃ�
	threadflag = true;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL, 0, &(UDPServer::threadfunc), this, 0, &threadID);
	return 1;
}

int UDPServer::stopRecving(){
	if (!threadflag) return 0;	// ���łɏI���ς݂ł���Ζ���
	else threadflag = false;	// �����łȂ���ΏI���t���O��������
	WaitForSingleObject(hTh, INFINITE);
	return 1;
}

// �X���b�h�p���[�v
unsigned __stdcall UDPServer::threadfunc(void *pArg){
	return ((UDPServer*)pArg)->recvloop();
}

unsigned UDPServer::recvloop(){
	while (1){
		recvBuffer();
		if (!threadflag) break;	// �X���b�h�N���t���O���I�t�ɂ���Ă�����I��
	}
	return 1;
}


// ---------------------- UDPClient function implementation -----------------------------------//


UDPClient::UDPClient(int portnum, char *ipaddr){
	// �\�P�b�g������
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP,
		0, 0,
		WSA_FLAG_OVERLAPPED |
		WSA_FLAG_MULTIPOINT_C_LEAF);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// ���M�p�ϐ��̏�����
	buffer = "";
	buffcount = 0;
	threadflag = false;
	// �X���b�h����p�ϐ�������
	hEv = CreateEvent(NULL, true, false, NULL); //���M�^�C�~���O�����p�C�x���g�n���h��
	hEvQt = CreateEvent(NULL, true, false, NULL); //���M�X���b�h�I���ʒm�p�C�x���g
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
int UDPClient::send(void* data, int length){
	int ret = ::sendto(sock, (const char*)data, length, 0, (struct sockaddr *)&addr, sizeof(addr));
	return ret;
}

int UDPClient::receive(void* data, int length){
	int ret = ::recv(sock, (char*)data, length, 0);
	return ret;
}
// �ʐM�p�X���b�h
// �X���b�h�J�n
int UDPClient::startSending(){
	DWORD retth = WaitForSingleObject(hTh, 0); //�X���b�h�͊J�n���Ă��邩�H
	if (retth == WAIT_TIMEOUT) return 0;//�X���b�h�N���ς݂Ȃ̂ŃG���[�Ԃ�
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL, 0, &(UDPClient::threadfunc), this, 0, &threadID);
	return 1;
}

int UDPClient::stopSending(){
	DWORD retth = WaitForSingleObject(hTh, 0); //�X���b�h�͊J�n���Ă��邩�H
	if (retth == WAIT_OBJECT_0) return 0;//�X���b�h�������Ă��Ȃ���Ζ���
	SetEvent(hEvQt);	//�X���b�h�I���w�߃C�x���g
	WaitForSingleObject(hTh, INFINITE);
	return 1;
}

// �X���b�h�p���[�v
unsigned __stdcall UDPClient::threadfunc(void *pArg){
	return ((UDPClient*)pArg)->sendloop();
}

unsigned UDPClient::sendloop(){
	HANDLE hs[2] = { hEvQt, hEv };
	DWORD rethd;
	while (1){
		rethd = WaitForMultipleObjects(2, hs, false, INFINITE);
		if (rethd == WAIT_OBJECT_0) break; //hEvQt���V�O�i����ԂɂȂ����Ȃ�I��
	}
	return 1;
}


