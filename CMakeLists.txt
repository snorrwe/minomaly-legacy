cmake_minimum_required(VERSION 3.7)

set(MASTER_NAME ENGINE)
project(${MASTER_NAME})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR})
set(SDL_TEXT_DIR ${PROJECT_SOURCE_DIR})

set(GCC_FLAGS "-std=c++17 -fcxx-exceptions")
set(MSVC_FLAGS "/EHa /std:c++latest")
set(CMAKE_CXX_FLAGS "${GCC_FLAGS} ${MSVC_FLAGS}")
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
                    src/engine/camera)

add_library(${MASTER_NAME} SHARED ${PROJECT_SOURCE_DIR}/src/engine/window/window.cpp 
                   ${PROJECT_SOURCE_DIR}/src/engine/input/input.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/renderer/renderer.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/camera/camera.cpp
                   ${PROJECT_SOURCE_DIR}/src/engine/core/core.cpp)

add_library(common ${PROJECT_SOURCE_DIR}/src/common/surface/surface.cpp
                   ${PROJECT_SOURCE_DIR}/src/common/texture/texture.cpp)


#[[ SDL ]]

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

if(WIN32)
  set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2-2.0.7)
  set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2_image-2.0.2)
  set(SDL2_TTF_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2_ttf-2.0.14)
endif(WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)
include_directories(${SDL2_TTF_DIR}/include)

#[[ Example projects ]]

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

target_link_libraries(${MASTER_NAME} common
                             ${SDL2_LIBRARIES} 
                             "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib" 
                             "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_ttf.lib"
                             )

MACRO(LINK_EXAMPLE name)
  target_link_libraries(${name} ${MASTER_NAME})

  #[[ SDL core ]]
  add_custom_command(TARGET ${name} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_if_different  
          "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2.dll" # <--this is in-file
          $<TARGET_FILE_DIR:${name}>) # <--this is out-file path

  #[[ SDL Image ]]
  add_custom_command(TARGET ${name} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_directory  
          "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
          $<TARGET_FILE_DIR:${name}>) # <--this is out-file path

  #[[ SDL TTF ]]
  add_custom_command(TARGET ${name} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_directory  
          "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
          $<TARGET_FILE_DIR:${name}>) # <--this is out-file path
ENDMACRO()

set(GAME_LOGIC_DIR ${PROJECT_SOURCE_DIR}/src/game_logic)
SUBDIRLIST(EXAMPLE_PROJECT_DIRS ${GAME_LOGIC_DIR})

FOREACH(subdir ${EXAMPLE_PROJECT_DIRS})
  add_subdirectory("${GAME_LOGIC_DIR}/${subdir}")
ENDFOREACH()
