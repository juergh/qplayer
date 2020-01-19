#include <experimental/filesystem>
#include <iostream>

#include "library.h"
#include "qplayer.h"

namespace fs = std::experimental::filesystem;

QPlayer::QPlayer(QWidget *parent)
        : QWidget(parent)
{
	QPixmap pixmap;
	QString icon_prefix;
	std::string storage_prefix;

	std::cout << __func__ << "\n";

	/* Create the player */
	player = new QMediaPlayer;
	player->setVolume(50);

	/* Setup the UI */
	ui.setupUi(this);

	/* Load the button images and assign shortcuts */
	icon_prefix = "./icons/";
	if (!fs::exists(icon_prefix.toStdString()))
		icon_prefix = "/usr/lib/qplayer/icons/";
	pixmap.load(icon_prefix + "prev_album.png");
	ui.prev_album->setIcon(QIcon(pixmap));
	ui.prev_album->setShortcut(QKeySequence("c"));
	pixmap.load(icon_prefix + "next_album.png");
	ui.next_album->setIcon(QIcon(pixmap));
	ui.next_album->setShortcut(QKeySequence("m"));
	pixmap.load(icon_prefix + "prev_track.png");
	ui.prev_track->setIcon(QIcon(pixmap));
	ui.prev_track->setShortcut(QKeySequence("v"));
	pixmap.load(icon_prefix + "next_track.png");
	ui.next_track->setIcon(QIcon(pixmap));
	ui.next_track->setShortcut(QKeySequence("n"));

	/* Play/pause shortcut */
	ui.album->setShortcut(QKeySequence("b"));

	/* Load the music library and set the current album */
	storage_prefix = "./storage/";
	if (!fs::exists(storage_prefix))
		storage_prefix = "/storage/";
	library = new Library(storage_prefix + "music");
	set_album(library->albums.begin());
}

void QPlayer::set_album(std::vector<Album>::iterator iter)
{
	std::vector<Album>::iterator thumb;

	std::cout << __func__ << "\n";

	/* Stop playing when we switch albums */
	player->stop();

	/* Set the current album and track */
	album = iter;
	set_track(album->tracks.begin());

	/* Update the image and thumbnail of the current album */
	ui.album->setIcon(album->cover);
	ui.album_thumbnail->setPixmap(album->cover);

	/* Update the album artist and name */
	ui.artist_label->setText(QString::fromStdString(album->artist));
	ui.album_label->setText(QString::fromStdString(album->name));

	/* Update the previous and next album thumbnails */
	thumb = library->next_album(album);
	ui.album_thumbnail_next1->setPixmap(thumb->cover);
	thumb = library->next_album(thumb);
	ui.album_thumbnail_next2->setPixmap(thumb->cover);
	thumb = library->next_album(album, true);
	ui.album_thumbnail_prev1->setPixmap(thumb->cover);
	thumb = library->next_album(thumb, true);
	ui.album_thumbnail_prev2->setPixmap(thumb->cover);
}

void QPlayer::set_track(std::vector<Track>::iterator iter)
{
	bool playing = false;

	/* Are we currently playing a track? */
	if (player->state() == QMediaPlayer::PlayingState)
		playing = true;

	track = iter;

	ui.track_label->setText(QString::fromStdString(track->title));
	player->setMedia(QUrl::fromLocalFile(QString::fromStdString(track->path)));

	if (playing)
		player->play();
}

void QPlayer::on_album_clicked()
{
	std::cout << __func__ << "\n";
	if (player->state() == QMediaPlayer::PlayingState)
		player->pause();
	else
		player->play();
}

void QPlayer::on_prev_album_clicked()
{
	std::cout << __func__ << "\n";
	set_album(library->next_album(album, true));
}

void QPlayer::on_next_album_clicked()
{
	std::cout << __func__ << "\n";
	set_album(library->next_album(album));
}

void QPlayer::on_prev_track_clicked()
{
	std::cout << __func__ << "\n";
	set_track(album->next_track(track, true));
}

void QPlayer::on_next_track_clicked()
{
	std::cout << __func__ << "\n";
	set_track(album->next_track(track));
}
