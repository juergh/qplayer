/*
 * album.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef ALBUM_H
#define ALBUM_H

#include <QDir>
#include <QMediaPlaylist>
#include <QPixmap>

class Album
{
public:
	explicit Album(QString artist_name, QString album_name,
		       QDir album_dir);

	QString artist;
	QString name;
	QPixmap cover;
	QMediaPlaylist *playlist = nullptr;
};

#endif // ALBUM_H
