ACLOCAL_AMFLAGS = -I m4 -I m4/cedar

AM_CPPFLAGS=$(BOOST_CPPFLAGS)

SUBDIRS = \
	cpp_application \
	tools \
	Environment \
	Analysis \
	VertigoHarvest \
	Vertigo \
	Plugins \
	Skins \
	python

if WITH_PYTHON
SUBDIRS+=python
endif

EXTRA_DIST = vertigo.i examples/python/raveversion.py examples/python/vertices.py examples/python/propagation.py

gridball:
	scripts/create_vertigo_gridball ${prefix}
