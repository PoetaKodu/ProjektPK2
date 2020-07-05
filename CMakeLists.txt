cmake_minimum_required(VERSION 3.11)

set(PRJ_SOURCE_FILES
		src/Main.c
	)

add_executable(Filtr ${PRJ_SOURCE_FILES})

set_target_properties(Filtr PROPERTIES C_STANDARD 11)