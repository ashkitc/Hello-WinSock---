#include "stdafx.h"
#include "SimpleServer.h"
#include "ServerResponseException.h"
#include <fstream>
#include <regex>
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


bool SimpleServer::run( ) //run a simple server
{
  bool succes = true;
  SOCKET clientSocket = INVALID_SOCKET; //initialize client socket descriptor
  try
  {
	  prepareServerSocket(); //create and bind server socket
	  
	  //Put socket into listening mode
	  if( listen( serverSocket, SOMAXCONN ) != 0 ) //begin listening
	  {
		  throw ServerResponseException("Cann't put socket into listening mode");
	  }

	  cout << "Waiting for connection ..." << endl;

	  sockaddr_in clientSockAddr;
	  int clientSockSize = sizeof(clientSockAddr);

	  clientSocket = accept(serverSocket ,reinterpret_cast<sockaddr*>(&clientSockAddr) ,&clientSockSize); //get the cliean socket
	  if ( clientSocket == INVALID_SOCKET )
	  {
		  throw ServerResponseException("Accept failed");
	  }

	  cout << "Accepted" << endl;
	  
	  HandleConnection(clientSocket,clientSockAddr); 
  }
  catch (ServerResponseException e)
  {
	  cerr << endl << "Error: " << e.what() << endl;
	  succes = false;
  }

  if( serverSocket != INVALID_SOCKET ) //close sockets
  {
	  closesocket(serverSocket);
  }

  if( clientSocket != INVALID_SOCKET )
  {
	  closesocket(clientSocket);
  }
}

string SimpleServer::getHostDescription(const sockaddr_in  &address) //gets clients ip an port
{
	ostringstream stringStream;
	stringStream << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port);
	return stringStream.str();
}

void SimpleServer::HandleConnection( SOCKET clientSocket, const sockaddr_in &sockAddr )
{
	cout << "Connected with " << getHostDescription(sockAddr) << endl;
	string result= "";
    char buf[1024]; //create buffer
    u_long len = 1024;
    while (true) {  //get clients request
   
    if (len > 1024) len = 1024;
	int byteCount= recv (clientSocket, buf, len, 0); //read part of request and chek count of readed bytes
    if (byteCount <= 0) break;
	else
	{
		std::string temp;
		temp.assign (buf, byteCount);
		result += temp;
		cout <<result << endl;
		break;
	}
	
  }
	if (send(clientSocket, answer(result).c_str() ,answer(result).length(), 0) == SOCKET_ERROR) //form and send answer
			throw ServerResponseException("socket error while sending.");
	
}

string SimpleServer::answer(const string &HTTPrequest) //forms answer
{
	string fileName = getAddressFromHTTPRequest(HTTPrequest); //get filename from HTTPRequest
	if(fileName == "") //if filename doesn't set
	{ 
		fileName = "index.htm"; //set filename as index.htm
	}
	ifstream htmlFile;
	char  currentLine[256];
	stringstream htmlContent;
	htmlFile.open(fileName,ifstream::in); 
	if(!htmlFile.is_open())
	{
		return "<html><head><title>ERROR</title></head><body>Page not fount</body></html>"; //return simple error message if file doesn't exist
	}
	else 
	{
		while(htmlFile.good()) //read file and form answer
		{
			htmlFile.getline(currentLine,256);
			htmlContent << currentLine;
		}
		htmlFile.close();
		return htmlContent.str(); //return answer (file's content)
	}
	
}

string SimpleServer::getAddressFromHTTPRequest( string HTTPRequest) //parse filename from HHTPRequest
{
	string firstLine = HTTPRequest.substr(HTTPRequest.find_first_of("/") + 1,HTTPRequest.find_first_of('\n'));
	return firstLine.substr(0,firstLine.find_first_of(' '));
}