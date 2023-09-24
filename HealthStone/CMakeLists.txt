cmake_minimum_required(VERSION 3.10)

#set project name
project(HealthStone)

#add the executable
add_executable(HealthStone main.cpp
                           src/Console.cpp)

find_package(Curses REQUIRED)
target_include_directories(HealthStone PUBLIC
"${CMAKE_SOURCE_DIR}/inc"
"${CURSES_INCLUDE_DIR}"
)

target_link_libraries(HealthStone ${CURSES_LIBRARIES})
