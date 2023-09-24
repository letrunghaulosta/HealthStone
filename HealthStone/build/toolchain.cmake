include(CMakeForceCompiler)

#Set COMPILER_PATH
set(COMPILER_PATH "/usr/bin" CACHE PATH "") #TODO

#Set COMPILER
set(CMAKE_C_COMPILER "${COMPILER_PATH}/gcc") #TODO
set(CMAKE_CXX_COMPILER "${COMPILER_PATH}/g++") #TODO

#Set FLAG
set(CMAKE_C_FLAGS "-W -Wall -Werror -Wextra -lncurses" CACHE STRING "C Flags" FORCE )
#set(CMAKE_CXX_FLAGS "-lncurses" CACHE STRING "C Flags" FORCE)
#set(CMAKE_EXE_LINKER_FLAGS "-lncurses" CACHE STRING "C Flags" FORCE)
#set(CMAKE_STATIC_LINKER_FLAGS "-lncurses" CACHE STRING "C Flags" FORCE)
