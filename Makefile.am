AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src freq
EXAMPLE_DIRS = demo0 demo1 demo2 demo3 demo4 bezier controlbox creq frac \
   multireq skel xmore xrootbg draw_demo 


# pcurve works only with experimental features
EXTRA_DIST = $(EXAMPLE_DIRS) pcurve Makefile.no_auto docs libsx_defs \
 CHANGES HELP HINTS LICENSE

pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA = libsx.pc

# tests don't use srcdir, so cannot be run out of source
check-local:
	if test ! -f libsx_defs; then exit 0; fi; \
	for dir in $(EXAMPLE_DIRS); do \
	  test -d $$dir || cp -pr $(srcdir)/$$dir .; \
	  (cd $$dir && make -f Makefile.no_auto) || exit 1; \
	done

clean-local:
	for dir in $(EXAMPLE_DIRS); do \
	  if test -d $$dir; then (cd $$dir && make -f Makefile.no_auto clean) || exit 1; fi; \
	done

# clean examples and remove CVS dirs before doing the archive
dist-hook: clean-local
	for dir in $(EXAMPLE_DIRS) pcurve docs docs/html docs/text; do \
	  d=$(distdir)/$$dir; \
	  if test -d $$d -a -d $$d/CVS; then rm -fr $$d/CVS; fi; \
	done
