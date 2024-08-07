#             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id$
#

CODECLIB := $(CODECDIR)/libcodec.a
CODECLIB_SRC := $(call preprocess, $(RBCODECLIB_DIR)/codecs/lib/SOURCES)
CODECLIB_OBJ := $(call c2obj, $(CODECLIB_SRC))
OTHER_SRC += $(CODECLIB_SRC)

$(CODECLIB): $(CODECLIB_OBJ)
	$(SILENT)$(shell rm -f $@)
	$(call PRINTS,AR $(@F))$(AR) rcs $@ $^ >/dev/null

CODECLIBFLAGS = $(CODECFLAGS)

# Do not use '-ffunction-sections' when compiling sdl-sim
ifneq ($(findstring sdl-sim, $(APP_TYPE)), sdl-sim)
    CODECLIBFLAGS += -ffunction-sections
endif

ifdef USE_LTO
# We do NOT want LTO on the GCC support file
$(CODECDIR)/lib/codeclib.o: CODECLIBFLAGS += -fno-lto
endif

$(CODECDIR)/lib/%.o: $(RBCODECLIB_DIR)/codecs/lib/%.c
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(ROOTDIR)/,,$<))$(CC) \
		-I$(dir $<) $(CODECLIBFLAGS) -c $< -o $@
