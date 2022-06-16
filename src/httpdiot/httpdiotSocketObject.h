// MIT license
#pragma once
#ifndef __HTTPDIOTSKOBJ_H_
#define __HTTPDIOTSKOBJ_H_

#include <WS2tcpip.h>

namespace httpdiot
{
	class SocketObject
	{
		SOCKET m_socket = INVALID_SOCKET;
		sockaddr_in m_adr;

	public:
		SocketObject();
		~SocketObject();
		
		enum type
		{
			type_tcp,
			type_udp
		};

		bool Create(type);

		std::string adr;
		std::string hostname;
		int m_port;

		void Drop();
		void Disconnect();
		bool Bind(int port);

		bool Listen();

		// will allocate new object
		
		bool Accept(SocketObject& acceptSocket, int recvTimeO, int sendTimeO);
		//int  Poll(SocketObject& socket, int recvTimeO, int sendTimeO);

		bool SetSocketBlockingEnabled(bool);
		bool Connect(char* szServerAddress, int iPort, int recvTimeO, int sendTimeO);
		int Send(char* szBuffer, int iBufLen, int iFlags);
		int Send(unsigned char* szBuffer, int iBufLen, int iFlags);

		int Recv(char* szBuffer, int iBufLen, int iFlags);
		int Recv(unsigned char* szBuffer, int iBufLen, int iFlags);
	};

}

#endif
