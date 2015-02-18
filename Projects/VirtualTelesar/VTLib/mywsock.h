/*================================================
* Winsock2をベースにした簡易通信用ライブラリ
*                      by Fukamachi Soichiro
*                      Version1.2 : 2007/7/3
================================================*/


// 多重インクルード防止
#ifndef MYWSOCK_
#define MYWSOCK_

#define MYWSOCK_VERSION	2.3

// UDPのServerとClientを、ReceiverとSenderという名前でも使えるようにする
#define UDPReceiver	UDPServer
#define UDPSender	UDPClient
#define BUFFLEN		500	//バッファ長

// ここから本文
#include <winsock2.h>	//ws2_32.libをリンクせよ
#include <process.h>
#include <string>

//============================================================
// TCPサーバ用クラス
//--------------------
// 宣言
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
// 定義
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
//============================================================
// TCPクライアント用クラス
//--------------------
// 宣言
class TCPClient{
private:
	// Socket用変数
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
// 定義
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
//============================================================
// UDPサーバ用クラス
class UDPServer{
private:
	// ソケット通信用変数
	WSAData wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	// 受信用変数
	int buffcount,thisbuffcount; //バッファが読み取られるとカウントアップ
	bool threadflag;
	std::string buffer; //内部バッファ
	//スレッド制御変数
	HANDLE hTh;
public:
	//変数
	UDPServer(int portnum);
	~UDPServer();
	//関数
	int recvstr(char *buf,int buflen);// 一回だけ読む
	int readBuffer(char *buf, int buflen);// バッファから文字列を読み取る
	int recvBuffer();// 受け取ってクラス内のバッファに格納
	std::string readBuffer();
	// スレッド用関数
	int startRecving();
	int stopRecving();
	unsigned recvloop();
	static unsigned __stdcall threadfunc(void *pArg);
};
//--------------------
// 定義
// コンストラクタ、デストラクタ
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
	strcpy(buf,buffer.c_str());
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
//
//============================================================
// UDPクライアント用クラス
//--------------------
// 宣言
class UDPClient{
private:
	// ソケット通信用変数
	WSAData wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	// その他変数
	std::string buffer; //バッファ
	int buffcount;	//バッファ番号
	bool threadflag;
	// スレッド制御用変数
	HANDLE hTh,hEv,hEvQt;
public:
	// ラクタ
	UDPClient(int portnum,const char *ipaddr);
	~UDPClient();
	// そのほか関数
	int sendstr(char* str); //単発で送信する関数
	int sendBuffer();	//バッファの内容を送信する
	int setBuffer(const std::string str); //送信用バッファを登録
	// マルチスレッド用関数
	int startSending();	// バッファ内容の連続送信を開始する
 	int stopSending();	// バッファ内容の連続送信を停止する
	static unsigned __stdcall threadfunc(void *pArg);
	unsigned sendloop();
};
//--------------------
// 定義
// コンストラクタ、デストラクタ
UDPClient::UDPClient(int portnum, const char *ipaddr){
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
	DWORD retth = WaitForSingleObject(hTh,0); //スレッドは開始しているか？
	if(retth == WAIT_TIMEOUT) return 0;//スレッド起動済みなのでエラー返し
	unsigned threadID;
	hTh = (HANDLE)_beginthreadex(NULL,0,&(UDPClient::threadfunc),this,0,&threadID);
	return 1;
}
int UDPClient::stopSending(){
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
		if(rethd == WAIT_OBJECT_0) break; //hEvQtがシグナル状態になったなら終了
		sendBuffer();
	}
	return 1;
}
//本文ここまで
#endif //MYWSOCK