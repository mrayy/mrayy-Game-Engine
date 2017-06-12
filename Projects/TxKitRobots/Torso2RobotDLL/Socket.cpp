
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
	// スレッドが起動していたら止める
	stopRecving();
	CloseHandle(hTh);
	// ソケットクローズ
	closesocket(sock);
	WSACleanup();
}

//単発の受信関数
int UDPServer::recvstr(char *buf, int buflen){//単発の受信
	memset(buf, 0, buflen);
	int ret = recv(sock, buf, buflen, 0);
	return ret;
}

//受信して内部バッファを更新する
int UDPServer::recvBuffer(){// 受信して内部バッファに格納
	char buf[BUFFLEN];
	memset(buf, 0, BUFFLEN);
	int ret = recv(sock, buf, BUFFLEN, 0);
	buffer = buf;
	buffcount++;
	return ret;
}

//内部バッファを読む
int UDPServer::readBuffer(char *buf, int buflen){//内部バッファから読む
	// 条件を満たさないものを蹴り出し
	if (buf == NULL) return 0;	// 与えられたbufがNULLの場合
	if (buflen < (int)buffer.size() + 1) return 0;	// 与えられた領域サイズが足りない場合
	//コピー
	strcpy(buf, buffer.c_str());
	thisbuffcount = buffcount;
	return buffcount;
}

//内部バッファを読む、簡単なstd::string返却タイプ
std::string UDPServer::readBuffer(){
	return buffer;
}

// 通信用スレッド
// スレッド開始
int UDPServer::startRecving(){
	if (threadflag) return 0; //スレッド起動済みなのでエラー返し
	threadflag = true;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL, 0, &(UDPServer::threadfunc), this, 0, &threadID);
	return 1;
}

int UDPServer::stopRecving(){
	if (!threadflag) return 0;	// すでに終了済みであれば無視
	else threadflag = false;	// そうでなければ終了フラグをさげる
	WaitForSingleObject(hTh, INFINITE);
	return 1;
}

// スレッド用ループ
unsigned __stdcall UDPServer::threadfunc(void *pArg){
	return ((UDPServer*)pArg)->recvloop();
}

unsigned UDPServer::recvloop(){
	while (1){
		recvBuffer();
		if (!threadflag) break;	// スレッド起動フラグがオフにされていたら終了
	}
	return 1;
}


// ---------------------- UDPClient function implementation -----------------------------------//


UDPClient::UDPClient(int portnum, char *ipaddr){
	// ソケット初期化
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP,
		0, 0,
		WSA_FLAG_OVERLAPPED |
		WSA_FLAG_MULTIPOINT_C_LEAF);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// 送信用変数の初期化
	buffer = "";
	buffcount = 0;
	threadflag = false;
	// スレッド制御用変数初期化
	hEv = CreateEvent(NULL, true, false, NULL); //送信タイミング同期用イベントハンドル
	hEvQt = CreateEvent(NULL, true, false, NULL); //送信スレッド終了通知用イベント
}

UDPClient::~UDPClient(){
	// スレッド終了処理
	stopSending();
	CloseHandle(hTh);
	CloseHandle(hEv);
	CloseHandle(hEvQt);
	// ソケット解放処理
	closesocket(sock);
	WSACleanup();
}

// 単発で文字列を送信する
int UDPClient::send(void* data, int length){
	int ret = ::sendto(sock, (const char*)data, length, 0, (struct sockaddr *)&addr, sizeof(addr));
	return ret;
}

int UDPClient::receive(void* data, int length){
	int ret = ::recv(sock, (char*)data, length, 0);
	return ret;
}
// 通信用スレッド
// スレッド開始
int UDPClient::startSending(){
	DWORD retth = WaitForSingleObject(hTh, 0); //スレッドは開始しているか？
	if (retth == WAIT_TIMEOUT) return 0;//スレッド起動済みなのでエラー返し
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL, 0, &(UDPClient::threadfunc), this, 0, &threadID);
	return 1;
}

int UDPClient::stopSending(){
	DWORD retth = WaitForSingleObject(hTh, 0); //スレッドは開始しているか？
	if (retth == WAIT_OBJECT_0) return 0;//スレッドが走っていなければ無視
	SetEvent(hEvQt);	//スレッド終了指令イベント
	WaitForSingleObject(hTh, INFINITE);
	return 1;
}

// スレッド用ループ
unsigned __stdcall UDPClient::threadfunc(void *pArg){
	return ((UDPClient*)pArg)->sendloop();
}

unsigned UDPClient::sendloop(){
	HANDLE hs[2] = { hEvQt, hEv };
	DWORD rethd;
	while (1){
		rethd = WaitForMultipleObjects(2, hs, false, INFINITE);
		if (rethd == WAIT_OBJECT_0) break; //hEvQtがシグナル状態になったなら終了
	}
	return 1;
}


