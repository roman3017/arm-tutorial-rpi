#   Part of the Raspberry-Pi Bare Metal Tutorials
#   Copyright (c) 2015, Brian Sidebotham
#   All rights reserved.

#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are met:

#   1. Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.

#   2. Redistributions in binary form must reproduce the above copyright notice,
#       this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.

#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required( VERSION 2.8 )

project( entry C ASM )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mlong-calls" )

add_executable(
	entry0
	entry0.S
	main.c
	pl011.c
	uart.c
)

add_executable(
	entry1
	entry1.S
	main.c
	pl011.c
	uart.c
)

set_target_properties(
    entry0
    PROPERTIES
    LINK_FLAGS "-Ttext 10000"
)

set_target_properties(
    entry1
    PROPERTIES
    LINK_FLAGS "-Ttext 5000"
)

add_custom_command(
    TARGET entry0 entry1 POST_BUILD
    #OUTPUT entry0.img entry0.asm entry1.img entry1.asm
    #DEPENDS entry0 entry1
    COMMAND ${CMAKE_OBJCOPY} entry0 -O binary entry0.img
    COMMAND ${CMAKE_OBJDUMP} -D entry0 > entry0.asm
    COMMAND ${CMAKE_OBJCOPY} entry1 -O binary entry1.img
    COMMAND ${CMAKE_OBJDUMP} -D entry1 > entry1.asm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Convert the ELF output file to a binary image"
)
