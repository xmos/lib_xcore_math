


PLATFORM ?= xcore
VERBOSE ?= 

PLATFORM_MF = ../../etc/platform/$(strip $(PLATFORM)).mk
COMMON_MF = ../../etc/common.mk
include $(PLATFORM_MF)
include $(COMMON_MF)

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info Building for platform: $(PLATFORM) )
endif

help:
	$(info *************************************************************************************)
	$(info *             make targets                                                          *)
	$(info *                                                                                   *)
	$(info *   help:      Display this message                                                 *)
	$(info *   clean:     Clean the build directory                                            *)
	$(info *   xcore:     Build the tests using the xCore-optimized lib_xs3_math.a             *)
	$(info *   ref:       Build the tests using the non-optimized lib_xs3_math.a               *)
	$(info *   build:     Build both xcore and ref                                             *)
	$(info *                                                                                   *)
	$(info *************************************************************************************)


APP_NAME := vect_tests

TARGET_DEVICE = XCORE-AI-EXPLORER

XSCOPE_CONFIG ?= config.xscope
XS3_MATH_PATH := ../../lib_xs3_math
XS3_MATH_FILE_NAME := lib_xs3_math.a

UNITY_PATH := ./../../third_party/Unity

BUILD_DIR := .build
BIN_DIR := bin
EXE_DIR   := $(BIN_DIR)/$(PLATFORM)
OBJ_DIR   := $(BUILD_DIR)/$(PLATFORM)
LIB_DIR   := $(OBJ_DIR)/lib
EMPTY_STR :=

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info XSCOPE_CONFIG: $(XSCOPE_CONFIG) )
  $(info XS3_MATH_PATH: $(XS3_MATH_PATH) )
  $(info XS3_MATH_FILE_NAME: $(XS3_MATH_FILE_NAME) )
  $(info BUILD_DIR: $(BUILD_DIR) )
  $(info OBJ_DIR: $(OBJ_DIR) )
endif

INCLUDES := $(XS3_MATH_PATH)/api $(UNITY_PATH)/src ../shared/testing
SOURCE_DIRS := src 
SOURCE_FILE_EXTENSIONS := c xc

SOURCE_FILES := 

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info SOURCE_FILE_EXTENTIONS: $(SOURCE_FILE_EXTENSIONS) )
  $(info INCLUDES: $(INCLUDES) )
  $(info SOURCE_DIRS: $(SOURCE_DIRS) )
endif

ifeq ($(strip $(PLATFORM)),$(strip xcore))
  PLATFORM_FLAGS += -target=$(TARGET_DEVICE)
endif

#######################################################
# SOURCE FILE SEARCH
#######################################################

# Recursively search within SOURCE_DIRS for files with extensions from SOURCE_FILE_EXTENSIONS
SOURCE_FILES += $(strip $(foreach src_dir,$(SOURCE_DIRS),\
                        $(call rwildcard,./$(src_dir),$(SOURCE_FILE_EXTENSIONS:%=*.%))))


ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info Library source files:)
  $(foreach f,$(SOURCE_FILES), $(info $f) )
  $(info )
endif


#######################################################
# COMPONENT OBJECT FILES
#######################################################

OBJECT_FILES := $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCE_FILES:./%=%))

# Set object file prerequisites
$(OBJECT_FILES) : $(OBJ_DIR)/%.o: %


ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info $(APP_NAME) object files:)
  $(foreach f,$(OBJECT_FILES), $(info $f) )
  $(info )
endif

#########
## Recipe-scoped variables for building objects.
#########

# OBJ_FILE_TYPE
# The source file's file type
$(eval $(foreach ext,$(SOURCE_FILE_EXTENSIONS),   \
           $(filter %.$(ext).o,$(OBJECT_FILES)): OBJ_FILE_TYPE = $(ext)$(newline)))

# OBJ_TOOL
# Maps from file extension to the tool type (not necessarily 1-to-1 mapping with
# file extension). This simplifies some of the code below.
$(OBJECT_FILES): OBJ_TOOL = $(MAP_COMP_$(OBJ_FILE_TYPE))

# OBJ_COMPILER: Compilation program for this object
$(OBJECT_FILES): OBJ_COMPILER = $($(OBJ_TOOL))

# $(1) - Tool
# $(2) - File extension
tf_combo_str = $(1)_$(2) $(1) $(2)
flags_combo_str = GLOBAL_FLAGS PLATFORM_FLAGS $(patsubst %,%_FLAGS,$(tf_combo_str))
includes_combo_str = INCLUDES PLATFORM_INCLUDES $(patsubst %,%_INCLUDES,$(tf_combo_str))

$(OBJECT_FILES): OBJ_FLAGS = $(strip $(foreach grp,$(call flags_combo_str,$(OBJ_TOOL),$(OBJ_FILE_TYPE)),$($(grp))))
$(OBJECT_FILES): OBJ_INCLUDES = $(strip $(foreach grp,$(call includes_combo_str,$(OBJ_TOOL),$(OBJ_FILE_TYPE)),$($(grp))))

