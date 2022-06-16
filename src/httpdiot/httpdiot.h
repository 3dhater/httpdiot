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

#ifdef HTTPDIOT_USE_OPENSSL
#include "httpdiotCrypto.h"
#endif

namespace httpdiot
{
	class Server
	{
		bool m_isActive = false;
		SocketObject m_serverSocket;

		ThreadContext m_threadContext_work;
		std::thread* m_thread_work = 0;

#ifdef HTTPDIOT_USE_OPENSSL
		Crypto m_crypto;
#endif

		int m_port = 80;

		friend void httpdiot::thread_work(ThreadContext* c);

	public:
		Server();
		~Server();

		bool Start();
		void Run();
		void Stop();
	};

}

#endif
