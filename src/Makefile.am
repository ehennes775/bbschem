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
    gui/bbcellrenderercolorswatch.c \
    gui/bbcolorcombobox.c \
    gui/bbcoloreditor.c \
    gui/bbcomponentselector.c \
    gui/bbdocumentwindow.c \
    gui/bbdocumentwindowtab.c \
    gui/bbfillstyleeditor.c \
    gui/bblinestyleeditor.c \
    gui/bbmainwindow.c \
    gui/bbpinpropertyeditor.c \
    gui/bbpropertycombobox.c \
    gui/bbschematicwindow.c \
    gui/bbschematicwindowinner.c \
    gui/bbschematicwrapper.c \
    gui/bbtextpropertyeditor.c \
	main.c \
	$(BUILT_SOURCES)

bbtest_CFLAGS = $(AM_CFLAGS) $(DEPS_CFLAGS) $(GTK_CFLAGS)
bbtest_LDADD = $(DEPS_LIBS) $(GTK_LIBS)
