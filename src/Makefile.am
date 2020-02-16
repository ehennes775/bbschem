bin_PROGRAMS = \
	bbtest

RESOURCES_FILE = gui/bbsch.gresource.ui

GLIB_COMPILE_RESOURCES = glib-compile-resources

BUILT_SOURCES = bbsch.gresource.c

$(BUILT_SOURCES) : $(RESOURCES_FILE) $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies $(RESOURCES_FILE))
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --generate-source $<

bbtest_SOURCES = \
    gui/bbapplication.c \
    gui/bbattributeeditor.c \
    gui/bbcoloreditor.c \
    gui/bbcomponentselector.c \
    gui/bbfillstyleeditor.c \
    gui/bblinestyleeditor.c \
    gui/bbmainwindow.c \
    gui/bbpinpropertyeditor.c \
    gui/bbtextpropertyeditor.c \
	main.c \
	$(BUILT_SOURCES)

bbtest_CFLAGS = $(AM_CFLAGS) $(DEPS_CFLAGS) $(GTK_CFLAGS)
bbtest_LDADD = $(DEPS_LIBS) $(GTK_LIBS)
