#
# Copyright © 2017 Samuel Holland <samuel@sholland.org>
# SPDX-License-Identifier: GPL-2.0
#

CROSS_COMPILE	?= or1k-linux-musl-
CC		 = $(CROSS_COMPILE)cc
CPP		 = $(CROSS_COMPILE)cpp
OBJCOPY		 = $(CROSS_COMPILE)objcopy

CFLAGS_ALL	 = -Os -pipe -std=c11 \
		   -fdata-sections \
		   -ffreestanding \
		   -ffunction-sections \
		   -fno-asynchronous-unwind-tables \
		   -fno-common \
		   -fomit-frame-pointer \
		   -funsigned-char \
		   $(call debug,-ggdb,-g0) \
		   -mcmov -mdelay -mhard-mul -msoft-float \
		   -Wa,--fatal-warnings \
		   $(warnings) \
		   $(CFLAGS)
CPPFLAGS_ALL	 = $(call debug,-DDEBUG=1,-DDEBUG=0) \
		   $(addprefix -I,$(incdirs)) \
		   -nostdinc \
		   -Werror=missing-include-dirs \
		   $(CPPFLAGS)
LDFLAGS_ALL	 = -nostdlib \
		   -Wl,-O1 \
		   -Wl,--build-id=none \
		   -Wl,--fatal-warnings \
		   -Wl,--gc-sections \
		   -Wl,--no-undefined \
		   $(LDFLAGS)

HOSTCC		 = cc
HOSTCFLAGS	 = -O2 -pipe -std=c11 \
		   $(warnings)
HOSTCPPFLAGS	 =
HOSTLDFLAGS	 =
HOSTLIBS	 =

debug		 = $(if $(filter-out 0,$(DEBUG)),$1,$2)
warnings	 = -Wall -Wextra -Wformat=2 -Wpedantic -Wshadow \
		   -Werror=implicit-function-declaration \
		   -Werror=implicit-int \
		   -Werror=pointer-arith \
		   -Werror=pointer-sign \
		   -Werror=strict-prototypes

boards		 = $(sort $(basename $(notdir $(wildcard $(srcdir)/board/*))))
boardconfig	 = $(srcdir)/board/$(BOARD).mk

incdirs		 = $(addprefix $(srcdir)/,include platform/$(PLATFORM)/include)
includes	 = $(wildcard $(addsuffix /*.h,$(incdirs)) $(addsuffix /*/*.h,$(incdirs))) \
		   $(wildcard $(addsuffix /*.S,$(incdirs)) $(addsuffix /*/*.S,$(incdirs)))

objdir		 = build
objdirs		 = $(sort $(dir $(objects)))
objects		 = $(patsubst $(srcdir)/%,$(objdir)/%.o,$(sources))

outputs		 = $(addprefix $(objdir)/,scp.bin scp.elf scp.map)

srcdir		 = .
srcdirs		 = $(addprefix $(srcdir)/,common drivers/* lib platform/$(PLATFORM))
sources		 = $(wildcard $(addsuffix /*.c,$(srcdirs)) $(addsuffix /*.S,$(srcdirs)))

tools		 = $(patsubst $(srcdir)/%.c,$(objdir)/%,$(toolsources))
toolsources	 = $(wildcard $(srcdir)/tools/*.c)

-include config.mk
ifeq ($(BOARD),)
$(error You must set BOARD, e.g. `make BOARD=pine64`. Available boards: $(boards))
endif

include $(boardconfig)
ifeq ($(PLATFORM),)
$(error The configuration for "$(BOARD)" ($(boardconfig)) must set PLATFORM)
endif

M := @$(if $(filter-out 0,$(V)),:,printf '  %-7s %s\n')
Q :=  $(if $(filter-out 0,$(V)),,@)

all: $(outputs) $(tools)

clean:
	$(M) CLEAN
	$(Q) rm -rf $(objdir)

image: $(outputs)

tools: $(tools)

$(objdir) $(objdirs) $(objdir)/tools:
	$(Q) mkdir -p $@

$(objdir)/%.bin: $(objdir)/%.elf | $(objdir)
	$(M) OBJCOPY $@
	$(Q) $(OBJCOPY) -O binary -S --reverse-bytes 4 $< $@

$(objdir)/%.elf $(objdir)/%.map: $(objdir)/%.ld $(objects) | $(objdir)
	$(M) CCLD $@
	$(Q) $(CC) $(CFLAGS_ALL) $(LDFLAGS_ALL) -Wl,-Map,$(objdir)/$*.map -o $(objdir)/$*.elf -T $^

$(objdir)/%.ld: $(srcdir)/scripts/%.ld.S $(incdirs) $(includes) | $(objdir)
	$(M) CPP $@
	$(Q) $(CPP) $(CPPFLAGS_ALL) -o $@ -P $<

$(objdir)/%.o: $(srcdir)/% $(incdirs) $(includes) | $(objdirs)
	$(M) CC $@
	$(Q) $(CC) $(CPPFLAGS_ALL) $(CFLAGS_ALL) -c -o $@ $<

$(objdir)/tools/%: $(srcdir)/tools/%.c | $(objdir)/tools
	$(M) HOSTCC $@
	$(Q) $(HOSTCC) $(HOSTCPPFLAGS) $(HOSTCFLAGS) $(HOSTLDFLAGS) -o $@ $< $(HOSTLIBS)

.PHONY: all clean image tools
.SECONDARY:
.SUFFIXES:
