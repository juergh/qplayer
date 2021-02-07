/*
 * qplayer.cpp
 *
 * Copyright (C) 2020 - Juerg Haefliger <juergh@gmail.com>
 */

#include <cmath>

#include "album.h"
#include "library.h"
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

	/* Load the music library */
	prefix.setPath("./storage");
	if (!prefix.exists())
		prefix.setPath("/storage");
	library = new Library(prefix.absolutePath() + "/music");

	/* Load the button images */
	prefix.setPath("./icons");
	if (!prefix.exists())
		prefix.setPath("/usr/lib/qplayer/icons");
	pixmap.load(prefix.absolutePath() + "/prev_album.png");
	ui.prev_album->setIcon(QIcon(pixmap));
	pixmap.load(prefix.absolutePath() + "/next_album.png");
	ui.next_album->setIcon(QIcon(pixmap));
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

	/* Set the current album and the display */
	update_album();
}

void QPlayer::update_album()
{
	Album album = library->album();

	/* Set the playlist */
	player->stop();
	player->setPlaylist(album.playlist);
	player->playlist()->setCurrentIndex(0);
	player->playlist()->setPlaybackMode(QMediaPlaylist::Sequential);

	/* Set the current album artist and name */
	ui.artist_label->setText(album.artist);
	ui.album_label->setText(album.name);

	/* Set the current album image and thumbnail */
	ui.album->setIcon(album.cover);
	ui.album_thumbnail->setPixmap(album.cover);

	/* Set the previous and next album thumbnails */
	ui.album_thumbnail_next1->setPixmap(library->album(-1).cover);
	ui.album_thumbnail_next2->setPixmap(library->album(-2).cover);
	ui.album_thumbnail_prev1->setPixmap(library->album(+1).cover);
	ui.album_thumbnail_prev2->setPixmap(library->album(+2).cover);
}

void QPlayer::update_track()
{
	QUrl track_url;
	QString track_name;

	track_url = player->playlist()->currentMedia().request().url();
	track_name = QFileInfo(track_url.path()).baseName();
	ui.track_label->setText(track_name);
}

void QPlayer::on_album_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (player->state() == QMediaPlayer::PlayingState)
		player->pause();
	else
		player->play();
}

void QPlayer::on_prev_album_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	library->prev_album();
	update_album();
}

void QPlayer::on_next_album_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	library->next_album();
	update_album();
}

void QPlayer::on_prev_track_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (player->playlist()->currentIndex() == 0)
		return;

	player->playlist()->previous();
}

void QPlayer::on_next_track_clicked()
{
	qDebug().nospace() << "qplayer::" << __func__;

	if (player->playlist()->currentIndex() ==
	    player->playlist()->mediaCount() - 1)
		return;

	player->playlist()->next();
}

void QPlayer::current_media_changed()
{
	qDebug().nospace() << "qplayer::" << __func__;

	update_track();
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
