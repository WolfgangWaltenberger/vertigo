#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(vertigo, 0.3.3, walten@hephy.oeaw.ac.at)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([Vertigo/Event/interface/Event.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# pkg-config
AC_PATH_PROG(PKG_CONFIG, [pkg-config], [no])
if test x$PKG_CONFIG = xno ; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/])
fi

if $PKG_CONFIG --atleast-pkgconfig-version 0.14 ; then
  :
else
  AC_MSG_ERROR([*** pkg-config too old; version 0.14 or better required.])
fi

# Checks for libraries.
#BOOST_REQUIRE([1.40]) ## ,AC_MSG_ERROR([couldnt find boost]))
#BOOST_FILESYSTEM([mt])
#BOOST_SYSTEM([mt])
#BOOST_SERIALIZATION([mt])
#BOOST_PROGRAM_OPTIONS([mt])
AX_BOOST_BASE([1.40.0])
AX_BOOST_FILESYSTEM
AX_BOOST_SYSTEM
AX_BOOST_SERIALIZATION
AX_BOOST_PROGRAM_OPTIONS

PKG_CHECK_MODULES(RAVE, rave-impl, [have_rave=yes], [have_rave=no])
if test x${have_rave} = xno; then
  AC_MSG_ERROR([*** Rave not found. See http://projects.hepforge.org/rave ])
fi

MAINLIB=../../Vertigo/libVertigo.la
AC_SUBST(MAINLIB)

AC_SEARCH_CLHEP([], [AC_MSG_ERROR([required clhep not found])])

# dataharvester support (need for calibration and "file" event generator)
PKG_CHECK_MODULES(DATAHARVESTER, dataharvester, [have_dataharvester=yes], [:])
AM_CONDITIONAL(HAVE_DATAHARVESTER, [test x${have_dataharvester} = xyes])

#WO_PROG_HEPMC()
AM_CONDITIONAL([HEPMC_AVAILABLE], [test "$enable_hepmc" = "yes"])

AC_SEARCH_ROOT([have_root=yes], [have_root=no])
AM_CONDITIONAL(HAVE_ROOT, [test x${have_root} = xyes])

# RaveVis ("Visualiser") Observer plugin
PKG_CHECK_MODULES(RAVEVIS, ravevis, [have_ravevis=yes], [have_ravevis=no])
# checking for jpeglib.h is an ugly workaround, we really need
# libjpeg.la ...
dnl if test x$have_ravevis = xyes ; then
dnl  AC_CHECK_HEADER(jpeglib.h,[],[have_ravevis=no])
dnl  if test x$have_ravevis = xno ; then
dnl    AC_MSG_ERROR([*** jpeglib.h (libjpeg.la) not found. I have to disable ravevis support.])
dnl  fi
dnl fi

AM_CONDITIONAL(HAVE_RAVEVIS, [test x${have_ravevis} = xyes])

# LCIO EventGenerator plugin, with no .pc file
#AC_CHECK_FILE(${LCIO}/src/cpp/include/lcio.h,[have_lcio=yes],[have_lcio=no])
#AC_SUBST([LCIO],[${LCIO}])
#AC_SUBST([LCIO_CFLAGS],[-I${LCIO}/src/cpp/include/])
#AC_SUBST([LCIO_LIBS],['-L${LCIO}/lib/ -llcio -L${LCIO}/sio/lib -lsio'])
#AM_CONDITIONAL(HAVE_LCIO, [test x${have_lcio} = xyes])

# LCIO EventGenerator plugin, with pkgconfig
have_lcio=yes
PKG_CHECK_MODULES(LCIO, lcio, [], [have_lcio=no])
#AC_CEDAR_LIBRARYANDHEADERS([LCIO], [?.?.?.?], [have_lcio=yes], [have_lcio=no])
AC_ARG_ENABLE([lcio],[AC_HELP_STRING([--disable-lcio],[disable lcio plugin])],
		[have_lcio=$enableval],[have_lcio=yes])
AM_CONDITIONAL(HAVE_LCIO, [test "x${have_lcio}" = "xyes"])

AC_ARG_ENABLE([python],
  [AC_HELP_STRING([--disable-python],[disable python interfaces])],
  [enable_python=$enableval], [enable_python=yes])

if test x$enable_python = xyes; then
  AM_PATH_PYTHON(2.3)
  AC_PROG_SWIG(1.3.21)
  SWIG_ENABLE_CXX
  SWIG_PYTHON
  if test -z "$SWIG_PYTHON_CPPFLAGS"; then
    enable_python="no"
  fi
fi
AM_CONDITIONAL([WITH_PYTHON],[test "x$enable_python" = "xyes"])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_CHECK_FUNCS([floor pow sqrt])

AC_CONFIG_FILES([
Makefile
Vertigo/Makefile
cpp_application/Makefile
tools/Makefile
VertigoHarvest/Makefile
Analysis/Makefile
Environment/Makefile
Skins/Makefile
Skins/DummySkin/Makefile
Skins/ZeroSkin/Makefile
Skins/TinySkin/Makefile
Skins/ILDSkin/Makefile
Skins/SIDSkin/Makefile
Skins/CMSSkin/Makefile
Skins/BelleSkin/Makefile
Skins/Belle2Skin/Makefile
Skins/SHiPSkin/Makefile
python/Makefile
Plugins/Makefile
Plugins/Visualiser/Makefile
Plugins/LcioEventGenerator/Makefile
Plugins/LcioWriter/Makefile
Plugins/HarvesterEventGenerator/Makefile
Plugins/HepMCEventGenerator/Makefile
Plugins/LHEEventGenerator/Makefile
Plugins/LHEObserver/Makefile
Plugins/ParticleDecayPrinter/Makefile
Plugins/VertexGun/Makefile
Plugins/TrackParameterHistogrammer/Makefile
Plugins/BTagAnalyser/Makefile
Plugins/TestAnalyser/Makefile
Plugins/BTagPrinter/Makefile
Plugins/EventWriter/Makefile
Plugins/EventHistogrammer/Makefile
Plugins/FitterTournament/Makefile
Plugins/ParameterScanner/Makefile
Plugins/EventPrinter/Makefile
Plugins/ObjectCoder/Makefile
Plugins/VertexHistogrammer/Makefile
Plugins/MagneticFieldWriter/Makefile
Plugins/FindingObserver/Makefile
Plugins/ListEventGenerator/Makefile
Plugins/CmsEventGenerator/Makefile
Plugins/KinematicsObserver/Makefile
])

AC_OUTPUT

AC_MSG_RESULT(

Configuration
=============
Compiler:               ${CXX}
CXXFLAGS:               ${CXXFLAGS}
Install prefix:         ${prefix}
Dataharvester:          ${have_dataharvester}
ROOT:                   ${have_root}
HEPMC available:        ${enable_hepmc}
HEPMC version:          ${HEPMC_VERSION}
Visualiser Plugin:      ${have_ravevis}
Lcio Plugin:            ${have_lcio}
Threads:                ${enable_threads}
Python support:         ${enable_python} ${PYTHON_VERSION}
)
