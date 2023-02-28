#include "include/ReqRes.h"
#include "include/HttpServer.h"
#include <iostream>
#include <fstream>
#include "include/Files.h"
#include <sstream>

static constexpr const char* SEND_API_PATH = "/api/sendmessage";
static constexpr const char* GET_API_PATH = "/api/getmessage";

// address, port
int main(int argc, char **argv)
{
	const char* addr = argv[1];
	const char* port = argv[2];

	std::string saddr;
	std::string sport;

	if (argc != 3)
	{
		std::cout << "Address: ";
		std::cin >> saddr;

		std::cout << "Port: ";
		std::cin >> sport;

		addr = saddr.c_str();
		port = sport.c_str();
	}

	HttpServer server{
		[](Request& req, HttpServer& res) {
			std::cout << Request::stringifyMethod(req.getMethod()) << ' ' << req.getPath() << '\n';

			Response response;
			response.setVersion(req.getVersion());

			switch (req.getMethod())
			{
			case METHOD::GET:
			{
				std::string path = "./frontend" + req.getPath();
				std::ifstream file;

				parseToFile(path);
				std::cout << path << '\n';

				if (req["Content-Type"].find("image/"))
					file.open(path, std::ios::binary);
				else
					file.open(path);

				if (!file.good())
				{
					response.setStatus(STATUS::NOT_FOUND);
					response["Content-Type"] = "*/*";
					response["Content-Length"] = "0";
					break;
				}

				std::string ext = getFileExtension(path);

				if (ext == ".html")
					response["Content-Type"] = "text/html";
				else if (ext == ".css")
					response["Content-Type"] = "text/css";
				else if (ext == ".js")
					response["Content-Type"] = "text/javascript";
				else if (ext == ".png")
					response["Content-Type"] = "image/png";
				else if (ext == ".jpeg")
					response["Content-Type"] = "image/jpeg";
				else
					response["Content-Type"] = "*/*";

				std::stringstream filestr;
				filestr << file.rdbuf();

				std::string data{ std::move(filestr.str()) };
				response.setData(data);
				response["Content-Length"] = std::to_string(data.length());
				response.setStatus(STATUS::OK);

				break;
			}
			case METHOD::POST:
			{
				if (req.getPath() == GET_API_PATH)
				{
					std::ifstream dataFile{ "./data/data.txt" };
					std::stringstream jsonOut;
					jsonOut << "{\"messages\": [";

					while (!dataFile.eof())
					{
						std::string msg;
						std::getline(dataFile, msg);
						jsonOut << '"' << msg << '"';
						if (!dataFile.eof()) jsonOut << ',';
					}
					jsonOut << "]}";

					std::string data = jsonOut.str();
					
					response.setData(data);
					response.setStatus(STATUS::OK);
					response["Content-Type"] = "application/json";
					response["Content-Length"] = std::to_string(data.length());
				}
				else if (req.getPath() == SEND_API_PATH)
				{
					std::ofstream dataFile{ "./data/data.txt", std::ios::app };
					std::map<std::string, std::string> data;
					HttpServer::parseJSON(req.getBody(), data);

					dataFile << data["msg"] << '\n';

					response.setStatus(STATUS::OK);
				}

				break;
			}
			}

			res.respond(response);
		}
	};
	try
	{
		server.beginListen(port, addr,
			[](const char* p, const char* a)
			{
				std::cout << "Server started on: " << a << ':' << p << std::endl;
			}
		);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}