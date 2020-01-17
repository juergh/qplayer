#ifndef QPLAYER_H
#define QPLAYER_H

#include <string>

#include <QMediaPlayer>

#include "album.h"
#include "library.h"
#include "track.h"
#include "ui_qplayer.h"

class QPlayer : public QWidget
{
	Q_OBJECT

public:
	explicit QPlayer(QWidget *parent = nullptr);

private slots:
	void on_album_clicked();
	void on_prev_album_clicked();
	void on_next_album_clicked();
	void on_prev_track_clicked();
	void on_next_track_clicked();

private:
	Ui::QPlayer ui;
	Library *library;
	std::vector<Album>::iterator album;
	std::vector<Track>::iterator track;
	QMediaPlayer *player;

	void set_album(std::vector<Album>::iterator iter);
	void set_track(std::vector<Track>::iterator iter);
};

#endif // QPLAYER_H
