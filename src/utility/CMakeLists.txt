cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(SOURCES application.cpp ../../include/utility/application.h
		data_map.cpp ../../include/utility/data_map.h
		data_map_lexer.cpp
		data_map_parser.cpp
		logger.cpp ../../include/utility/logger.h
		strings_res.cpp ../../include/utility/strings_res.h
		filesystem.cpp ../../include/utility/filesystem.h
		../../include/utility/config.h
		../../include/utility/exceptions.h
		../../include/utility/input.h
		../../include/utility/math.h
		../../include/utility/renderer.h
		../../include/utility/string.h
		../../include/utility/system.h
		../../include/utility/ui.h)

add_library(e2dit-utility ${SOURCES})
