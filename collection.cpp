/*
 * collection.cpp
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QDebug>

#include "collection.h"

Collection::Collection(QDir collection_dir)
{
	QStringList artists;
	QDir artist_dir;
	QStringList albums;
	QDir album_dir;
	Album *album;

	qDebug() << "collection:" << collection_dir.absolutePath();

	/* Read the artists sorted by name */
	artists = collection_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,
					QDir::Name);

	/* Loop over all artists */
	for (const auto& artist_name : artists) {
		/* Read the albums sorted by name */
		artist_dir.setPath(collection_dir.absolutePath() + "/" +
				   artist_name);
		albums = artist_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,
					      QDir::Name);

		/* Loop over all albums */
		for (const auto& album_name : albums) {
			album_dir.setPath(artist_dir.absolutePath() + "/" +
					  album_name);

			/* Create the album and add it to the collection */
			album = new Album(artist_name, album_name, album_dir);
			add_album(album);
		}
	}

	/* Add a dummy album if the list is empty (to prevent crashes) */
	if (!first) {
		qDebug() << "no albums found";
		//album = new Album(QString(), QString(), QDir());
		//add_album(album);
	}
}

void Collection::add_album(Album *album)
{
	if (!first) {
		/* Add first album */
		album->next = album;
		album->prev = album;
		first = curr = album;
	} else {
		/* Append to end of list (before first album) */
		album->next = first;
		album->prev = first->prev;
		first->prev = album;
		album->prev->next = album;
	}
}

Album *Collection::first_album()
{
	curr = first;
	return curr;
}

Album *Collection::next_album()
{
	if (!curr)
		return nullptr;

	curr = curr->next;
	return curr;
}

Album *Collection::prev_album()
{
	if (!curr)
		return nullptr;

	curr = curr->prev;
	return curr;
}
