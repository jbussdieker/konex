bin_PROGRAMS = konex


# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
konex_LDFLAGS = $(all_libraries) 
noinst_HEADERS = console.h defs.h in_linux.h interface.h in_win.h konex.h map.h \
	missile.h planets.h player.h ships.h stars.h sys_main.h target.h tm_main.h \
	vector.h vid_image.h vid_linux.h vid_main.h vid_win32.h view.h vid_model.h \
	explosion.h snd_main.h ai.h config.h menu.h game.h in_main.h net_main.h vid_sdl.h \
	entity.h xml.h tinystr.h tinyxml.h
konex_SOURCES = console.cpp interface.cpp konex.cpp map.cpp missile.cpp \
	planets.cpp player.cpp ships.cpp stars.cpp sys_linux.cpp target.cpp tm_linux.cpp \
	vid_image.cpp vid_linux.cpp vid_main.cpp view.cpp vid_model.cpp explosion.cpp \
	sys_main.cpp ai.cpp config.cpp menu.cpp game.cpp in_main.cpp net_main.cpp entity.cpp \
	xml.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp item.cpp \
	spawn.cpp
