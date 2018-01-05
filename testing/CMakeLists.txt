set(_MINO_TEST MinoTest)
project(${_MINO_TEST})

#[[ gtest ]]

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(gmock gmock_main)

#[[ Tests ]]

set(TEST_DIRS engine/core)

include_directories(${TEST_DIRS})

set(TEST_SOURCES ${TEST_DIRS}/test_core.cpp)

FOREACH(subdir ${COMMON_INCLUDES} ${ENGINE_INCLUDES})
    include_directories("${MINO_PATH}/${subdir}")
ENDFOREACH()

add_executable(${_MINO_TEST} main.cpp ${TEST_SOURCES})
target_link_libraries(${_MINO_TEST} ${MINO_ENGINE} gtest gtest_main gmock gmock_main)

COPY_SDL_DLLS(${_MINO_TEST})
