/*
 * library.cpp
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QDebug>

#include "album.h"
#include "library.h"

Library::Library(QDir library_dir)
{
	QStringList artists;
	QDir artist_dir;
	QStringList albums;
	QDir album_dir;
	Album *album;

	qDebug().nospace() << __func__ << ": library: " <<	\
		library_dir.absolutePath();

	/* Read the artists sorted by name */
	artists = library_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,
					QDir::Name);

	/* Loop over all artists */
	for (const auto& artist_name : artists) {
		/* Read the albums sorted by name */
		artist_dir = library_dir.absolutePath() + "/" + artist_name;
		albums = artist_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,
					      QDir::Name);

		/* Loop over all albums */
		for (const auto& album_name : albums) {
			album_dir = artist_dir.absolutePath() + "/" + album_name;

			/* Create the album and add it to the playlist */
			album = new Album(artist_name, album_name, album_dir);
			album_list.push_back(*album);
		}
	}

	/* Set the current album */
	album_iter = album_list.begin();
}

Album Library::album(int offset)
{
	return *next(album_iter, offset);
}

void Library::next_album()
{
	album_iter = next(album_iter, 1);
}

void Library::prev_album()
{
	album_iter = next(album_iter, -1);
}

std::vector<Album>::iterator Library::next(std::vector<Album>::iterator iter,
					   int step)
{
	if (step == 0)
		return iter;

	for (int i = 0; i < abs(step); i++)
		if (step < 0) {
			if (iter == album_list.begin())
				iter = album_list.end();
			std::advance(iter, -1);
		} else {
			std::advance(iter, 1);
			if (iter == album_list.end())
				iter = album_list.begin();
		}

	return iter;
}
