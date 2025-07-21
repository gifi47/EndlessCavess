**Endless Cavess**

This is a voxel game with random generated world written in C++ and OpenGL

![There should be a screenshot](/media/screenshots/screenshot.png)

**Build guide**

*WINDOWS:*

1) install dependences: glew, glfw, opengl, soil, glm

2) run build_lib.bat

3) run build.bat or debug.bat

*LINUX:*

1) install dependences:

    1.1. libraries: glew, glfw, opengl, soil, glm

    1.2. programs: make

2) run make

**Additional info**

You can specify world name when launching executable from console: EndlessCaves.exe world1

Tested with msys2 ucrt64 g++ compiler on Windows 10 x64

Tested with g++ on Linux Fedora 38

*Controls:*

`WASD` - move

`Shift` - speedup while pressed

`Alt` - extreme speed up while pressed

`F` - toggle flying mode

`Space` - jump

`Mouse Left Click` - destroy block

`Mouse Right Click` - place block

`Arrow UP` - Select block that are you looking at

`Arrow Left` - Select previous block by Id

`Arrow Right` - Select next block by Id

`Arrow Down` - Load all nearby chunks in a 6x1x6 volume

`-` - Unload distant/not visible chunks

`Backspace` - Set all chunks isDirty=true, so when chunk update triggers all chunks gonna be updated. Great for fixing lighting on chunk borders.
