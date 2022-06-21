// MIT license

#include "httpdiot.h"
#include <mutex>

using namespace httpdiot;

ServerLog g_log;

void httpdiot::ServLogWrite(ServerLogType lt, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	httpdiot::VServLogWrite(lt, fmt, va);
	va_end(va);
}

void httpdiot::VServLogWrite(ServerLogType lt, const char* fmt, va_list args)
{
	vsnprintf(g_log.m_buffer, ServerLogBufferSize, fmt, args);
	g_log.OnWrite(lt, g_log.m_buffer);
}

ServerLog::ServerLog()
{
}

ServerLog::~ServerLog()
{
}

void httpdiot::ServLogSetOnWrite(void(*f)(ServerLogType lt, const char* message))
{
	g_log.OnWrite = f;
}
