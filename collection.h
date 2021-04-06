/*
 * collection.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QDir>

#include "album.h"

class Collection
{
public:
	explicit Collection(QDir);

	Album *first_album();
	Album *next_album();
	Album *prev_album();

	void add_album(Album *);

private:
	Album *first = nullptr;  /* First album (list head) */
	Album *curr = nullptr;   /* Current album */
};

#endif // COLLECTION_H
