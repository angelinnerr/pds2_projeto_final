CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude `pkg-config --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5`

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

all: main

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/constants.h $(INC_DIR)/cadastro.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/main.o -c $(SRC_DIR)/main.cpp

$(OBJ_DIR)/fundo.o: $(SRC_DIR)/fundo.cpp $(INC_DIR)/fundo.hpp $(INC_DIR)/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/fundo.o -c $(SRC_DIR)/fundo.cpp  

$(OBJ_DIR)/jogador.o: $(SRC_DIR)/jogador.cpp $(INC_DIR)/jogador.hpp $(INC_DIR)/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/jogador.o -c $(SRC_DIR)/jogador.cpp

$(OBJ_DIR)/tubo.o: $(SRC_DIR)/tubo.cpp $(INC_DIR)/tubo.hpp $(INC_DIR)/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/tubo.o -c $(SRC_DIR)/tubo.cpp

$(OBJ_DIR)/pontos.o: $(SRC_DIR)/pontos.cpp $(INC_DIR)/pontos.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/pontos.o -c $(SRC_DIR)/pontos.cpp

$(OBJ_DIR)/excecoes.o: $(SRC_DIR)/excecoes.cpp $(INC_DIR)/excecoes.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/excecoes.o -c $(SRC_DIR)/excecoes.cpp

$(OBJ_DIR)/inicializador.o: $(SRC_DIR)/inicializador.cpp $(INC_DIR)/inicializador.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/inicializador.o -c $(SRC_DIR)/inicializador.cpp

$(OBJ_DIR)/cadastro.o: $(SRC_DIR)/cadastro.cpp $(INC_DIR)/cadastro.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/cadastro.o -c $(SRC_DIR)/cadastro.cpp

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/cadastro.o
	g++ $(OBJ_DIR)/main.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o obj/cadastro.o -o main `pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`

clean:
	rm -rf obj main

copy_assets:
	@cp -r assets/ bin/