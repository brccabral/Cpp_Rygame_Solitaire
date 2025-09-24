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
    code/resourcemanager.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE rygame)
