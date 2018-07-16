####### Atlas Engine #######

---- Current Features ----

Platforms:
	Windows

Graphics:
	2D/3D geometry					- OpenGL Support and a range of primitive shapes and loading of 3D meshes in .obj format
	camera							- Done but currently fairly basic
	2D sprites and 2D text output	- sprites supported and text implemented using FreeType2 and opengl textured sprites
	3D Meshes						- loading of meshes is possible using Wavefront .obj format. Currently only loads mesh data not materials etc.
	Instanced Geometry				- Done, entities will only be loaded once and instanced when more are created
	Dynamic Lighting				- Supports ambient + single point light
	
Audio:
	Play sounds & music				- Done, audio implemented with FMOD, currently only 2D sound

Input:
	Keyboard input					- Supports key presses, toggle keys and cooldown key presses 
	Mouse input						- Supported for mouse clicks and mouse movement (e.g. mouse look)
	
Physics:
	Gravity							- Done, physics implemented with Bullet Physics, support physics
		
Gameplay:
	Level file format 				- XML file format for loading levels
	Scenes							- Easily transition between scenes (levels)	
	
---- Roadmap ----

These are items not currently supported but plannned

Graphics:
	Basic keyframe based animations
	Shadows
	Text fade in/out
	
Audio:
	Support 3D audio 
	
Input:
	Mouse clicks on UI
	Mouse rays in 3D space
	
Physics:
	Collision Detection 			- Collisions are working through Bullet however Atlas needs the API hooks
	
Networking:
	Basic networking support
	
---- Wishlist ----

Graphics:
	Skeleton animation
	
	
	