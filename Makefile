SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := -Iinclude/
LDFLAGS := -ggdb -Llib/ -lgdi32 -luser32 -lwinmm -lkernel32 -lglfw3 -lm -lopengl32 -lopenal -lglew32 -llibconfig
CPPFLAGS := -Wall -ggdb

all:
	gcc $(SRC_FILES) $(CPPFLAGS) $(INCLUDES) $(LDFLAGS) -o bin/program

#bin/program: $(OBJ_FILES)
#	gcc -o $@ $(LDFLAGS) $^

#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
#	gcc $(CPPFLAGS) $(INCLUDES) -c -o $@ $<
