Atlas engine uses the following third party dependencies which I have no supplied.

Most dependencies need to be in this dependencies directory. The only exception is FMOD.

GLEW: The OpenGL Extension Wrangler Library:  http://glew.sourceforge.net/

OpenGL Mathematics 0.9.8: http://glm.g-truc.net/0.9.8/index.html

	Copy into the glm folder

Bullet Physics 2.86: http://bulletphysics.org/wordpress/
	Note: I built bullet using vs2017 so the libs required are e.g. "LinearMath_vs2017_debug.lib". Also note that I changed the runtime library
	to Multi-Threaded DLL (/MDd or /MD). Change this under code generation in vs project properties. You will get linker errors if bullet isn't built with this set.

FMOD:
	Sound is implemented using FMOD SDK. Install it to the default install location.

Freetype 2.8.1
	For displaying text within the engine FreeType is required.
	
TinyXML 2
	File formats used by the engine are in XML format, TinyXML is used to parse them.
	
	
There are x86 and x64 versions of each dependency