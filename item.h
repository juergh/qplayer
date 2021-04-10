/*
 * item.h - Base class for a collection item
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef ITEM_H
#define ITEM_H

#include <QDir>
#include <QMediaPlaylist>
#include <QPixmap>

class Item
{
public:
	Item(QString, QString);

	QString artist;
	QString album;
	QPixmap cover;

	Item *next = nullptr;
	Item *prev = nullptr;

	QMediaPlaylist *playlist = nullptr;
};

#endif // ITEM_H
