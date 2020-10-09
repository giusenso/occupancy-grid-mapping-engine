## Occupancy Grid Mapping Engine
<br/>
This is a sketch of low-level/low-resources graphic engine for robot navigation tasks based on occupancy grid mapping. Currently it has no real applications, it is just an idea for future projects.<br/>
<br/>
<p align="center">
  <img src="img/occupancy_grid_preview.gif"/>
</p>

### Software
- OS: Linux Ubuntu 20.04.1
- language: C/C++
- compiler: gcc 9.3.0
- lib: SDL2 2.0.13

directory tree:
```
.
├── compile.sh
├── gui
├── img
├── maps
│   ├── map_1.txt
│   ├── map_2.txt
│   └── map_3.txt
├── occupancy_grid.cpp
├── occupancy_grid.h
├── README.md
├── Robot.cpp
├── Robot.h
├── Tile.h
└── tiles
    ├── bmp

```
<br/>

### How to run it

Make sure to have SDL2 installed: https://wiki.libsdl.org/Installation <br/>

Navigate into the repository folder and complile everything
  ```
  bash compile.sh
  ```

Run the program
```
./gui
```

Drive the robot with keyboard arrows and *wasd* keys, or spacebar for random autonomous motion.
