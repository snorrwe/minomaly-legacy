set(_MINO_ENGINE MinomalyEngine)
set(_MINO_COMMON Common)
project(${_MINO_ENGINE})

#[[ Dependencies ]]

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)

#[[ Engine ]]

set(_COMMON_INCLUDES common
                     common/surface 
                     common/texture 
                     common/observer 
                     common/color 
                     common/sdl
                     common/rect
                    )

set(_ENGINE_INCLUDES engine
                     engine/window 
                     engine/input 
                     engine/renderer 
                     engine/core 
                     engine/camera
                    )

include_directories(${_COMMON_INCLUDES})
include_directories(${_ENGINE_INCLUDES})

add_library(${_MINO_ENGINE} ${PROJECT_SOURCE_DIR}/engine/window/window.cpp 
                   ${PROJECT_SOURCE_DIR}/engine/input/input.cpp
                   ${PROJECT_SOURCE_DIR}/engine/renderer/renderer.cpp
                   ${PROJECT_SOURCE_DIR}/engine/camera/camera.cpp
                   ${PROJECT_SOURCE_DIR}/engine/core/core.cpp)

add_library(${_MINO_COMMON} ${PROJECT_SOURCE_DIR}/common/surface/surface.cpp
                   ${PROJECT_SOURCE_DIR}/common/texture/texture.cpp
                   ${PROJECT_SOURCE_DIR}/common/sdl/sdl_subsystems.cpp
                   )

# [[ Link SDL ]]

target_link_libraries(${_MINO_COMMON} ${SDL2_LIBRARIES} 
                             "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib")
target_link_libraries(${_MINO_ENGINE} ${_MINO_COMMON})

#[[ Set exported variables ]]

set(MINO_ENGINE ${_MINO_ENGINE} PARENT_SCOPE)
set(MINO_COMMON ${_MINO_COMMON} PARENT_SCOPE)
set(MINO_PATH ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
set(COMMON_INCLUDES ${_COMMON_INCLUDES} PARENT_SCOPE)
set(ENGINE_INCLUDES ${_ENGINE_INCLUDES} PARENT_SCOPE)