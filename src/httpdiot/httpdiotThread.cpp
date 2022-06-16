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
			ClientInfo ci;
			ci.m_socket = newClientSocket;
			server->m_clientList.push_back(ci);
		}

		if (c->m_state == ThreadState::NeedToStop)
			break;
	}
	c->m_state = ThreadState::Dead;
}

void httpdiot::thread_work(ThreadContext* c)
{
	Server* server = (Server*)c->m_userData;

	c->m_state = ThreadState::Live;
	while (1)
	{
		Sleep(1);

		if (server->m_clientList.m_head)
		{
			auto curr = server->m_clientList.m_head;
			auto last = server->m_clientList.m_head->m_left;
			while (true)
			{
				auto next = curr->m_right;

				server->ServClient(&curr->m_data);

				server->m_clientList.erase_by_node(curr);

				if (curr == last)
					break;

				curr = next;
			}
		}

		if (c->m_state == ThreadState::NeedToStop)
			break;
	}

	if (server->m_clientList.m_head)
	{
		auto curr = server->m_clientList.m_head;
		auto last = server->m_clientList.m_head->m_left;
		while (true)
		{
			curr->m_data.m_socket.Disconnect();

			if (curr == last)
				break;

			curr = curr->m_right;
		}
	}

	c->m_state = ThreadState::Dead;
}

void Server::ServClient(ClientInfo* ci)
{
	int rv = ci->m_socket.Recv(m_receiveBuffer, 0xffff, 0);
	if (rv > 0)
	{
		HTTPProcess(&ci->m_socket, (char*)m_receiveBuffer, rv);
	}
}
