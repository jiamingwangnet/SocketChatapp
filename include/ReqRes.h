#pragma once

#include <map>
#include <string>

#undef DELETE

enum class METHOD
{
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH,
	UNKNOWN
};

enum class STATUS
{
	CONTINUE = 100,
	SWITCHING_PROTOCOLS = 101,
	PROCESSING = 102,
	EARLY_HINTS = 103,

	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NONAUTHORITATIVE_INFO = 203,
	NO_CONTENT = 204,
	RESET_CONTENT = 205,
	PARTIAL_CONTENT = 206,
	MULTISTATUS = 207,
	ALREADY_REPORTED = 208,
	IM_USED = 226,

	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY = 301,
	FOUND = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,

	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	NOT_ACCEPTABLE = 406,
	PROXY_AUTH_REQUIRED = 407,
	REQUEST_TIMEOUT = 408,
	CONFLICT = 409,
	GONE = 410,
	LENGTH_REQUIRED = 411,
	PRECONDITION_FAILED = 412,
	PAYLOAD_TOO_LARGE = 413,
	URI_TOO_LONG = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	RANGE_NOT_SATISFIABLE = 416,
	EXPECTATION_FAILED = 417,
	IM_A_TEAPOT = 418,
	MISDIRECTED_REQUEST = 421,
	UNPROCESSABLE_ENTITY = 422,
	LOCKED = 423,
	FAILED_DEPENDENCY = 424,
	UPGRADE_REQUIRED = 426,
	PRECONDITION_REQUIRED = 428,
	TOO_MAY_REQUESTS = 429,
	REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	UNAVAILABLE_FOR_LEGAL_REASONS = 451,

	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIMEOUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	VARIANT_ALSO_NEGOTIATES = 506,
	INSUFFICIENT_STORATE = 507,
	LOOP_DETECTED = 508,
	NOT_EXTENDED = 510,
	NETWORK_AUTH_REQUIRED = 511,
};

class Request
{
public:
	Request() = default;
	Request(const char* request);
	std::string operator[](std::string header) const;
	
	METHOD getMethod() const;
	std::string getPath() const;
	std::string getVersion() const;
	std::string getBody() const;

	static METHOD parseMethod(std::string method);
	static std::string stringifyMethod(METHOD method);
private:
	METHOD method;
	std::string path;
	std::string version;
	std::string body;

	std::map<std::string, std::string> headers;
};

class Response
{
public:
	std::string& operator[](const std::string header);
	// TODO: add getters
	void setVersion(const std::string version);
	void setStatus(const STATUS status);
	void setData(std::string& data);
	void setData(std::string&& data);

	static std::string stringifyStatus(const STATUS status);
	std::string stringify() const;
private:
	std::string version;
	STATUS status;
	std::map<std::string, std::string> headers;
	std::string data;
};