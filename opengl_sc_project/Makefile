# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lm -lOpenCL -lEGL -lSDL2 -DC_OPENCL_HOST

# Detect architecture
ARCH := $(shell uname -m)

# Check for aarch64 and adjust flags and lib directory
ifeq ($(ARCH), aarch64)
    CFLAGS += -DSDL_DISABLE_IMMINTRIN_H
    LIBDIR = ./libs_64 ./GLSC_64/libGLSCv2.opencl.so
else
    LIBDIR = ./libs_x86 ./GLSC_x86/libGLSCv2.opencl.so
endif

# Linker flags
LDFLAGS = -L$(LIBDIR)

# Directories
SRCDIR = ./src
INCDIR = ./include
OBJDIR = ./obj
BINDIR = ./bin

# Target executable
TARGET = $(BINDIR)/scs

# List all source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# List corresponding object files
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Rule for kernel compilation
KERNEL_OCL = kernel.ocl
KERNEL_CL = kernel.cl

# Default rule
all: $(TARGET) oclcompile

# Conditionally run the OpenCL compiler based on GEN_OCL
oclcompile:
ifdef GEN_OCL
	$(MAKE) $(KERNEL_OCL)
else
	@echo "Skipping OpenCL Kernel Compilation (set GEN_OCL=1 to enable)"
endif

# Rule to compile the kernel.cl file to kernel.ocl
$(KERNEL_OCL): $(KERNEL_CL)
	./clcompiler $< $@ -cl-kernel-arg-info

# Rule to link the program
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

# Rule to compile each .c file into .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Rule to force recompilation
rebuild: clean all

# Phony targets to prevent filename conflicts
.PHONY: all clean rebuild oclcompile
