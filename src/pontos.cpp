#include <iostream> 
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <string>

#include "pontos.hpp"
#include "constants.h"
#include "inicializador.hpp"
#include "excecoes.hpp"

ALLEGRO_FONT* Pontos::fonte_score = nullptr;
ALLEGRO_FONT* Pontos::sombra_fonte = nullptr;

ALLEGRO_FONT* Pontos::fonte_game_over = nullptr;
ALLEGRO_FONT* Pontos::sombra_game_over = nullptr;

Pontos::Pontos(bool deve_carregar_fontes) {
    this->score = 0;

    if(deve_carregar_fontes) {
        try{
            carrega_fontes();
        }catch (const ErroDeInicializacao& erro){
            std::cerr << erro.what() << std::endl;
            exit(1);
        }
    }
}

void Pontos::carrega_fontes() {
    fonte_score = al_load_ttf_font(FONTE_GAME_OVER, 180, 0);
    verificarInicializacao(fonte_score, "fonte do score em pontos");

    sombra_fonte = al_load_ttf_font(FONTE_GAME_OVER, 190, 0);
    verificarInicializacao(sombra_fonte, "fonte de sombra em pontos");  
}

void Pontos::verificar(int posicaoJogador, int posicaoTubo){
    if (posicaoJogador == posicaoTubo) this->score++;
}

void Pontos::atualizar(){
    std::string score_text = std::to_string(score);
    al_draw_text(sombra_fonte, al_map_rgb(130,0,130), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTER, score_text.c_str());
    al_draw_text(fonte_score, al_map_rgb(255,255,255), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTER, score_text.c_str());
}

void Pontos::reset() {
    this->score = 0;
}

int Pontos::getScore() const {
    return score;
}


