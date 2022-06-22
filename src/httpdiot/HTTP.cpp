// MIT license

#include "httpdiot.h"

#include <iostream>
#include <string>
#include <filesystem>

using namespace httpdiot;

std::string g_getRequestStringBuffer;
extern Server* g_server;

// `GET` `/` `/favicon.ico` `HTTP/1.1` `Host:`
// Only for current line.
const char* HTTPGetRequestToken(const char* buffer, std::string* str, bool tolow = false)
{
	while (*buffer)
	{
		if (*buffer == '\r' || *buffer == '\n')
			return buffer;

		if (isgraph((unsigned char)*buffer))
			str->push_back(tolow ? (char)tolower((unsigned char)*buffer): *buffer);
		else
		{
			// ' ' or '\t'
			while (*buffer)
			{
				++buffer;

				if (*buffer == '\r' || *buffer == '\n')
					return buffer;
				else if (isgraph((unsigned char)*buffer))
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
		if (isgraph((unsigned char)*buffer))
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
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'u':
			if (strcmp(g_getRequestStringBuffer.c_str(), "user-agent:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->user_agent);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "upgrade-insecure-requests:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->upgrade_insecure_requests);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'a':
			if (strcmp(g_getRequestStringBuffer.c_str(), "accept:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "accept-encoding:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept_encoding);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "accept-language:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->accept_language);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
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
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'r':
			if (strcmp(g_getRequestStringBuffer.c_str(), "referer:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->referer);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "range:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->range);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'o':
			if (strcmp(g_getRequestStringBuffer.c_str(), "origin:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->origin);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'f':
			if (strcmp(g_getRequestStringBuffer.c_str(), "from:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->from);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'd':
			if (strcmp(g_getRequestStringBuffer.c_str(), "dnt:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->dnt);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		case 'i':
			if (strcmp(g_getRequestStringBuffer.c_str(), "if-none-match:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->if_none_match);
			else if (strcmp(g_getRequestStringBuffer.c_str(), "if-modified-since:") == 0)
				buffer = HTTPGetRestLine(buffer, &req->if_modified_since);
			else
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
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
				ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		default:
			ServLogWrite(httpdiot::ServerLogType::text, "UNSUPPORTED HEADER: %s\n", g_getRequestStringBuffer.c_str());
			break;
		}

		++buffer;
	}
	return true;
}

bool Client::HTTPGetRequest(const char* buffer, HTTPRequest* req)
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

//void Server::RESPONSEIndex(WebsiteInfo* wi)
//{
	/*static std::string s;
	s.clear();
	s = wi->m_path;
	s += "root\\index.html";
	if()*/
//}
bool Client::ReadTextFile(std::string* path, std::string* str)
{
	str->clear();

	char buf[21];
	FILE* f = 0;
	fopen_s(&f, path->c_str(), "rb");
	if (f)
	{
		while (1)
		{
			size_t rn = fread(buf, 1, 20, f);
			if (!rn)
				break;

			buf[rn] = 0;

			str->append(buf);
		}
		fclose(f);
	}
	return true;
}

void Client::HTTPProcess(SocketObject* sk, const char* buffer, size_t len)
{
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	for (int i = 0; i < len; ++i)
	{
		unsigned char c = (unsigned char)buffer[i];
		
		//char txt[2] = { c, 0 };
		LogWrite(httpdiot::ServerLogType::text, "%c", c);
	//	printf("%c", c);

		//if (isgraph(c))
			//printf("%c", buffer[i]);

		//if (isspace(c))
			//printf("%c", buffer[i]);
	}
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

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

		static std::string sendStr;
		static std::string headerStr;
		static std::string fileStr;
		sendStr.clear();
		headerStr.clear();
		fileStr.clear();

		if (req.host.size())
		{
			//printf("REQUEST! : host [%s]\n", req.host.c_str());

			auto it = g_server->m_websites.find(req.host);
			if (it != g_server->m_websites.end())
			{
				LogWrite(httpdiot::ServerLogType::text, "WEBSITE! : path [%s]\n", it->second->m_path.c_str());
				LogWrite(httpdiot::ServerLogType::text, "req path! : [%s]\n", req.path.c_str());

				//std::string s = it->second->m_path;
				//s += "root\\";
				static std::string filePath;
				filePath = it->second->m_path;
				filePath += "root";
				//printf("filePath! : [%s]\n", filePath.c_str());

				int err = 501;

				if (strcmp(req.path.c_str(), "/") == 0)
				{
					filePath += "\\index.html";
				}
				else
				{
					filePath += req.path;
				}
				//printf("filePath! : [%s]\n", filePath.c_str());

				bool isTextFile = true;

				if (std::filesystem::exists(filePath))
				{
					LogWrite(httpdiot::ServerLogType::text, "exist : [%s]\n", filePath.c_str());

					std::filesystem::path pth(filePath);
					auto ext = pth.extension();
					if (ext == ".html"
						|| ext == ".xml"
						|| ext == ".css"
						|| ext == ".txt"
						|| ext == ".h"
						|| ext == ".c"
						|| ext == ".js"
						|| ext == ".cpp")
					{
						err = 0;
						LogWrite(httpdiot::ServerLogType::text, "read file: [%s]\n", filePath.c_str());

						ReadTextFile(&filePath, &fileStr);
					}
					else
					{
						err = 0;
						isTextFile = false;
						LogWrite(httpdiot::ServerLogType::text, "as binary: [%s]\n", filePath.c_str());
						//err = 404;
					}
				}
				else
				{
					LogWrite(httpdiot::ServerLogType::text, "not exist : [%s]\n", filePath.c_str());
					err = 404;
				}

				switch (req.version)
				{
				case HTTPVersion::HTTP_1_0:
					headerStr += "HTTP/1.0";
					break;
				case HTTPVersion::HTTP_1_1:
					headerStr += "HTTP/1.1";
					break;
				case HTTPVersion::HTTP_2:
					headerStr += "HTTP/2";
					break;
				case HTTPVersion::HTTP_3:
					headerStr += "HTTP/3";
					break;
				}

				switch (err)
				{
				case 0:
				{
					headerStr += " 200 OK\r\n";
				}break;
				case 404:
				{
					headerStr += " 404 Not Found\r\n";
					fileStr.clear();
					filePath = it->second->m_path;
					filePath += "root\\errors\\404.html";
					if (std::filesystem::exists(filePath))
					{
						ReadTextFile(&filePath, &fileStr);
					}
					else
					{
						fileStr = "<html><body><h1>404</h1></body></html>";
					}
				}break;
				case 501:
				{
					headerStr += " 501 Not Implemented\r\n";
					fileStr.clear();
					filePath = it->second->m_path;
					filePath += "root\\errors\\501.html";
					if (std::filesystem::exists(filePath))
					{
						ReadTextFile(&filePath, &fileStr);
					}
					else
					{
						fileStr = "<html><body><h1>501</h1></body></html>";
					}
				}break;
				}


				if (err == 0)
				{
					if(!isTextFile)
					{
						std::filesystem::path pth = filePath;
						headerStr += "Content-Disposition: attachment; filename=\"";
						headerStr += pth.filename().generic_string();
						headerStr += "\"\r\n";
						headerStr += "Content-Length: ";

						auto fileSz = std::filesystem::file_size(pth);
						char cb[40];
						snprintf(cb, 39, "%llu", fileSz);

						headerStr += cb;
						headerStr += "\r\n";
					}
				}

				LogWrite( httpdiot::ServerLogType::Info, "HEADER: %s\n", headerStr.c_str());

				sendStr = headerStr;
				sendStr += "\r\n";
				if (fileStr.size())
				{
					sendStr += fileStr;
					sendStr += "\r\n";
				}

				int rv = sk->Send((unsigned char*)sendStr.c_str(), sendStr.size(), 0);

				if (err == 0)
				{
					if (!isTextFile)
					{
						FILE* f = 0;
						fopen_s(&f, filePath.c_str(), "rb");
						if (f)
						{
							unsigned char charBuf[0xffff];
							while (1)
							{
								size_t readSize = fread_s(charBuf, 0xffff, 1, 0xffff, f);
								if (readSize)
								{
									rv = sk->Send(charBuf, readSize, 0);
								}
								else
								{
									break;
								}
							}
						}
						else
						{
						}
					}
				}
			}
		}

		//if (req.referer.size())
		{
		}
	}

	//this->Sleep();
	this->Disconnect();
}

