/*
 * album.cpp - Music album class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include "album.h"

Album::Album(QString artist, QString album, QDir dir) :
	Item(artist, album)
{
	QStringList tracks;
	QString path = dir.absolutePath();

	qDebug() << "album: artist:" << artist;
	qDebug() << "album: name:  " << album;
	qDebug() << "album: path:  " << path;

	/* Read the cover image */
	cover.load(path + "/cover.jpg");

	/* Create the playlist */
	playlist = new QMediaPlaylist();

	/* Loop over all tracks and add them to the playlist */
	tracks = dir.entryList({"*.mp3"}, QDir::Files | QDir::Readable,
			       QDir::Name);
	for (const auto& track : tracks) {
		qDebug() << "album: track: " << track;
		playlist->addMedia(QUrl("file://" + path + "/" + track));
	}
}
