// MIT license
#pragma once
#ifndef __HTTPDIOT_LOG_H_
#define __HTTPDIOT_LOG_H_

namespace httpdiot
{

	enum class ServerLogType
	{
		Error,
		Warning,
		Info
	};

	void ServLogWrite(ServerLogType lt, const char* fmt, ...);
	void VServLogWrite(ServerLogType lt, const char* fmt, va_list args);
	void ServLogSetOnWrite(void(*)(ServerLogType lt, const char* message));

	constexpr size_t ServerLogBufferSize = 1024 * 10;

	class ServerLog
	{
	public:
		ServerLog();
		~ServerLog();

		char m_buffer[ServerLogBufferSize];

		void (*OnWrite)(ServerLogType lt, const char* message);
	};

}

#endif
