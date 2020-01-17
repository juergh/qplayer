#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

#include "album.h"

class Library
{
public:
	explicit Library(std::string library_path);

	std::string path;
	std::vector<Album> albums;

	std::vector<Album>::iterator next_album(std::vector<Album>::iterator iter, bool reverse=false);
};

#endif // LIBRARY_H
