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
    gui/bbboxbuilder.c \
    gui/bbcellrenderercolorswatch.c \
    gui/bbcirclebuilder.c \
    gui/bbcolorcombobox.c \
    gui/bbcoloreditor.c \
    gui/bbcomponentselector.c \
    gui/bbcomponentselectorplugin.c \
    gui/bbdocumentwindow.c \
    gui/bbdocumentwindowtab.c \
    gui/bbdrawingtool.c \
    gui/bbfillstyleeditor.c \
    gui/bbint32combobox.c \
    gui/bbitembuilder.c \
    gui/bbitembuildtool.c \
    gui/bblinebuilder.c \
    gui/bblinestyleeditor.c \
    gui/bbmainwindow.c \
    gui/bbpinpropertyeditor.c \
    gui/bbpropertycombobox.c \
    gui/bbschematicwindow.c \
    gui/bbschematicwindowinner.c \
    gui/bbschematicwrapper.c \
    gui/bbtextpropertyeditor.c \
    gui/bbtoolpalette.c \
    lib/bbabsolutemoveto.c \
    lib/bbangle.c \
    lib/bbbounds.c \
    lib/bbboundscalculator.c \
    lib/bbcoord.c \
    lib/bbgraphicarc.c \
    lib/bbgraphicbox.c \
    lib/bbgraphiccircle.c \
    lib/bbgraphicline.c \
    lib/bbgraphicpath.c \
    lib/bbitemparams.c \
    lib/bbitemrenderer.c \
    lib/bbpathcommand.c \
    lib/bbschematic.c \
    lib/bbschematicitem.c \
    lib/bbsweep.c \
	main.c \
	$(BUILT_SOURCES)

bbtest_CFLAGS = $(AM_CFLAGS) $(DEPS_CFLAGS) $(GTK_CFLAGS)
bbtest_LDADD = $(DEPS_LIBS) $(GTK_LIBS) -lm
