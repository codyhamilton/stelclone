cmake_minimum_required(VERSION 3.16)
project(Stelclone)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL_mixer EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL_ttf EXCLUDE_FROM_ALL)
#find_package(SDL3 REQUIRED)

# Create your game executable target as usual
add_executable(Stelclone
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/game.c
    ${CMAKE_SOURCE_DIR}/src/render.c
    ${CMAKE_SOURCE_DIR}/src/game/asteroid.c
)
add_executable(PerfAsteroids
    ${CMAKE_SOURCE_DIR}/src/perf/perf_asteroids.c
    ${CMAKE_SOURCE_DIR}/src/game/asteroid.c
    ${CMAKE_SOURCE_DIR}/src/data/resources.c
)

# Link to the actual SDL3 library.
target_link_libraries(Stelclone PRIVATE
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)