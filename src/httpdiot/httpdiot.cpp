// MIT license

#include "httpdiot.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <signal.h>
#include <mutex>

using namespace httpdiot;

Server* g_server = nullptr;

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

Server::Server() 
{
	g_server = this;
}

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
					
					WebsiteInfo* wi = new WebsiteInfo;
					wi->m_path = "..\\www\\";
					wi->m_path += str;
					wi->m_path += "\\";

					if (!std::filesystem::exists(wi->m_path.c_str()))
					{
						ServLogWrite(ServerLogType::Error, "Bad -w website_name\n");
					}
					else
					{
						std::string hostsPath = wi->m_path;
						hostsPath += "hosts.txt";

						if (!std::filesystem::exists(hostsPath.c_str()))
						{
							ServLogWrite(ServerLogType::Error, "File %s not exist\n", hostsPath.c_str());
						}
						else
						{
							std::ifstream in(hostsPath.c_str());
							for (std::string line; std::getline(in, line); )
							{
								ServLogWrite(ServerLogType::Info, "+ host [%s]\n", line.c_str());
								m_websites[line] = wi;
							}
						}
					}
				}
			}
		}
	}

	if (!m_websites.size())
	{
		ServLogWrite(ServerLogType::Info, "How to use: httpdiot.exe -w [websitename_in_www_folder]\n");
		ServLogWrite(ServerLogType::Info, "\thttpdiot.exe -w test1\n");
		ServLogWrite(ServerLogType::Info, "\thttpdiot.exe -w test1 -w test2\n");
		ServLogWrite(ServerLogType::Info, "\n");
		return false;
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

			m_thread_accept = new std::thread(httpdiot::thread_accept, &m_threadContext_accept);

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
		if (m_thread_accept)
		{
			m_threadContext_accept.m_state = ThreadState::NeedToStop;
			m_serverSocket.Disconnect();
			if (m_thread_accept->joinable())
				m_thread_accept->join();
			delete m_thread_accept;
		}

		if (m_clientList.m_head)
		{
			auto curr = m_clientList.m_head;
			auto last = m_clientList.m_head->m_left;
			while (true)
			{
				curr->m_data->m_threadContextMain.m_state = ThreadState::NeedToStop;
				delete curr->m_data;

				if (curr == last)
					break;

				curr = curr->m_right;
			}
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

void int_handler(int sig)
{
	ServLogWrite(ServerLogType::Warning, "SIGINT\n");
	g_server->Stop();
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
//	std::filesystem::path exePath = argv[0];
//	std::filesystem::path parentPath = exePath.parent_path();
//	SetCurrentDirectory(L"D:\\Code\\httpdiot\\bin32\\");

	httpdiot::ServLogSetOnWrite(httpdiotLogWriter);

	signal(SIGINT, int_handler);

	if (std::filesystem::exists("./logs"))
		std::filesystem::remove_all("./logs");
	std::filesystem::create_directories("./logs");

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


