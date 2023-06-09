﻿# CMakeList.txt : CMake project for TinksWorld, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TinksWorld" C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_compile_definitions("EMSCRIPTEN")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file assets --use-preload-plugins --shell-file TinksWorld.html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    find_package(sdl2-mixer REQUIRED)
    find_package(sdl2-image REQUIRED)
    find_package(sdl2-ttf REQUIRED)
endif()

# Add source to this project's executable.
add_executable (TinksWorld "src/main.c" "src/scene.c" "src/vec2.c" "src/color.c" "src/resources.c" "src/entity.c")

target_include_directories(TinksWorld PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

target_include_directories(TinksWorld PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(TinksWorld PRIVATE ${SDL2_LIBRARIES})

target_link_libraries(TinksWorld PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(TinksWorld PRIVATE ${SDL2_MIXER_LIBRARIES}) # ensure sdl2-mixer "extensions" are installed for .ogg: https://www.reddit.com/r/cataclysmdda/comments/glxgtb/fix_for_sound_problem_when_compiling_in_windows/
target_link_libraries(TinksWorld PRIVATE ${SDL2_TTF_LIBRARIES})
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(TinksWorld PUBLIC ${MATH_LIBRARY})
endif()

if (WIN32)
    # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
    set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()

target_link_options(TinksWorld PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(TinksWorld copy_assets)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_custom_target(copy_html
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/web/TinksWorld.html ${CMAKE_CURRENT_BINARY_DIR}/TinksWorld.html
    )
    add_dependencies(TinksWorld copy_html)
endif()