###
# make target for each object file.
#
$(OBJECT_FILES):
	$(info [$(APP_NAME)] Compiling $<)
	@$(OBJ_COMPILER) $(OBJ_FLAGS) $(addprefix -I,$(OBJ_INCLUDES)) -o $@ -c $<

###
# If the -MMD flag is used when compiling, the .d files will contain additional header 
# file prerequisites for each object file. Otherwise it won't know to recompile if only
# header files have changed, for example.
-include $(OBJECT_FILES:%.o=%.d)


#######################################################
# LIBRARY TARGETS
#######################################################

# Libraries are built using a recursive make call.

ALL_STATIC_LIBS := $(LIB_DIR)/xcore/$(XS3_MATH_FILE_NAME) \
					 $(LIB_DIR)/ref/$(XS3_MATH_FILE_NAME)

LIB_MAKE_OPTS := VERBOSE=$(VERBOSE) BUILD_DIR=$(abspath $(BUILD_DIR)/lib_xs3_math) LIB_DIR=$(abspath $(LIB_DIR)) \
                 PLATFORM=$(PLATFORM) TARGET_DEVICE=$(TARGET_DEVICE)

.PHONY: build_deps

DEP_MAKE_OPTS := VERBOSE=$(VERBOSE) OBJ_DIR=$(abspath $(OBJ_DIR)) PLATFORM=$(PLATFORM) \
                 TARGET_DEVICE=$(TARGET_DEVICE) ADDITIONAL_INCLUDES=../../../lib_xs3_math/api

build_deps:
	@$(MAKE) -C $(XS3_MATH_PATH) all $(LIB_MAKE_OPTS)
	@$(MAKE) -C ../shared/testing $(abspath $(LIB_DIR)/testing.a ) $(DEP_MAKE_OPTS)
	@$(MAKE) -C ../shared/Unity $(abspath $(LIB_DIR)/unity.a ) $(DEP_MAKE_OPTS)
	

ALL_STATIC_LIBS += $(LIB_DIR)/testing.a $(LIB_DIR)/unity.a

#######################################################
# HOUSEKEEPING
#######################################################

# Annoying problem when doing parallel build is directory creation can fail if two threads both try to do it.
# To solve that, make all files in the build directory dependent on a sibling "marker" file, the recipe for which
# is just the creation of that directory and file.
$(eval  $(foreach bfile,$(OBJECT_FILES),       \
            $(bfile): | $(dir $(bfile)).marker $(newline)))
			
$(eval  $(foreach bfile,$(ALL_STATIC_LIBS),       \
            $(bfile): | $(dir $(bfile)).marker $(newline)))

$(BUILD_DIR)/%.marker:
	$(info Creating dir: $(dir $@))
	$(call mkdir_cmd,$@)
	@touch $@



#######################################################
# APPLICATION TARGETS
#######################################################

DEPENDENCY_LIBS = $(LIB_DIR)/unity.a $(LIB_DIR)/testing.a

#
# Application executable files
XCORE_APP_EXE_FILE = $(EXE_DIR)/$(APP_NAME).xcore$(PLATFORM_EXE_SUFFIX)
CREF_APP_EXE_FILE = $(EXE_DIR)/$(APP_NAME).ref$(PLATFORM_EXE_SUFFIX)

$(XCORE_APP_EXE_FILE): $(OBJECT_FILES) $(LIB_DIR)/xcore/$(XS3_MATH_FILE_NAME) $(DEPENDENCY_LIBS) | build_deps
$(CREF_APP_EXE_FILE): $(OBJECT_FILES) $(LIB_DIR)/ref/$(XS3_MATH_FILE_NAME) $(DEPENDENCY_LIBS) | build_deps

$(XCORE_APP_EXE_FILE): REQUIRED_LIBRARIES = $(LIB_DIR)/xcore/$(XS3_MATH_FILE_NAME) $(DEPENDENCY_LIBS)
$(CREF_APP_EXE_FILE): REQUIRED_LIBRARIES = $(LIB_DIR)/ref/$(XS3_MATH_FILE_NAME) $(DEPENDENCY_LIBS)


$(XCORE_APP_EXE_FILE) $(CREF_APP_EXE_FILE): $(XSCOPE_CONFIG)
	$(call mkdir_cmd,$@)
	$(info Linking binary $@)
	@$(XCC) $(LDFLAGS)                      \
		$(APP_FLAGS)                        \
		$(PLATFORM_FLAGS)                   \
		$(OBJECT_FILES)                     \
		$(XSCOPE_CONFIG)					\
		-o $@                               \
		$(REQUIRED_LIBRARIES)
		

# #######################################################
# # OTHER TARGETS
# #######################################################

.PHONY: help all build clean xcore ref

all: build

compile: $(OBJECT_FILES)

xcore: build_deps $(XCORE_APP_EXE_FILE)

ref: build_deps $(CREF_APP_EXE_FILE)

build: xcore ref

clean:
	$(info Cleaning project...)
	rm -rf $(BUILD_DIR)



