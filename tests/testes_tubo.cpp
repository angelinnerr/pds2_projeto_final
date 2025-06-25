#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tubo.hpp"
#include "constants.h"

TEST_CASE("Verificar funcionamento do tubo") {

    SUBCASE("Colocar tubo colorido") {
        Tubo::set_usar_imagens_coloridas(true);
        CHECK(Tubo::usar_tubo_colorido == true);
    }

    int pos_inicial = 100;
    Tubo tubo(pos_inicial);

    SUBCASE("Construtor") {
        CHECK(tubo.x == pos_inicial);
        // como altura_abertura é um valor aleatório que está entre 75 e 324:
        CHECK(tubo.altura_abertura <= 324);
        CHECK(tubo.altura_abertura >= 75);
    }

    SUBCASE("Atualização da posição") {
        int xTeste = tubo.x;
        tubo.atualizar();
        CHECK(tubo.x == xTeste - VELOCIDADE_TUBO);
    }

    SUBCASE("Verificar se está fora da tela"){
        int pos_inicial = 200;
        Tubo tubo(pos_inicial);

        for(int i = 0; i < 100; i++){ // Torna a posição do tubo = 0
            tubo.atualizar(); 
        }

        CHECK(tubo.x == 0);
       
        // SE tubo.x + LARGURA_TUBO > 0, estaForaDaTela() deve retornar false
        CHECK(tubo.x + LARGURA_TUBO == 110);
        CHECK(tubo.estaForaDaTela() == false);

        // loop para colocar tubo.x + LARGURA_TUBO < 0:
        for(int i = 0; i < 60; i++){ 
            tubo.atualizar(); 
        }
        CHECK(tubo.x + LARGURA_TUBO < 0);
        CHECK(tubo.estaForaDaTela() == true);
    }

    SUBCASE("Resetar tubo") {
        tubo.resetar(328);
        CHECK(tubo.x == 328);
        //Verificar se está fora da tela após resetar" :
        // tubo.x + LARGURA_TUBO = - (LARGURA_TUBO + 2) + LARGURA_TUBO = -2 < 0
        tubo.resetar( - (LARGURA_TUBO + 2) ); 
        CHECK(tubo.estaForaDaTela() == true);
    }

    SUBCASE("Detectar colisão") {
        // Pássaro está entre a abertura dos tubos
        int x_jogador = tubo.x;
        int y_jogador = tubo.altura_abertura - 10;
        // se o jogador tiver 10 de largura ele colide
        CHECK(tubo.colide(x_jogador, y_jogador, 10, 0) == true);

        // Pássaro está na zona que não tem tubos, não tem como ter colisão
        x_jogador = tubo.x + LARGURA_TUBO + 10;
        CHECK(tubo.colide(x_jogador, y_jogador, 10, 10) == false);
    }
}
