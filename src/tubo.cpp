#include "tubo.hpp"
#include "constants.h"
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <cstdio>

ALLEGRO_BITMAP* Tubo::imagem_cima = nullptr;
ALLEGRO_BITMAP* Tubo::imagem_baixo = nullptr;

void carregar_imagens_tubo() {
    Tubo::imagem_cima = al_load_bitmap("assets/cimaori.png");
    Tubo::imagem_baixo = al_load_bitmap("assets/baixoori.png");
}

void destruir_imagens_tubo() {
    al_destroy_bitmap(Tubo::imagem_cima);
    al_destroy_bitmap(Tubo::imagem_baixo);
}

Tubo::Tubo(int posicao_x) {
    this->x = posicao_x;
    this->altura_abertura = rand() % (ALTURA_TELA - ESPACO_ENTRE_TUBOS - 150) + 75;
}

void Tubo::atualizar() {
    this->x -= VELOCIDADE_TUBO;
}

bool Tubo::estaForaDaTela() {
    return (this->x + LARGURA_TUBO < 0);
}

void Tubo::resetar(int nova_posicao_x) {
    this->x = nova_posicao_x;
    this->altura_abertura = rand() % (ALTURA_TELA - ESPACO_ENTRE_TUBOS - 150) + 75;
}

void Tubo::desenhar() {
    al_draw_bitmap(imagem_cima, this->x, this->altura_abertura - ALTURA_TUBO, 0);
    al_draw_bitmap(imagem_baixo, this->x, this->altura_abertura + ESPACO_ENTRE_TUBOS, 0);
}

bool Tubo::colide(int jogador_x, int jogador_y, int jogador_largura, int jogador_altura) {
    if (jogador_x + jogador_largura > this->x && jogador_x < this->x + LARGURA_TUBO) {
        if (jogador_y < this->altura_abertura) {
            return true;
        }
        
        if (jogador_y + jogador_altura > this->altura_abertura + ESPACO_ENTRE_TUBOS) {
            return true;
        }
    }