
#include <winsock2.h>			// link 32bit/64bit ws2_32.lib
#include <process.h>
#include <string>

#ifndef __SOCKET___
#define __SOCKET___

#define BUFFLEN			500				// buffer length

class UDPServer{
private:
	WSAData wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	int buffcount, thisbuffcount;
	bool threadflag;
	std::string buffer;
	HANDLE hTh;
public:
	UDPServer(int portnum);
	~UDPServer();
	int recvstr(char *buf, int buflen);
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
	HANDLE hTh, hEv, hEvQt;
public:
	UDPClient(int portnum, char *ipaddr);
	~UDPClient();
	int send(void* data, int length);
	int receive(void* data, int length);
	int startSending();
	int stopSending();
	unsigned sendloop();
	static unsigned __stdcall threadfunc(void *pArg);
};

#endif