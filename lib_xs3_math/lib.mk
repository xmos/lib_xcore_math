

# The following variables should already be defined:
#    $(OBJ_DIR)      - The absolute path within the build directory where object files should be placed for this build.
#    $(PLATFORM)     - Either "x86" or "xcore" (no quotes) depending on whether it's being built for x86 or xcore
#    $(lib_xs3_math_PATH) - Path containing this lib.mk file, relative to application directory

# The following variables modify the build behavior:
#    $(lib_xs3_math_NO_ASM)	- If this is NOT defined with the value 1 then .S files will be included as source files.
#    $(NO_OPTIMIZE)	- -O0 is added to c file build flags.

# Note: This .mk file assumes it is being included by the Makefile in /test/shared/Makefile
#	Many of the values defined here will only make sense in that context.
#

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
SOURCE_DIRS := src/vect src/bfp

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
GEN_SRC_DIR = $(OBJ_DIR)/lib_xs3_math/.generated
INCLUDES += $(GEN_SRC_DIR)

MAX_FFT_SIZE_LOG2 ?= 10
GEN_FFT_TABLE_FLAGS ?= --dit --dif

GEN_FFT_TABLE_SCRIPT = $(lib_xs3_math_PATH)/script/gen_fft_table.py
FFT_TABLE_C_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.c
FFT_TABLE_H_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.h

SOURCE_FILES += $(FFT_TABLE_C_FILE)

#Apparent bug in GNU Make for windows causes 'abspath' to not be idempotent.
$(abspath $(FFT_TABLE_H_FILE)): $(abspath $(FFT_TABLE_C_FILE)) $(GEN_FFT_TABLE_SCRIPT)
$(abspath $(FFT_TABLE_C_FILE)): $(GEN_FFT_TABLE_SCRIPT)
	$(info Generating FFT look-up tables..)
	$(call mkdir_cmd, $(dir $@))
	python $(GEN_FFT_TABLE_SCRIPT) --out_file xs3_fft_lut --out_dir $(GEN_SRC_DIR) --max_fft_log2 $(MAX_FFT_SIZE_LOG2) $(GEN_FFT_TABLE_FLAGS)

#####
### Other

#Generate the FFT tables prior to compiling any source.
$(PRECOMP_BARRIER): $(abspath $(FFT_TABLE_H_FILE)) $(abspath $(FFT_TABLE_C_FILE))


DOC_DIR := $(LIB_PATH)/doc
DOC_BUILD_DIR := $(DOC_DIR)/.build

$(eval  $(foreach bfile,$(ALL_BUILD_FILES),       \
            $(bfile): | $(dir $(bfile)).marker $(newline)))


docs:
	$(info Building documentation..)
	cd $(DOC_DIR) && doxygen && echo Documentation generated. Open $(DOC_BUILD_DIR)/html/index.html to view documentation.

clean_doc:
	$(info Cleaning documentation...)
	rm -rf $(DOC_BUILD_DIR)

clean: clean_doc
