/*
 * collection.cpp - Music collection class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QDebug>

#include "album.h"
#include "collection.h"

Collection::Collection(QString artist, QDir dir) :
	Item(artist, QString())
{
	QString path = dir.absolutePath();
	QStringList artists;
	QDir artist_dir;
	QStringList albums;
	QDir album_dir;

	qDebug() << "collection: artist:" << artist;
	qDebug() << "collection: path:  " << path;

	/* Load the cover image if it exists */
	if (QFile::exists(path + "/cover.jpg")) {
		cover.load(path + "/cover.jpg");
	}

	/* We're at the top-level if artist is empty */
	if (artist.size() == 0) {
		/* Loop over all artists */
		artists = dir.entryList(QDir::Dirs |
					QDir::NoDotAndDotDot,
					QDir::Name);
		for (const auto& artist : artists) {
			artist_dir.setPath(path + "/" + artist);
			albums = artist_dir.entryList(QDir::Dirs |
						      QDir::NoDotAndDotDot,
						      QDir::Name);
			if (albums.size() == 0) {
				/* No albums */
				continue;
			} else if (albums.size() == 1) {
				/* Only a single album, add it */
				album = albums.at(0);
				album_dir.setPath(path + "/" + album);
				add_item(new Album(artist, album, album_dir));
			} else {
				/* Multiple albums, add a (sub-)collection */
				add_item(new Collection(artist, artist_dir));
			}
		}
	} else {
		/* Loop over all albums */
		artist_dir.setPath(path);
		albums = artist_dir.entryList(QDir::Dirs |
					      QDir::NoDotAndDotDot,
					      QDir::Name);
		for (const auto& album : albums) {
			album_dir.setPath(path + "/" + album);
			add_item(new Album(artist, album, album_dir));
		}
	}

	if (!first)
		qDebug() << "collection: no items found";
}

void Collection::add_item(Item *item)
{
	if (!first) {
		/* Add first item */
		item->next = item;
		item->prev = item;
		first = curr = item;
	} else {
		/* Append to end of list (before first item) */
		item->next = first;
		item->prev = first->prev;
		first->prev = item;
		item->prev->next = item;
	}
}

Item *Collection::first_item()
{
	curr = first;
	return curr;
}

Item *Collection::curr_item()
{
	return curr;
}

Item *Collection::next_item()
{
	if (!curr)
		return nullptr;

	curr = curr->next;
	return curr;
}

Item *Collection::prev_item()
{
	if (!curr)
		return nullptr;

	curr = curr->prev;
	return curr;
}
