# Raycasting
A pseudo-3D engine built upon Wolf3D's ray-casting algorithm

## Features
* [x] Wall renderer
* [x] Texture mapping
* [x] Level editor
* [x] Direct lighting
* [x] Data serialization
* [x] Entity-Component-System
* [ ] Floor/ceiling renderer
* [ ] Reflections
* [ ] Indirect lighting

## How to compile

* Download [CMake](https://cmake.org/download/)
* Clone using `git clone --recursive https://github.com/novosadkry/Raycasting.git`

### Windows

* Open a new command prompt inside `Raycasting` folder
* Run `mkdir build && cd build && cmake ..`
* Open the generated .sln file inside `build` folder with Visual Studio
* Build and run the project

### Linux

* Open a new shell inside `Raycasting` folder
* Run `mkdir build && cd build && cmake ..`
* Run `cd build && make`
* Open the executable inside `bin` folder

## Dependencies
* [SFML](https://www.sfml-dev.org/)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml)
* [cereal](https://github.com/USCiLab/cereal)
* [entt](https://github.com/skypjack/entt)
