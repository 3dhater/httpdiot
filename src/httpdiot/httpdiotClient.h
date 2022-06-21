// MIT license
#pragma once
#ifndef __HTTPDIOT_CLIENT_H_
#define __HTTPDIOT_CLIENT_H_

namespace httpdiot
{
	struct HTTPRequest;

	class Client
	{
		Timer m_connectionTimer;
		bool m_run = true;

		void(Client::* m_action)() = 0;
		void action_sleep();
		void action_idle();
		void action_disconnect();

		char m_receiveBuffer[0xfff];
		char m_logbuffer[0x60];

	public:
		Client();
		~Client();

		ThreadContext m_threadContextMain;
		std::thread* m_thread = 0;
		std::thread::id m_threadID;
		SocketObject m_socket;

		bool Update();
		void Disconnect();
		void Sleep();

		bool HTTPGetRequest(const char* buffer, HTTPRequest*);
		void HTTPProcess(SocketObject* sk, const char* buffer, size_t len);
		bool ReadTextFile(std::string* path, std::string* str);

		void LogWrite(ServerLogType lt, const char* fmt, ...);
		void VaLogWrite(ServerLogType lt, const char* fmt, va_list args);
	};

	void thread_client(ThreadContext* c);
}

#endif
