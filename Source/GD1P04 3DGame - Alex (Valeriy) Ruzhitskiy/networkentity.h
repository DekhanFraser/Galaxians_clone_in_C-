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

#ifndef __NETWORKENTITY_H__
#define __NETWORKENTITY_H__


class INetworkEntity
{
public:
	virtual bool Initialise() = 0; //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(const char* _pcDataToSend) = 0;
	virtual void ReceiveData(char* _pcBufferToReceiveData) = 0;
	virtual void GetRemoteIPAddress(char *_pcSendersIP) = 0;
	virtual unsigned short GetRemotePort() = 0;
};
#endif 