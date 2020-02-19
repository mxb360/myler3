CC = gcc
TARGET = myler3

ifeq ($(shell uname), Linux)
	ARCH = linux
else
	ARCH = windows
endif

MYLER_PATH=src
ARCH_PATH=$(MYLER_PATH)/$(ARCH)

CCFLAGS = -g -Wall -MD -Werror -Iinclude -std=c99
LDFLAGS =

ifeq ($(ARCH), windows)
	LDFLAGS += -lwinmm
endif

ARCH_OBJ = $(ARCH_PATH)/myler_console.o     \
           $(ARCH_PATH)/myler_music.o       \
           $(ARCH_PATH)/myler_system.o      \

MYLER_OBJ = $(MYLER_PATH)/myler_utils.o     \
			$(MYLER_PATH)/myler_window.o    \
            $(MYLER_PATH)/myler_ui.o     	\
            $(MYLER_PATH)/myler_song.o     	\
            $(MYLER_PATH)/myler_lyric.o     \
            $(MYLER_PATH)/myler_list.o      \
            $(MYLER_PATH)/myler_listwin.o   \

MAIN_OBJ = $(MYLER_PATH)/main.o


all: $(TARGET)

$(TARGET): $(ARCH_OBJ) $(MYLER_OBJ) $(MAIN_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

-include $(MYLER_PATH)/*.d
-include $(ARCH_PATH)/*.d

%.o: %.c Makefile
	$(CC) -c $< -o $@ $(CCFLAGS)

clean:
	rm -f $(TARGET)
	rm -f $(MYLER_PATH)/*.o $(MYLER_PATH)/windows/*.o $(MYLER_PATH)/linux/*.o
	rm -f $(MYLER_PATH)/*.d $(MYLER_PATH)/windows/*.d $(MYLER_PATH)/linux/*.d
