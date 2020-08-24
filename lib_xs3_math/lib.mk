

# The following variables should already be defined:
#    $(BUILD_DIR)    - The absolute path of the build directory -- where object files should be placed
#    $(PLATFORM)     - Either "x86" or "xcore" (no quotes) depending on whether it's being built for x86 or xcore

lib_xs3_math_PATH ?= ./
LIB_PATH := $(abspath $(lib_xs3_math_PATH))

INCLUDES += $(LIB_PATH)/api

lib_xs3_math_INCLUDES := 

# SOURCE_FILES = $(wildcard $(LIB_PATH)/$(SRC_DIR)/c/*.c)

# ifeq ($(strip $(PLATFORM)),$(strip xcore))
#   SOURCE_FILES += $(wildcard $(LIB_PATH)/$(SRC_DIR)/asm/*.S)
# endif


###### 
### [optional] Directories, relative to the dependency folder, to look for source files.
###            defaults to nothing.
SOURCE_DIRS := src/low src/high

###### 
### [optional] Source file extentions. Defaults to: c cc xc cpp S
###

SOURCE_FILE_EXTS := c

# If the application has defined lib_xs3_math_NO_ASM to 1, do not include assembly functions as source.
ifneq ($(strip $(lib_xs3_math_NO_ASM)),1)
  SOURCE_FILE_EXTS += S
  SOURCE_DIRS += src/arch/$(PLATFORM)
  lib_xs3_math_INCLUDES += $(LIB_PATH)/src/arch/$(PLATFORM)
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

#####
### Deal with auto-generated FFT look-up tables
GEN_SRC_DIR_PART = lib_xs3_math/.generated
GEN_SRC_DIR = $(OBJ_DIR)/$(GEN_SRC_DIR_PART)
INCLUDES += $(GEN_SRC_DIR)

MAX_FFT_SIZE_LOG2 ?= 10
GEN_FFT_TABLE_FLAGS ?= --dit --dif

GEN_FFT_TABLE_SCRIPT = $(lib_xs3_math_PATH)/script/gen_fft_table.py
FFT_TABLE_C_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.c
FFT_TABLE_H_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.h

SOURCE_FILES += $(FFT_TABLE_C_FILE)
vpath $(GEN_SRC_DIR_PART)/% $(GEN_SRC_DIR)/../..

PRECOMP_TARGETS += $(FFT_TABLE_C_FILE) $(FFT_TABLE_H_FILE)

$(GEN_SRC_DIR_PART)/xs3_fft_lut.c: $(FFT_TABLE_C_FILE)
$(FFT_TABLE_H_FILE): $(FFT_TABLE_C_FILE) $(GEN_FFT_TABLE_SCRIPT)
$(FFT_TABLE_C_FILE): $(GEN_FFT_TABLE_SCRIPT)
	$(info Generating FFT look-up tables..)
	$(call mkdir_cmd, $(dir $@))
	python $(GEN_FFT_TABLE_SCRIPT) --out_file xs3_fft_lut --out_dir $(GEN_SRC_DIR) --max_fft_log2 $(MAX_FFT_SIZE_LOG2) $(GEN_FFT_TABLE_FLAGS)

#####
### Other


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
