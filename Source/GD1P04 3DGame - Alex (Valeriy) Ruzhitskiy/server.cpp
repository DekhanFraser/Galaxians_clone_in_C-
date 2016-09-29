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
#include <iostream>


//Local Includes
#include "network.h"
#include "consoletools.h"
#include "socket.h"


//Local Includes
#include "server.h"

CServer::CServer()
	:m_pcPacketData(0),
	m_pServerSocket(0)
{
	ZeroMemory(&m_ClientAddress, sizeof(m_ClientAddress));
}

CServer::~CServer()
{
	delete[] m_pcPacketData;
	m_pcPacketData = 0;
	
	delete m_pServerSocket;
	m_pServerSocket = 0;
}

bool CServer::Initialise()
{
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	
	//Create a socket object
	m_pServerSocket = new CSocket();

	//Get the port number to bind the socket to
	unsigned short _usServerPort = QueryPortNumber(DEFAULT_SERVER_PORT);

	//Initialise the socket to the local loop back address and port number
	m_pServerSocket->Initialise("127.0.0.1", _usServerPort);

	return true;
}

bool CServer::SendData(const char* _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;
	
	int iNumBytes = sendto(
		m_pServerSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet target
		sizeof(m_ClientAddress)							// size of the above address struct.
		);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}
	return true;
}

void CServer::ReceiveData(char* _pcBufferToReceiveData)
{
	
	int iSizeOfAdd = sizeof(m_ClientAddress);
	int _iNumOfBytesReceived;
	int _iPacketSize;
	// pull off the packet(s) using recvfrom()
	_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
		m_pServerSocket->GetSocketHandle(),						// client-end socket being used to read from
		m_pcPacketData,							// incoming packet to be filled
		MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
		0,										// flags
		reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet source
		&iSizeOfAdd								// size of the above address struct.
		);
	if (_iNumOfBytesReceived < 0)
	{
		int _iError = WSAGetLastError();
		ErrorRoutines::PrintWSAErrorInfo(_iError);
		//return false;
	}
	_iPacketSize = strlen(m_pcPacketData) + 1;
	strcpy_s(_pcBufferToReceiveData, _iPacketSize, m_pcPacketData);
	//std::cout << "Server Received " << _pcBufferToReceiveData << "from " << ntohs(m_ClientAddress.sin_port) << std::endl;

}

void CServer::GetRemoteIPAddress(char *_pcSendersIP)
{
	char _temp[MAX_ADDRESS_LENGTH];
	int _iAddressLength;
	inet_ntop(AF_INET, &(m_ClientAddress.sin_addr), _temp, sizeof(_temp));
	_iAddressLength = strlen(_temp) + 1;
	strcpy_s(_pcSendersIP, _iAddressLength, _temp);
}

unsigned short CServer::GetRemotePort()
{
	return ntohs(m_ClientAddress.sin_port);
}
