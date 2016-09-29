//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2015 Media Design School
//
// File Name	: 
// Description	: 
// Author		: Your Name
// Mail			: your.name@mediadesign.school.nz
//

//Library Includes
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <thread>

//Local Includes
#include "consoletools.h"
#include "network.h"
#include "networkentity.h"
#include "socket.h"

//This includes
#include "client.h"


CClient::CClient()
	:m_pcPacketData(0)
	, m_pClientSocket(0)
{
	ZeroMemory(&m_ServerSocketAddress, sizeof(m_ServerSocketAddress));
}

CClient::~CClient()
{
	delete[] m_pcPacketData;
	m_pcPacketData = 0;

	delete m_pClientSocket;
	m_pClientSocket = 0;
}

/***********************
* Initialise: Initialises a client object by creating a client socket and filling out the socket address structure with details of server to send the data to.
* @author: 
* @parameter: none
* @return: void
********************/
bool CClient::Initialise()
{
	char _cServerIPAddress[128];
	unsigned short _usServerPort;
	//Create a Packet Array
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];

	//Create a socket object
	m_pClientSocket = new CSocket();
	
	//Get the port number to bind the socket to
	unsigned short _usClientPort = QueryPortNumber(DEFAULT_CLIENT_PORT);
	//Initialise the socket to the local loop back address and port number
	m_pClientSocket->Initialise("127.0.0.1", _usClientPort);

	//Get the IP address of the server to send data to
	std::cout << "Enter server IP or empty for localhost: ";
	
	gets_s(_cServerIPAddress);
	if (_cServerIPAddress[0] == 0)
	{
		strcpy_s(_cServerIPAddress, "127.0.0.1");
	}
	//Get the Port Number of the server
	std::cout << "Enter server's port number or 0 for default server port: ";
	std::cin >> _usServerPort;
	
	if (_usServerPort == 0)
	{
		_usServerPort = DEFAULT_SERVER_PORT;
	}

	std::cout << "Attempting to connect to server at " << _cServerIPAddress << ":" << _usServerPort << std::endl;
	//Fill in the details of the server's socket address structure.
	//This will be used when stamping address on outgoing packets
	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_port = htons(_usServerPort);
	inet_pton(AF_INET, _cServerIPAddress, &m_ServerSocketAddress.sin_addr);
	return true;
}

bool CClient::SendData(const char* _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;
	char _RemoteIP[MAX_ADDRESS_LENGTH];
	inet_ntop(AF_INET, &m_ServerSocketAddress.sin_addr, _RemoteIP, sizeof(_RemoteIP));
	//std::cout << "Trying to send " << _pcDataToSend << " to " << _RemoteIP << ":" << ntohs(m_ServerSocketAddress.sin_port) << std::endl;
	int iNumBytes = sendto(
		m_pClientSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ServerSocketAddress),	// address to be filled with packet target
		sizeof(m_ServerSocketAddress)							// size of the above address struct.
		);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}
	return true;
}

void CClient::ReceiveData(char* _pcBufferToReceiveData)
{
	sockaddr_in _FromAddress; // Make a local variable to extract the IP and port number of the sender from whom we are receiving
	//In this case; it should be the details of the server; since the client only ever receives from the server
	int iSizeOfAdd = sizeof(_FromAddress);
	int _iNumOfBytesReceived;
	
	while(true)
	{
		// pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			m_pClientSocket->GetSocketHandle(),						// client-end socket being used to read from
			m_pcPacketData,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&_FromAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
			);
		if (_iNumOfBytesReceived < 0)
		{
			//Error in receiving data 
			_pcBufferToReceiveData = 0;
		}
		else if (_iNumOfBytesReceived == 0)
		{
			//The remote end has shutdown the connection
			_pcBufferToReceiveData = 0;
		}
		else
		{
			//There is valid data received.
			strcpy_s(_pcBufferToReceiveData, strlen(m_pcPacketData) + 1, m_pcPacketData);
		}
		std::this_thread::yield(); //Yield the processor; giving the main a chance to run.
	}
}

void CClient::GetRemoteIPAddress(char *_pcSendersIP)
{
	inet_ntop(AF_INET, &(m_ServerSocketAddress.sin_addr), _pcSendersIP, sizeof(_pcSendersIP));
	return;
}

unsigned short CClient::GetRemotePort()
{
	return ntohs(m_ServerSocketAddress.sin_port);
}

void CClient::GetPacketData(char* _pcLocalBuffer)
{
	strcpy_s(_pcLocalBuffer, strlen(m_pcPacketData) + 1, m_pcPacketData);
}