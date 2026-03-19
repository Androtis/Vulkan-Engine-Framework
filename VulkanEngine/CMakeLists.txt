# CMake metadata
cmake_minimum_required(VERSION 3.10)
project(VulkanProject)

#Sets the C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(volk REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)


add_executable(VulkanEngineFramework main.cpp)

# Link libraries using correct target names
target_link_libraries(VulkanEngineFramework PRIVATE 
    Vulkan::Vulkan 
    glfw                      # Changed from glfw::glfw
    volk::volk
    glm::glm                  # Use the namespaced glm target
    GPUOpen::VulkanMemoryAllocator # Correct VMA target name
)
