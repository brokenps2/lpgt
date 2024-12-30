SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := -Iinclude/
LDFLAGS := -L/usr/lib -g -l:libglfw.so -lm -lpthread -lGL -lopenal -lcimgui -l:libGLEW.so -l:libconfig.so 
CPPFLAGS := -Wall -O2 -pthread

bin/gtma: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CPPFLAGS) -c -o $@ $<
