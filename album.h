/*
 * album.h - Music album class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef ALBUM_H
#define ALBUM_H

#include <QDir>

#include "item.h"

class Album : public Item
{
public:
	Album(QString, QString, QDir);
};

#endif // ALBUM_H
