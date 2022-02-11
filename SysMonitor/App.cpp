#include "sysinteraction.h"
#include <iostream>
#include <WS2tcpip.h>

using namespace std;

int main()
{
    
    SystemInformation si;
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 8080;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return 0;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "Connection established to Server." << endl;
	}


	// This needs modification to work for client side data fetching. Modify as needed.
	// Stored data for passing to the server only one time. Further modification can add more robustness
	char buf[4096];
	string userInput;
	userInput = si.getData();
	if (userInput.size() > 0)				// Make sure the user has typed in something
	{
		// Send the text
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			// Waiting for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				// Echo client response to console
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
			}
		}
	}

	// Shut down everything
	closesocket(sock);
	WSACleanup();
	return 0;
}


