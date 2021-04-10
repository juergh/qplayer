/*
 * item.cpp - Base class for a collection or an album
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QDebug>

#include "item.h"

Item::Item(QString artist, QString album) :
	artist(artist),
	album(album)
{
	//	qDebug() << "item artist: " << artist;
	//	qDebug() << "item album:  " << album;
}
