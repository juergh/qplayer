/*
 * qplayer.h
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#ifndef QPLAYER_H
#define QPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QShortcut>
#include <QTimer>

#include "item.h"
#include "collection.h"
#include "ui_qplayer.h"

class QPlayer : public QWidget
{
	Q_OBJECT

public:
	explicit QPlayer(QWidget *parent = nullptr, int timeout = 0);

private slots:
	void on_cover_clicked();
	void on_cover_thumb_clicked();
	void on_next_cover_thumb_clicked();
	void on_prev_cover_thumb_clicked();
	void on_up_item_clicked();
	void on_prev_item_clicked();
	void on_next_item_clicked();
	void on_prev_track_clicked();
	void on_next_track_clicked();
	void current_media_changed();
	void timer_start(int timeout = 0);
	void timer_stop();
	void timer_timeout();
#ifndef RASPI_KIDZ
	void volume_up_pressed();
	void volume_down_pressed();
	void seek_left_pressed();
	void seek_right_pressed();
#endif

private:
	Ui::QPlayer ui;
	Collection *collection = nullptr;
	Collection *prev_collection = nullptr;
	QMediaPlayer *player = nullptr;
	QTimer *timer = nullptr;
#ifndef RASPI_KIDZ
	int volume = 20;
	QShortcut *volume_up = nullptr;
	QShortcut *volume_down = nullptr;
	QShortcut *seek_left = nullptr;
	QShortcut *seek_right = nullptr;
#endif

	void update_display();
	void update_track_label();
};

#endif // QPLAYER_H
