cmake_minimum_required(VERSION 3.11)

set(PRJ_SOURCE_FILES
		include/Filter/ParseArgs.h
		include/Filter/PrefixTree.h
		include/Filter/Filter.h
		include/Filter/App.h

		src/Main.c
		src/ParseArgs.c
		src/PrefixTree.c
		src/Filter.c
		src/App.c
	)

add_executable(Filter ${PRJ_SOURCE_FILES})

target_include_directories(Filter PUBLIC
		include
	)

set_target_properties(Filter PROPERTIES C_STANDARD 11)