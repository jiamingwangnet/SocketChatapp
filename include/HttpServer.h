#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <functional>
#include "ReqRes.h"

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 2048

class HttpServer;

typedef std::function<void(Request&, HttpServer&)> callback_t;
typedef std::function<void(const char*, const char*)> success_t;

class HttpServer
{
public:
	HttpServer(callback_t callback);
	~HttpServer();
	void beginListen(const char* port, const char* addr = "0.0.0.0", success_t success = nullptr);
	void respond(Response& res);
	static void parseJSON(std::string json, std::map<std::string, std::string>& map);
private:
	void initSocket(const char* port, const char* addr);
	void acceptClients();
	void receiveData(Request& out);
private:
	SOCKET listener = INVALID_SOCKET;
	SOCKET connection = INVALID_SOCKET;
	callback_t callback;
};