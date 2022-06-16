// MIT license

#include "httpdiot.h"

using namespace httpdiot;

SocketObject::SocketObject(){}

SocketObject::~SocketObject()
{
}

bool SocketObject::Create(type t)
{
    assert(m_socket == INVALID_SOCKET);
    int _type = 0;
    switch (t)
    {
    case httpdiot::SocketObject::type_tcp:
        _type = SOCK_STREAM;
        break;
    case httpdiot::SocketObject::type_udp:
        _type = SOCK_DGRAM;
        break;
    }

    m_socket = socket(AF_INET, _type, 0);
    if (m_socket == INVALID_SOCKET)
    {
        ServLogWrite(ServerLogType::Error, "SocketObject::SocketObject Code %i\n", WSAGetLastError());
        return false;
    }
    return true;
}

void SocketObject::Drop()
{
    m_socket = INVALID_SOCKET;
}

void SocketObject::Disconnect()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

bool SocketObject::Bind(int port)
{
    sockaddr_in saServerAddress;
    memset(&saServerAddress, 0, sizeof(sockaddr_in));
    saServerAddress.sin_family = AF_INET;
    saServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    saServerAddress.sin_port = htons(port);
    if (bind(m_socket, (sockaddr*)&saServerAddress, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        ServLogWrite(ServerLogType::Error, "SocketObject::Bind:bind Code %i\n", WSAGetLastError());
        Disconnect();
        return false;
    }
    else
    {
        return true;
    }
}

bool SocketObject::Listen()
{
    if (listen(m_socket, 32) == SOCKET_ERROR)
    {
        ServLogWrite(ServerLogType::Error, "SocketObject::Listen:listen Code %i\n", WSAGetLastError());
        return false;
    }
    return true;
}

bool SocketObject::Accept(SocketObject& acceptSocket, int recvTimeO, int sendTimeO)
{
    sockaddr_in saClientAddress;
    int iClientSize = sizeof(sockaddr_in);
    
    acceptSocket.m_socket = accept(m_socket, (struct sockaddr*)&saClientAddress, &iClientSize);
    if (acceptSocket.m_socket == INVALID_SOCKET)
    {
        ServLogWrite(ServerLogType::Warning, "SocketObject::Accept:accept Code %i\n", WSAGetLastError());
        return false;
    }
    else
    {
        acceptSocket.m_adr = saClientAddress;
   //   ServLogWrite(ServerLogType::Info, "Client adr: %s:%d\n", inet_ntoa(saClientAddress.sin_addr), ntohs(saClientAddress.sin_port));

        char adrBuf[100];
        adrBuf[0] = 0;

        switch (saClientAddress.sin_family) {
        case AF_INET:
            inet_ntop(AF_INET, &saClientAddress.sin_addr, adrBuf, 100);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6*)&saClientAddress)->sin6_addr), adrBuf, 100);
            break;

        default:
            return NULL;
        }

        acceptSocket.adr = adrBuf;
        acceptSocket.m_port = ntohs(saClientAddress.sin_port);

        ServLogWrite(ServerLogType::Info, "Client adr: %s:%d\n", adrBuf, acceptSocket.m_port);

        char hostname[NI_MAXHOST];
        char servInfo[NI_MAXSERV];
        if (!getnameinfo(((SOCKADDR*)&saClientAddress), sizeof(saClientAddress), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV))
        {
            ServLogWrite(ServerLogType::Info, "Client host name: %s %s\n", hostname, servInfo);
            acceptSocket.hostname = hostname;
            //snprintf(acceptSocket.m_adrHostName, 0xff, "%s", hostname);
        }
        ServLogWrite(ServerLogType::Info, "SocketObject::Accept:accept Done\n");

        DWORD timeO = recvTimeO;
        if (setsockopt(acceptSocket.m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeO, sizeof(timeO)) == SOCKET_ERROR)
        {
            ServLogWrite(ServerLogType::Warning, "SocketObject::Accept:SOL_SOCKET, SO_RCVTIMEO Code %i\n", WSAGetLastError());
        }
        timeO = sendTimeO;
        if (setsockopt(acceptSocket.m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeO, sizeof(timeO)) == SOCKET_ERROR)
        {
            ServLogWrite(ServerLogType::Warning, "SocketObject::Accept:SOL_SOCKET, SO_SNDTIMEO Code %i\n", WSAGetLastError());
        }

        return true;
    }
}

