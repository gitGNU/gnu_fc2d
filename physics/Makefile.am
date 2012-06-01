noinst_LTLIBRARIES = libphysics.la

physicsinclude_HEADERS = physics.h
physicsincludedir = $(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/physics/

libphysics_la_SOURCES = \
	physics.cpp \
	$(physicsinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/physics/