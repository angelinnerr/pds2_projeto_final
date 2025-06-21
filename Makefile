CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude `pkg-config --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5`

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

all: main

main: obj/main.o obj/fundo.o obj/jogador.o obj/tubo.o obj/pontos.o obj/excecoes.o obj/inicializador.o obj/cadastro.o
	g++ obj/main.o obj/fundo.o obj/jogador.o obj/tubo.o obj/pontos.o obj/excecoes.o obj/inicializador.o obj/cadastro.o -o main `pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 allegro_dialog-5`

obj/main.o: src/main.cpp include/constants.h include/cadastro.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/main.o -c src/main.cpp

obj/fundo.o: src/fundo.cpp include/fundo.hpp include/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/fundo.o -c src/fundo.cpp

obj/jogador.o: src/jogador.cpp include/jogador.hpp include/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/jogador.o -c src/jogador.cpp

obj/tubo.o: src/tubo.cpp include/tubo.hpp include/constants.h
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/tubo.o -c src/tubo.cpp

obj/pontos.o: src/pontos.cpp include/pontos.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/pontos.o -c src/pontos.cpp

obj/excecoes.o: src/excecoes.cpp include/excecoes.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/excecoes.o -c src/excecoes.cpp

obj/inicializador.o: src/inicializador.cpp include/inicializador.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/inicializador.o -c src/inicializador.cpp
obj/cadastro.o: src/cadastro.cpp include/cadastro.hpp
	mkdir -p obj
	g++ $(CXXFLAGS) -o obj/cadastro.o -c src/cadastro.cpp

clean:
	rm -rf obj main

copy_assets:
	@cp -r assets/ bin/