

######
### [required]
### Application name. Used as output file name.
###
APP_NAME = fft_test

######
### [required if $(PLATFORM) is xcore]
### xcore target device
###
TARGET_DEVICE = XCORE-AI-EXPLORER

######
### [optional] List of libraries on which this application depends
###
DEPENDENCIES := lib_xs3_math Unity testing floating_fft

######
### [currently required]
### Paths to the dependencies, because we don't have a way of searching
### for them at the moment.
###
lib_xs3_math_PATH := ../../lib_xs3_math
Unity_PATH := ../../../Unity
testing_PATH := ../shared/testing
floating_fft_PATH := ../shared/floating_fft

## Uncomment to tell lib_xs3_math not to compile .S files (i.e. test C implementations)
# lib_xs3_math_NO_ASM := 1

GEN_SRC_DIR = src/.gen

INCLUDES += $(GEN_SRC_DIR)

###### 
### [optional] Directories, relative to the application project, to look for source files.
###            defaults to nothing.
SOURCE_DIRS := src

###### 
### [optional] Source file extentions. Defaults to: c cc xc cpp S
###
# SOURCE_FILE_EXTS := c cc xc cpp S

######
### [optional] List of source files to compile.
###            
# SOURCE_FILES :=  $(wildcard src/*.c src/*.xc)


######
### [optional] list of static libraries that
### should be linked into the executable
###
# LIBRARIES := foo.a

# If the application makefile sets this to any value other than 1, no static 
# libraries (.a files) will be created for dependencies, and the executable
# will be linked directly against library object files.
#
BUILD_STATIC_LIBRARIES := 0

c_FLAGS += -Wno-unused-function
xc_FLAGS += -Wno-unused-function
c_FLAGS += -DUNITY_USE_COMMAND_LINE_ARGS
LDFLAGS += -fcmdline-buffer-bytes=200

ifdef QUICK_TEST
  $(info Enabling 'quick test' mode.)
  c_FLAGS += -DQUICK_TEST=1
endif

ifdef NO_ASM
  $(info Disabling ASM compilation for lib_xs3_math.)
  lib_xs3_math_NO_ASM = 1
endif


#####
### Indicates the maximum FFT size to generate a look-up table for. 
### The maximum size will be 2^(MAX_FFT_SIZE_LOG2)
###
MAX_FFT_SIZE_LOG2 := 10
GEN_FFT_TABLE_FLAGS := --dit --dif
GEN_FFT_TABLE_SCRIPT = $(lib_xs3_math_PATH)/script/gen_fft_table.py
FFT_TABLE_C_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.c
FFT_TABLE_H_FILE = $(GEN_SRC_DIR)/xs3_fft_lut.h

SOURCE_FILES += $(FFT_TABLE_C_FILE)


######
### [required]
### Default make target. Use this for a project description?
###
app_help:
	$(info *************************************************************)
	$(info unit_test: Unit tests for the FFT functions in lib_xs3_math  )
	$(info                                                              )
	$(info make targets:                                                )
	$(info |       help:    Display this message (default)              )
	$(info |        all:    Build application                           )
	$(info |      clean:    Remove build files and folders from project )
	$(info |        run:    Run the unit tests in xsim                  )
	$(info | fft_tables:    Generate required FFT tables                )
	$(info *************************************************************)


#####################################
### Application-specific targets
#####################################

$(FFT_TABLE_H_FILE): $(FFT_TABLE_C_FILE) $(GEN_FFT_TABLE_SCRIPT)
$(FFT_TABLE_C_FILE): $(GEN_FFT_TABLE_SCRIPT)
	$(info Generating FFT look-up tables..)
	$(call mkdir_cmd, $(dir $@))
	python $(GEN_FFT_TABLE_SCRIPT) --out_file xs3_fft_lut --out_dir $(GEN_SRC_DIR) --max_fft_log2 $(MAX_FFT_SIZE_LOG2) $(GEN_FFT_TABLE_FLAGS)

.PHONY: fft_tables
fft_tables: $(FFT_TABLE_C_FILE) $(FFT_TABLE_H_FILE)

build : fft_tables

run: build
	xsim $(APP_EXE_FILE)

clean: clean_app
clean_app:
	$(info Removing auto-generated files..)
	rm -rf $(GEN_SRC_DIR)