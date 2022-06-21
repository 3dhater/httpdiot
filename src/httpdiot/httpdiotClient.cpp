// MIT license

#include "httpdiot.h"

using namespace httpdiot;

Client::Client()
{
	m_action = &Client::action_idle;
}

Client::~Client()
{
	if (m_thread)
	{
		if (m_thread->joinable())
			m_thread->join();
		delete m_thread;
	}
}

void Client::LogWrite(ServerLogType lt, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	VaLogWrite(lt, fmt, va);
	va_end(va);
}

void Client::VaLogWrite(ServerLogType lt, const char* fmt, va_list args)
{
	vsnprintf(m_logbuffer, 0x60, fmt, args);
	char lognamebuf[30];
	snprintf(lognamebuf, 30, "logs/%i.txt", m_threadID);
	FILE* f = fopen(lognamebuf, "a");
	if (f)
	{
		fwrite(m_logbuffer, 1, strlen(m_logbuffer), f);
		fclose(f);
	}
}

void Client::Disconnect()
{
	m_action = &Client::action_disconnect;
}

void Client::Sleep()
{
	m_action = &Client::action_sleep;
}

bool Client::Update()
{
	::Sleep(1);
	
	(this->*m_action)();

	if (m_threadContextMain.m_state == ThreadState::NeedToStop)
	{
		m_socket.Disconnect();
		m_run = false;
	}

	return m_run;
}

void Client::action_sleep()
{
}

void Client::action_idle()
{
	int rv = m_socket.Recv(m_receiveBuffer, 0xfff, 0);
	if (rv > 0)
	{
		//m_connectionTimer.start();

		HTTPProcess(&m_socket, (char*)m_receiveBuffer, rv);
	}
}

void Client::action_disconnect()
{
	m_socket.Disconnect();
	m_run = false;
}

void httpdiot::thread_client(ThreadContext* c)
{
	Client* client = (Client*)c->m_userData;
	c->m_state = ThreadState::Live;
	while (client->Update()) {}
	c->m_state = ThreadState::Dead;
}

