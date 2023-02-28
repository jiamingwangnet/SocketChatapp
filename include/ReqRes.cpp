#include "ReqRes.h"

#include <sstream>

Request::Request(const char* request)
{
	std::stringstream sreq{ request };
	std::string header;
	size_t index;

	// parse first line
	std::getline(sreq, header);
	index = header.find(' ');
	method = Request::parseMethod(header.substr(0, index));

	size_t tmp = header.find(' ', index + 1);
	path = header.substr(index + 1, tmp - index - 1);
	index = tmp;

	version = header.substr(index + 1, header.find('\r', index + 1) - index - 1);

	while (std::getline(sreq, header) && header != "\r")
	{
		index = header.find(':');
		std::string value = header.substr(index + 1, header.find('\r'));
		if (value[0] == ' ')
			value.erase(0, 1);

		headers[header.substr(0, index)] = value;
	}

	while (std::getline(sreq, header) && header != "")
		body += header;
}

std::string Request::operator[](std::string header) const
{
	if (headers.find(header) == headers.end())
		return "\0";
	return headers.at(header);
}

METHOD Request::getMethod() const
{
	return method;
}

std::string Request::getPath() const
{
	return path;
}

std::string Request::getVersion() const
{
	return version;
}

std::string Request::getBody() const
{
	return body;
}

METHOD Request::parseMethod(std::string method)
{
	if (method == "GET")
		return METHOD::GET;
	else if (method == "HEAD")
		return METHOD::HEAD;
	else if (method == "POST")
		return METHOD::POST;
	else if (method == "PUT")
		return METHOD::PUT;
	else if (method == "DELETE")
		return METHOD::DELETE;
	else if (method == "CONNECT")
		return METHOD::CONNECT;
	else if (method == "OPTIONS")
		return METHOD::OPTIONS;
	else if (method == "TRACE")
		return METHOD::TRACE;
	else if (method == "PATCH")
		return METHOD::PATCH;
	return METHOD::UNKNOWN;
}

std::string Request::stringifyMethod(METHOD method)
{
	// TODO: add all methods
	switch (method)
	{
	case METHOD::GET:
		return "GET";
	case METHOD::HEAD:
		return "HEAD";
	case METHOD::POST:
		return "POST";
	default:
		return " ";
	}
}

std::string& Response::operator[](const std::string header)
{
	return headers[header];
}

void Response::setVersion(const std::string version)
{
	this->version = version;
}

void Response::setStatus(const STATUS status)
{
	this->status = status;
}

void Response::setData(std::string& data)
{
	this->data = data;
}

void Response::setData(std::string&& data)
{
	//https://stackoverflow.com/questions/31213539/c-stdmove-with-rvalue-reference-is-not-moving-contents
	// data is a lvalue of type std::string&& which requires std::move to turn it into an rvalue reference
	// anything with a name is an lvalue
	this->data = std::move(data);
}

// only supports common statuses
std::string Response::stringifyStatus(const STATUS status)
{
	// TODO: add everything
	switch (status)
	{
	case STATUS::OK:
		return "OK";
	case STATUS::BAD_REQUEST:
		return "Bad Request";
	case STATUS::UNAUTHORIZED:
		return "Unauthorized";
	case STATUS::FORBIDDEN:
		return "Forbidden";
	case STATUS::NOT_FOUND:
		return "Not Found";
	case STATUS::METHOD_NOT_ALLOWED:
		return "Method Not Allowed";
	case STATUS::IM_A_TEAPOT:
		return "I'm a teapot";
	case STATUS::INTERNAL_SERVER_ERROR:
		return "Internal Server Error";
	case STATUS::NOT_IMPLEMENTED:
		return "Not Implemented";
	case STATUS::BAD_GATEWAY:
		return "Bad Gateway";
	default:
		return "\0";
	}
}

std::string Response::stringify() const
{
	std::stringstream ostr;

	ostr << version << ' ' << (int)status << ' ' << Response::stringifyStatus(status) << "\r\n";

	for (std::pair<std::string, std::string> header : headers)
	{
		ostr << header.first << ": " << header.second << "\r\n";
	}

	ostr << "\r\n";

	ostr << data;

	return ostr.str();
}
