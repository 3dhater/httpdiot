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
#include <map>

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
#include "httpdiotTimer.h"
#include "httpdiotClient.h"
#include "HTTP.h"

#ifdef HTTPDIOT_USE_OPENSSL
#include "httpdiotCrypto.h"
#endif

namespace httpdiot
{
	constexpr int GetMaxClients() { return 1000; }

	struct WebsiteInfo
	{
		std::string m_path;
	};

	class Server
	{
		bool m_isActive = false;
		SocketObject m_serverSocket;

		ThreadContext m_threadContext_accept;
		std::thread* m_thread_accept = 0;

		std::map<std::string, WebsiteInfo*> m_websites;

		//unsigned char m_receiveBuffer[0xffff];
		//unsigned char m_sendBuffer[0xffff];

#ifdef HTTPDIOT_USE_OPENSSL
		Crypto m_crypto;
#endif

		int m_port = 80;

		httpdiot::List<Client*> m_clientList;
		
		friend class Client;
		friend void httpdiot::thread_accept(ThreadContext* c);

	public:
		Server();
		~Server();

		bool Start(int argc, char** argv);
		void Run();
		void Stop();

	};

}

#endif
