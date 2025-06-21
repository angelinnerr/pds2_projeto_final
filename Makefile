all: main

main: obj/main.o obj/fundo.o obj/jogador.o obj/tubo.o obj/pontos.o obj/excecoes.o obj/inicializador.o
	g++ obj/main.o obj/fundo.o obj/jogador.o obj/tubo.o obj/pontos.o obj/excecoes.o obj/inicializador.o -o main `pkg-config --libs allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5`

obj/main.o: src/main.cpp include/constants.h
	mkdir -p obj
	g++ -o obj/main.o -c src/main.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/fundo.o: src/fundo.cpp include/fundo.hpp include/constants.h
	mkdir -p obj
	g++ -o obj/fundo.o -c src/fundo.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/jogador.o: src/jogador.cpp include/jogador.hpp include/constants.h
	mkdir -p obj
	g++ -o obj/jogador.o -c src/jogador.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/tubo.o: src/tubo.cpp include/tubo.hpp include/constants.h
	mkdir -p obj
	g++ -o obj/tubo.o -c src/tubo.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/pontos.o: src/pontos.cpp include/pontos.hpp
	mkdir -p obj
	g++ -o obj/pontos.o -c src/pontos.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/excecoes.o: src/excecoes.cpp include/excecoes.hpp
	mkdir -p obj
	g++ -o obj/excecoes.o -c src/excecoes.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

obj/inicializador.o: src/inicializador.cpp include/inicializador.hpp
	mkdir -p obj
	g++ -o obj/inicializador.o -c src/inicializador.cpp -Iinclude -I/opt/homebrew/Cellar/allegro/5.2.10.1_1/include

clean:
	rm -rf obj main
