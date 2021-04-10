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
	collection = new Collection("main", prefix.absolutePath());

	/* Load the button images */
	prefix.setPath("./icons");
	if (!prefix.exists())
		prefix.setPath("/usr/lib/qplayer/icons");
	pixmap.load(prefix.absolutePath() + "/prev_album.png");
	ui.prev_item->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/next_album.png");
	ui.next_item->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/prev_track.png");
	ui.prev_track->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/next_track.png");
	ui.next_track->setIcon(QIcon(pixmap));

#ifndef RASPI_KIDZ
	/* Set the keyboard shortcuts */
	ui.prev_album->setShortcut(QKeySequence(Qt::Key_Left));
	ui.prev_track->setShortcut(QKeySequence(Qt::Key_Less));
	ui.album->setShortcut(QKeySequence(Qt::Key_Space));
	ui.next_track->setShortcut(QKeySequence(Qt::Key_Greater));
	ui.next_album->setShortcut(QKeySequence(Qt::Key_Right));

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
#endif

	/* Update the display */
	update_display();
}

void QPlayer::update_display()
{
	Item *item = collection->curr_item();
	Item *next1, *next2, *prev1, *prev2;

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
	ui.cover_thumbnail->setPixmap(item->cover);

	/* Set the previous and next thumbnails */
	next1 = item->next;
	next2 = next1->next;
	prev1 = item->prev;
	prev2 = prev1->prev;
	ui.cover_thumbnail_next1->setPixmap(next1->cover);
	ui.cover_thumbnail_next2->setPixmap(next2->cover);
	ui.cover_thumbnail_prev1->setPixmap(prev1->cover);
	ui.cover_thumbnail_prev2->setPixmap(prev2->cover);

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
	qDebug().nospace() << "qplayer::" << __func__;

	if (player->state() == QMediaPlayer::PlayingState)
		player->pause();
	else
		player->play();
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
#endif
