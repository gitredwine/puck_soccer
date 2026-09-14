cmake_minimum_required(VERSION 3.16)
project(puckSoccer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Build pinned engine versions locally. The MSYS2 packages currently provide
# SFML 3 and Box2D 3, whose APIs are incompatible with this project.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/SFML EXCLUDE_FROM_ALL)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/box2d EXCLUDE_FROM_ALL)

# Step 8: player aiming plus RandomAI turns.
add_executable(puckSoccer
    src/main.cpp
    src/ai/greedyAI.cpp
    src/ai/randomAI.cpp
    src/game.cpp
    src/core/eventbus.cpp
    src/entity/ball.cpp
    src/entity/puck.cpp
    src/input/aimcontroller.cpp
    src/physics/bodyfactory.cpp
    src/physics/contact.cpp
    src/physics/world.cpp
    src/render/renderer.cpp
    src/rules/scoremanage.cpp
    src/rules/turnmanage.cpp
    src/levels/levels.cpp
)

target_include_directories(puckSoccer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(puckSoccer PRIVATE
    $<$<TARGET_EXISTS:SFML::Graphics>:SFML::Graphics>
    $<$<NOT:$<TARGET_EXISTS:SFML::Graphics>>:sfml-graphics>
    $<$<NOT:$<TARGET_EXISTS:SFML::Graphics>>:sfml-window>
    $<$<NOT:$<TARGET_EXISTS:SFML::Graphics>>:sfml-system>
    box2d
)

if(MSVC)
    target_compile_options(puckSoccer PRIVATE /W4)
else()
    target_compile_options(puckSoccer PRIVATE -Wall -Wextra -Wpedantic)
endif()
