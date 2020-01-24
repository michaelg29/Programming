# OpenGL Installation Process

Following instructions starting at [Learn OpenGL](https://learnopengl.com/Getting-started/OpenGL).  
Make sure everything you configure is set for **_x64_** in your IDE.

## Initial
1. Create project in IDE (These directions are for Visual Studio 2019)
2. Create directory named 'Linking' in Solution Directory
    * eg. $(SolutionDir)\Linking
3. Set Properties
    * Linker -> Input
        * Additional Dependencies
            \+ opengl32.lib
    * VC++ Directories
        * Library Directories
            \+ $(SolutionDir)\Linking\lib;
        * Include Directories
            \+ $(SolutionDir)\Linking\include;

## GLFW
1. Download package from [GLFW](https://www.glfw.org/download.html)
2. Unzip download and add files to linking directory
    * GLFW\include -> Linking\include
    * all .lib files from corresponding VS folder -> Linking\lib\GLFW
3. Add files to project directory
    * glfw3.dll from corresponding VS folder -> $(ProjectDir)
4. Set Properties
    * Linker -> Input
        * Additional Dependencies
            \+ GLFW\glfw3.lib

## GLAD
1. Download package from [GLAD](https://glad.dav1d.de/)
    * Only change these default settings:
        * API -> gl: Version 3.3
        * Options -> Generate a Loader: checked **(required)**
2. Drag include directory in download to Linking directory in solution
    * This should add the *glad* and *KHR* directories to your *Linking\include* directory
3. Drag *src* directory in download to Project directory
    * This should add the *glad.c* file to your $(ProjectDir)\src directory

## GLM
1. Find latest package from [GLM](https://github.com/g-truc/glm/tags)
    * Click on the package and download the .zip file
2. Drag the *glm* directory that is next to the other directories like *doc* from the download folder to the *Linking\include* directory
