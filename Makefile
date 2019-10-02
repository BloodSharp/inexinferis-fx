CXX ?= g++
SRC_PATH = $(PWD)
BIN_NAME = Inexinferis
SRC_EXT = cpp

COMPILE_FLAGS = -std=gnu++11 -fexceptions -fpermissive -fvisibility=hidden -m32
RELEASE_FLAGS = -s -Os
DEBUG_FLAGS = -g

ifeq ($(OS), Windows_NT)
	MAKE = mingw32-make
	EXE_EXTENSION = .exe
	LIB_EXTENSION = .dll
	COMPILE_FLAGS := $(COMPILE_FLAGS) -Wl,--exclude-all-symbols
	EXE_EXTRA_FLAGS = -mwindows
	LIBS = -lOpenGL32 -lXInput -lGdi32 -ld3d9
endif
ifeq ($(shell uname), Linux)
	MAKE = make
	COMPILE_FLAGS := $(COMPILE_FLAGS) -fPIC
	LIB_EXTENSION = .so
	LIBS = -lSDL2 -lGL -ldl
endif
ifeq ($(shell uname), Darwin)
	MAKE = make
	LIB_EXTENSION = .dylib
	LIBS = -lSDL2 -lGL -ldl
endif

#INCLUDES = -I include/ -I /usr/local/include

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(RELEASE_FLAGS)
release: export BUILD_PATH = Release
release: paths
release: dirs
	@$(MAKE) all

.PHONY: debug
debug: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(DEBUG_FLAGS)
debug: export BUILD_PATH = Debug
debug: paths
debug: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(LIB_OBJ_PATH)
	@mkdir -p $(BIN_PATH)

.PHONY: paths
BIN_PATH = $(BUILD_PATH)/bin
LIB_OBJ_PATH = $(BUILD_PATH)/obj/Library

LIB_PATH = $(SRC_PATH)/linuxfork

LIB_SOURCES = $(shell find $(LIB_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)

LIBRARY_OBJECTS = $(LIB_SOURCES:$(LIB_PATH)/%.$(SRC_EXT)=$(LIB_OBJ_PATH)/%.o)

LIBRARY_DEPENDENCIES = $(LIBRARY_OBJECTS:.o=.d)

.PHONY: clean
clean:
	@echo "Deleting directories"
	@rm -Rf Release/
	@rm -Rf Debug/

.PHONY: all
all: $(BIN_PATH)/lib$(BIN_NAME)$(LIB_EXTENSION)

# Final Builds
$(BIN_PATH)/lib$(BIN_NAME)$(LIB_EXTENSION): $(LIBRARY_OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(LIBRARY_OBJECTS) $(LIBS) -m32 -shared -o $@

# Add dependency files, if they exist
-include $(IMGUI_DEPENDENCIES) #$(LIBRARY_DEPENDENCIES) $(EXECUTABLE_DEPENDENCIES) $(MENUTESTER_DEPENDENCIES)

# Compile Library
$(LIB_OBJ_PATH)/%.o: $(LIB_PATH)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@