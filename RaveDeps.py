#!/usr/bin/env python
# find Rave dependencies

# input: Rave source directory

# Scan Rave directory for header files
# Scan source files in and below current directory for dependencies.
# Drop non-Rave dependencies
# Build dependency tree for remaining Rave headers
 
import string, os, sys

if len(sys.argv) < 2:
	print 'Usage: python RaveDeps.py [path to rave source]'
	sys.exit()

raveDir = sys.argv[1]
if not raveDir.endswith('/'): raveDir += '/'
print "Using Rave headers from ", raveDir

# Collect all header files from the given Rave directory and parse their name to */*/interface/*.h
raveHeaders = [ reduce( (lambda x, y: x+'/'+y), x.strip().split('/')[-4:] ) for x in os.popen(r'find %s -name "*.h" -print' % raveDir).readlines() ]

# Get all *.cc and *.h files in the current directory
findCmd = reduce( (lambda x, y: x + ' && ' + y), [ 'find . -name "*.%s" -print' % x for x in ['h', 'cc', 'cpp'] ] )
currFiles = [ x.strip() for x in os.popen( findCmd ).readlines() ]

raveDict = {}
depsDict = {}
for x in raveHeaders: raveDict[x] = x
	
# Returns a list of dependencies for a given filename
def DepChecker( filename ):
	currDeps = [ x.strip() for x in os.popen(r"grep '#include' %s" % filename).readlines() ]
	currDeps = filter( (lambda x: len( x.split('"') ) > 2), currDeps )
	currDeps = [ x.split('"')[1] for x in currDeps ]
	return currDeps
	
# Collects all dependencies of Rave headers in the current directory and stores them in the depsDict dictionary
for filename in currFiles:
	currDeps = DepChecker(filename)
	for dep in currDeps:
		if raveDict.has_key( dep ):
			depsDict[dep] = dep

# Updates the depsDict with the full-depth dependencies of a given dependency
def DepTreeBuilder( raveDict, depsDict, dep ):
	currDeps = DepChecker(raveDir + dep)
	for newdep in currDeps:
		if raveDict.has_key( newdep ) and ( not depsDict.has_key( newdep ) ):
			depsDict[newdep] = newdep		
			depsDict = DepTreeBuilder( raveDict, depsDict, newdep )
	return depsDict

# Builds the full dependency tree by issuing DepTreeBuilder for all "first-Level" aka direct dependencies
firstLevel = list( depsDict.iterkeys() )
firstLevel.sort()
print 'First-level dependencies:'
for dep in firstLevel:
	print ' ',dep
	depsDict = DepTreeBuilder( raveDict, depsDict, dep )

# Print all unused Rave headers
privateHeaders = []
publicHeaders  = []
for dep in raveDict.iterkeys():
	if depsDict.has_key( dep ):
		publicHeaders  += [dep]
	else:
		privateHeaders += [dep]
		
privateHeaders.sort()
publicHeaders.sort()

print 'Private Rave headers:'
for x in privateHeaders:
	print ' ', x

print 'Public Rave headers:'
for x in publicHeaders:
	print ' ', x
	
# Write all public dependencies to a text file for usage with the scons installation routine
print 'Saving public header list ... ',
filePublic = file( raveDir + 'public.txt', 'w' )
filePublic.writelines( [ x + '\n' for x in publicHeaders ] )
filePublic.close()
print 'ok'
