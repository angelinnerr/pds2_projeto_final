/*
TESTE EM DOCTEST PARA MÉTODOS DA CLASSE 'Jogo', NO ARQUIVO "inicializador"
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "inicializador.hpp"

TEST_CASE("Teste do inicializador") {
    Jogo jogo;

    SUBCASE("Teste da inicialização") {
        // verificar se o método inicializar() lança alguma exceção
        CHECK_NOTHROW(jogo.inicializar());

        // verificar se a inicialização funciona e os getters não recebem nullptr
        CHECK(jogo.getDisplay() != nullptr);
        CHECK(jogo.getFilaEventos() != nullptr);
        CHECK(jogo.getTimer() != nullptr);
        CHECK(jogo.getFonte() != nullptr);
        CHECK(jogo.getFonte2() != nullptr);
        CHECK(jogo.getSombraFonte2() != nullptr);
        CHECK(jogo.getMusicaTema() != nullptr);
        CHECK(jogo.getMusicaTransicao() != nullptr);
    }

    SUBCASE("Teste da destruição") {
        jogo.inicializar();
        jogo.finalizar();

        // verificar se os elementos foram devidamente destruídos e tiveram nullptr atribuído a eles
        CHECK(jogo.getDisplay() == nullptr);
        CHECK(jogo.getFilaEventos() == nullptr);
        CHECK(jogo.getTimer() == nullptr);
        CHECK(jogo.getFonte() == nullptr);
        CHECK(jogo.getFonte2() == nullptr);
        CHECK(jogo.getSombraFonte2() == nullptr);
        CHECK(jogo.getMusicaTema() == nullptr);
        CHECK(jogo.getMusicaTransicao() == nullptr);
    }
}
