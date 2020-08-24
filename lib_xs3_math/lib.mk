

# The following variables should already be defined:
#    $(BUILD_DIR)    - The absolute path of the build directory -- where object files should be placed
#    $(PLATFORM)     - Either "x86" or "xcore" (no quotes) depending on whether it's being built for x86 or xcore

lib_xs3_math_PATH ?= ./
LIB_PATH := $(abspath $(lib_xs3_math_PATH))

INCLUDES += $(LIB_PATH)/api

lib_xs3_math_INCLUDES := $(LIB_PATH)/src/asm

# SOURCE_FILES = $(wildcard $(LIB_PATH)/$(SRC_DIR)/c/*.c)

# ifeq ($(strip $(PLATFORM)),$(strip xcore))
#   SOURCE_FILES += $(wildcard $(LIB_PATH)/$(SRC_DIR)/asm/*.S)
# endif


###### 
### [optional] Directories, relative to the dependency folder, to look for source files.
###            defaults to nothing.
SOURCE_DIRS := src

###### 
### [optional] Source file extentions. Defaults to: c cc xc cpp S
###

SOURCE_FILE_EXTS := c

# If the application has defined lib_xs3_math_NO_ASM to 1, do not include assembly functions as source.
ifneq ($(strip $(lib_xs3_math_NO_ASM)),1)
  SOURCE_FILE_EXTS += S
endif

ifeq ($(PLATFORM),xcore)

  ifeq ($(lib_xs3_math_NO_ASM),1)
    $(info Disabling ASM compilation..)
	NO_OPTIMIZE = 1
  else
    SOURCE_FILE_EXTS += S
  endif
endif

ifeq ($(NO_OPTIMIZE),1)
  $(info Disabling C optimizations..)
  c_FLAGS += -O0
endif

######
### [optional] List of source files to compile.
###            
# SOURCE_FILES :=  $(wildcard src/*.c src/*.xc)

######
### [optional] list of static libraries that
### should be linked into the executable
###
# LIBRARIES := foo.a


DOC_DIR := $(LIB_PATH)/doc
DOC_BUILD_DIR := $(DOC_DIR)/.build

$(eval  $(foreach bfile,$(ALL_BUILD_FILES),       \
            $(bfile): | $(dir $(bfile)).marker $(newline)))


docs:
	$(info Building documentation..)
	cd $(DOC_DIR) && doxygen

clean_doc:
	$(info Cleaning documentation...)
	rm -rf $(DOC_BUILD_DIR)

clean: clean_doc
