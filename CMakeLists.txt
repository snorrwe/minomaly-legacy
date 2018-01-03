cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME SDL_TEST)
project(${PROJECT_NAME})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR})
set(SDL_TEXT_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-std=c++1z -fcxx-exceptions /EHa")
set(OUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
set(ASSETS "${PROJECT_SOURCE_DIR}/assets")

#[[ MSVC ]]

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
    if(${variable} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
    endif()
  endforeach()
endif()


#[[ Engine ]]

include_directories(src/engine/input 
                    src/common/surface 
                    src/common/texture 
                    src/common/observer 
                    src/common/color 
                    src/engine/window 
                    src/engine/renderer 
                    src/engine/core 
                    src/engine/camera 
                    )

add_library(engine ${PROJECT_SOURCE_DIR}/src/engine/window/window.cpp 
                   ${PROJECT_SOURCE_DIR}/src/engine/input/input.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/renderer/renderer.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/camera/camera.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/core/core.cpp)

add_library(common ${PROJECT_SOURCE_DIR}/src/common/surface/surface.cpp
                   ${PROJECT_SOURCE_DIR}/src/common/texture/texture.cpp)


##########################
#########  SDL  ##########
##########################

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

if(WIN32)
  set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2-2.0.7)
  set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2_image-2.0.2)
endif(WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)

#[[ Example projects ]]

add_subdirectory("${PROJECT_SOURCE_DIR}/src/game_logic/bloke")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game_logic/spritesheet")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game_logic/alpha")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game_logic/walk")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game_logic/rotation")
