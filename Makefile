SRC_DIR=src
OBJ_DIR=build
DIST_DIR=dist
INCLUDE_DIR=include

OBJ_FILES=$(OBJ_DIR)/main.o $(OBJ_DIR)/highscore.o $(OBJ_DIR)/vektor.o

all: tetris

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -I $(INCLUDE_DIR) -c $< -o $@

tetris: $(OBJ_FILES)
	gcc -I $(INCLUDE_DIR) -o $(DIST_DIR)/tetris $(OBJ_FILES) -lrt -lm
	
clean:
	rm ./build/*
	rm ./dist/*