#include "fundo.hpp"
#include "constants.h"
#include <allegro5/allegro.h>
#include <string>
#include <allegro5/allegro_image.h>

using namespace std;

Fundo::Fundo() {
    imagem = al_load_bitmap("assets/fundoori2.png");
    posicao_x = 0;
}

Fundo::~Fundo() {
    al_destroy_bitmap(imagem);
}

void Fundo::atualizar() {
    posicao_x -= VELOCIDADE_FUNDO;
    if (posicao_x <= -3200 + LARGURA_TELA) {
        posicao_x = 0;
    }
}

void Fundo::desenhar() {
    al_draw_bitmap(imagem, posicao_x, 0, 0);
}

void Fundo::carregar_imagem(const string& caminho_imagem) {
    if (imagem) {
        al_destroy_bitmap(imagem);
    }
    imagem = al_load_bitmap(caminho_imagem.c_str());
    posicao_x = 0; 
}