# wrapper for vertigo module

import sys, ctypes

# this should only fail on windows platforms... fixme
__old_dlopen_flags = sys.getdlopenflags()
sys.setdlopenflags(__old_dlopen_flags | ctypes.RTLD_GLOBAL) 

from vertigo import *
__doc__ = vertigo.__doc__

# restore old dlopenflags
sys.setdlopenflags(__old_dlopen_flags)
