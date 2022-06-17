// MIT license

#pragma once
#ifndef _HTTPDIOT_HTTP_
#define _HTTPDIOT_HTTP_

#include <string>

namespace httpdiot
{

	// https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
	enum class HTTPRequestMethod
	{
		// The GET method requests a representation of the specified resource. 
		// Requests using GET should only retrieve data.
		m_GET,

		// The HEAD method asks for a response identical to a GET request,
		// but without the response body.
		m_HEAD,

		// The POST method submits an entity to the specified resource, often
		// causing a change in state or side effects on the server.
		m_POST,

		// The PUT method replaces all current representations of the target
		// resource with the request payload.
		m_PUT,

		// The DELETE method deletes the specified resource.
		m_DELETE,

		// The CONNECT method establishes a tunnel to the server identified 
		// by the target resource.
		m_CONNECT,

		// The OPTIONS method describes the communication options for the target resource.
		m_OPTIONS,

		// The TRACE method performs a message loop - back test along the path 
		// to the target resource.
		m_TRACE,

		// The PATCH method applies partial modifications to a resource.
		m_PATCH,
	};

	enum class HTTPVersion
	{
		// GET /mypage.html
		// 
		// <html>HTML page</html>
		HTTP_0_9,

		HTTP_1_0,
		HTTP_1_1,
		HTTP_2,
		HTTP_3,
	};

	struct HTTPRequest
	{
		HTTPRequestMethod method = HTTPRequestMethod::m_GET;
		HTTPVersion version = HTTPVersion::HTTP_1_1;
		std::string path;

		std::string host;
		std::string user_agent;
		std::string accept;
		std::string accept_encoding;
		std::string accept_language;
		std::string connection;
		std::string upgrade_insecure_requests;
		std::string referer;
		std::string origin;
		std::string content_type;
		std::string content_length;
		std::string from;
		std::string cache_control;
		std::string dnt;
		std::string sec_ch_ua;
		std::string sec_ch_ua_mobile;
		std::string sec_ch_ua_platform;
		std::string sec_fetch_site;
		std::string sec_fetch_mode;
		std::string sec_fetch_dest;
		std::string sec_fetch_user;
		std::string range;
	};

}

#endif

