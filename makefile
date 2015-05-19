boost_include=/home/int64/dev/boost/
boosl_libs=/home/int64/dev/boost/stage/lib/
stdcpp=-std=gnu++1y -stdlib=libc++
include_pathes=-I./src/ -I./include/ -I/usr/include/freetype2/ -I$(boost_include)
libs_pathes=-L. -L$(boosl_libs)

source_dirs := src src/core src/map_editor src/renderer src/ui src/utility
utility_dirs := src/utility
gapi_dirs := src/renderer
ui_dirs := src/ui
core_dirs := src src/core src/map_editor

CC = clang++
platform = _linux_
arch = _x86_64_

override compile_flags += -pipe

all: libe2dit-utility.so libe2dit-gapi.so libe2dit-ui.so e2dit

# Utility

search_wildcards := $(addsuffix /*.cpp,$(utility_dirs)) 
libs_utility = -lboost_system -lboost_filesystem

libe2dit-utility.so: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(CC) $(stdcpp) -shared -g -fPIC -D$(platform) -D$(arch) $(libs_utility) $(compile_flags) $(include_pathes) $(libs_pathes) $^ -o $@ 

# Renderer

search_wildcards := $(addsuffix /*.cpp,$(gapi_dirs)) 
libs_renrerer = -lftgl -lfreetype -lboost_system -lboost_filesystem -le2dit-utility

libe2dit-gapi.so: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(CC) $(stdcpp) -shared -g -fPIC -D$(platform) -D$(arch) $(libs_renrerer) $(compile_flags) $(include_pathes) $(libs_pathes) $^ -o $@ 

# UI

search_wildcards := $(addsuffix /*.cpp,$(ui_dirs)) 
libs_ui = -lftgl -lfreetype -lboost_system -lboost_filesystem -le2dit-gapi -le2dit-utility

libe2dit-ui.so: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(CC) $(stdcpp) -shared -g -fPIC -D$(platform) -D$(arch) $(libs_ui) $(compile_flags) $(include_pathes) $(libs_pathes) $^ -o $@ 

# Main Program

search_wildcards := $(addsuffix /*.cpp,$(core_dirs)) 
libs_core  = -lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lftgl -lsfml-graphics-2-2 -lsfml-window-2-2 -lsfml-system-2-2 -lboost_system -lboost_filesystem
libs_core += -le2dit-ui -le2dit-gapi -le2dit-utility

e2dit: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(CC) $(stdcpp) -D$(platform) -D$(arch) $(libs_core) $(compile_flags) $(include_pathes) $(libs_pathes) $^ -o $@ 

VPATH := $(source_dirs)
 
%.o: %.cpp
	$(CC) $(stdcpp) -fPIC -D$(platform) -D$(arch) -c -MD $(addprefix -I,$(source_dirs)) $(compile_flags) $(include_pathes) $<

clean:
	rm -rf $(e2dit) *.o
	rm -rf $(e2dit) *.d

include $(wildcard *.d)