/*
 * qplayer.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QApplication>

#include "qplayer.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QPlayer qplayer;
	qplayer.show();
	return app.exec();
}
