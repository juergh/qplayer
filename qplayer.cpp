/*
 * qplayer.cpp
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <cmath>

#include "album.h"
#include "collection.h"
#include "qplayer.h"

QPlayer::QPlayer(QWidget *parent) :
	QWidget(parent)
{
	QPixmap pixmap;
	QDir prefix;

	/* Setup the UI */
	ui.setupUi(this);

	/* Create the player */
	player = new QMediaPlayer;
#ifdef RASPI_KIDZ
	player->setVolume(100);
#else
	player->setVolume(volume);
#endif
	connect(player, &QMediaPlayer::currentMediaChanged, this,
		&QPlayer::current_media_changed);

	/* Load the music collection */
	prefix.setPath("./storage/music");
	if (!prefix.exists())
		prefix.setPath("/storage/music");
	collection = new Collection("", prefix.absolutePath());

	/* Load the button images */
	prefix.setPath("./icons");
	if (!prefix.exists())
		prefix.setPath("/usr/lib/qplayer/icons");
	pixmap.load(prefix.absolutePath() + "/up_item.png");
	ui.up_item->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/prev_item.png");
	ui.prev_item->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/next_item.png");
	ui.next_item->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/prev_track.png");
	ui.prev_track->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/next_track.png");
	ui.next_track->setIcon(QIcon(pixmap));

#ifndef RASPI_KIDZ
	/* Set the keyboard shortcuts */
	ui.prev_item->setShortcut(QKeySequence(Qt::Key_Left));
	ui.prev_track->setShortcut(QKeySequence(Qt::Key_Comma));
	ui.cover->setShortcut(QKeySequence(Qt::Key_Space));
	ui.next_track->setShortcut(QKeySequence(Qt::Key_Period));
	ui.next_item->setShortcut(QKeySequence(Qt::Key_Right));
	ui.up_item->setShortcut(QKeySequence(Qt::Key_Escape));

	/* Set the volume up keystroke handler */
	volume_up = new QShortcut(this);
	volume_up->setKey(Qt::Key_Up);
	connect(volume_up, SIGNAL(activated()), this,
		SLOT(volume_up_pressed()));

	/* Set the volume down keystroke handler */
	volume_down = new QShortcut(this);
	volume_down->setKey(Qt::Key_Down);
	connect(volume_down, SIGNAL(activated()), this,
		SLOT(volume_down_pressed()));

	/* Set the seek left keystroke handler */
	seek_left = new QShortcut(this);
	seek_left->setKey(Qt::Key_Z);
	connect(seek_left, SIGNAL(activated()), this,
		SLOT(seek_left_pressed()));

	/* Set the seek right keystroke handler */
	seek_right = new QShortcut(this);
	seek_right->setKey(Qt::Key_X);
	connect(seek_right, SIGNAL(activated()), this,
		SLOT(seek_right_pressed()));
#endif

	/* Update the display */
	update_display();
}

void QPlayer::update_display()
{
	Item *item = collection->curr_item();

	if (!item)
		return;

	/* Stop the player */
	player->stop();

	/* Set the artist, album and track labels */
	ui.artist_label->setText(item->artist);
	ui.album_label->setText(item->album);
	ui.track_label->setText(QString());

	/* Set the cover image and thumbnail */
	ui.cover->setIcon(item->cover);
	ui.cover_thumb->setIcon(item->cover);

	/* Set the previous and next thumbnails */
	ui.next_cover_thumb->setIcon(item->next->cover);
	ui.prev_cover_thumb->setIcon(item->prev->cover);

	/* Set the playlist if it's an album */
	if (item->playlist) {
		player->setPlaylist(item->playlist);
		player->playlist()->setCurrentIndex(0);
		player->playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
	}
}

void QPlayer::update_track_label()
{
	QUrl track_url;
	QString track_name;

	if (!player->playlist())
		return;

	track_url = player->playlist()->currentMedia().request().url();
	track_name = QFileInfo(track_url.path()).baseName();
	ui.track_label->setText(track_name);
}

void QPlayer::on_cover_clicked()
{
	Item *item = collection->curr_item();

	qDebug().nospace() << "qplayer::" << __func__;

	/* The current item is an album if it has a playlist, otherwise it's
	 * a sub-collection, in which case we switch to it */
	if (item->playlist) {
		if (player->state() == QMediaPlayer::PlayingState)
			player->pause();
		else
			player->play();
	} else {
		/* Save the current collection and switch to the new one */
		prev_collection = collection;
		collection = (Collection *)item;
		collection->first_item();
		update_display();
	}
}

void QPlayer::on_cover_thumb_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	on_cover_clicked();
}

void QPlayer::on_next_cover_thumb_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	on_next_item_clicked();
}

void QPlayer::on_prev_cover_thumb_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	on_prev_item_clicked();
}

void QPlayer::on_up_item_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (prev_collection) {
		collection = prev_collection;
		prev_collection = nullptr;
		update_display();
	}
}

void QPlayer::on_prev_item_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	collection->prev_item();
	update_display();
}

void QPlayer::on_next_item_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	collection->next_item();
	update_display();
}

void QPlayer::on_prev_track_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (!player->playlist() || (player->playlist()->currentIndex() == 0))
		return;

	player->playlist()->previous();
}

void QPlayer::on_next_track_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (!player->playlist() || (player->playlist()->currentIndex() ==
				    player->playlist()->mediaCount() - 1))
		return;

	player->playlist()->next();
}

void QPlayer::current_media_changed()
{
	qDebug().nospace() << "qplayer::" << __func__;

	update_track_label();
}

#ifndef RASPI_KIDZ
int volume_lin2log(int value)
{
	return std::round((100 *
			   (std::exp((float)value / 100.0) - 1) /
			   (std::exp(1.0) - 1)));
}

void QPlayer::volume_up_pressed()
{
	qDebug().nospace() << "qplayer::" << __func__;

	volume = volume + 5;
	if (volume > 100)
		volume = 100;

	player->setVolume(volume_lin2log(volume));
}

void QPlayer::volume_down_pressed()
{
	qDebug().nospace() << "qplayer::" << __func__;

	volume = volume - 5;
	if (volume < 0)
		volume = 0;

	player->setVolume(volume_lin2log(volume));
}

void QPlayer::seek_left_pressed()
{
	qint64 position;

	qDebug().nospace() << "qplayer::" << __func__;

	position = player->position() - 5000;
	if (position < 0)
		position = 0;

	player->setPosition(position);
}

void QPlayer::seek_right_pressed()
{
	qint64 position;

	qDebug().nospace() << "qplayer::" << __func__;

	position = player->position() + 5000;
	if (position > player->duration())
		position = player->duration() - 1;

	player->setPosition(position);
}
#endif
