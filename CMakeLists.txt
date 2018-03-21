cmake_minimum_required(VERSION 3.9)

# Options. Turn off with 'cmake -Dmyvarname=OFF'. e.g. -Dtest=OFF
option(test "Build tests." ON) # Makes boolean 'test' available.
option(examples "Build examples." ON) # Makes boolean 'examples' available.
option(ci "Is it a CI build?" OFF)

set(MASTER_NAME Minomaly CXX)
project(${MASTER_NAME})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR})
set(SDL_TEXT_DIR ${PROJECT_SOURCE_DIR})

set(SILENCED_WARNINGS "-Wno-pragmas -Wno-unused-command-line-argument -Wno-unknown-argument -Wno-deprecated-declarations -Wno-c++17-extensions")

set(CMAKE_C_FLAGS " ${SILENCED_WARNINGS}")
set(GNU_FLAGS "")
set(CLANG_FLAGS "-fcxx-exceptions ${SILENCED_WARNINGS}")
if(${ci})
    set(CLANG_FLAGS "${CLANG_FLAGS} -Werror")
    set(MSVC_FLAGS "${MSVC_FLAGS} /WX")
endif()
if(MSVC)
    set(CMAKE_CXX_FLAGS ${MSVC_FLAGS})
    message(STATUS "1")
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") # LLVM toolchain support
        message(STATUS "2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_FLAGS} -stdlib=libc++")
    endif()
else()
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        message(STATUS "3")
        set(CMAKE_CXX_FLAGS "${GNU_FLAGS}")
    else()
        message(STATUS "4")
        set(CMAKE_CXX_FLAGS ${CLANG_FLAGS})
    endif()
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
    set(SDL2_ROOT ${PROJECT_SOURCE_DIR}/lib/sdl/windows-vs)
    set(SDL2_DIR ${SDL2_ROOT}/SDL2-2.0.7)
    set(SDL2_IMAGE_DIR ${SDL2_ROOT}/SDL2_image-2.0.2)
    set(SDL2_TTF_DIR ${SDL2_ROOT}/SDL2_ttf-2.0.14)
    set(SDL2_MIXER_DIR ${SDL2_ROOT}/SDL2_mixer-2.0.2)
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

add_subdirectory(minomaly)

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

    set(GTEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/lib/googletest)

    enable_testing()
    add_subdirectory("${PROJECT_SOURCE_DIR}/testing")
endif()
