# dwm version
VERSION = 6.8

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Xinerama, comment if you don't want it
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2
# OpenBSD (uncomment)
#FREETYPEINC = ${X11INC}/freetype2
#MANPREFIX = ${PREFIX}/man

# includes and libs
# pkg-config gives the flags when it finds every package of PKGS, as in the
# dev shell of flake.nix. Otherwise the fixed paths above apply. If you drop
# Xinerama above, drop xinerama from PKGS too.
PKG_CONFIG = pkg-config
PKGS = x11 xinerama xft xrender fontconfig
INCS != ${PKG_CONFIG} --cflags ${PKGS} 2>/dev/null || echo "-I${X11INC} -I${FREETYPEINC}"
LIBS != ${PKG_CONFIG} --libs ${PKGS} 2>/dev/null || echo "-L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS} -lXrender"

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
#CFLAGS   = -g -std=c99 -pedantic -Wall -O0 ${INCS} ${CPPFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
