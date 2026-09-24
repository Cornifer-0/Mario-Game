CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
ifeq "$(MODE)" "release"
CXXFLAGS += -O3
else
CXXFLAGS += -g3
endif
# Files to include in the archive
TAR_SRCS := $(shell find src -type f)
TAR_INCS := $(shell find include -type f)
TAR_FILES := $(TAR_SRCS) $(TAR_INCS) Makefile /doxyfile README.md
# Afegim llibreries segons el sistema operatiu
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lgdi32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS += -lX11
    endif
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS += -framework Cocoa
    endif
endif

# File structure
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin
TARGET := $(BINDIR)/mario_pro_2

TAR_FILE = mario-pro-2-$(USER)-$(shell date +%s).tgz

# Source and object files
SRC := $(shell find $(SRCDIR) -name "*.cc")
OBJ := $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SRC))

# Default target
all: $(TARGET)

# Build final binary
$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(OBJ:.o=.d)

# Create a .tar.gz archive
tgz: clean
	tar -czf $(TAR_FILE) *

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean