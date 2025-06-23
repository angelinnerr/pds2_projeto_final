#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

void verificarInicializacao(bool condicao, const std::string& descricao);

class Jogo {
public:
    Jogo();
    ~Jogo();

    void inicializar();
    void finalizar();

    // getters
    ALLEGRO_DISPLAY* getDisplay() const;
    ALLEGRO_EVENT_QUEUE* getFilaEventos() const;
    ALLEGRO_TIMER* getTimer() const;
    ALLEGRO_FONT* getFonte() const;
    ALLEGRO_FONT* getFonte2() const;
    ALLEGRO_FONT* getSombraFonte2() const;
    ALLEGRO_SAMPLE_INSTANCE* getMusicaTema() const;
    ALLEGRO_SAMPLE_INSTANCE* getMusicaTransicao() const;

private:

    ALLEGRO_DISPLAY* tela = nullptr;
    ALLEGRO_EVENT_QUEUE* fila_eventos = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_FONT* fonte = nullptr;
    ALLEGRO_FONT* fonte2 = nullptr;
    ALLEGRO_FONT* sombra_fonte2 = nullptr;
    ALLEGRO_SAMPLE* musica_tema = nullptr;
    ALLEGRO_SAMPLE* musica_transicao = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* inst_musica_tema = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* inst_musica_transicao= nullptr;
};

#endif
