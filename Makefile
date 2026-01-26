# Configurable variables (override on the make command line)
# Prefer system clang on macOS to avoid picking up XMOS toolchain clang
# UNAME_S := $(shell uname -s)
# ifeq ($(UNAME_S),Darwin)
# CC := /usr/bin/clang
# else
CC := clang
# endif
ROOT_DIR := $(shell pwd)
ARCH ?= vx4b

SRC_BFP_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/bfp_tests/src/misc/*.c) \
			$(wildcard $(ROOT_DIR)/tests/bfp_tests/src/*.c) \
			$(wildcard $(ROOT_DIR)/tests/bfp_tests/src/bfp/*/*/*.c) \
			$(wildcard $(ROOT_DIR)/tests/bfp_tests/src/bfp/*/*.c) 

# Source files (common + arch-specific)
SRC_DCT_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/dct_tests/src/lib_dsp/*.c) \
			$(wildcard $(ROOT_DIR)/tests/dct_tests/src/*.c)

SRC_FFT_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/fft_tests/src/*.c)

SRC_FILTER_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/dct_tests/src/filter/*.c) \
			$(wildcard $(ROOT_DIR)/tests/dct_tests/src/*.c)

SRC_FILTER_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/filter_tests/src/filter/*.c) \
			$(wildcard $(ROOT_DIR)/tests/filter_tests/src/*.c)

SRC_SCALAR_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/scalar_tests/src/basic/*.c) \
			$(wildcard $(ROOT_DIR)/tests/scalar_tests/src/float/*.c) \
			$(wildcard $(ROOT_DIR)/tests/scalar_tests/src/util/*.c) \
			$(wildcard $(ROOT_DIR)/tests/scalar_tests/src/*.c)

SRC_VECT_COMMON := \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/matrix/*.c) \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/vect/*.c) \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/vect/complex/*.c) \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/vect/float/*.c) \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/vect/stat/*.c) \
			$(wildcard $(ROOT_DIR)/tests/vect_tests/src/*.c)

SRC_ARCH_vx4b := \
			$(wildcard $(ROOT_DIR)/../lib_unity/lib_unity/Unity/extras/*/src/*.c) \
			$(wildcard $(ROOT_DIR)/../lib_unity/lib_unity/Unity/src/*.c) \
			$(wildcard $(ROOT_DIR)/tests/shared/*/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/vect/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/scalar/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/filter/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/etc/xmath_fft_lut/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/fft/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/dct/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/bfp/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/bfp/*/*.c) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/**/*.S) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/*/*/*.S) \
			$(wildcard $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/*.c) \
			$(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/vect_s16/vect_s16_dot.c \
			$(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/chunk_s16/chunk_s16_accumulate.c \
			$(ROOT_DIR)/lib_xcore_math/src/arch/ref/vpu_scalar_ops.c


# Exclude problematic assembly file from the arch source list
# (keeps rest of the wildcard-based list intact but removes this one file)
SRC_ARCH_vx4b := $(filter-out $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/scalar/scalar_op_s16.S,$(SRC_ARCH_vx4b))
SRC_ARCH_vx4b := $(filter-out $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/scalar/scalar_op_s32.S,$(SRC_ARCH_vx4b))
SRC_ARCH_vx4b := $(filter-out $(ROOT_DIR)/lib_xcore_math/src/arch/vx4b/scalar/scalar_op_s8.S,$(SRC_ARCH_vx4b))

# SRC_ARCH_vx4b += $(ROOT_DIR)/lib_xcore_math/lib_xcore_math/src/arch/ref/vpu_scalar_ops.c

# Default include paths (adjust as required)
COMMON_INCLUDES := -I lib_nn/api \
	     -I $(ROOT_DIR)/../lib_unity/lib_unity/Unity/extras/fixture/src \
	     -I $(ROOT_DIR)/../lib_unity/lib_unity/Unity/src \
	     -I $(ROOT_DIR)/../lib_unity/lib_unity/Unity/extras/memory/src \
		 -I $(ROOT_DIR)/lib_xcore_math/api\
		 -I $(ROOT_DIR)/tests/shared/pseudo_rand\
		 -I $(ROOT_DIR)/tests/shared/testing\
		 -I $(ROOT_DIR)/tests/shared/floating_fft\
		 -I $(ROOT_DIR)/lib_xcore_math/src/etc/xmath_fft_lut \
		 -I $(ROOT_DIR)/lib_xcore_math/src/vect

# Arch-specific flags
ARCH_FLAGS_vx4b := -mcpu=xmos-vx4b -D__VX4B__ -DSMOKE_TEST

# XMOS toolchain (used for assembling/linking vx4 targets)
XCC := /Applications/XMOS_XTC_0.2.0/riscv-toolchain/bin/clang

# By default use CC for C compile. When building vx4/vx4b with the XMOS
# toolchain available, use the XMOS clang for C compile, assembling .S
# files and linking the final binaries so all object files target the
# same architecture (prevents mixed-format objects).
ASM_CC := $(CC)
LINK_CC := $(CC)
ifeq ($(findstring vx4,$(ARCH)),vx4)
ifneq ($(wildcard $(XCC)),)
CC := $(XCC)
ASM_CC := $(XCC)
LINK_CC := $(XCC)
endif
endif

# Common compile flags
COMMON_FLAGS := -Os

# Enable section-level GC: compile into individual sections and ask the linker
# to drop unused sections. These flags are safe for host builds but are
# essential for cross-built vx4 binaries to reduce final size.
COMMON_FLAGS += -ffunction-sections -fdata-sections -DUNITY_SUPPORT_64=1

# Linker flags (pass to the linker via the compiler driver)
LDFLAGS := -Wl,--gc-sections

CFLAGS := $(COMMON_FLAGS) $(ARCH_FLAGS_$(ARCH)) $(COMMON_INCLUDES)  -I $(ROOT_DIR)/tests/bfp_tests/src 

# Build directories
OBJDIR := $(ROOT_DIR)/build/$(ARCH)/obj
BINDIR := $(ROOT_DIR)/build/$(ARCH)/bin

# Ensure arch-specific variable names resolve
SRC_ARCH := $(SRC_ARCH_$(ARCH))

# Helper: convert source list to object list under OBJDIR
define objs_from_src
  $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(patsubst $(ROOT_DIR)/%,$(OBJDIR)/%,$(1))))
endef

OBJ_BFP := $(call objs_from_src,$(SRC_BFP_COMMON) $(SRC_ARCH))
OBJ_DCT := $(call objs_from_src,$(SRC_DCT_COMMON) $(SRC_ARCH))
OBJ_FFT := $(call objs_from_src,$(SRC_FFT_COMMON) $(SRC_ARCH))
OBJ_FILTER := $(call objs_from_src,$(SRC_FILTER_COMMON) $(SRC_ARCH))
OBJ_SCALAR := $(call objs_from_src,$(SRC_SCALAR_COMMON) $(SRC_ARCH))
OBJ_VECT := $(call objs_from_src,$(SRC_VECT_COMMON) $(SRC_ARCH))

# Pattern rules to build objects
$(OBJDIR)/%.o: $(ROOT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: $(ROOT_DIR)/%.S
	@mkdir -p $(dir $@)
	$(ASM_CC) -c $< -o $@ $(CFLAGS)

# .PHONY: all build clean
all: filter bfp dct fft scalar vect
all: vect

bfp: $(BINDIR)/bfp


$(BINDIR)/bfp: $(OBJ_BFP)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

dct: $(BINDIR)/dct


$(BINDIR)/dct: $(OBJ_DCT)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

fft: $(BINDIR)/fft


$(BINDIR)/fft: $(OBJ_FFT)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

filter: $(BINDIR)/filter


$(BINDIR)/filter: $(OBJ_FILTER)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

scalar: $(BINDIR)/scalar


$(BINDIR)/scalar: $(OBJ_SCALAR)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

vect: $(BINDIR)/vect


$(BINDIR)/vect: $(OBJ_VECT)
	@mkdir -p $(dir $@)
	$(LINK_CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) -I $(ROOT_DIR)/lib_xcore_math/src/vect -I $(ROOT_DIR)/tests/vect_tests/src


clean:
	@echo "Cleaning $(ROOT_DIR)/build/$(ARCH)"
	rm -rf $(ROOT_DIR)/build/$(ARCH)

run: 
# 	- xsim $(BINDIR)/bfp --config-file config.xml
# 	- xsim $(BINDIR)/dct --config-file config.xml
	- xsim $(BINDIR)/fft --config-file config.xml
# 	- xsim $(BINDIR)/filter --config-file config.xml
# 	- xsim $(BINDIR)/scalar --config-file config.xml
# 	- xsim $(BINDIR)/vect --config-file config.xml

trace: 
	- xsim $(BINDIR)/fft --config-file config.xml -t > trace.txt