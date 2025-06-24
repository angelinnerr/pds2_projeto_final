/*
TESTE EM DOCTEST PARA MÉTODOS DA CLASSE 'Fundo', NO ARQUIVO "fundo.cpp"
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "doctest.h"
#include "fundo.hpp"
#include "constants.h"

TEST_CASE("Teste do Fundo") {
    // Inicializa Allegro para ter addon de imagem dos testes
    al_init();
    al_init_image_addon();

    SUBCASE("Inicialização e carregamento de imagem") {
        Fundo fundo;
        // Como o método getPosicaoX só é chamado quando a imagem é carregada com sucesso, então verificar seu retorno 
        // significa verificar se a imagem foi carregada e se a posição foi setada como 0 no início.
        CHECK(fundo.getPosicaoX() == 0); 
    }

    SUBCASE("Movimentação do fundo") {
        Fundo fundo;
        float pos_inicial = fundo.getPosicaoX();

        // Verificar se o fundo se movimenta da forma esperada:
        /// Uma atualização
        fundo.atualizar();
        // Comparação entre X e o tanto que o fundo deveria se mover. 
        CHECK(fundo.getPosicaoX() == doctest::Approx(pos_inicial - VELOCIDADE_FUNDO));

        /// Várias atualizações
        for (int i = 0; i < 300; i++) {
            fundo.atualizar();
        }
        // Verifica se a posição está dentro do limite de -3200 + LARGURA_TELA, quando o fundo deve voltar à posição X = 0
        CHECK(fundo.getPosicaoX() >= -3200 + LARGURA_TELA);
    }

    SUBCASE("Mudança de imagem de fundo") {
        Fundo fundo;
        // Teste com a imagem que é inserida depois do fundo do cadastro
        fundo.carregar_imagem(FUNDO_JOGO);  
        CHECK(fundo.getPosicaoX() == 0);
    }

}