CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Iinclude `pkg-config --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5`

SRC_DIR = src
TESTS_DIR = tests
BIN_DIR = bin
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

$(OBJ_DIR)/cadastro.o: $(SRC_DIR)/cadastro.cpp $(INC_DIR)/cadastro.hpp $(INC_DIR)/excecoes.hpp 
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/cadastro.o -c $(SRC_DIR)/cadastro.cpp

$(OBJ_DIR)/definicoes_assets.o: $(SRC_DIR)/definicoes_assets.cpp $(INC_DIR)/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o $(OBJ_DIR)/definicoes_assets.o -c $(SRC_DIR)/definicoes_assets.cpp

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/definicoes_assets.o
	mkdir -p $(BIN_DIR)
	g++ $(OBJ_DIR)/main.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/definicoes_assets.o -o ./$(BIN_DIR)/programa `pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`

clean:
	rm -rf obj main

copy_assets:
	@cp -r assets/ bin/

# TESTES:

# teste da classe 'Jogo'
$(OBJ_DIR)/testes_inicializador.o: $(TESTS_DIR)/testes_inicializador.cpp $(INC_DIR)/doctest.h $(INC_DIR)/inicializador.hpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_inicializador.o -c $(TESTS_DIR)/testes_inicializador.cpp

teste_inicializador: $(OBJ_DIR)/testes_inicializador.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/definicoes_assets.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_inicializador \
	$(OBJ_DIR)/testes_inicializador.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/definicoes_assets.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_inicializador

# teste das classes 'RegistroJogador' e 'Cadastro'
$(OBJ_DIR)/testes_cadastro.o: $(TESTS_DIR)/testes_cadastro.cpp $(INC_DIR)/doctest.h $(INC_DIR)/cadastro.hpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_cadastro.o -c $(TESTS_DIR)/testes_cadastro.cpp 

teste_cadastro: $(OBJ_DIR)/testes_cadastro.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/definicoes_assets.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_cadastro \
	$(OBJ_DIR)/testes_cadastro.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/definicoes_assets.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_cadastro

# teste da classe 'Jogador'
$(OBJ_DIR)/testes_jogador.o: $(TESTS_DIR)/testes_jogador.cpp $(INC_DIR)/doctest.h $(INC_DIR)/jogador.hpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_jogador.o -c $(TESTS_DIR)/testes_jogador.cpp 

teste_jogador: $(OBJ_DIR)/testes_jogador.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/definicoes_assets.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_jogador \
	$(OBJ_DIR)/testes_jogador.o $(OBJ_DIR)/jogador.o $(OBJ_DIR)/excecoes.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/definicoes_assets.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_jogador

# teste da classe 'Fundo'
$(OBJ_DIR)/testes_fundo.o: $(TESTS_DIR)/testes_fundo.cpp $(INC_DIR)/doctest.h $(INC_DIR)/fundo.hpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_fundo.o -c $(TESTS_DIR)/testes_fundo.cpp 

teste_fundo: $(OBJ_DIR)/testes_fundo.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_fundo \
	$(OBJ_DIR)/testes_fundo.o $(OBJ_DIR)/fundo.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_fundo

# teste da classe 'Pontos'
$(OBJ_DIR)/testes_pontos.o: $(TESTS_DIR)/testes_pontos.cpp $(INC_DIR)/doctest.h $(INC_DIR)/pontos.hpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_pontos.o -c $(TESTS_DIR)/testes_pontos.cpp 

teste_pontos: $(OBJ_DIR)/testes_pontos.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_pontos \
	$(OBJ_DIR)/testes_pontos.o $(OBJ_DIR)/pontos.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_pontos

# teste da classe 'Tubo'
$(OBJ_DIR)/testes_tubo.o: $(TESTS_DIR)/testes_tubo.cpp $(INC_DIR)/doctest.h $(INC_DIR)/tubo.hpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/testes_tubo.o -c $(TESTS_DIR)/testes_tubo.cpp 

teste_tubo: $(OBJ_DIR)/testes_tubo.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/teste_tubo \
	$(OBJ_DIR)/testes_tubo.o $(OBJ_DIR)/tubo.o $(OBJ_DIR)/inicializador.o $(OBJ_DIR)/definicoes_assets.o $(OBJ_DIR)/excecoes.o \
	`pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`
	./$(BIN_DIR)/teste_tubo
