// Hello WinSock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerResponseException.h"
#include "SimpleServer.h"
#include <conio.h>
using namespace std;

const int  REQ_WINSOCK_VER   = 2;	


int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
 SimpleServer server;
	cout << "Initializing winsock... ";
 
	if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER,0), &wsaData)==0)
	{
		
		if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER)
		{
			cout << "initialized" << endl;
 
			int port = DEFAULT_PORT;
			if (argc > 1)
				port = atoi(argv[1]);
			while(true)
			{
				server.run();
			}
		}
		else
		{
			cerr << "required version not supported!";
		}
 
		cout << "Cleaning up winsock... ";
 
	
		if (WSACleanup()!=0)
		{
			cerr << "cleanup failed!" << endl;
		}   
		cout << "done" << endl;
	}
	else
	{
		cerr << "startup failed!" << endl;
	}
	_getch();
	return 0;
}

