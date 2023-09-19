include(CMakeForceCompiler)

#Set COMPILER_PATH
set(COMPILER_PATH "C:/Users/LHA9HC/.babun/cygwin/bin" CACHE PATH "")

#Set COMPILER
set(CMAKE_C_COMPILER "${COMPILER_PATH}/gcc.exe") #TODO
set(CMAKE_CXX_COMPILER "${COMPILER_PATH}/g++.exe") #TODO

#Set FLAG
set(CMAKE_C_FLAG "-c" CACHE STRING "C Flags" FORCE )
set(CMAKE_CXX_FLAG "-c" CACHE STRING "CPP Flags" FORCE)