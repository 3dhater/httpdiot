// MIT license
#pragma once
#ifndef __HTTPDIOT_H_
#define __HTTPDIOT_H_

#include <stdlib.h>
#include <stdio.h>
//#include <signal.h>
#include <stdarg.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

//#define HTTPDIOT_USE_OPENSSL

#include "httpdiotLog.h"
#include "httpdiotSocketInitializer.h"
#include "httpdiotSocketObject.h"
#include "httpdiotThread.h"
#include "httpdiotList.h"
#include "HTTP.h"

#ifdef HTTPDIOT_USE_OPENSSL
#include "httpdiotCrypto.h"
#endif

namespace httpdiot
{
	struct ClientInfo
	{
		SocketObject m_socket;

	};

	class Server
	{
		bool m_isActive = false;
		SocketObject m_serverSocket;

		ThreadContext m_threadContext_accept;
		std::thread* m_thread_accept = 0;

		ThreadContext m_threadContext_work;
		std::thread* m_thread_work = 0;

		unsigned char m_receiveBuffer[0xffff];
		unsigned char m_sendBuffer[0xffff];

#ifdef HTTPDIOT_USE_OPENSSL
		Crypto m_crypto;
#endif

		int m_port = 80;

		httpdiot::List<ClientInfo> m_clientList;

		friend void httpdiot::thread_work(ThreadContext* c);
		friend void httpdiot::thread_accept(ThreadContext* c);

	public:
		Server();
		~Server();

		bool Start();
		void Run();
		void Stop();

		void ServClient(ClientInfo*);
		
		bool HTTPGetRequest(const char* buffer, HTTPRequest*);
		void HTTPProcess(SocketObject* sk, const char* buffer, size_t len);
	};

}

#endif
