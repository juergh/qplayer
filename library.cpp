#include <algorithm>
#include <iostream>    // for cout
#include <experimental/filesystem>
#include <vector>

#include "album.h"
#include "library.h"

namespace fs = std::experimental::filesystem;

Library::Library(std::string library_path)
{
	Album *album;
	std::string artist_name;
	std::string album_name;

	std::cout << __func__ << "\n";

	if (!fs::is_directory(library_path)) {
		return;
	}

	/* The absolute path to the library */
	path = fs::absolute(library_path);

	/* Iterate over the artists */
	for (const auto & artist_iter : fs::directory_iterator(path)) {
		if (!fs::is_directory(artist_iter))
			continue;
		artist_name = artist_iter.path().filename();

		/* Iterate over the albums */
		for (const auto & album_iter : fs::directory_iterator(artist_iter.path())) {
			if (!fs::is_directory(album_iter))
				continue;
			album_name = album_iter.path().filename();

			/* Create the album */
			album = new Album(fs::absolute(album_iter.path()));
			album->artist = artist_name;
			album->name = album_name;

			/* Push the album to the list */
			albums.push_back(*album);
		}
	}

	/* Sort the albums */
	std::sort(albums.begin(), albums.end(), compare_albums);
}

std::vector<Album>::iterator Library::next_album(std::vector<Album>::iterator iter, bool reverse)
{
	if (reverse) {
		if (iter == albums.begin())
			iter = albums.end();
		std::advance(iter, -1);
	} else {
		std::advance(iter, 1);
		if (iter == albums.end())
			iter = albums.begin();
	}

	return iter;
}
