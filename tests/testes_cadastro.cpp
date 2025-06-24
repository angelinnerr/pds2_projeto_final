/*
TESTE EM DOCTEST PARA MÉTODOS DA CLASSE 'RegistroJogador' e 'Cadastro, NO ARQUIVO "cadastro.cpp"
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <cstdio>

#include "doctest.h"
#include "cadastro.hpp"

TEST_CASE("RegistroJogador constructor") {
    ALLEGRO_COLOR test_color = al_map_rgb(0, 0, 0);
    RegistroJogador player("TestPlayer", 100, 200, test_color);

    CHECK(player.apelido == "TestPlayer");
    CHECK(player.ultima_pontuacao == 100);
    CHECK(player.recorde == 200);
    // não há teste das cores pois são randomizadas
}

TEST_CASE("Testando funcionalidades do Cadastro") {
    // Inicializa Allegro para ter a fonte para testes
    al_init();
    al_init_font_addon();
    ALLEGRO_FONT* fonte = al_create_builtin_font();
    REQUIRE(fonte != nullptr);

    // Criação de um arquivo limpo para testes
    std::string arquivo_teste = "dados_teste.txt";
    std::remove(arquivo_teste.c_str());

    // Cria instância de cadastro, mas sem inicializar o carregamento de imagens.
    Cadastro cadastro(fonte, arquivo_teste, false);

    SUBCASE("Registrar novo jogador") {
        Cadastro cadastro(fonte, arquivo_teste, false);
        CHECK(cadastro.registrar_jogador("novoJogador") == true);
        CHECK(cadastro.registrar_jogador("novoJogador") == true);  // Verifica se o mesmo jogador pode se identificar novamente
        CHECK(cadastro.registrar_jogador("") == false);

        // Salvamento e carregamento dos dados 
        CHECK(cadastro.salvar_dados() == true);
        Cadastro novo_cadastro(fonte, arquivo_teste, false);
        CHECK(novo_cadastro.carregar_dados() == true);

        // Registro da pontuação do jogador salvo no arquivo_teste
        CHECK(novo_cadastro.registrar_pontuacao("novoJogador", 50) == true);
    }

    SUBCASE("Registrar diferentes pontuações") {
        cadastro.registrar_jogador("outroJogador");

        // Verificar se diferentes pontuações podem ser registradas para um jogador 
        CHECK(cadastro.registrar_pontuacao("outroJogador", 30) == true);
        CHECK(cadastro.registrar_pontuacao("outroJogador", 80) == true);  // Novo recorde
        CHECK(cadastro.registrar_pontuacao("outroJogador", 50) == true);  // Não altera recorde
    }


    SUBCASE("Atribuir pontuação a jogador não cadastrado") {
        // Se o jogador "Inexistente" não foi cadastrado antes com o método registrar_jogador, registrar_pontuação deve retornar false
        CHECK(cadastro.registrar_pontuacao("Inexistente", 100) == false);
    }

    // Liberação de memória alocada para o teste
    al_destroy_font(fonte);
    std::remove(arquivo_teste.c_str());
}