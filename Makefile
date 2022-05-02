ifeq ($(HEAP_SIZE),)
HEAP_SIZE      = 8388208
endif
ifeq ($(STACK_SIZE),)
STACK_SIZE     = 61800
endif

ifeq ($(PRODUCT),)
PRODUCT = CoroutineTest.pdx
endif

SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

$(shell mkdir -p Source)
$(shell echo 'function playdate.update() end' > Source/main.lua)

# List C source files here
SRC += pdco.c main.c

# List all user directories here
UINCDIR += $(SELF_DIR)/mini3d-plus

# List all user C define here, like -D_DEBUG=1
UDEFS += -DPLAYDATE=1

# Define ASM defines here
UADEFS +=

# List the user directory to look for the libraries here
ULIBDIR +=

# List all user libraries here
ULIBS +=

CLANGFLAGS = -DPLAYDATE=1

include $(SDK)/C_API/buildsupport/common.mk	