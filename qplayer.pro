QT += multimedia \
      widgets

FORMS = qplayer.ui

HEADERS = album.h \
          library.h \
          qplayer.h

SOURCES = main.cpp \
          album.cpp \
          library.cpp \
          qplayer.cpp

# Install the qplayer binary
target.path = /usr/bin/
INSTALLS += target

# Install the icons
icons.path = /usr/lib/qplayer/icons/
icons.files = icons/*.png
INSTALLS += icons
