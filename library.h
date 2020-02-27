/*
 * library.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QDir>

#include <vector>

class Library
{
public:
	explicit Library(QDir library_dir);

	Album album(int offset=0);
	void next_album();
	void prev_album();

private:
	std::vector<Album> album_list;
	std::vector<Album>::iterator album_iter;
	std::vector<Album>::iterator next(std::vector<Album>::iterator iter,
					  int step);
};

#endif // LIBRARY_H
