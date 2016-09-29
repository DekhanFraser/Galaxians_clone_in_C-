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

//Library Inludes
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>

//Local Includes 
#include "network.h"

//This Include
#include "socket.h"


CSocket::CSocket()
	:m_hSocket(0)
{
	ZeroMemory(&m_SocketAddress, sizeof(m_SocketAddress));
}

CSocket::~CSocket()
{
	closesocket(m_hSocket);
}

//Implicit Assumption: We are creating only UDP sockets.....
bool CSocket::Initialise(char* _pcIPAddress, unsigned short _usPortNumber)
{
	int _iError;
	//Create a UDP socket 
	m_hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_hSocket)
	{
		_iError = WSAGetLastError();
		ErrorRoutines::PrintWSAErrorInfo(_iError);
		std::cout << "unable to create socket\n";
		return false;
	}
	else
	{
		std::cout << "Successfully created the socket" << std::endl;
	}

	// now bind the socket to the address and port number supplied.
	m_SocketAddress.sin_family = AF_INET;
	m_SocketAddress.sin_port = htons(_usPortNumber);
	inet_pton(AF_INET, _pcIPAddress, &m_SocketAddress.sin_addr);
	m_SocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	if (0 != bind(m_hSocket, reinterpret_cast<sockaddr*>(&m_SocketAddress), sizeof(m_SocketAddress)))
	{
		// error
		closesocket(m_hSocket);
		std::cout << "unable to bind socket to " << _pcIPAddress << ":" << _usPortNumber << " - is something else running ? " << std::endl;
		return false;
	}
	else
	{
		std::cout << "Successfully bound the socket " << "to " << _pcIPAddress << ":" << _usPortNumber << std::endl;
	}
	return true;
}

SOCKET CSocket::GetSocketHandle()
{
	return m_hSocket;
}