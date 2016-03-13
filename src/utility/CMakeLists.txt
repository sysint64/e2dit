cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(SOURCES application.cpp
		data_map.cpp
		data_map_lexer.cpp
		data_map_parser.cpp
		logger.cpp
		strings_res.cpp)

add_library(e2dit-utility ${SOURCES})
