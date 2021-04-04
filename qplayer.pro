QT += multimedia \
      widgets

FORMS = qplayer.ui

HEADERS = album.h \
          collection.h \
          qplayer.h

SOURCES = main.cpp \
          album.cpp \
          collection.cpp \
          qplayer.cpp

# Install the qplayer binary
target.path = /usr/bin/
INSTALLS += target

# Install the icons
icons.path = /usr/lib/qplayer/icons/
icons.files = icons/*.png
INSTALLS += icons
