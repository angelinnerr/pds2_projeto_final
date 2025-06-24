/*
TESTE EM DOCTEST PARA MÉTODOS DA CLASSE 'Pontos', NO ARQUIVO "pontos.cpp"
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "pontos.hpp"

TEST_CASE("Testes da classe Pontos") {

    SUBCASE("Construtor") {
        Pontos pontos(false); // não carrega as fontes, que já têm verificação
        CHECK(pontos.getScore() == 0);
    }

    SUBCASE("Verificar se deve ou não somar pontos") {
        Pontos pontos(false);
        // Jogador e tubo na mesma posição:
        pontos.verificar(100, 100); // soma score
        pontos.verificar(200, 200); // soma score
        CHECK(pontos.getScore() == 2);
        // Jogador e tubo em posições diferentes:
        pontos.verificar(100, 200); // não soma score
        CHECK(pontos.getScore() == 2);
    }

    SUBCASE("Verificar reset") {
        Pontos pontos(false);
        pontos.verificar(100, 100);
        pontos.verificar(20, 10);
        CHECK(pontos.getScore() == 1);
        // resetar a ponturação para 0
        pontos.reset();
        CHECK(pontos.getScore() == 0);
    }
}
