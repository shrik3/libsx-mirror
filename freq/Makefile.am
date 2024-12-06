AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_HEADERS=freq.h

check_PROGRAMS = freq simple_freq

freq_SOURCES = main.c
freq_LDADD = ../src/libsx.la libfreq.a

simple_freq_SOURCES = main.c
simple_freq_LDADD = ../src/libsx.la libsimplefreq.a

check_LIBRARIES=libfreq.a libsimplefreq.a

libfreq_a_SOURCES=dirlist.c freq.c simple_freq.c
libsimplefreq_a_SOURCES= simple_freq.c

EXTRA_DIST = makefile.no_auto
