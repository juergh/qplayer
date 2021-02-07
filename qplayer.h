/*
 * qplayer.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef QPLAYER_H
#define QPLAYER_H

#define RASPI_KIDZ

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QShortcut>

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
	void current_media_changed();
#ifndef RASPI_KIDZ
	void volume_up_pressed();
	void volume_down_pressed();
#endif

private:
	Ui::QPlayer ui;
	Library *library = nullptr;
	QMediaPlayer *player = nullptr;
#ifndef RASPI_KIDZ
	QShortcut *volume_up = nullptr;
	QShortcut *volume_down = nullptr;
	int volume = 20;
#endif

	void update_album();
	void update_track();
};

#endif // QPLAYER_H
