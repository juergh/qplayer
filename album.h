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
	explicit Album(QString, QString, QDir);

	QString artist;
	QString name;
	QPixmap cover;
	QMediaPlaylist *playlist = nullptr;

	Album *next = nullptr;
	Album *prev = nullptr;
};

#endif // ALBUM_H
