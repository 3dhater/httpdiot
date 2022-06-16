// MIT license

#include "httpdiot.h"

#include <iostream>
#include <string>

using namespace httpdiot;

std::string g_getRequestStringBuffer;

// `GET` `/` `/favicon.ico` `HTTP/1.1` `Host:`
// Only for current line.
const char* HTTPGetRequestToken(const char* buffer, std::string* str, bool tolow = false)
{
	while (*buffer)
	{
		if (*buffer == '\r' || *buffer == '\n')
			return buffer;

		if (isgraph(*buffer))
			str->push_back(tolow ? (char)tolower(*buffer): *buffer);
		else
		{
			// ' ' or '\t'
			while (*buffer)
			{
				++buffer;

				if (*buffer == '\r' || *buffer == '\n')
					return buffer;
				else if (isgraph(*buffer))
					return buffer;
			}
		}

		++buffer;
	}
	return buffer;
}

const char* HTTPSkipLine(const char* buffer)
{
	while (*buffer)
	{
		if (*buffer == '\n')
		{
			++buffer;
			return buffer;
		}

		++buffer;
	}
	return buffer;
}

const char* HTTPGetRestLine(const char* buffer, std::string* str)
{
	str->clear();
	while (*buffer)
	{
		if (isgraph(*buffer))
			str->push_back(*buffer);
		else if(*buffer == ' ')
			str->push_back(*buffer);

		if (*buffer == '\n')
			return buffer;

		++buffer;
	}
	return buffer;
}

