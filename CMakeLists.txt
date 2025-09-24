cmake_minimum_required(VERSION 3.21)
project(Solitaire)

if (NOT TARGET rygame)
    find_package(rygame REQUIRED)
endif ()

add_executable(
    ${PROJECT_NAME}
    code/main.cpp
    code/game.cpp
    code/card.cpp
    code/resourcemanager.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE rygame)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
