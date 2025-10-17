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

if (EMSCRIPTEN)
    target_link_options(
        ${PROJECT_NAME}
        PRIVATE
        -sALLOW_MEMORY_GROWTH=1
        -sEXPORTED_RUNTIME_METHODS=[requestFullscreen]
        -sUSE_GLFW=3
        -sSTACK_SIZE=131072
        -sASYNCIFY=1
        --shell-file
        "$ENV{EMSCRIPTEN}/src/shell_brc.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    set(resources_dir ${CMAKE_CURRENT_SOURCE_DIR}/resources)
    if (EXISTS ${resources_dir})
        # The local resources path needs to be mapped to /resources virtual path
        string(APPEND resources_dir "@resources")
        target_link_options(
            ${PROJECT_NAME}
            PRIVATE
            --preload-file
            "${resources_dir}")
    endif ()
endif ()
