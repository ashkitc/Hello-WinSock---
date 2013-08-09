#include "stdafx.h"
#include "SimpleServer.h"
#include "ServerResponseException.h"
#include <vector>
void SimpleServer::prepareServerSocket()
{

	   //Creating a socket
		cout << "Creating socket..." << endl;
		serverSocket = socket(AF_INET, SOCK_STREAM , IPPROTO::IPPROTO_TCP);
		if( serverSocket == INVALID_SOCKET)
		{
			throw ServerResponseException("Cann't create socket");
		}
		cout << "Socket created successfully" << endl;
		
		// Binding a socket
		cout << "Binding a socket" << endl;
		setSocketAddress();
		if( ( bind( serverSocket, reinterpret_cast<sockaddr*>( &sockAddress ), sizeof( sockAddress ) ) ) != 0 )
		{
			throw ServerResponseException("Cann't bind socket");
		}
		cout << "Socket bound successfully" << endl;
}

void SimpleServer::setSocketAddress()
{
	//setup family, IP address and port number
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_port = htons(portNumber);
	sockAddress.sin_addr.S_un.S_addr = INADDR_ANY;
}
bool SimpleServer::run( )
{
  bool succes = true;
  SOCKET clientSocket = INVALID_SOCKET;
  try
  {
	  prepareServerSocket();
	  
	  //Put socket into listening mode
	  if( listen( serverSocket, SOMAXCONN ) != 0 )
	  {
		  throw ServerResponseException("Cann't put socket into listening mode");
	  }

	  cout << "Waiting for connection ..." << endl;

	  sockaddr_in clientSockAddr;
	  int clientSockSize = sizeof(clientSockAddr);

	  clientSocket = accept(serverSocket ,reinterpret_cast<sockaddr*>(&clientSockAddr) ,&clientSockSize);
	  if ( clientSocket == INVALID_SOCKET )
	  {
		  throw ServerResponseException("Accept failed");
	  }

	  cout << "Accepted" << endl;
	  
	  //TODO:Handle connection here
	  HandleConnection(clientSocket,clientSockAddr);
  }
  catch (ServerResponseException e)
  {
	  cerr << endl << "Error: " << e.what() << endl;
	  succes = false;
  }

  if( serverSocket != INVALID_SOCKET )
  {
	  closesocket(serverSocket);
  }

  if( clientSocket != INVALID_SOCKET )
  {
	  closesocket(clientSocket);
  }
}

string SimpleServer::getHostDescription(const sockaddr_in  &address)
{
	ostringstream stringStream;
	stringStream << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port);
	return stringStream.str();
}

void SimpleServer::HandleConnection( SOCKET clientSocket, const sockaddr_in &sockAddr )
{
	cout << "Connected with " << getHostDescription(sockAddr) << endl;
	string result= "";
    char buf[1024];
    u_long len = 1024;
    while (true) {
   
    if (len > 1024) len = 1024;
	int rv = recv (clientSocket, buf, len, 0);
    if (rv <= 0) break;
	else
	{
		std::string t;
		t.assign (buf, rv);
		result += t;
		cout <<result << endl;
		break;
	}
	
  }
	
	if (send(clientSocket, answer(result).c_str() ,answer(result).length(), 0) == SOCKET_ERROR)
			throw ServerResponseException("socket error while sending.");
	
}

string SimpleServer::answer(string HTTPrequest)
{
	return "<html><head><title>It works!!!!</title></head><body>Hello fucking World</body></html>";
}
