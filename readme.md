# TERRA
This is a GitHub repository about the game "Terra".

# What is Terra?
Terra is a 2D game with a tile based map. It's focused on survival, automation and travelling to new worlds.

# How is the Terra file structure organized?
The entirety of Terra is contained in the "Terra" folder. This folder is structured to have 3 subfolders: libs, resources and src.  
It also includes various utility files like cmake, the license and other Terra.* files.

[`data`](#the-data-folder) contains all the data saved by the game. For now, this is only the region files.  
[`libs`](#the-libs-folder) contains the third-party or external libraries used by this project.  
[`resources`](#the-resources-folder) contains the files that are not c++ code and made by us. This includes images and shaders.  
[`src`](#the-src-folder) contains the actual code run by the program. This folder contains the main and terra files. 
It also contains sub-folders for organization.


## The `data` folder
This folder is meant to store all the data that Terra saves.  
For now, this folder only contains one sub-folder called "regions". In the future, this folder will store the player data (position, health, inventory), entity data, achievements (if we wish to add) and world data (not only region files but also time and seed).
This folder contains:
[`regions`](#the-regions-folder) which contains the saved region files.
### The `regions` folder
This folder is meant to store all the `region_<x>_<y>.dat`. These files are clumps of 16x16 chunks that get stored to disk so they don't have to get regenerated all the time.

## The `libs` folder
This is a very simple folder. All it contains is:  
`glad` this is the OpenGL loading library. You can download it [here](https://glad.dav1d.de). **This website does _not_ download or include a cmake file for this library!**  
`glfw` this is the window management library. You can download it [here](https://www.glfw.org).  
`glm` this is the math library for c++ applications based on GLSL. You can download it [here](https://github.com/g-truc/glm).  
`stb_image` this is the image loading library. You can download it [here](https://github.com/nothings/stb/blob/master/stb_image.h). **Note that we only use _one_ file because we only need to _read_ from images!**

## The `resources` folder
This folder contains assets and non c++ code. There are the vertex shader, the tile shader, the player shader, the game icon, the player atlas, resources file for glfw and the tile atlas.

## The `src` folder
This is the most complex folder by far. It contains all the code!  
There are three files in this folder: `Main.cpp`, `Terra.cpp` and `Terra.h`.  
This folder also contains four sub-folders used for organization: [`entity`](#the-entity-folder), [`misc`](#the-misc-folder), [`renderer`](#the-renderer-folder) and [`world`](#the-world-folder).
### The `entity` folder
This folder contains code for the games entities/entity system.  
It contains a general "Entity" class and specific code for the player. In the future, this folder will contain various files for every entity in the game.
### The `misc` folder
This folder stores miscellaneous files and utilities.  
It contains: program constants, the logger, rng, OS dependant code and utility functions.
### The `renderer` folder
This folder has all the code responsible for rendering stuff to the screen.  
It contains the camera, a "Renderer" class for rendering quads, a "Shader" helper class, a shader functions helper, a texture helper and a "Window" class.
### The `world` folder
This folder contains the code required for storing, loading, managing and generating the Terra world.  
It contains: a "Tile" class used for all tiles in the game, an "AutoTile" class for all the tiles in the game that connect to each other, a registry to manage various tile data, a "Chunk" class to subdivide the world and the main "World" class.

# How does Terra work?
e  
this section needs to be written by ppl who actually know how Terra works at a lower level
