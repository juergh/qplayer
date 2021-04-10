/*
 * collection.cpp - Music collection class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QDebug>

#include "album.h"
#include "collection.h"

Collection::Collection(QString name, QDir dir) :
	Item(name, QString())
{
	QStringList artists;
	QDir artist_dir;
	QStringList albums;
	QDir album_dir;
	QString path = dir.absolutePath();

	qDebug() << "collection: name:" << name;
	qDebug() << "collection: path:" << path;

	/* Read the cover image */
        cover.load(path + "/cover.jpg");

	/* Loop over all artists */
	artists = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
	for (const auto& artist : artists) {
		artist_dir.setPath(dir.absolutePath() + "/" + artist);

                if (QFile::exists(artist_dir.absolutePath() + "/cover.jpg")) {
			/* There is a cover.jpg file at the artist directory
			 * level, so this is a (sub-)collection */
			add_item(new Collection(artist, artist_dir));

		} else {
			/* Loop over all albums */
			albums = artist_dir.entryList(QDir::Dirs |
						      QDir::NoDotAndDotDot,
						      QDir::Name);
			for (const auto& album : albums) {
				album_dir.setPath(artist_dir.absolutePath() +
						  "/" + album);
				add_item(new Album(artist, album, album_dir));
			}
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
