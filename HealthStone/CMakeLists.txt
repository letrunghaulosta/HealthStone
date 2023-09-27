cmake_minimum_required(VERSION 3.10)

set(CURSES_INCLUDE_PATH "C:/Users/LHA9HC/.babun/cygwin/usr/include")

#set project name
project(HealthStone)

#add the executable
add_executable(HealthStone main.cpp
                           src/Console.cpp
                           src/GameController.cpp
                           src/HeroManager.cpp
                           src/CardManager.cpp)
find_package(Curses REQUIRED)
target_include_directories(HealthStone PUBLIC
"${CMAKE_SOURCE_DIR}/inc"
"${CURSES_INCLUDE_PATH}"
)

target_link_libraries(HealthStone ${CURSES_LIBRARIES})
