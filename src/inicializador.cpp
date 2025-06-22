#include "inicializador.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "constants.h"
#include "excecoes.hpp"

void verificarInicializacao(bool condicao, const std::string& descricao) {
    if (!condicao) {
        throw ErroDeInicializacao("Não foi possível inicializar " + descricao);
    }
}

Jogo::Jogo() {}

Jogo::~Jogo() {
    finalizar();
}

void Jogo::inicializar() {
    srand(time(0));

    verificarInicializacao(al_init(), "Allegro");
    verificarInicializacao(al_install_keyboard(), "keyboard");
    verificarInicializacao(al_init_image_addon(), "addon de imagem");
    verificarInicializacao(al_init_font_addon(), "addon de fonte");
    verificarInicializacao(al_init_ttf_addon(), "addon ttf");
    verificarInicializacao(al_init_primitives_addon(), "addon de primitivas");
    verificarInicializacao(al_install_audio(), "áudio");
    verificarInicializacao(al_init_acodec_addon(), "addon acodec");
    al_reserve_samples(1);

    tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    verificarInicializacao(tela, "display");

    fila_eventos = al_create_event_queue();
    verificarInicializacao(fila_eventos, "fila de eventos");

    timer = al_create_timer(1.0 / 50);
    verificarInicializacao(timer, "timer");

    fonte = al_load_ttf_font(FONTE_BASE, 32, 0);
    verificarInicializacao(fonte, "fonte principal");

    fonte2 = al_load_ttf_font(FONTE_GAME_OVER, 150, 0);
    verificarInicializacao(fonte2, "fonte de game over");

    sombra_fonte2 = al_load_ttf_font(FONTE_GAME_OVER, 155, 0);
    verificarInicializacao(sombra_fonte2, "sombra da fonte de game over");

    musica_tema = al_load_sample(MUSICA);
    verificarInicializacao(musica_tema, "música tema");

    inst_musica_tema = al_create_sample_instance(musica_tema);
    al_attach_sample_instance_to_mixer(inst_musica_tema, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_musica_tema, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_musica_tema, 0.8);

    al_register_event_source(fila_eventos, al_get_display_event_source(tela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    al_start_timer(timer);
}

void Jogo::finalizar() {
    if (inst_musica_tema) al_destroy_sample_instance(inst_musica_tema);
    if (musica_tema) al_destroy_sample(musica_tema);
    if (sombra_fonte2) al_destroy_font(sombra_fonte2);
    if (fonte2) al_destroy_font(fonte2);
    if (fonte) al_destroy_font(fonte);
    if (timer) al_destroy_timer(timer);
    if (fila_eventos) al_destroy_event_queue(fila_eventos);
    if (tela) al_destroy_display(tela);

    tela = nullptr;
    fila_eventos = nullptr;
    timer = nullptr;
    fonte = nullptr;
    fonte2 = nullptr;
    sombra_fonte2 = nullptr;
    musica_tema = nullptr;
    inst_musica_tema = nullptr;
}

ALLEGRO_DISPLAY* Jogo::getDisplay() const { return tela; }
ALLEGRO_EVENT_QUEUE* Jogo::getFilaEventos() const { return fila_eventos; }
ALLEGRO_TIMER* Jogo::getTimer() const { return timer; }
ALLEGRO_FONT* Jogo::getFonte() const { return fonte; }
ALLEGRO_FONT* Jogo::getFonte2() const { return fonte2; }
ALLEGRO_FONT* Jogo::getSombraFonte2() const { return sombra_fonte2; }
ALLEGRO_SAMPLE_INSTANCE* Jogo::getMusicaTema() const { return inst_musica_tema; }
