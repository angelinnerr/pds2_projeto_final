#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <allegro5/allegro_native_dialog.h>

#include "inicializador.hpp"
#include "excecoes.hpp"
#include "constants.h"
#include "fundo.hpp"
#include "tubo.hpp"
#include "jogador.hpp"
#include "pontos.hpp"
#include "cadastro.hpp"

enum EstadoDoJogo { INICIO, JOGANDO, FIM_DE_JOGO };

int main() {
    srand(time(NULL));

    Jogo jogo;

    try {
        jogo.inicializar();
    } catch (const ErroDeInicializacao& erro) {
        std::cerr << erro.what() << std::endl;
        return 1;
    }

    // Sistema de Cadastro
    ALLEGRO_FONT* fonteCadastro = al_load_ttf_font("assets/font.ttf", 24, 0);
    if (!fonteCadastro) {
        al_show_native_message_box(jogo.getDisplay(), "Erro", "Falha ao carregar fonte",
                                   "Não foi possível carregar a fonte do cadastro",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        jogo.finalizar();
        return 1;
    }

    Cadastro cadastro("dados_jogadores.dat", fonteCadastro);
    std::string apelidoJogador;

    // Tela de Cadastro
    bool cadastroOK = cadastro.processar_tela_cadastro(jogo.getFilaEventos(), jogo.getDisplay(), apelidoJogador);
    if (!cadastroOK) {
        al_destroy_font(fonteCadastro);
        jogo.finalizar();
        return 0;
    }

    bool reiniciar = false;

    carregar_imagens_tubo(); 

    do {
        float ultimoTuboUltrapassado = -1000; 
        reiniciar = false;
        bool sair = false;
        bool tecla_espaco = false;
        bool pontuacaoRegistrada = false;

        EstadoDoJogo estado_atual = INICIO;

        const int NUM_TUBOS = 3;
        Tubo tubos[NUM_TUBOS] = { Tubo(LARGURA_TELA), Tubo(LARGURA_TELA + 350), Tubo(LARGURA_TELA + 700) };
        tubos[0].altura_abertura = ALTURA_TELA / 2;

        Fundo fundo;
        Jogador jogador;
        Pontos pontuacao;
        pontuacao.reset();

        while (!sair) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(jogo.getFilaEventos(), &evento);

            switch (evento.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                    sair = true;
                    reiniciar = false;
                    break;
                }

                case ALLEGRO_EVENT_KEY_DOWN: {
                    if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                        if (estado_atual == FIM_DE_JOGO) {
                            reiniciar = true;
                            sair = true;
                        }
                    }

                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        reiniciar = false;
                        sair = true;
                    }

                    if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        if (estado_atual == INICIO || estado_atual == JOGANDO) {
                            tecla_espaco = true;
                        }
                    }
                    break;
                }

                case ALLEGRO_EVENT_TIMER: {
                    int hitbox_largura = 36;
                    int hitbox_altura = 26;
                    int hitbox_x = 8;
                    int hitbox_y = 6;

                    if (estado_atual == INICIO) {
                        fundo.atualizar();
                        if (tecla_espaco) {
                            estado_atual = JOGANDO;
                            jogador.pular();
                            al_play_sample_instance(jogo.getMusicaTema());
                        }
                    } else if (estado_atual == JOGANDO) {
                        if (tecla_espaco) {
                            jogador.pular();
                            tecla_espaco = false;
                        }
                        fundo.atualizar();
                        jogador.atualizar();

                        for (int i = 0; i < NUM_TUBOS; ++i) {
                            pontuacao.verificar(jogador.x, tubos[i].x);
                        }
                    }

                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        if (estado_atual == JOGANDO) {
                            tubos[i].atualizar();
                        }

                        if (tubos[i].colide(jogador.x + hitbox_x, jogador.y + hitbox_y,
                                            hitbox_largura, hitbox_altura)) {
                            estado_atual = FIM_DE_JOGO;
                            al_stop_sample_instance(jogo.getMusicaTema());
                        }
                    }

                    if (jogador.y >= ALTURA_TELA - 38 || jogador.y < 0) {
                        estado_atual = FIM_DE_JOGO;
                        al_stop_sample_instance(jogo.getMusicaTema());
                    }

                    tecla_espaco = false;

                    // desenho
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    fundo.desenhar();

                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        tubos[i].desenhar();
                    }

                    jogador.desenhar();
                    pontuacao.atualizar();

                    if (estado_atual == INICIO) {
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 50,
                                     ALLEGRO_ALIGN_CENTER,
                                     "Pressione espaço para começar!");
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA - 100,
                                     ALLEGRO_ALIGN_CENTER,
                                     "Pressione ESC para sair");

                    } else if (estado_atual == FIM_DE_JOGO) {
                        if (!pontuacaoRegistrada) {
                            cadastro.registrar_pontuacao(apelidoJogador, pontuacao.getScore());
                            cadastro.salvar_dados();
                            pontuacaoRegistrada = true;
                        }

                        std::string textoPontuacao = apelidoJogador + ": " +
                                                     std::to_string(pontuacao.getScore()) + " pontos";

                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 100,
                                     ALLEGRO_ALIGN_CENTER,
                                     textoPontuacao.c_str());

                        al_draw_text(jogo.getFonte2(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 40,
                                     ALLEGRO_ALIGN_CENTER, "GAME OVER");
                        al_draw_text(jogo.getSombraFonte2(), al_map_rgb(255, 0, 0),
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 40,
                                     ALLEGRO_ALIGN_CENTER, "GAME OVER");

                        cadastro.exibir_ranking(LARGURA_TELA / 2, ALTURA_TELA / 2 + 30, jogo.getDisplay());

                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 + 120,
                                     ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255),
                                     LARGURA_TELA / 2, ALTURA_TELA - 90,
                                     ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
                    }

                    al_flip_display();
                    break;
                }
            }
        }
    } while (reiniciar);

    al_destroy_font(fonteCadastro);
    jogo.finalizar();
    return 0;
}
