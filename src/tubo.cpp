#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <cstdio>

#include "tubo.hpp"
#include "constants.h"
#include "inicializador.hpp"

ALLEGRO_BITMAP* Tubo::imagem_cima = nullptr;
ALLEGRO_BITMAP* Tubo::imagem_baixo = nullptr;

ALLEGRO_BITMAP* Tubo::tubo_colorido_cima = nullptr;
ALLEGRO_BITMAP* Tubo::tubo_colorido_baixo = nullptr;

bool Tubo::usar_tubo_colorido = false;

void Tubo::set_usar_imagens_coloridas(bool usar) {
    Tubo::usar_tubo_colorido = usar;
}

void carregar_imagens_tubo() {
    Tubo::imagem_cima = al_load_bitmap(TUBO_CIMA);
    Tubo::imagem_baixo = al_load_bitmap(TUBO_BAIXO);
    verificarInicializacao(Tubo::imagem_cima && Tubo::imagem_baixo, "imagem do tubo");
}

void carregar_tubo_colorido() {
    Tubo::tubo_colorido_cima = al_load_bitmap(TUBO_COLORIDO_CIMA);
    Tubo::tubo_colorido_baixo = al_load_bitmap(TUBO_COLORIDO_BAIXO);
    verificarInicializacao(Tubo::tubo_colorido_cima && Tubo::tubo_colorido_baixo, "tubo colorido");
}

void destruir_imagens_tubo() {
    al_destroy_bitmap(Tubo::imagem_cima);
    al_destroy_bitmap(Tubo::imagem_baixo);
    al_destroy_bitmap(Tubo::tubo_colorido_cima);
    al_destroy_bitmap(Tubo::tubo_colorido_baixo);
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

    if (Tubo::usar_tubo_colorido) {
        al_draw_bitmap(Tubo::tubo_colorido_cima, this->x, this->altura_abertura - ALTURA_TUBO, 0);
        al_draw_bitmap(Tubo::tubo_colorido_baixo, this->x, this->altura_abertura + ESPACO_ENTRE_TUBOS, 0);
    }
    else{
        al_draw_bitmap(imagem_cima, this->x, this->altura_abertura - ALTURA_TUBO, 0);
        al_draw_bitmap(imagem_baixo, this->x, this->altura_abertura + ESPACO_ENTRE_TUBOS, 0);
    }
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
    
    return false;
}