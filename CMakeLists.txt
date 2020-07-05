cmake_minimum_required(VERSION 3.11)

set(PRJ_SOURCE_FILES
		include/Filter/ParseArgs.h
		
		src/Main.c
		src/ParseArgs.c
	)

add_executable(Filter ${PRJ_SOURCE_FILES})

target_include_directories(Filter PUBLIC
		include
	)

set_target_properties(Filter PROPERTIES C_STANDARD 11)