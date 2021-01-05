


PLATFORM ?= xcore
VERBOSE ?= 
TARGET_DEVICE ?= XCORE-AI-EXPLORER
BUILD_DIR ?= .build
LIB_DIR ?= lib


PLATFORM_MF = ../etc/platform/$(strip $(PLATFORM)).mk
COMMON_MF = ../etc/common.mk
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
	$(info *   docs:      Build the documentation (requires Doxygen)                           *)
	$(info *   clean:     Clean the build directory                                            *)
	$(info *   xcore:     Build the xCore-optimized lib_xs3_math.a                             *)
	$(info *   ref:       Build lib_xs3_math.a using unoptimized C implementations             *)
	$(info *   build:     Build both xcore and ref                                             *)
	$(info *                                                                                   *)
	$(info *************************************************************************************)

LIB_NAME := lib_xs3_math

OBJ_DIR   := $(BUILD_DIR)/$(PLATFORM)
SOURCE_FILE_EXTENSIONS := c
EMPTY_STR :=

ifeq ($(PLATFORM),xcore)
	SOURCE_FILE_EXTENSIONS += S
endif

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info BUILD_DIR: $(BUILD_DIR) )
  $(info OBJ_DIR: $(OBJ_DIR) )
  $(info LIB_BUILD_DIR: $(LIB_BUILD_DIR) )
  $(info LIB_DIR: $(LIB_DIR) )
endif

INCLUDES := api
SOURCE_DIRS := src

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

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info Library object files:)
  $(foreach f,$(OBJECT_FILES), $(info $f) )
  $(info )
endif

# Source file is first prerequisite for object files
$(OBJECT_FILES): $(OBJ_DIR)/%.o: %

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
# make target for each component object file.
#
$(OBJECT_FILES):
	$(info [$(LIB_NAME)] Compiling $<)
	@$(OBJ_COMPILER) $(OBJ_FLAGS) $(addprefix -I,$(OBJ_INCLUDES)) -o $@ -c $<

###
# If the -MMD flag is used when compiling, the .d files will contain additional header 
# file prerequisites for each object file. Otherwise it won't know to recompile if only
# header files have changed, for example.
-include $(OBJECT_FILES:%.o=%.d)

#######################################################
# HOUSEKEEPING
#######################################################

# Annoying problem when doing parallel build is directory creation can fail if two threads both try to do it.
# To solve that, make all files in the build directory dependent on a sibling "marker" file, the recipe for which
# is just the creation of that directory and file.
$(eval  $(foreach bfile,$(OBJECT_FILES),       \
            $(bfile): | $(dir $(bfile)).marker $(newline)))

$(BUILD_DIR)/%.marker:
	$(info Creating dir: $(dir $@))
	$(call mkdir_cmd,$@)
	@touch $@



#######################################################
# STATIC LIBRARY TARGETS
#######################################################

###
# xcore optimized
XCORE_LIB_FILE := $(LIB_DIR)/xcore/$(LIB_NAME).a
XCORE_OBJECT_FILES := $(filter-out $(OBJ_DIR)/src/arch/ref/%, $(OBJECT_FILES))

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info xcore-specific object files:)
  $(foreach f,$(XCORE_OBJECT_FILES), $(info $f) )
  $(info )
endif

$(XCORE_LIB_FILE): $(XCORE_OBJECT_FILES)

###
# C reference
CREF_LIB_FILE := $(LIB_DIR)/ref/$(LIB_NAME).a

CREF_OBJECT_FILES := $(filter-out $(OBJ_DIR)/src/arch/xcore/%, $(OBJECT_FILES))

ifneq ($(VERBOSE),$(EMPTY_STR))
  $(info C reference object files:)
  $(foreach f,$(CREF_OBJECT_FILES), $(info $f) )
  $(info )
endif

$(CREF_LIB_FILE): $(CREF_OBJECT_FILES)


#
# Recipe for building the archive files.
#   They get placed in $(LIB_DIR)
#
LIB_FILES := $(XCORE_LIB_FILE) $(CREF_LIB_FILE)

$(LIB_FILES): $(LIB_DIR)/%.a :	
	$(call mkdir_cmd,$@)
	$(info Generating archive $@)
	@$(AR) $(AR_FLAGS) $@ $^




# #######################################################
# # OTHER TARGETS
# #######################################################

.PHONY: help all build clean xcore ref libs docs


all: build

compile: $(OBJECT_FILES)

xcore: $(XCORE_LIB_FILE)

ref: $(CREF_LIB_FILE)

libs: xcore ref

build: libs

clean:
	$(info Cleaning project...)
	rm -rf $(LIB_DIR)
	rm -rf $(BUILD_DIR)

docs:
	cd doc && doxygen && cd ..


