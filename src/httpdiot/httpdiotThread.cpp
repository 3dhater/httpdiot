// MIT license

#include "httpdiot.h"

using namespace httpdiot;

void httpdiot::thread_accept(ThreadContext* c)
{
	Server* server = (Server*)c->m_userData;
	
	SocketObject newClientSocket;

	c->m_state = ThreadState::Live;

	while (1)
	{
		Sleep(1);
		if (server->m_serverSocket.Accept(newClientSocket, 10, 10))
		{
			if (server->m_clientList.size() > GetMaxClients())
			{
				newClientSocket.Disconnect();
			}
			else
			{
				Client* newClient = new Client;
				//ci.m_connectionTimer.start();
				newClient->m_socket = newClientSocket;
				newClient->m_threadContextMain.m_userData = newClient;
				newClient->m_thread = new std::thread(thread_client, &newClient->m_threadContextMain);
				newClient->m_threadID = newClient->m_thread->get_id();

				server->m_clientList.push_back(newClient);
			}


			Sleep(10);
		}

		if (c->m_state == ThreadState::NeedToStop)
			break;
	}

	c->m_state = ThreadState::Dead;
}

//void Server::ServClient(ClientInfo* ci)
//{
//	int rv = ci->m_socket.Recv(m_receiveBuffer, 0xffff, 0);
//	if (rv > 0)
//	{
//		ci->m_connectionTimer.start();
//		HTTPProcess(&ci->m_socket, (char*)m_receiveBuffer, rv);
//	}
//}
