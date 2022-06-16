// MIT license
#pragma once
#ifndef __HTTPDIOT_SKINIT_H_
#define __HTTPDIOT_SKINIT_H_

#include <WS2tcpip.h>

namespace httpdiot
{

	class SocketInitializer
	{
		bool m_isReady = false;
	public:
		SocketInitializer();
		~SocketInitializer();

		bool m_good = true;
	};

}

#endif
