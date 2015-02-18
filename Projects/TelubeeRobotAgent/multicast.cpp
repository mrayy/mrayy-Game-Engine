#include <winsock2.h> // for socker communication
#include <ws2tcpip.h> 
#include <sys/types.h>
#include "multicast.h"

#include <stdio.h>

long i=0;



// ---------------------- TCPServer function implementation -----------------------------------//

TCPServer::TCPServer(int portnum, int maxcon){
	WSAStartup(MAKEWORD(2,0), &wsaData);
	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// TCPクライアントからの接続要求を待てる状態にする
	listen(sock0, maxcon);
	// TCPクライアントからの接続要求を受け付ける
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

}

TCPServer::~TCPServer(){
	closesocket(sock);
	// winsock2の終了処理
	WSACleanup();
}

int TCPServer::sendstr(char *str){
	int ret = send(sock, str, (int)strlen(str), 0);
	// TCPセッションの終了
	return ret;
}

int TCPServer::recvstr(char *buf, int buflen){
	memset(buf, 0, buflen);
	int n = recv(sock, buf, buflen, 0);
	return n;
}



// ---------------------- TCPClient function implementation -----------------------------------//

TCPClient::TCPClient(int portnum, char *ipaddr){
	// winsock2の初期化
	WSAStartup(MAKEWORD(2,0), &wsaData);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(portnum);
	server.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));

}

TCPClient::~TCPClient(){
	closesocket(sock);
	// winsock2の終了処理
	WSACleanup();
}

int TCPClient::recvstr(char *buf, int buflen){
	memset(buf, 0, buflen);
	int n = recv(sock, buf, buflen, 0);
	return n;
}

int TCPClient::sendstr(char *str){
	int ret = send(sock, str, (int)strlen(str), 0);
	// TCPセッションの終了
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
	// スレッドが起動していたら止める
	stopRecving();
	CloseHandle(hTh);
	// ソケットクローズ
	closesocket(sock);
	WSACleanup();
}

//単発の受信関数
int UDPServer::recvstr(char *buf,int buflen){//単発の受信
	memset(buf,0,buflen);
	int ret = recv(sock, buf, buflen, 0);
	return ret;
}

//受信して内部バッファを更新する
int UDPServer::recvBuffer(){// 受信して内部バッファに格納
	char buf[BUFFLEN];
	memset(buf,0,BUFFLEN);
	int ret = recv(sock,buf,BUFFLEN,0);
	buffer = buf;
	buffcount++;
	return ret;
}

//内部バッファを読む
int UDPServer::readBuffer(char *buf, int buflen){//内部バッファから読む
	// 条件を満たさないものを蹴り出し
	if(buf == NULL) return 0;	// 与えられたbufがNULLの場合
	if(buflen < (int)buffer.size()+1) return 0;	// 与えられた領域サイズが足りない場合
	//コピー
	strcpy_s(buf,buflen,buffer.c_str());
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
	if(threadflag) return 0; //スレッド起動済みなのでエラー返し
	threadflag = true;
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPServer::threadfunc),this,0,&threadID);
	return 1;
}

int UDPServer::stopRecving(){
	if(!threadflag) return 0;	// すでに終了済みであれば無視
	else threadflag = false;	// そうでなければ終了フラグをさげる
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}

// スレッド用ループ
unsigned __stdcall UDPServer::threadfunc(void *pArg){
	return ((UDPServer*)pArg)->recvloop();
}

unsigned UDPServer::recvloop(){
	while(1){
		recvBuffer();
		if(!threadflag) break;	// スレッド起動フラグがオフにされていたら終了
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
	// ソケット初期化
	WSAStartup(MAKEWORD(2,0), &wsaData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnum);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	// 送信用変数の初期化
	buffer = "";
	buffcount = 0;
	threadflag = false;
	// スレッド制御用変数初期化
	hEv = CreateEvent(NULL,true,false,NULL); //送信タイミング同期用イベントハンドル
	hEvQt = CreateEvent(NULL,true,false,NULL); //送信スレッド終了通知用イベント
	return true;
}

void UDPClient::Close(){
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
int UDPClient::sendstr(char *str){
	int ret = sendto(sock, str, (int)strlen(str), 0, (struct sockaddr *)&addr, sizeof(addr));
	return ret;
}

// 送信用バッファをセットする
int UDPClient::setBuffer(const std::string str){
	buffer = str;
	buffcount++;
	PulseEvent(hEv);
	return 1;
}

// バッファ内容を送信
int UDPClient::sendBuffer(){
	static int buffcountnow=buffcount;
	if(buffcountnow==buffcount) return 0;
	int ret = sendto(sock, buffer.c_str(), (int)buffer.size(), 0, (struct sockaddr *)&addr, (int)sizeof(addr));
	return ret;
}

// 通信用スレッド
// スレッド開始
int UDPClient::startSending(){
	stopped=false;
	DWORD retth = WaitForSingleObject(hTh,0); //スレッドは開始しているか？
	if(retth == WAIT_TIMEOUT) return 0;//スレッド起動済みなのでエラー返し
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPClient::threadfunc),this,0,&threadID);
	return 1;
}

int UDPClient::stopSending(){
	stopped=true;
	DWORD retth = WaitForSingleObject(hTh,0); //スレッドは開始しているか？
	if(retth == WAIT_OBJECT_0) return 0;//スレッドが走っていなければ無視
	SetEvent(hEvQt);	//スレッド終了指令イベント
	WaitForSingleObject(hTh,INFINITE);
	return 1;
}

// スレッド用ループ
unsigned __stdcall UDPClient::threadfunc(void *pArg){
	return ((UDPClient*)pArg)->sendloop();
}

unsigned UDPClient::sendloop(){
	HANDLE hs[2] = {hEvQt,hEv};
	DWORD rethd;
	while(1){
		rethd = WaitForMultipleObjects(2,hs,false,INFINITE);
		if(rethd == WAIT_OBJECT_0 || stopped) break; //hEvQtがシグナル状態になったなら終了
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
	strcpy_s(buf,buflen,buffer.c_str());
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