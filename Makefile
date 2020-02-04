CC=gcc
ARCH?=windows
TARGET=myler3

MYLER_PATH=myler
ARCH_PATH=$(MYLER_PATH)/$(ARCH)

CCFLAGS = -Wall  -Iinclude -std=c99 -g
LDFLAGS = 

ifeq ($(ARCH), windows)
	CCFLAGS += -fexec-charset=GBK
	LDFLAGS += -lwinmm
endif

ARCH_OBJ = $(ARCH_PATH)/myler_console.o  	\
           $(ARCH_PATH)/myler_music.o    	\

MYLER_OBJ = $(MYLER_PATH)/myler_utils.o  	\
			$(MYLER_PATH)/myler_window.o   \
            $(MYLER_PATH)/myler_ui.o     	\

MAIN_OBJ = $(MYLER_PATH)/main.o



all: $(TARGET)

$(TARGET): $(ARCH_OBJ) $(MYLER_OBJ) $(MAIN_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CCFLAGS)

clean:  
	rm -f $(TARGET) $(MYLER_PATH)/*.o $(ARCH_PATH)/*.o
