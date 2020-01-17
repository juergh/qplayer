#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>

class Track
{
public:
	explicit Track(std::string track_path);

	std::string path;
	std::string title;
	std::size_t number;
};

bool compare_tracks(Track t1, Track t2);

#endif // TRACK_H
