MEDIA_DIR = $(shell pwd)/media
#/home/gifi47/Media

# Building static library
LIB_BUILD_DIR = lib
LIB_NAME = MyGraphics2
LIB_FILE = $(LIB_BUILD_DIR)/lib$(LIB_NAME).a
SOURCE_DIR1 = src/Rendering
SOURCE_DIR2 = src/Test
SOURCE_DIR3 = src/TestObjects
SOURCE_DIR4 = src/Utils

# Define your source files
LIB_SRCS = $(wildcard $(SOURCE_DIR1)/*.cpp) $(wildcard $(SOURCE_DIR2)/*.cpp) $(wildcard $(SOURCE_DIR3)/*.cpp) $(wildcard $(SOURCE_DIR4)/*.cpp)

# Automatically generate object file names from source files
# OBJS = $(LIB_BUILD_DIR)/$(notdir $(SRCS:.cpp=.o))
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

# Building application
BUILD_DIR = build
EXECUTABLE_NAME = EndlessCavess
SOURCE_DIR = src
EXECUTABLE = $(BUILD_DIR)/$(EXECUTABLE_NAME)
MEDIA_DEFINE=-Dmedia=\"$(MEDIA_DIR)/\"

SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)

OBJS = $(SRCS:.cpp=.o)

# Compiler and archiver commands
CXX = g++
AR = ar
ARFLAGS = rcs
COMPILER_FLAGS = -std=c++20 -c -DGLEW_STATIC -DDEBUG
LD_FLAGS = -L$(LIB_BUILD_DIR) -l$(LIB_NAME) -lOpenGL -lGLEW -lglfw -lSOIL

# Default target: build the static library
all: $(LIB_FILE) $(EXECUTABLE)

# Rule to create the static library from object files
$(LIB_FILE): $(LIB_OBJS) | make_build_dir
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@ # Optional: Create an index for faster linking

$(EXECUTABLE) : $(OBJS) | make_build_dir
	$(CXX) $(OBJS) $(LD_FLAGS) -o $@

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(COMPILER_FLAGS) $(MEDIA_DEFINE) $< -o $@

make_build_dir:
	mkdir -p $(LIB_BUILD_DIR)
	mkdir -p $(BUILD_DIR)

# Clean up object files and the static library
clean: clean_lib clean_exe

clean_lib:
	rm -f $(LIB_OBJS) $(LIB_FILE)

clean_exe:
	rm -f $(OBJS) $(EXECUTABLE)

print_media:
	@echo $(MEDIA_DEFINE)

print_obj:
	@echo $(LIB_OBJS)
	@echo $(OBJS)

.PHONY: all clean clean_exe clean_lib print_obj print_media