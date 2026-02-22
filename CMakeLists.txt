cmake_minimum_required(VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Mack")

# Grab all source files automatically
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Find libraries installed via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Define the executable and its source files
add_executable(Mack ${SOURCES} "components/RendererComponent.h" "systems/RenderSystem.cpp" "systems/RenderSystem.h" "headers/Renderer.h")

# Include directories
target_include_directories(Mack PRIVATE "src" "headers" "GLAD/include" "components" "systems")

# Use C++17
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Mack PROPERTY CXX_STANDARD 17)
endif()

# Link libraries to the executable
target_link_libraries(Mack PRIVATE glfw glm::glm)