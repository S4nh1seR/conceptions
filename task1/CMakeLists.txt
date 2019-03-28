cmake_minimum_required(VERSION 3.13)
project(Assembler)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-lstdc++fs")
#set(CMAKE_EXE_LINKER_FLAGS "-lstdc++fs")
link_libraries(stdc++fs)

add_executable(Assembler main.cpp Assembler.cpp Assembler.h VirtualMachine.cpp VirtualMachine.h Disassembler.cpp Disassembler.h)