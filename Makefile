SRC_DIR=src
OBJ_DIR=build
DIST_DIR=dist
INCLUDE_DIR=include

INSTALLATION_PACKAGES_DIR=$(DIST_DIR)/packages
DEB_INSTALLATION_DIR=$(INSTALLATION_PACKAGES_DIR)/linux/deb
DEB_PACKAGE_NAME=tetris_1.0-1_amd64
DEB_PACKAGE=$(INSTALLATION_PACKAGES_DIR)/linux/deb/$(DEB_PACKAGE_NAME)

WIN_INSTALLATION_DIR=$(INSTALLATION_PACKAGES_DIR)/windows

OBJ_FILES=$(OBJ_DIR)/main.o $(OBJ_DIR)/highscore.o $(OBJ_DIR)/vektor.o

all: tetris 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -I $(INCLUDE_DIR) -c $< -o $@

tetris: $(OBJ_FILES)
	gcc -I $(INCLUDE_DIR) -o $(DIST_DIR)/tetris $(OBJ_FILES) -lrt -lm

win: $(OBJ_FILES)
	gcc -I $(INCLUDE_DIR) -o $(WIN_INSTALLATION_DIR)/tetris $(OBJ_FILES) 

deb: tetris
	mkdir $(DEB_INSTALLATION_DIR)/$(DEB_PACKAGE_NAME)
	mkdir -p $(DEB_PACKAGE)/usr/local/bin
	cp $(DIST_DIR)/tetris $(DEB_PACKAGE)/usr/local/bin
	mkdir -p $(DEB_PACKAGE)/DEBIAN 
	cp $(DEB_INSTALLATION_DIR)/controlFile $(DEB_PACKAGE)/DEBIAN/control 
	dpkg-deb --build --root-owner-group $(DEB_PACKAGE)
		
clean:
	rm ./build/*.o
	rm -rf ./dist/packages/windows/*.exe
	rm -rf $(DEB_PACKAGE)
	rm -rf $(DEB_PACKAGE).deb
	rm -rf ./dist/packages/mac/*
	rm ./dist/tetris
	rm ./dist/highscore.txt