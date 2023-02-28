#include "HttpServer.h"
#include <exception>
#include <string>

HttpServer::HttpServer(callback_t callback)
	: callback(callback)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		std::string msg = "WSAStartup failed: ";
		msg += std::to_string(result);
		throw std::exception{ msg.c_str() };
	}
}

HttpServer::~HttpServer()
{
	if (connection != INVALID_SOCKET)
		closesocket(connection);
	if (listener != INVALID_SOCKET)
		closesocket(listener);
}

void HttpServer::beginListen(const char* port, const char* addr, std::function<void(const char*, const char*)> success)
{
	initSocket(port, addr);

	if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(listener);
		WSACleanup();
		std::string msg = "Listen failed: ";
		msg += WSAGetLastError();
		throw std::exception{ msg.c_str() };
	}

	if (success != nullptr) success(port, addr);

	while (true)
	{
		acceptClients();
		
		Request req;
		receiveData(req);

		callback(req, *this);

		if (connection != INVALID_SOCKET) closesocket(connection);
	}
}

void HttpServer::respond(Response& res)
{
	if (connection != INVALID_SOCKET)
	{
		// TODO: change to use std::move for large files
		std::string strRes = res.stringify();
		send(connection, strRes.c_str(), (int)strRes.length(), NULL);
	}
	else
		throw std::exception{ "connection is invalid." };
}

// assuming data is on one line - newlines get stripped
// TODO: check multiline and parse things that are not strings
void HttpServer::parseJSON(std::string json, std::map<std::string, std::string>& map)
{
	// strip line ends
	for (auto c = json.begin(); c != json.end();)
	{
		if (*c == '\n' || *c == '\r')
			c = json.erase(c);
		else
			++c;
	}

	size_t d_end = 0;

	if (json[1] == '}') return;
	do
	{
		size_t start = json.find('"', d_end);
		size_t end = json.find('"', start + 1);

		std::string key = json.substr(start + 1, end - start - 1);

		size_t d_start = json.find(':', end + 1);
		d_end = json.find(',', d_start + 1);
		if (d_end == std::string::npos) d_end = json.find('}', d_start + 1);

		std::string value = json.substr(d_start + 1, d_end - d_start - 1);
		// strip quotes
		if (value[0] == '"' && value[value.length() - 1] == '"')
		{
			value.erase(0, 1);
			value.erase(value.length() - 1, 1);
		}

		map[key] = value;
	} while (json.find(',', d_end) != std::string::npos);
}

void HttpServer::initSocket(const char* port, const char* addr)
{
	int iresult;
	/*addrinfo* result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iresult = getaddrinfo(NULL, port, &hints, &result);
	if (iresult != 0)
	{
		WSACleanup();

		std::string msg = "getaddrinfo failed: ";
		msg += iresult;
		throw std::exception{ msg.c_str() };
	}*/

	sockaddr_in hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.sin_port = htons(atoi(port));
	hints.sin_family = AF_INET;
	if (strcmp(addr, "0.0.0.0") == 0)
		hints.sin_addr.S_un.S_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, addr, &(hints.sin_addr));

	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == INVALID_SOCKET)
	{
		WSACleanup();

		std::string msg = "Listen socket creation failed: ";
		msg += std::to_string(WSAGetLastError());
		throw std::exception{ msg.c_str() };
	}

	iresult = bind(listener, (sockaddr*)&hints, sizeof(hints));
	if (iresult == SOCKET_ERROR)
	{
		closesocket(listener);
		WSACleanup();

		std::string msg = "listener bind failed: ";
		msg += std::to_string(WSAGetLastError());
		throw std::exception{ msg.c_str() };
	}
}

void HttpServer::acceptClients()
{
	if ((connection = accept(listener, NULL, NULL)) == SOCKET_ERROR)
	{
		closesocket(listener);
		WSACleanup();

		std::string msg = "Accept failed: ";
		msg += WSAGetLastError();

		throw std::exception{ msg.c_str() };
	}
}

void HttpServer::receiveData(Request& out)
{
	if (connection != INVALID_SOCKET)
	{
		char buffer[BUFFER_SIZE] = { 0 };
		recv(connection, buffer, BUFFER_SIZE, NULL);

		Request req{ buffer };
		out = req;
	}
	else
		throw std::exception{ "connection is invalid" };
}
