

# The following variables should already be defined:
#    $(BUILD_DIR)    - The absolute path of the build directory -- where object files should be placed
#    $(PLATFORM)     - Either "x86" or "xcore" (no quotes) depending on whether it's being built for x86 or xcore

floating_fft_PATH ?= ./
LIB_PATH := $(abspath $(floating_fft_PATH))

INCLUDES += $(LIB_PATH)

# floating_fft_INCLUDES := $(LIB_PATH)/src/asm

# SOURCE_FILES = $(wildcard $(LIB_PATH)/src/*.c)

# ifeq ($(strip $(PLATFORM)),$(strip xcore))
#   SOURCE_FILES += $(wildcard $(LIB_PATH)/$(SRC_DIR)/asm/*.S)
# endif

###### 
### [optional] Directories, relative to the dependency folder, to look for source files.
###            defaults to nothing.
SOURCE_DIRS := .

###### 
### [required] Source file extentions. ( c cc xc cpp S )
###
SOURCE_FILE_EXTS := c

######
### [optional] List of source files to compile.
###            
# SOURCE_FILES :=  $(wildcard src/*.c src/*.xc)

######
### [optional] list of static libraries that
### should be linked into the executable
###
# LIBRARIES := foo.a

