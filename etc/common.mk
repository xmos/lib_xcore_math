

# ifeq ($(OS),Windows_NT)
# getdir_cmd 
# else

# endif

ifeq ($(OS),Windows_NT)
mkdir_cmd = @test -d $(subst /,\,$(dir $(1))) || mkdir $(subst /,\,$(dir $(1)))
else
mkdir_cmd = @mkdir -p $(dir $(1))
endif

check_defined = $(strip $(foreach 1,$1, $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = $(if $(value $1),, $(error Undefined $1$(if $2, ($2))))

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

define newline


endef

#####
# rename_variables(prefix, var_names[])
#    Renames each of the variables X in argument $(2) to $(1)_X, and then
#    deletes X
rename_variables=$(eval $(foreach var,$(2),$(1)_$(var):=$($(var))$(newline)$(var):=$(newline)))
# rename_variables_print=$(info $(foreach var,$(2),$(1)_$(var):=$($(var))$(newline)undefine $(var)$(newline)))



###
# Load the dependency with name $(1)
# Afterwards, rename each variable X in $(2) to $(1)_X
# The optional third argument $(3) is the .mk file to be
# loaded. If $(3) is not provided, ./etc/$(1).mk will be
# loaded instead.
# TODO: Make it so it looks for lib.mk file in the library path
#		if it isn't in ./etc/
define load_dependency_

  ifneq ($$(strip $(3)),)
    MK_FILE := $(3)
    MK_FILE := $$(wildcard $$(MK_FILE))
    ifeq ($$(strip $$(MK_FILE)),)
      $$(error Unable to find .mk file for dependency $(1). Looked for '$$($(1)_MK_FILE)` (via $(1)_MK_FILE))
    endif
  else
    MK_FILE := ./etc/$(1).mk
    MK_FILE := $$(wildcard $$(MK_FILE))
    ifeq ($$(strip $$(MK_FILE)),)
      MK_FILE := $($(1)_PATH)/lib.mk
    endif
  endif

  MK_FILE := $$(wildcard $$(MK_FILE))
  ifeq ($$(strip $$(MK_FILE)),)
    $$(error Unable to find .mk file for dependency $(1). The paths checked are ./etc/$(1).mk, followed by $$$$($(1)_PATH)/lib.mk. Alternatively, the exact location can be specified by defining $(1)_MK_FILE)
  endif

  MK_FILE := $$(abspath $$(MK_FILE))
  include $$(MK_FILE)
  $$(call rename_variables,$(1),$(2))
endef
load_dependency=$(eval $(call load_dependency_,$(1),$(2),$(3)))
# load_dependency=$(info $(call load_dependency_,$(1),$(2),$(3)))



MAP_COMP_c   = CC
MAP_COMP_cc  = CC
MAP_COMP_xc  = XCC
MAP_COMP_cpp = CXX
MAP_COMP_S   = AS