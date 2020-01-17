#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <vector>

#include <QPixmap>

#include "track.h"

class Album
{
public:
	explicit Album(std::string album_path);

	std::string path;
	std::string artist;
	std::string name;
	QPixmap cover;
	std::vector<Track> tracks;

	std::vector<Track>::iterator next_track(std::vector<Track>::iterator iter, bool reverse=false);
};

bool compare_albums(Album a1, Album a2);

#endif // ALBUM_H
