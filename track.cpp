#include <experimental/filesystem>
#include <string>

#include "track.h"

namespace fs = std::experimental::filesystem;

Track::Track(std::string track_path)
{
	std::string name = fs::path(track_path).filename().replace_extension("");

	path = track_path;
	title = name.substr(5);
	number = std::stoi(name.substr(0, 2));
}

/* Compare two tracks for alphabetic sorting */
bool compare_tracks(Track t1, Track t2)
{
	if (t1.number == t2.number)
		return (t1.title < t2.title);
	return (t1.number < t2.number);
}
