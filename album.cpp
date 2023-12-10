/*
 * album.cpp - Music album class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QCollator>

#include "album.h"

Album::Album(QString artist, QString album, QDir dir) :
	Item(artist, album)
{
	QStringList tracks;
	QString path = dir.absolutePath();
	QCollator coll;

	qDebug() << "album: artist:" << artist;
	qDebug() << "album: name:  " << album;
	qDebug() << "album: path:  " << path;

	/* Read the cover image */
	cover.load(path + "/cover.jpg");

	/* Create the playlist */
	playlist = new QMediaPlaylist();

	/* Loop over all tracks and add them to the playlist */
	tracks = dir.entryList({"*.mp3"}, QDir::Files | QDir::Readable,
			       QDir::NoSort);
	/* Sort list numerically */
	coll.setNumericMode(true);
	std::sort(tracks.begin(), tracks.end(),
		  [&](const QString& s1, const QString& s2)
		  { return coll.compare(s1, s2) < 0; });
	for (const auto &track : tracks) {
		qDebug() << "album: track: " << track;
		playlist->addMedia(QUrl("file://" + path + "/" + track));
	}
}
