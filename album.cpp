/*
 * album.cpp
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include "album.h"

Album::Album(QString artist_name, QString album_name, QDir album_dir) :
	artist(artist_name),
	name(album_name)
{
	QStringList tracks;

	qDebug() << "album artist:" << artist;
	qDebug() << "album name:  " << name;

	/* Load the cover image */
	cover.load(album_dir.absolutePath() + "/cover.jpg");

	/* Read the tracks sorted by name */
	tracks = album_dir.entryList({"*.mp3"}, QDir::Files | QDir::Readable,
				     QDir::Name);

	/* Create the playlist and add the tracks */
	playlist = new QMediaPlaylist();
	for (const auto& track : tracks) {
		qDebug() << "album track: " << track;
		playlist->addMedia(QUrl("file://" + album_dir.absolutePath() +
					"/" + track));
	}
}
