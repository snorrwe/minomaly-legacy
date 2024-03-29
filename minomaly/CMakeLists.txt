
set(_MINO_ENGINE MinomalyEngine)
set(_MINO_COMMON MinoCommon)
set(_MINO_COMPONENT MinoComponents)
set(_MINO_SERVICES MinoServices)
project(${_MINO_ENGINE})

#[[ Dependencies ]]

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)
include_directories(${SDL2_TTF_DIR}/include)
include_directories(${SDL2_MIXER_DIR}/include)

#[[ Engine ]]

set(_ENGINE_INCLUDES engine
                     engine/window 
                     engine/input_system 
                     engine/render_system 
                     engine/core 
                     engine/camera
                     engine/sdl_subsystems
                     engine/physics_system
                     engine/application
                     engine/asset_system
                     engine/audio_system)

set(_COMMON_INCLUDES common
                     common/surface 
                     common/texture 
                     common/observer 
                     common/color 
                     common/audio
                     common/rect
                     common/sprite_animation
                     common/font
                     common/quadtree
                     common/bounding_box
                     common/json
                     common/asset
                     common/matrix
                     common/object_pool
                     common/vector2)

set(_COMPONENT_INCLUDES component
                        component/transform
                        component/renderer
                        component/game_object
                        component/rigidbody
                        component/audio_player
                        component/collider
                        component/camera
                        component/box_collider
                        component/sprite_animator
                        component/sprite_renderer)

set(_SERVICE_INCLUDES services
                      services/time_service
                      services/services
                      services/work_service
                      services/log_service)

include_directories(${_COMMON_INCLUDES})
include_directories(${_ENGINE_INCLUDES})
include_directories(${_COMPONENT_INCLUDES})
include_directories(${_SERVICE_INCLUDES})

add_library(${_MINO_ENGINE} ${PROJECT_SOURCE_DIR}/engine/window/window.cpp 
                            ${PROJECT_SOURCE_DIR}/engine/input_system/input_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/render_system/render_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/camera/camera.cpp
                            ${PROJECT_SOURCE_DIR}/engine/application/application.cpp
                            ${PROJECT_SOURCE_DIR}/engine/physics_system/physics_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/asset_system/asset_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/audio_system/audio_system.cpp
                            ${PROJECT_SOURCE_DIR}/engine/sdl_subsystems/sdl_subsystems.cpp
                            ${PROJECT_SOURCE_DIR}/engine/core/core.cpp)

add_library(${_MINO_COMMON} ${PROJECT_SOURCE_DIR}/common/surface/surface.cpp
                            ${PROJECT_SOURCE_DIR}/common/texture/texture.cpp
                            ${PROJECT_SOURCE_DIR}/common/bounding_box/bounding_box.cpp
                            ${PROJECT_SOURCE_DIR}/common/font/font.cpp
                            ${PROJECT_SOURCE_DIR}/common/audio/audio.cpp)

add_library(${_MINO_COMPONENT} ${PROJECT_SOURCE_DIR}/component/component.cpp
                               ${PROJECT_SOURCE_DIR}/component/sprite_renderer/sprite_renderer.cpp
                               ${PROJECT_SOURCE_DIR}/component/renderer/renderer.cpp
                               ${PROJECT_SOURCE_DIR}/component/game_object/game_object.cpp
                               ${PROJECT_SOURCE_DIR}/component/sprite_animator/sprite_animator_component.cpp
                               ${PROJECT_SOURCE_DIR}/component/camera/camera_component.cpp
                               ${PROJECT_SOURCE_DIR}/component/audio_player/audio_player.cpp
                               ${PROJECT_SOURCE_DIR}/component/collider/collider.cpp
                               ${PROJECT_SOURCE_DIR}/component/box_collider/box_collider.cpp
                               ${PROJECT_SOURCE_DIR}/component/rigidbody/rigidbody.cpp
                               ${PROJECT_SOURCE_DIR}/component/transform/transform.cpp)

add_library(${_MINO_SERVICES} ${PROJECT_SOURCE_DIR}/services/services/services.cpp
                              ${PROJECT_SOURCE_DIR}/services/work_service/work_service.cpp
                              ${PROJECT_SOURCE_DIR}/services/log_service/log_service.cpp)

set_property(TARGET ${_MINO_ENGINE} PROPERTY CMAKE_CXX_STANDARD 17)
set_property(TARGET ${_MINO_COMMON} PROPERTY CMAKE_CXX_STANDARD 17)
set_property(TARGET ${_MINO_COMPONENT} PROPERTY CMAKE_CXX_STANDARD 17)
set_property(TARGET ${_MINO_SERVICES} PROPERTY CMAKE_CXX_STANDARD 17)

# [[ Link ]]

target_link_libraries(${_MINO_ENGINE}
                      ${_MINO_COMMON} 
                      ${_MINO_COMPONENT} 
                      ${_MINO_SERVICES} 
                      ${SDL2_LIBRARIES} 
                      "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib" 
                      "${SDL2_MIXER_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_mixer.lib"
                      "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_ttf.lib")

#[[ Set exported variables ]]

set(MINO_PATH ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
set(MINO_ENGINE ${_MINO_ENGINE} PARENT_SCOPE)
set(MINO_INCLUDES ${_COMMON_INCLUDES} ${_ENGINE_INCLUDES} ${_COMPONENT_INCLUDES} ${_SERVICE_INCLUDES} PARENT_SCOPE)
