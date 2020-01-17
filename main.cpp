#include <QApplication>

#include "qplayer.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QPlayer qplayer;
	qplayer.show();
	return app.exec();
}
