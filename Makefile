PLUGIN_NAME = fiber_recruiment

RTXI_INCLUDES =

HEADERS = fiber_rec.h\

SOURCES = fiber_rec.cpp\
          moc_fiber_rec.cpp\

LIBS = -lqwt -lrtplot

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
