set(_MINO_TEST MinoTest)
project(${_MINO_TEST})

#[[ gtest ]]

set(gtest_include_dirs "${GTEST_ROOT_DIR}/googletest"
                       "${GTEST_ROOT_DIR}/googletest/include"
                       "${GTEST_ROOT_DIR}/googlemock"
                       "${GTEST_ROOT_DIR}/googlemock/include")


include_directories(${gtest_include_dirs})
 
add_library(gtest STATIC "${GTEST_ROOT_DIR}/googletest/src/gtest-all.cc"
                         "${GTEST_ROOT_DIR}/googletest/src/gtest_main.cc")
add_library(gmock STATIC "${GTEST_ROOT_DIR}/googlemock/src/gmock-all.cc"
                         "${GTEST_ROOT_DIR}/googlemock/src/gmock_main.cc")


#[[ Tests ]]

FILE(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/test_*.cpp")

FOREACH(subdir ${MINO_INCLUDES})
    include_directories("${MINO_PATH}/${subdir}")
ENDFOREACH()

add_executable(${_MINO_TEST} main.cpp ${TEST_SOURCES})
target_link_libraries(${_MINO_TEST} ${MINO_ENGINE} gtest gmock)

COPY_SDL_DLLS(${_MINO_TEST})

message(STATUS ${GTEST_ROOT_DIR})
