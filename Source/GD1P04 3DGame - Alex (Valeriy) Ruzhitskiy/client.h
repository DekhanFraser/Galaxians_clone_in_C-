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

#ifndef __CLIENT_H__
#define __CLIENT_H__

// Library Includes

// Local Includes
#include "networkentity.h"

// Types

// Constants


//Forward Declaration
class CSocket;

class CClient : public INetworkEntity
{
public:
	// Default Constructors/Destructors
	CClient();
	~CClient();

	// Methods
	
	virtual bool Initialise(); //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(const char* _pcDataToSend);
	virtual void ReceiveData(char* _pcBufferToReceiveData);
	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();

	void GetPacketData(char* _pcLocalBuffer);
private:
	//A buffer to contain all packet data for the client
	char* m_pcPacketData;
	//A client has a socket object to create the UDP socket at its end.
	CSocket* m_pClientSocket;
	// A Sockaddress structure which will have the details of the server 
	sockaddr_in m_ServerSocketAddress;
};

#endif