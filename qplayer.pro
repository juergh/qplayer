QT += multimedia \
      widgets

LIBS += -lstdc++fs

FORMS = qplayer.ui

HEADERS = album.h \
          library.h \
          qplayer.h \
          track.h

SOURCES = album.cpp \
          library.cpp \
          main.cpp \
          qplayer.cpp \
          track.cpp

# Install the qplayer binary
target.path = /usr/bin/
INSTALLS += target

# Install the icons
icons.path = /usr/lib/qplayer/icons/
icons.files = icons/*.png
INSTALLS += icons
