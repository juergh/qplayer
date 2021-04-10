/*
 * collection.h - Music collection class
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QDir>

#include "item.h"

class Collection : public Item
{
public:
	Collection(QString, QDir);

	Item *first_item();
	Item *curr_item();
	Item *next_item();
	Item *prev_item();

	void add_item(Item *item);

private:
	Item *first = nullptr;  /* First item */
	Item *curr = nullptr;   /* Current item */
};

#endif // COLLECTION_H
