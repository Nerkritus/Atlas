Atlas
----------------
This library represents the bulk of functionality in the Atlas engine

Entities
----
Provides all of the basic entity types ranging from primitives like triangles, spheres, to more complex entities affected by physics, complex meshes etc.

Enums
----
Enums serving various purposes

Game
----
	Graphics
	----
	Provides graphical functions such as rendering geometry, fonts, cameras etc.

	Input
	----
	Nothing here yet

----
Managers
----
Atlas Engine follows a design pattern where the engine is comprised of compartmentalised subsystems. Each subsystem is responsible for managing a certain aspect of the engine.

	AtlasManager
	----
	This is the main manager under which all subsytems are created. This manager is responsible for building up and tearing down the window, loading/unloading game scenes and passing events down to subsystems

	Audio
	----
	This manager initialises and controls interaction with the audio library provided by FMOD. Handles loading and playing sounds

	Fonts
	----
	Handles loading and rendering of fonts

	Input
	----
	Handles all forms of user input into the system (e.g. keyboard, mouse, controllers)

	IO
	----
	Provides IO methods specific to AtlasEngine, such as providing methods to find the location of data files etc. as well as loading file formats such as 3D meshes

	Physics
	-----
	Provides physics simulation through the Bullet physics engine

	TextureManager
	----
	Handles loading and unloading of textures ensuring that the same texture is only loaded once etc.

----
Windowing
----
Provides the basic windowing functions to create the operating system window to display the engine output. An abstraction (AtlasWindow) provides cross-platform support, and native window implementations are underneath this layer
