set(_MINO_ENGINE MinomalyEngine)
set(_MINO_COMMON Common)
set(_MINO_COMPONENT Components)
project(${_MINO_ENGINE})

#[[ Dependencies ]]

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)
include_directories(${SDL2_TTF_DIR}/include)

#[[ Engine ]]

set(_COMMON_INCLUDES common
                     common/surface 
                     common/texture 
                     common/observer 
                     common/color 
                     common/sdl
                     common/rect
                     common/font
                     common/object_pool
                     common/vector2
                    )

set(_ENGINE_INCLUDES engine
                     engine/window 
                     engine/input 
                     engine/render_system 
                     engine/core 
                     engine/camera
                     engine/game_object
                     engine/scene
                    )

set(_COMPONENT_INCLUDES component
                        component/transform
                        component/renderer
                        component/sprite_renderer)

include_directories(${_COMMON_INCLUDES})
include_directories(${_ENGINE_INCLUDES})
include_directories(${_COMPONENT_INCLUDES})

add_library(${_MINO_ENGINE} ${PROJECT_SOURCE_DIR}/engine/window/window.cpp 
                            ${PROJECT_SOURCE_DIR}/engine/input/input.cpp
                            ${PROJECT_SOURCE_DIR}/engine/render_system/render_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/camera/camera.cpp
                            ${PROJECT_SOURCE_DIR}/engine/game_object/game_object.cpp
                            ${PROJECT_SOURCE_DIR}/engine/scene/scene.cpp
                            ${PROJECT_SOURCE_DIR}/engine/core/core.cpp)

add_library(${_MINO_COMMON} ${PROJECT_SOURCE_DIR}/common/surface/surface.cpp
                            ${PROJECT_SOURCE_DIR}/common/texture/texture.cpp
                            ${PROJECT_SOURCE_DIR}/common/sdl/sdl_subsystems.cpp
                            ${PROJECT_SOURCE_DIR}/common/font/font.cpp)

add_library(${_MINO_COMPONENT} ${PROJECT_SOURCE_DIR}/component/component.cpp
                               ${PROJECT_SOURCE_DIR}/component/sprite_renderer/sprite_renderer.cpp
                               ${PROJECT_SOURCE_DIR}/component/renderer/renderer.cpp
                               ${PROJECT_SOURCE_DIR}/component/transform/transform.cpp)

# [[ Link ]]

target_link_libraries(${_MINO_ENGINE} 
                      ${_MINO_COMMON} 
                      ${__MINO_COMPONENT}  
                      ${SDL2_LIBRARIES} 
                      "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib"
                      "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_ttf.lib")

#[[ Set exported variables ]]

set(MINO_ENGINE ${_MINO_ENGINE} PARENT_SCOPE)
set(MINO_COMMON ${_MINO_COMMON} PARENT_SCOPE)
set(MINO_COMPONENT ${_MINO_COMPONENT} PARENT_SCOPE)
set(MINO_PATH ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
set(COMMON_INCLUDES ${_COMMON_INCLUDES} PARENT_SCOPE)
set(ENGINE_INCLUDES ${_ENGINE_INCLUDES} PARENT_SCOPE)
set(COMPONENT_INCLUDES ${_COMPONENT_INCLUDES} PARENT_SCOPE)
