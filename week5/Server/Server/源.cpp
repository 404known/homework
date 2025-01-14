
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	const int BUF_SIZE = 64;
	WSADATA			wsd;			//WSADATA变量
	SOCKET			sServer;		//服务器套接字
	SOCKADDR_IN		addrServ;;		//服务器地址
	char			buf[BUF_SIZE];	//接收数据缓冲区
	char			sendBuf[BUF_SIZE];//返回给客户端得数据
	int				retVal;			//返回值
	//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//创建套接字
	sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源;
		return  -1;
	}

	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);	//关闭套接字
		WSACleanup();			//释放套接字资源;
		return -1;
	}
	SOCKADDR clntAddr;  //客户端地址信息
	int nSize = sizeof(SOCKADDR);
	while (true)
	{
		//接收客户端数据
		ZeroMemory(buf, BUF_SIZE);
		retVal = recvfrom(sServer, buf, BUF_SIZE, 0,&clntAddr,&nSize);
		if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer);	//关闭套接字		
			WSACleanup();			//释放套接字资源;
			return -1;
		}
		if (buf[0] == '0')
			break;
		cout << "客户端发送的数据: " << buf << endl;

		cout << "向客户端发送数据: ";
		cin >> sendBuf;

		sendto(sServer, sendBuf, strlen(sendBuf), 0, &clntAddr, nSize);
	}

	//退出
	closesocket(sServer);	//关闭套接字
	WSACleanup();			//释放套接字资源;

	return 0;
}