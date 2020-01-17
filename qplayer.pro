QT += multimedia \
      widgets

#QMAKE_CXXFLAGS += -DLIBRARY_PATH="storage/music"

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
