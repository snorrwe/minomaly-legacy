get_filename_component(GAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" GAME_NAME ${GAME_NAME})
project(${GAME_NAME})

#[[ Game ]]
include_directories(src/game_logic/${GAME_NAME})

#[[ Game executables ]]
add_executable(${GAME_NAME} ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/${GAME_NAME}.cpp)
target_link_libraries(${GAME_NAME} engine common)

add_custom_command(TARGET ${GAME_NAME} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_directory  
          ${ASSETS}/${GAME_NAME}
          $<TARGET_FILE_DIR:${GAME_NAME}>/assets/${GAME_NAME})

if(WIN32)
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS})
  include_directories(${SDL2_IMAGE_DIR}/include)
  target_link_libraries(${GAME_NAME} ${SDL2_LIBRARIES} "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib")
  add_custom_command(TARGET ${GAME_NAME} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_if_different  
          "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2.dll" # <--this is in-file
          $<TARGET_FILE_DIR:${GAME_NAME}>) # <--this is out-file path

  add_custom_command(TARGET ${GAME_NAME} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_directory  
          "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}" # <--this is in-file
          $<TARGET_FILE_DIR:${GAME_NAME}>) # <--this is out-file path
endif(WIN32)