bool HTTPGetHeaders(const char* buffer, HTTPRequest* req)
{
	buffer = HTTPSkipLine(buffer);

	while (*buffer)
	{
		g_getRequestStringBuffer.clear();
		buffer = HTTPGetRequestToken(buffer, &g_getRequestStringBuffer, true);
		if (!g_getRequestStringBuffer.size())
			break;



		switch (g_getRequestStringBuffer[0])
		{
		case 'h':
			if (strcmp(g_getRequestStringBuffer.c_str(), "host:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->host);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'u':
			if (strcmp(g_getRequestStringBuffer.c_str(), "user-agent:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->user_agent);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "upgrade-insecure-requests:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->upgrade_insecure_requests);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'a':
			if (strcmp(g_getRequestStringBuffer.c_str(), "accept:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "accept-encoding:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept_encoding);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "accept-language:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept_language);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'c':
			if (strcmp(g_getRequestStringBuffer.c_str(), "connection:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->connection);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "content-type:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->content_type);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "content-length:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->content_length);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "cache-control:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->cache_control);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'r':
			if (strcmp(g_getRequestStringBuffer.c_str(), "referer:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->referer);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'o':
			if (strcmp(g_getRequestStringBuffer.c_str(), "origin:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->origin);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'f':
			if (strcmp(g_getRequestStringBuffer.c_str(), "from:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->from);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'd':
			if (strcmp(g_getRequestStringBuffer.c_str(), "dnt:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->dnt);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 's':
			if (strcmp(g_getRequestStringBuffer.c_str(), "sec-ch-ua:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_ch_ua);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-ch-ua-mobile:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_ch_ua_mobile);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-ch-ua-platform:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_ch_ua_platform);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-fetch-site:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_fetch_site);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-fetch-mode:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_fetch_mode);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-fetch-dest:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_fetch_dest);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "sec-fetch-user:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->sec_fetch_user);
			else
				printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		default:
			printf("UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		}

		++buffer;
	}
	return true;
}

bool Server::HTTPGetRequest(const char* buffer, HTTPRequest* req)
{
	g_getRequestStringBuffer.clear();
	buffer = HTTPGetRequestToken(buffer, &g_getRequestStringBuffer);
	if (!g_getRequestStringBuffer.size())
		return false;

	if (strcmp(g_getRequestStringBuffer.c_str(), "GET") == 0)
	{
		req->method = HTTPRequestMethod::m_GET;

		g_getRequestStringBuffer.clear();
		buffer = HTTPGetRequestToken(buffer, &g_getRequestStringBuffer);
		if(!g_getRequestStringBuffer.size())
			return false;

		if (g_getRequestStringBuffer[0] == '/')
		{
			req->path = g_getRequestStringBuffer;

			g_getRequestStringBuffer.clear();
			buffer = HTTPGetRequestToken(buffer, &g_getRequestStringBuffer);
			if (!g_getRequestStringBuffer.size())
			{
				req->version = HTTPVersion::HTTP_0_9;
				return true;
			}
			else
			{
				if (strcmp("HTTP/1.0", g_getRequestStringBuffer.c_str()) == 0)
				{
					req->version = HTTPVersion::HTTP_1_0;
					return HTTPGetHeaders(buffer, req);
				}
				else if (strcmp("HTTP/1.1", g_getRequestStringBuffer.c_str()) == 0)
				{
					req->version = HTTPVersion::HTTP_1_1;
					return HTTPGetHeaders(buffer, req);
				}
				else if (strcmp("HTTP/2", g_getRequestStringBuffer.c_str()) == 0)
				{
					req->version = HTTPVersion::HTTP_2;
					return HTTPGetHeaders(buffer, req);
				}
				else if (strcmp("HTTP/3", g_getRequestStringBuffer.c_str()) == 0)
				{
					req->version = HTTPVersion::HTTP_3;
					return HTTPGetHeaders(buffer, req);
				}
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void Server::HTTPProcess(SocketObject* sk, const char* buffer, size_t len)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	for (int i = 0; i < len; ++i)
	{
		unsigned char c = (unsigned char)buffer[i];
		if (isgraph(c))
			printf("%c", buffer[i]);

		if (isspace(c))
			printf("%c", buffer[i]);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	//snprintf((char*)http_send_buffer, 0xffff, "HTTP/1.1 200\r\n\r\n<p>404</p>");
	//int rv = sk->Send(http_send_buffer, 27, 0);

	HTTPRequest req;
	if (HTTPGetRequest(buffer, &req))
	{
		/*printf("REQUEST!\n");
		printf("Accept: %s\n", req.accept.data());
		printf("Accept-Encoding: %s\n", req.accept_encoding.data());
		printf("Accept-Language: %s\n", req.accept_language.data());
		printf("Connection: %s\n", req.connection.data());
		printf("Content-Length: %s\n", req.content_length.data());
		printf("From: %s\n", req.from.data());
		printf("Host: %s\n", req.host.data());
		printf("Origin: %s\n", req.origin.data());
		printf("Referer: %s\n", req.referer.data());
		printf("Upgrade-Insecure-Request: %s\n", req.upgrade_insecure_requests.data());
		printf("User-Agent: %s\n", req.user_agent.data());
		printf("Cache-Control: %s\n", req.cache_control.data());
		printf("DNT: %s\n", req.dnt.data());*/

		if (req.host.size())
		{
			//printf("REQUEST! : host [%s]\n", req.host.c_str());

			auto it = m_websites.find(req.host);
			if (it != m_websites.end())
			{
			//	printf("WEBSITE! : path [%s]\n", it->second->m_path.c_str());

				std::string s = it->second->m_path;
				s += "root\\";
				s += "index.html";

				FILE* f = 0;
				fopen_s(&f, s.c_str(), "rb");
				if (f)
				{
					fseek(f, 0, SEEK_END);
					long fsz = ftell(f);
					fseek(f, 0, SEEK_SET);
					
			//		printf("FILE!\n");

					if (fsz)
					{
						if (fsz > 0xffff)
							fsz = 0xffff;
						
			//			printf("SEND!\n");

						fread(m_sendBuffer, fsz, 1, f);
						int rv = sk->Send(m_sendBuffer, fsz, 0);
					}

					fclose(f);
				}
			}
		}

//		snprintf((char*)m_sendBuffer, 0xffff, "HTTP/1.1 200\r\n\r\n<p>404</p>");
//		int rv = sk->Send(m_sendBuffer, 27, 0);
	}

	//snprintf((char*)http_send_buffer, 0xffff, "%s");

	/*static std::string str;
	str.clear();
	str += "Adr: ";
	str += sk->m_adrCharBuf;
	str += "\r\n";
	str += "Host name: ";
	str += sk->m_adrHostName;
	str += "\r\n";
	for (int i = 0; i < len; ++i)
	{
		unsigned char c = (unsigned char)buffer[i];
		if (isgraph(c))
			str += buffer[i];

		if (isspace(c))
			str += buffer[i];
	}
	int rv = sk->Send((char*)str.data(), str.size(), 0);*/

	sk->Disconnect();
}

