#include "pontos.hpp"
#include "constants.h"
#include <iostream> 
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <string>

ALLEGRO_FONT* Pontos::fonte_score = nullptr;
ALLEGRO_FONT* Pontos::sombra_fonte = nullptr;

ALLEGRO_FONT* Pontos::fonte_game_over = nullptr;
ALLEGRO_FONT* Pontos::sombra_game_over = nullptr;

Pontos::Pontos() {
    this->score = 0;
    if (!fonte_score or !sombra_fonte) {
        fonte_score = al_load_ttf_font("assets/game_over.ttf", 180, 0);
        sombra_fonte = al_load_ttf_font("assets/game_over.ttf", 190, 0);
        if (!fonte_score or !sombra_fonte) {
            std::cerr << "Não foi possível carregar fonte do score" << std::endl;
        }
    }
}

void Pontos::verificar(int posicaoJogador, int posicaoTubo){
    if (posicaoJogador == posicaoTubo) this->score++;
}

void Pontos::atualizar(){
    std::string score_text = std::to_string(score);
    al_draw_text(sombra_fonte, al_map_rgb(130,0,130), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTER, score_text.c_str());
    al_draw_text(fonte_score, al_map_rgb(255,255,255), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTER, score_text.c_str());
}

