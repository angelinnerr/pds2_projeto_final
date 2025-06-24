//bibliotecas necessarias para o jogo funcionar
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 

//header files utilizados
#include "inicializador.hpp"
#include "excecoes.hpp"
#include "constants.h"
#include "fundo.hpp"
#include "tubo.hpp"
#include "jogador.hpp"
#include "pontos.hpp"
#include "cadastro.hpp"

//um estado seria cada tela diferente que o jogo tem
enum EstadoDoJogo { INICIO, JOGANDO, FIM_DE_JOGO };

int main() {
    //geracao da semente aleatoria
    srand(time(NULL));

    Jogo jogo; //objeto principal do jogo
    
    //variáveis para trocar os estados do jogo
    bool reiniciar = false;
    bool voltar_cadastro = false;

    //espirais do tubo colorido
    std::vector<ALLEGRO_BITMAP*> overlay_frames_espiral;
    int overlay_frame_atual = 0;
    int overlay_tempo_frame = 0;
    
    //controle de audio
    bool musica_fase_colorida = false;
    bool musica_tema_baixa = false; 

    //tratamento de excecão caso o jogo não abra
    try {
        jogo.inicializar();
    } catch (const ErroDeInicializacao& erro) {
        std::cerr << erro.what() << std::endl;
        return 1;
    }

    //fonte do cadastro
    ALLEGRO_FONT* fonteCadastro = al_load_ttf_font(FONTE_BASE, TAMANHO_FONTE_CAD, 0); 
    if (!fonteCadastro) {
        al_show_native_message_box(jogo.getDisplay(), "Erro", "Falha ao carregar fonte",
                                 "Não foi possível carregar a fonte do cadastro",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        jogo.finalizar();
        return 1;
    }

    //inicialização da classe cadastro e do apelido único
    Cadastro cadastro(fonteCadastro, DADOS_JOGADORES);
    std::string apelidoJogador;

    //configura o fundo inicial
    Fundo fundo;
    fundo.carregar_imagem(FUNDO_CADASTRO); 

    //processa a tela de cadastro
    bool cadastroOK = cadastro.processar_tela_cadastro(jogo.getFilaEventos(), jogo.getDisplay(), apelidoJogador, fundo);
    if (!cadastroOK) {
        al_destroy_font(fonteCadastro);
        jogo.finalizar();
        return 0;
    }

    //carrega o fundo do jogo,tubos e tubo colorido
    fundo.carregar_imagem(FUNDO_JOGO); 
    carregar_imagens_tubo();
    carregar_tubo_colorido(); 

    //carrega os frames da animação de espiral
    overlay_frames_espiral.push_back(al_load_bitmap(FUNDO_ESPIRAL_FRAME_1));
    verificarInicializacao(overlay_frames_espiral.back(), "frame 1 da espiral");
    overlay_frames_espiral.push_back(al_load_bitmap(FUNDO_ESPIRAL_FRAME_2));
    verificarInicializacao(overlay_frames_espiral.back(), "frame 2 da espiral");
    overlay_frames_espiral.push_back(al_load_bitmap(FUNDO_ESPIRAL_FRAME_3));
    verificarInicializacao(overlay_frames_espiral.back(), "frame 3 da espiral");

    //loop principal que controla o fluxo de reinicialização no jogo e o retorno ao cadastro
    do {
        //variavel para trocar de estado
        bool exibirRanking = false;

        //transicao do gameover para o cadastro
        if (voltar_cadastro) {
            al_flush_event_queue(jogo.getFilaEventos());
            bool cadastroOK = cadastro.processar_tela_cadastro(jogo.getFilaEventos(), jogo.getDisplay(), apelidoJogador, fundo);
            if (!cadastroOK) {
                al_destroy_font(fonteCadastro);
                jogo.finalizar();
                return 0;
            }
        }

        //reseta os estados de jogo
        reiniciar = false;
        Tubo::set_usar_imagens_coloridas(false);
        bool sair = false;
        bool tecla_espaco = false;
        bool pontuacaoRegistrada = false;
        bool fase_colorida = false;

        //reseta estados de audio
        musica_fase_colorida = false; 
        musica_tema_baixa = false;

        //garante que as musicas vao tocar corretamente
        al_stop_sample_instance(jogo.getMusicaTransicao());
        al_set_sample_instance_gain(jogo.getMusicaTema(), 0.8); 

        //reseta a animacao em espiral
        overlay_frame_atual = 0;
        overlay_tempo_frame = 0; 

        //configuração do estado de inicio do jogo
        EstadoDoJogo estado_atual = INICIO;

        //cria tubos inicias
        Tubo tubos[NUM_TUBOS] = {
            Tubo(LARGURA_TELA),
            Tubo(LARGURA_TELA + 100 + ESPACO_HORIZONTAL_ENTRE_TUBOS),
            Tubo(LARGURA_TELA + 100 + 2 * ESPACO_HORIZONTAL_ENTRE_TUBOS)
        };

        //inicializa o jogador e a pontuacao
        Jogador jogador;
        Pontos pontuacao;
        pontuacao.reset();

        while (!sair) { //enquanto o jogo não for encerrado
            ALLEGRO_EVENT evento;
            al_wait_for_event(jogo.getFilaEventos(), &evento); //espera um evento

            switch (evento.type) { //verifica o tipo de evento
                case ALLEGRO_EVENT_DISPLAY_CLOSE: { //fecha a tela
                    sair = true;
                    reiniciar = false;
                    break;
                }

                case ALLEGRO_EVENT_KEY_DOWN: { //caso uma dessas teclas seja pressionada
                    if (evento.keyboard.keycode == ALLEGRO_KEY_R && estado_atual == FIM_DE_JOGO) { reiniciar = true; voltar_cadastro = false; sair = true; } //reincia
                    if (evento.keyboard.keycode == ALLEGRO_KEY_C && estado_atual == FIM_DE_JOGO) { voltar_cadastro = true; sair = true; } //volta ao cadastro
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { reiniciar = false; voltar_cadastro = false; sair = true; }//sai do jogo
                    if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && (estado_atual == INICIO || estado_atual == JOGANDO)) { tecla_espaco = true; } //espaço para escapar do obstaculos
                    if (evento.keyboard.keycode == ALLEGRO_KEY_1 && estado_atual == FIM_DE_JOGO) { exibirRanking = !exibirRanking; } //exibe o ranking
                    break;
                }

                //esse caso cuida das variações de tempo de cada elemento na tela
                case ALLEGRO_EVENT_TIMER: {
                    //colisões 
                    int hitbox_largura = 36;
                    int hitbox_altura = 26;
                    int hitbox_x = 8;
                    int hitbox_y = 6;

                    //mexer o fundo
                    fundo.atualizar();

                    if (estado_atual == INICIO) {
                        if (tecla_espaco) {
                            estado_atual = JOGANDO;//troca o estado
                            jogador.pular();//faz o jogador pular
                            al_play_sample_instance(jogo.getMusicaTema());//toca a musica tema
                        }
                    } else if (estado_atual == JOGANDO) {
                        if (tecla_espaco) {
                            jogador.pular(); //pular durante a partida
                            tecla_espaco = false;  //reseta a flag para não dar problema
                        }
                        
                        jogador.atualizar(); //atualiza a posição do personagem

                        for (int i = 0; i < NUM_TUBOS; ++i) {
                            //verifica  apontuação do jogador com base nos tubos passados
                            pontuacao.verificar(jogador.x, tubos[i].x);
                        }

                        //getter da pontuaçao do jogador
                        int score = pontuacao.getScore();

                        //variaveis usadas para definir a partir de qual pontuação os tubos coloridos aparecem
                        int duracao_ciclo = 30;
                        int inicio_fase_colorida = 20;
                        int fim_fase_colorida = 30;
                        int posicao_no_ciclo = score % duracao_ciclo;

                        bool deve_ativar_cor = (score > 0 && posicao_no_ciclo >= inicio_fase_colorida && posicao_no_ciclo <= fim_fase_colorida);

                        Tubo::set_usar_imagens_coloridas(deve_ativar_cor);
                        
                        //configuracao fase colorida
                        if (deve_ativar_cor) {
                            fase_colorida = true; 

                            overlay_tempo_frame++;
                            if (overlay_tempo_frame > 10) { // velocidade dos frames
                                overlay_frame_atual = (overlay_frame_atual + 1) % overlay_frames_espiral.size();
                                overlay_tempo_frame = 0;
                            }
                    
                            if (!musica_fase_colorida) {
                                al_play_sample_instance(jogo.getMusicaTransicao());
                                musica_fase_colorida = true;

                                if (!musica_tema_baixa) {
                                    al_set_sample_instance_gain(jogo.getMusicaTema(),0.0); //abaixa o volume da musica principal
                                    musica_tema_baixa = true;
                               }
                            }

                         } 
                         //volta ao audio normal
                         else {
                            fase_colorida = false; 
                            overlay_frame_atual = 0; 
                            overlay_tempo_frame = 0;
                            
                            if (musica_fase_colorida) {
                            al_stop_sample_instance(jogo.getMusicaTransicao());

                            if (musica_tema_baixa) {
                                al_set_sample_instance_gain(jogo.getMusicaTema(), 0.8); // volta ao volume normal
                                musica_tema_baixa = false;
                            }

                            musica_fase_colorida = false;
                          }
                       }  
                    }

                    //atualiza e verifica colisoes com os tubos
                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        if (estado_atual == JOGANDO) {
                            tubos[i].atualizar();
                        }
                        if (tubos[i].colide(jogador.x + hitbox_x, jogador.y + hitbox_y, hitbox_largura, hitbox_altura)) {
                            estado_atual = FIM_DE_JOGO;
                            al_stop_sample_instance(jogo.getMusicaTema());
                            al_stop_sample_instance(jogo.getMusicaTransicao());
                        }
                    }
                    
                    //reposiciona tubos que sairam da tela
                    if (estado_atual == JOGANDO) {
                        for (int i = 0; i < NUM_TUBOS; ++i) {
                            if (tubos[i].estaForaDaTela()) {
                                int max_x = 0;
                                for (int j = 0; j < NUM_TUBOS; ++j) {
                                    if (tubos[j].x > max_x) {
                                        max_x = tubos[j].x;
                                    }
                                }
                                tubos[i].resetar(max_x + ESPACO_HORIZONTAL_ENTRE_TUBOS);
                            }
                        }
                    }

                    //verifica se o jogador da tela
                    if (jogador.y >= ALTURA_TELA - 38 || jogador.y < 0) {
                        if(estado_atual == JOGANDO) { // Evita parar a música múltiplas vezes
                            estado_atual = FIM_DE_JOGO;
                            al_stop_sample_instance(jogo.getMusicaTema());
                            al_stop_sample_instance(jogo.getMusicaTransicao());
                        }
                    }

                    tecla_espaco = false;

                    //renderizacao do jogo
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    fundo.desenhar();

                    //desenha os tubos
                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        tubos[i].desenhar();
                    }

                    //desenha o jogador e a pontuacao
                    jogador.desenhar();
                    pontuacao.atualizar();

                    //desenha a animação espiral 
                    if (fase_colorida && !overlay_frames_espiral.empty()) {
                        al_draw_bitmap(overlay_frames_espiral[overlay_frame_atual], 0, 0, 0); 
                    }
                    
                    //renderizacao do estado inicial
                    if (estado_atual == INICIO) {
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "Pressione espaco para comecar!");
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 100, ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
                    } 

                    //renderizacao do game over
                    else if (estado_atual == FIM_DE_JOGO) {
                       //registra a pontuacao se ela nao foi registrada
                        if (!pontuacaoRegistrada) {
                            cadastro.registrar_pontuacao(apelidoJogador, pontuacao.getScore());
                            cadastro.salvar_dados();
                            pontuacaoRegistrada = true;
                        }

                        //texto da pontuaacao
                        std::string textoPontuacao = apelidoJogador + ": " + std::to_string(pontuacao.getScore()) + " pontos";

                        //desenha o texto de game over
                        al_draw_text(jogo.getFonte2(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                        al_draw_text(jogo.getSombraFonte2(), al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "GAME OVER");

                        //exibe o ranking
                        if (exibirRanking) { 
                            cadastro.exibir_ranking(LARGURA_TELA / 2, ALTURA_TELA / 2 - 90, jogo.getDisplay());
                        } 
                        else { 
                            //desenha caixa de instrucoes
                            ALLEGRO_BITMAP* caixa_instrucoes = cadastro.getImagemCaixaInstrucoes();
                            float caixa_instrucoes_x = 0.0f; 
                            float caixa_instrucoes_y = 0.0f; 

                        if (caixa_instrucoes) {
                            
                            caixa_instrucoes_x = LARGURA_TELA / 2 - al_get_bitmap_width(caixa_instrucoes) / 2; 
                            caixa_instrucoes_y = ALTURA_TELA - al_get_bitmap_height(caixa_instrucoes) - 10; 
                            al_draw_bitmap(caixa_instrucoes, caixa_instrucoes_x - 10, caixa_instrucoes_y - 85, 0);
                            al_draw_bitmap(caixa_instrucoes, caixa_instrucoes_x - 10, caixa_instrucoes_y - 135, 0);
                            al_draw_bitmap(caixa_instrucoes, caixa_instrucoes_x - 10, caixa_instrucoes_y - 185, 0);
                            al_draw_bitmap(caixa_instrucoes, caixa_instrucoes_x - 10, caixa_instrucoes_y - 313, 0);
                            al_draw_bitmap(caixa_instrucoes, caixa_instrucoes_x - 10, caixa_instrucoes_y - 410, 0);
                            al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 160, ALLEGRO_ALIGN_CENTER, textoPontuacao.c_str());

                        }

                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 60, ALLEGRO_ALIGN_CENTER, "Pressione '1' para ver o Ranking");
                    }
                        //ajusta a posicao quando o ranking aparece
                        float desce_y = exibirRanking ? 90 : 0;

                        //desenha as opcoes pos gameover
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 70 + desce_y, ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 180 + desce_y, ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
                        al_draw_text(jogo.getFonte(), al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 130 +desce_y, ALLEGRO_ALIGN_CENTER, "Pressione C para voltar ao cadastro");
                    }

                    al_flip_display();
                    break;
                }
            }
        }
    } while (reiniciar || voltar_cadastro);

    //limpeza final
    destruir_imagens_tubo();
    al_destroy_font(fonteCadastro);
    for (ALLEGRO_BITMAP* frame : overlay_frames_espiral) {
        if (frame) al_destroy_bitmap(frame);
    } // destroi as espirais
    jogo.finalizar();
    return 0;
}