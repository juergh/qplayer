/*
 * qplayer.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <QApplication>
#include <QCommandLineParser>

#include "qplayer.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCommandLineParser parser;
	QCommandLineOption timeoutOption(QStringList() << "t" << "timeout",
		QApplication::translate("main", "Timeout."),
		QApplication::translate("main", "seconds"));

	parser.addHelpOption();
	parser.addOption(timeoutOption);
	parser.process(app);

	int timeout = parser.value(timeoutOption).toInt();
	if (timeout < 1) {
		qDebug().nospace() << "Invalid timeout value: " << timeout;
		return -1;
	}

	QPlayer qplayer(nullptr, timeout);
	qplayer.show();
	return app.exec();
}
