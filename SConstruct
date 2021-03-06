# SConstruct - root SCons script for SuperSpaceShipsShootingStuff

from scons_common import *
import os
import glob

# Tries to detect the path to the installation of Qt with
# the highest version number
def detectLatestQtDir():
	if sys.platform.startswith("linux"):
		# Simple check: inspect '/usr/share/qt' and then '/usr/local/Trolltech/*'
		paths = glob.glob('/usr/share/*')
		if len(paths):
			for p in paths:
				if p == "/usr/share/qt":
					return p
		paths = glob.glob('/usr/local/Trolltech/*')
		if len(paths):
			paths.sort()
			return paths[-1]
		else:
			return os.environ.get("QTDIR")
	else:
		# Simple check: inspect only 'C:\Qt'
		paths = glob.glob('C:\\Qt\\*')
		if len(paths):
			paths.sort()
			return paths[-1]
		else:
			return os.environ.get("QTDIR")


   ###### Sconstruct ######


env = createEnvironment(generate_help=True)	# generate the help

qtdir = detectLatestQtDir()
if not qtdir:
	qtdir = os.environ.get('QT4DIR')
	if not qtdir :
		print "You must define the QT4DIR environment variable"
		exit(1)

if os.name == 'posix':
	env['LINK'] = 'g++'	# We got some problem of gcc being used instead of g++ when using VariantDir()...

env['ENV']['PKG_CONFIG_PATH'] = os.path.join(qtdir, 'lib/pkgconfig')
env['QT4DIR'] = qtdir
env['QT4_DEBUG'] = 1
env.Tool('qt4')
env.EnableQt4Modules([
		'QtGui',
		'QtCore',
		'QtOpenGL',
		'QtXml'
	])

env.Append(LFLAGS=['-Wl'])
env.Append(RPATH=[os.path.join(qtdir, 'lib')])

### Libs
# core
env.Append(LIBS=['-lpthread','-lm'])
# gui
if sys.platform.startswith("linux"):
	env.Append(LIBS=['-lXext','-lX11'])
# opengl
if env['PLATFORM'] == 'win32':
	env.Append(LIBPATH=['dep/glew/lib-win32'])
	env.Append(CPPPATH = ['dep/glew/include'])
	env.Append(LIBS=['-lopengl32','-lglew32','-lglu32'])
else:
	env.Append(LIBS=['-lGL','-lGLU','-lGLEW'])
# openal
if env['PLATFORM'] == 'win32':
	env.Append(CPPPATH=['dep/openal/include'])
	env.Append(LIBPATH=['dep/openal/lib-win32'])
	env.Append(LIBS=['-lOpenAL32','-lalut'])
else:
	env.Append(LIBS=['-lopenal','-lalut'])
# assimp
if env['PLATFORM'] == 'win32':
	env.Append(LIBPATH=['dep/assimp/lib-win32'])
else:
	env.Append(LIBPATH=['dep/assimp/lib'])
env.Append(CPPPATH = ['dep/assimp/include'])
env.Append(LIBS=['-lassimp'])
# bullet
env.Append(LIBS=
		['BulletSoftBody',
		'BulletDynamics',
		'BulletCollision',
		'LinearMath'])
if env['PLATFORM'] == 'win32':
	env.Append(CPPPATH=['dep/bullet/src'])
	env.Append(LIBPATH=['dep/bullet/lib-win32'])
else:
	env.ParseConfig('pkg-config bullet --cflags --libs')

SConscript('SConscript_S5engine','env')
SConscript('SConscript_demo_game','env')
