SUBDIRS= VertexGun EventPrinter ObjectCoder LHEEventGenerator LHEObserver ParticleDecayPrinter

if HAVE_ROOT
  SUBDIRS += TestAnalyser
endif

if HEPMC_AVAILABLE
  SUBDIRS += HepMCEventGenerator
endif

if HAVE_DATAHARVESTER
  SUBDIRS += HarvesterEventGenerator ListEventGenerator TrackParameterHistogrammer EventWriter VertexHistogrammer BTagAnalyser EventHistogrammer FitterTournament ParameterScanner FindingObserver MagneticFieldWriter BTagPrinter
if HAVE_RAVEVIS
  SUBDIRS += Visualiser
endif
endif

if HAVE_ROOT
  SUBDIRS += CmsEventGenerator
endif

if HAVE_LCIO
  SUBDIRS += LcioEventGenerator LcioWriter
endif

#if HAVE_KINEMATICS
  SUBDIRS += KinematicsObserver
#endif
