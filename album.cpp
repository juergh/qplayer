#include <experimental/filesystem>
#include <string>

#include "album.h"
#include "track.h"

namespace fs = std::experimental::filesystem;

Album::Album(std::string album_path)
{
	Track *track;
	std::string cover_path;
	std::string track_name;

	path = album_path;

	/* Load the cover image */
	cover_path = path + "/cover.jpg";
	if (fs::exists(cover_path))
		cover.load(QString::fromStdString(cover_path));

	/* Load the tracks and sort them */
	for (const auto & track_iter : fs::directory_iterator(path)) {
		if (track_iter.path().extension() != ".mp3")
			continue;
		track = new Track(fs::absolute(track_iter.path()));
		tracks.push_back(*track);
	}
	std::sort(tracks.begin(), tracks.end(), compare_tracks);
}

std::vector<Track>::iterator Album::next_track(std::vector<Track>::iterator iter, bool reverse)
{
	if (reverse) {
		if (iter == tracks.begin())
			iter = tracks.end();
		std::advance(iter, -1);
	} else {
		std::advance(iter, 1);
		if (iter == tracks.end())
			iter = tracks.begin();
	}

	return iter;
}

/* Compare two albums for alphabetic sorting */
bool compare_albums(Album a1, Album a2)
{
	if (a1.artist == a2.artist)
		return (a1.name < a2.name);
	return (a1.artist < a2.artist);
}
