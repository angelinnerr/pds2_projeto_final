#include <allegro5/allegro.h>
#include <string>
#include <allegro5/allegro_image.h>

#include "fundo.hpp"
#include "constants.h"
#include "inicializador.hpp"

using namespace std;

Fundo::Fundo() {
    imagem = al_load_bitmap(FUNDO_JOGO);
    verificarInicializacao(imagem, "imagem do fundo");
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
    verificarInicializacao(imagem, "imagem do fundo");
    posicao_x = 0; 
}

float Fundo::getPosicaoX() const{
    return posicao_x;
}