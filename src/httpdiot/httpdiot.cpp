// MIT license

#include "httpdiot.h"

#include <iostream>
#include <filesystem>

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

bool Server::Start(int argc, char** argv)
{
	m_isActive = false;

	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			char* str = argv[i];
			if (strcmp(str, "-w") == 0)
			{
				++i;
				if (i < argc)
				{
					str = argv[i];
					m_website.m_path = str;
				}
			}
		}
	}

	if (m_website.m_path.size())
	{
		if (!std::filesystem::exists(m_website.m_path.c_str()))
		{
			ServLogWrite(ServerLogType::Error, "Bad -w website_name\n");
			return false;
		}
	}
	else
	{
		ServLogWrite(ServerLogType::Info, "How to use: httpdiot.exe -w [relative_path_or_absolute_path to www\\websitename]\n");
		ServLogWrite(ServerLogType::Info, "\thttpdiot.exe -w ..\\www\\test1\n");
		ServLogWrite(ServerLogType::Info, "\thttpdiot.exe -w C:\\httpdiot\\www\\test2\n");
		ServLogWrite(ServerLogType::Info, "\n");
		return false;
	}

	if (m_website.m_path[m_website.m_path.size() - 1] != '/'
		&& m_website.m_path[m_website.m_path.size() - 1] != '\\')
	{
		m_website.m_path.push_back('\\');
	}

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
			m_threadContext_accept.m_userData = this;
			m_threadContext_work.m_userData = this;

			m_thread_accept = new std::thread(httpdiot::thread_accept, &m_threadContext_accept);
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
			if (m_thread_work->joinable())
				m_thread_work->join();
			delete m_thread_work;
		}

		if (m_thread_accept)
		{
			m_threadContext_accept.m_state = ThreadState::NeedToStop;
			m_serverSocket.Disconnect();
			if (m_thread_accept->joinable())
				m_thread_accept->join();
			delete m_thread_accept;
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
		if (server.Start(argc, argv))
		{
			server.Run();
			server.Stop();

			Sleep(2000);
			return EXIT_SUCCESS;
		}
	}
	Sleep(2000);
	return EXIT_FAILURE;
}
