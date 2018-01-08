cmake_minimum_required(VERSION 2.8)

# Options. Turn off with 'cmake -Dmyvarname=OFF'. e.g. -Dtest=OFF
option(test "Build tests." ON) # Makes boolean 'test' available.
option(examples "Build examples." ON) # Makes boolean 'examples' available.

set(MASTER_NAME Minomaly CXX)
project(${MASTER_NAME})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR})
set(SDL_TEXT_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_C_FLAGS NULL)
set(GCC_FLAGS "-std=c++1z -fcxx-exceptions")
set(MSVC_FLAGS "/EHa /std:c++latest")
if(WIN32)
  set(CMAKE_CXX_FLAGS ${MSVC_FLAGS})
else()
  set(CMAKE_CXX_FLAGS ${GCC_FLAGS})
endif()
set(OUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
set(ASSETS "${PROJECT_SOURCE_DIR}/examples/assets")

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
  set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/lib/SDL2_mixer-2.0.2)
endif(WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)
include_directories(${SDL2_TTF_DIR}/include)
include_directories(${SDL2_MIXER_DIR}/include)

MACRO(COPY_SDL_DLLS name)
      # SDL core
      add_custom_command(TARGET ${name} POST_BUILD  
          COMMAND ${CMAKE_COMMAND} -E copy_if_different  
              "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2.dll" # <--this is in-file
              $<TARGET_FILE_DIR:${name}>) # <--this is out-file path

      # SDL Image
      add_custom_command(TARGET ${name} POST_BUILD  
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
              $<TARGET_FILE_DIR:${name}>) # <--this is out-file path
      
      # SDL TTF
      add_custom_command(TARGET ${name} POST_BUILD   
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
              $<TARGET_FILE_DIR:${name}>) # <--this is out-file path

      # SDL_Mixer
      add_custom_command(TARGET ${name} POST_BUILD   
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_MIXER_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
              $<TARGET_FILE_DIR:${name}>) # <--this is out-file path
ENDMACRO()

#[[ Engine ]]

add_subdirectory(src)

#[[ Example projects ]]

if(examples)

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

  MACRO(LINK_EXAMPLE name)

    #[[ Includes ]]

    FOREACH(subdir ${MINO_INCLUDES})
      include_directories("${MINO_PATH}/${subdir}")
    ENDFOREACH()

    #[[ Link ]]

    target_link_libraries(${name} ${MINO_ENGINE})

    #[[ Copy DLLs ]]

    COPY_SDL_DLLS(${name})

  ENDMACRO()

  set(GAME_LOGIC_DIR ${PROJECT_SOURCE_DIR}/examples/src)
  SUBDIRLIST(EXAMPLE_PROJECT_DIRS ${GAME_LOGIC_DIR})

  FOREACH(subdir ${EXAMPLE_PROJECT_DIRS})
    add_subdirectory("${GAME_LOGIC_DIR}/${subdir}")
  ENDFOREACH()
endif()

#[[ TESTS ]]

if(test)

  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  #[[ gtest ]]

  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(${PROJECT_SOURCE_DIR}/lib/googletest)
   
  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_subdirectory("${PROJECT_SOURCE_DIR}/testing")
endif()
