

################################################
# These files are intended to be compiled through a recursive make call from the application Makefile.
# To ensure this works correctly, explicitly set the following variables in the recursive make call.
PLATFORM ?= xcore
VERBOSE ?= 
TARGET_DEVICE ?= XCORE-AI-EXPLORER
OBJ_DIR ?= 
################################################


PLATFORM_MF = ../../../etc/platform/$(strip $(PLATFORM)).mk
COMMON_MF = ../../../etc/common.mk
include $(PLATFORM_MF)
include $(COMMON_MF)

SELF_NAME = Unity

SOURCE_FILE := ../../../third_party/Unity/src/unity.c	
OBJECT_FILE := $(OBJ_DIR)/unity.c.o

ifeq ($(strip $(PLATFORM)),$(strip xcore))
  PLATFORM_FLAGS += -target=$(TARGET_DEVICE)
endif

$(OBJECT_FILE): $(SOURCE_FILE)

OBJECT_FILES := $(OBJECT_FILE)

#########
## Recipe-scoped variables for building objects.
#########

# OBJ_FILE_TYPE
# The source file's file type
$(filter %.c.o,$(OBJECT_FILES)):  OBJ_FILE_TYPE = c

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
$(OBJECT_FILES):
	$(info [$(SELF_NAME)] Compiling $<)
	@$(OBJ_COMPILER) $(OBJ_FLAGS) $(addprefix -I,$(OBJ_INCLUDES)) -o $@ -c $<

###
# If the -MMD flag is used when compiling, the .d files will contain additional header 
# file prerequisites for each object file. Otherwise it won't know to recompile if only
# header files have changed, for example.
-include $(OBJECT_FILES:%.o=%.d)


####
# Static Lib
#
$(OBJ_DIR)/%.a: $(OBJECT_FILES)
	$(call mkdir_cmd,$@)
	$(info Generating archive $@)
	@$(AR) $(AR_FLAGS) $@ $^


