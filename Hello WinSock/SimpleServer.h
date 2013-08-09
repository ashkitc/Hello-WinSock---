#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
const int  DEFAULT_PORT  = 8080;	
const int  TEMP_BUFFER_SIZE  = 1024;
class SimpleServer
{
public:
	SimpleServer(void):serverSocket(INVALID_SOCKET),sockAddress(sockaddr_in()),portNumber(DEFAULT_PORT){};
	bool run( );
private:
	SOCKET serverSocket;
	sockaddr_in	sockAddress;
	int portNumber;
	void prepareServerSocket(); //Creates and binds socket
	void setSocketAddress();
	void HandleConnection( SOCKET clientSocket, const sockaddr_in &sockAddr );
	string getHostDescription( const sockaddr_in &address);
	string answer(string HTTPrequest);
};

