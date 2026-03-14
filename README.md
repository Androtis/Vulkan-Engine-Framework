# Vulkan Engine Framework

## Abstract / Purpose

*Using the Vulkan Graphics API as a framework for a variety of
graphical implementations.*

I intend to implement computer graphic related ideas within this framework including The Computer Graphics Triangle, Rotations, OBJ Model Loading, Shadow Mapping, BRDFs, Ray Tracing, PBR Rendering, Screen-Space Effects (**i.e. Bloom**), etc.

I believe that the best way to learn is to implement concepts yourself, I made this with the end goal of improving my understanding of computer graphics, graphics card programming, and to keep myself consistently working on modern graphical techniques.

I choose Vulkan for this framework because the market is trending towards greater support for Vulkan in general purpose graphical programming, superseeding OpenGL. This comes with the promise of greater control over the GPU and I believe that as Vulkan becomes more widespread, the techniques implemented here will have a long-standing impact.

I created this repository to keep track of where this project will go and to organize my work, as well as demonstrate to others my work.

## Building From Source

In order to compile the code from source code you will first need:

- Your choice of C++ compiler set to **C++20**, For Windows I'd recommend [G++](https://www.mingw-w64.org/) meanwhile on MacOS or Linux you can use your own Package Manager.
- Install [Make](https://www.gnu.org/software/make/).
- Install [CMake](https://cmake.org/getting-started/).
- Build [VCPKG](https://github.com/microsoft/vcpkg) from source and set the build folder as an enviornment variable under $VCPKG_ROOT.

``` bash
# Make a safe folder to store the repo
mkdir VulkanEngineFramework
cd VulkanEngineFramework/

# Clone the repository
git clone git@github.com:Androtis/Vulkan-Engine-Framework.git
cd VulkanEngine

# Build
cmake build
cd build/
make

# Run
./VulkanEngine
```

**This README is subject to revision and the above is not finalized.**
