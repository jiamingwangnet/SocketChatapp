#include "Files.h"
#include <fstream>

std::string getFileName(std::string path, char delimiter)
{
    size_t last = path.find_last_of(delimiter);
    return path.substr(last + 1, path.length() - last - 1);
}

std::string getFileExtension(std::string path, char delimiter)
{
    std::string filename = getFileName(path, delimiter);
    size_t last = filename.find_last_of('.');
    return filename.substr(last, filename.length() - last);
}

void parseToFile(std::string& path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			if (path[path.length() - 1] != '/')
				path += '/';
			path += "index.html";
		}
	}
}
