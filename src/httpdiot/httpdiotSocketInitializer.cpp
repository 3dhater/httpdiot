// MIT license

#include "httpdiot.h"

#pragma comment(lib, "ws2_32.lib")

using namespace httpdiot;

SocketInitializer::SocketInitializer() 
{
    ServLogWrite(ServerLogType::Info, "Init WinSock...\n");
    WSADATA wsaData;
    WORD res = WSAStartup(MAKEWORD(2, 0), &wsaData);
    if (res)
    {
        m_good = false;
        ServLogWrite(ServerLogType::Error, "WSAStartup: Code %i\n", res);

        char buf[100];
        snprintf(buf, 100, "WSAStartup error. Code %i", res);
        MessageBoxA(0, buf, "Error", MB_OK);
    }
    else
    {
        m_isReady = true;
    }
}
SocketInitializer::~SocketInitializer() 
{
    if (m_isReady)
    {
        ServLogWrite(ServerLogType::Info, "Shutdown WinSock...\n");
        if (WSACleanup())
        {
            ServLogWrite(ServerLogType::Warning, "WSACleanup: Code %i\n", WSAGetLastError());
        }
    }
}