//int SocketObject::Poll(SocketObject& socket, int recvTimeO, int sendTimeO)
//{
//    //WSAPOLLFD pollfd;
//    fd_set set;
//    FD_ZERO(&set);
//    FD_SET(socket.m_socket, &set);
//    
//    struct timeval timeout;
//    timeout.tv_sec = 1;
//    timeout.tv_usec = 0;
//
//    int result = select(socket.m_socket, &set, 0, 0, &timeout);
//
//    if (result > 0)
//    {
//        Accept(socket, recvTimeO, sendTimeO);
//    }
//
//    return result;
//}

//https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
/** Returns true on success, or false if there was an error */
bool SocketObject::SetSocketBlockingEnabled(bool blocking)
{
    if (m_socket < 0) return false;

#ifdef _WIN32
    unsigned long mode = blocking ? 0 : 1;
    return (ioctlsocket(m_socket, FIONBIO, &mode) == 0) ? true : false;
#else
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) return false;
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

bool SocketObject::Connect(char* szServerAddress, int iPort, int recvTimeO, int sendTimeO)
{
    struct sockaddr_in serv_addr;
    int err;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET)
    {
        ServLogWrite(ServerLogType::Error, "SocketObject::ConnectForTCP:socket Code %i\n", WSAGetLastError());
        return false;
    }
    memset(&serv_addr, 0, sizeof(sockaddr_in));
    serv_addr.sin_family = AF_INET;

    SetSocketBlockingEnabled(false);

    DWORD timeO = recvTimeO;
    if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeO, sizeof(timeO)) == SOCKET_ERROR)
        ServLogWrite(ServerLogType::Warning, "SocketObject::Accept:SOL_SOCKET, SO_RCVTIMEO Code %i\n", WSAGetLastError());
    timeO = sendTimeO;
    if (setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeO, sizeof(timeO)) == SOCKET_ERROR)
        ServLogWrite(ServerLogType::Warning, "SocketObject::Accept:SOL_SOCKET, SO_SNDTIMEO Code %i\n", WSAGetLastError());

    inet_pton(AF_INET, szServerAddress, &serv_addr.sin_addr.s_addr);

    serv_addr.sin_port = htons(iPort);

    err = connect(m_socket, (struct sockaddr*)&serv_addr, sizeof(sockaddr));
    /*if (err == SOCKET_ERROR)
    {
        ServLogWrite(ServerLogType::Error, "SocketObject::ConnectForTCP:connect Code %i\n", WSAGetLastError());
        Disconnect();
        return false;
    }*/
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(m_socket, &fdset);
    struct timeval tv;
    tv.tv_sec = 1;             /* 10 second timeout */
    tv.tv_usec = 0;

    int rv = select(m_socket + 1, NULL, &fdset, NULL, &tv);
    if ( rv == 1)
    {
        return true;
        //int so_error;
        //socklen_t len = sizeof so_error;
        //getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);
        //if (so_error == 0) {
        //    //printf("%s:%d is open\n", addr, port);
        //}
    }

    return false;
}

int SocketObject::Send(char* szBuffer, int iBufLen, int iFlags)
{
    return send(m_socket, szBuffer, iBufLen, iFlags);
}

int SocketObject::Send(unsigned char* szBuffer, int iBufLen, int iFlags)
{
    return send(m_socket, (char*)szBuffer, iBufLen, iFlags);
}

int SocketObject::Recv(char* szBuffer, int iBufLen, int iFlags)
{
    return recv(m_socket, szBuffer, iBufLen, iFlags);
}

int SocketObject::Recv(unsigned char* szBuffer, int iBufLen, int iFlags)
{
    return recv(m_socket, (char*)szBuffer, iBufLen, iFlags);
}
