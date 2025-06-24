/*
TESTE EM DOCTEST PARA MÉTODOS DA CLASSE 'Jogador', NO ARQUIVO "jogador.cpp"
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "jogador.hpp"
#include "constants.h"
#include "doctest.h"

TEST_CASE("Testes da classe Jogador") {
    // Inicializa Allegro para ter addon de imagem
    al_init();
    al_init_image_addon();

    Jogador jogador;

    SUBCASE("Verificação das atribuições feitas no construtor") {
        CHECK(jogador.x == 300);
        CHECK(jogador.y == ALTURA_TELA / 2);
        CHECK(jogador.imagem != nullptr);
        CHECK(jogador.frame_atual == 0);
        CHECK(jogador.tempo_frame == 0);
    }

    SUBCASE("Atualização do jogador sem pulo") {
        int posicaoYinicial = jogador.y;
        jogador.atualizar(); // y deve aumentar de acordo com a gravidade
        CHECK(jogador.y > posicaoYinicial); // verifica se caiu
        CHECK(jogador.y == doctest::Approx(ALTURA_TELA / 2 + GRAVIDADE)); // Verifica se caiu na proporção esperada
    }

    SUBCASE("Atualização do jogador com pulo") {
        int posicaoYinicial = jogador.y;
        jogador.pular();
        CHECK(jogador.y == posicaoYinicial); // Antes da atualização a posição deve se manter a mesma
        CHECK(jogador.velocidade_y == FORCA_PULO);
        jogador.atualizar(); // y deve diminuir de acordo com o pulo, mas considerando também a gravidade
        CHECK(jogador.velocidade_y == doctest::Approx(FORCA_PULO + GRAVIDADE)); 
        CHECK(jogador.y == posicaoYinicial + jogador.velocidade_y);
    }

    SUBCASE("Mudança dos frames") {
        int frame_anterior = jogador.frame_atual;
        for (int i = 0; i <= 6; ++i) {
            jogador.atualizar();
        }
        CHECK(jogador.frame_atual != frame_anterior);
    }

}

