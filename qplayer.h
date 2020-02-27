/*
 * qplayer.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef QPLAYER_H
#define QPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "album.h"
#include "library.h"
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
	void on_current_media_changed();

private:
	Ui::QPlayer ui;
	Library *library = nullptr;
	QMediaPlayer *player = nullptr;

	void update_album();
	void update_track();
};

#endif // QPLAYER_H
