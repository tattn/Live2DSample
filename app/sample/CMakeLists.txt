# ---- #
# META #
# ---- #

cmake_minimum_required(VERSION 3.6)
include(ExternalProject)


# ------------ #
# USER OPTIONS #
# ------------ #

# Path to native Cubism Core.
set(CSM_CORE_DIRECTORY "../../Core" CACHE STRING "Path to Live2D Cubism Core for native development.")


# ----------------------- #
# OPTIONS INTERNALIZATION #
# ----------------------- #

# Find Cubism Core root and include directory.
if (NOT IS_ABSOLUTE ${CSM_CORE_DIRECTORY})
  set(_CSM_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${CSM_CORE_DIRECTORY})
else ()
  set(_CSM_CORE_DIR ${CSM_CORE_DIRECTORY})
endif ()


# Detect desktop.
if (NOT ANDROID AND NOT IOS AND NOT EMSCRIPTEN AND NOT RPI)
  set(_DESKTOP ON)
endif ()


# Set glad flag on desktop.
if (_DESKTOP)
  set(_USE_GLAD ON)


# ... and OpenGL ES 2.0 flag else.
else ()
  set(_USE_GLES20 ON)
endif ()


# -------------------- #
# OPTIONS SANITIZATION #
# -------------------- #

# Make sure Core directory exists.
if (NOT EXISTS "${_CSM_CORE_DIR}")
  message(FATAL_ERROR "[Live2D Cubism Components] Live2D Cubism Core directory not found.")
endif ()


# --------------------------- #
# DEPENDENCIES INITIALIZATION #
# --------------------------- #

# Running CMake for the first time will trigger download of dependencies.
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/vendor/SDL2-2.0.5/CMakeLists.txt")

  # Download SDL 2.
  ExternalProject_Add(Sdl
    URL "https://www.libsdl.org/release/SDL2-2.0.5.zip"
    

    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/vendor/SDL2-2.0.5"


    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND "")


  # Download stb.
  ExternalProject_Add(Stb
    GIT_REPOSITORY "https://github.com/nothings/stb"
    GIT_TAG "master"
    

    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/vendor/stb-master"


    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND "")


# Re-run of CMake will build actual sample.
else ()


# ------------------ #
# LIVE2D CUBISM CORE #
# ------------------ #

add_subdirectory(${_CSM_CORE_DIR} Core)


# ------------------------ #
# LIVE2D CUBISM COMPONENTS #
# ------------------------ #

# Set options.
set(CSM_COMPONENTS_CORE_INCLUDE_DIRECTORY ${CSM_CORE_INCLUDE_DIR} CACHE STRING "" FORCE)
set(CSM_COMPONENTS_BUILD_GL_RENDERER ON CACHE BOOL "" FORCE)


if (_DESKTOP)
  set(CSM_COMPONENTS_GL_H "${CMAKE_CURRENT_LIST_DIR}/vendor/glad/include/glad/glad.h" CACHE STRING "" FORCE)
endif ()


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../ Components)


# ---- #
# SDL2 #
# ---- #

# Set options.
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)


# Build minimal SDL2 on Android.
if (ANDROID)
  set(SDL_AUDIO      OFF CACHE BOOL "" FORCE)
  set(SDL_ATOMIC     OFF CACHE BOOL "" FORCE)
  set(SDL_CPUINFO    OFF CACHE BOOL "" FORCE)
  set(SDL_FILE       OFF CACHE BOOL "" FORCE)
  set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
  set(SDL_RENDER     OFF CACHE BOOL "" FORCE)
  set(SDL_THREADS    OFF CACHE BOOL "" FORCE)
  set(PTHREADS       OFF CACHE BOOL "" FORCE)
endif ()


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/SDL2-2.0.5)


# ------ #
# SAMPLE #
# ------ #

# Set include directories.
set(_INCLUDE_DIRS 
  ${CSM_CORE_INCLUDE_DIR}
  ${CSM_COMPONENTS_INCLUDE_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/vendor/SDL2-2.0.5/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/stb-master)


if (_USE_GLAD)
  list(APPEND _INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/vendor/glad/include)
endif ()


# Set source files.
set(_SRC_FILES
  ${CMAKE_CURRENT_LIST_DIR}/src/HelloCubism.c

  ${CMAKE_CURRENT_LIST_DIR}/src/Allocation.c
  ${CMAKE_CURRENT_LIST_DIR}/src/App.c
  ${CMAKE_CURRENT_LIST_DIR}/src/Asset.c)


if (_USE_GLAD)
  list(APPEND _SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/vendor/glad/src/glad.c)
endif ()


# Set link libraries.
set(_LIBS
  ${CSM_CORE_LIBS}
  ${CSM_COMPONENTS_LIBS}
  SDL2-static SDL2main)


if (_USE_GLAD)
  if (NOT WIN32)
    find_package(OpenGL REQUIRED)


    list(APPEND _LIBS ${OPENGL_LIBRARIES})
  endif ()
elseif (_USE_GLES20)
  list(APPEND _LIBS GLESv2)
endif()


# Set definitions.
if (_USE_GLAD)
  set(_DEFINITIONS -D_USE_GLAD)
else (_USE_GLES20)
  set(_DEFINITIONS -D_USE_GLES20)
endif ()


# Configure library/executable.
if (ANDROID)
  add_library(HelloCubism SHARED ${_SRC_FILES})
else ()
  add_executable(HelloCubism ${_SRC_FILES})
endif ()


target_compile_definitions(HelloCubism PRIVATE ${_DEFINITIONS})
target_include_directories(HelloCubism PRIVATE ${_INCLUDE_DIRS})


# HACK  Prevent link errors on Android
if (ANDROID)
  target_link_libraries(HelloCubism PRIVATE ${_LIBS} EGL GLESv2 android log)
else ()
  target_link_libraries(HelloCubism PRIVATE ${_LIBS})
endif ()


if (CSM_CORE_DEPS)
  add_dependencies(HelloCubism ${CSM_CORE_DEPS})
endif ()


# Copy assets on desktop.
if (_DESKTOP)
  add_custom_command(TARGET HelloCubism
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR})
endif ()


endif ()
