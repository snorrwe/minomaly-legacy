set(_MINO_TEST MinoTest)
project(${_MINO_TEST})

#[[ gtest ]]

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(gmock gmock_main)

#[[ Tests ]]

FILE(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/test_*.cpp")

FOREACH(subdir ${COMMON_INCLUDES} ${ENGINE_INCLUDES} ${COMPONENT_INCLUDES})
    include_directories("${MINO_PATH}/${subdir}")
ENDFOREACH()

add_executable(${_MINO_TEST} main.cpp ${TEST_SOURCES})
target_link_libraries(${_MINO_TEST} ${MINO_ENGINE} ${MINO_COMPONENT} ${MINO_COMMON} gtest gtest_main gmock gmock_main)

COPY_SDL_DLLS(${_MINO_TEST})
