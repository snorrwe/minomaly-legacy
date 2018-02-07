get_filename_component(GAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" GAME_NAME ${GAME_NAME})
project(${GAME_NAME})

#[[ Game ]]
include_directories(${GAME_NAME})

#[[ Game executables ]]
add_executable(${GAME_NAME} ${GAME_LOGIC_DIR}/${GAME_NAME}/main.cpp)

add_custom_command(TARGET ${GAME_NAME} POST_BUILD  
      COMMAND ${CMAKE_COMMAND} -E copy_directory  
          ${ASSETS}/${GAME_NAME}
          $<TARGET_FILE_DIR:${GAME_NAME}>/assets/${GAME_NAME})

if(WIN32)
  find_package(SDL2 REQUIRED)
  LINK_EXAMPLE(${GAME_NAME})
endif(WIN32)
