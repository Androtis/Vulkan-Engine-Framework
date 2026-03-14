# CMake metadata
cmake_minimum_required(VERSION 3.10)
project(VulkanProject)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

add_executable(VulkanEngineFramework main.cpp)

# Link libraries using correct target names
target_link_libraries(VulkanEngineFramework PRIVATE 
    Vulkan::Vulkan 
    glfw                      # Changed from glfw::glfw
    glm::glm                  # Use the namespaced glm target
    GPUOpen::VulkanMemoryAllocator # Correct VMA target name
)
