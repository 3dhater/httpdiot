// MIT license

#include "httpdiot.h"

#include <iostream>

using namespace httpdiot;

void httpdiotLogWriter(ServerLogType lt, const char* message)
{
	switch (lt)
	{
	case httpdiot::ServerLogType::Error:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Error: ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	case httpdiot::ServerLogType::Warning:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("Warning: ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	case httpdiot::ServerLogType::Info:
		printf("Info: ");
		break;
	}
	printf("%s", message);
}

Server::Server() {}
Server::~Server() {}

bool Server::Start()
{
	ServLogWrite(ServerLogType::Info, "`Start()`\n");

	if (!m_serverSocket.Create(SocketObject::type_tcp))
	{
		ServLogWrite(ServerLogType::Error, "!m_serverSocket.Create()\n");
		return false;
	}

	ServLogWrite(ServerLogType::Info, "Init port %i\n", m_port);
	if (m_serverSocket.Bind(m_port))
	{
		ServLogWrite(ServerLogType::Info, "Listen\n");
		if (m_serverSocket.Listen())
		{
			m_thread_work = new std::thread(httpdiot::thread_work, &m_threadContext_work);

			m_isActive = true;
		}
		else
		{
			ServLogWrite(ServerLogType::Error, "Failed to listen\n");
		}
	}
	else
	{
		ServLogWrite(ServerLogType::Error, "Failed to bind\n");
	}

	return m_isActive;
}

void Server::Stop()
{
	ServLogWrite(ServerLogType::Info, "`Stop()`\n");
	if (m_isActive)
	{
		if (m_thread_work)
		{
			m_threadContext_work.m_state = ThreadState::NeedToStop;
			m_thread_work->detach();
			delete m_thread_work;
		}

		m_isActive = false;
	}
	else
	{
		ServLogWrite(ServerLogType::Warning, "Stop(), m_isActive is false\n");
	}
}

void Server::Run()
{
	std::string cmd;
	while (1)
	{
		Sleep(1);

		std::cin >> cmd;
		if (strcmp(cmd.c_str(), "quit") == 0)
		{
			ServLogWrite(ServerLogType::Info, "`quit` command...\n");
			break;
		}

		if (strcmp(cmd.c_str(), "cls") == 0)
		{
			system("cls");
		}
	}
}

int main(int argc, char* argv[])
{
	httpdiot::ServLogSetOnWrite(httpdiotLogWriter);

	httpdiot::SocketInitializer soketInitializer;
	if (soketInitializer.m_good)
	{
		Server server;
		if (server.Start())
		{
			server.Run();
			server.Stop();

			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}
