#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "constants.h"
#include "fundo.hpp"
#include "tubo.hpp"
#include "jogador.hpp"
#include "pontos.hpp"

enum EstadoDoJogo {INICIO, JOGANDO, FIM_DE_JOGO};

int main() {
    bool reiniciar = false; // reiniciar se apertar R; ta fora e antes dos controles de jogo pra reiniciar tudo

    //inicializa funções do allegro
        srand(time(0));
        al_init();
        al_install_keyboard();
        al_init_image_addon();
        al_init_font_addon();
        al_init_ttf_addon();
        al_init_primitives_addon();
        al_install_audio();
        al_init_acodec_addon();
        al_reserve_samples(1);
        
        //carrega os recursos do jogo
        ALLEGRO_DISPLAY* tela = al_create_display(LARGURA_TELA, ALTURA_TELA); // cria a janela
        ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue(); // teclado e timer (eventos)
        ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
        ALLEGRO_FONT* fonte = al_load_ttf_font("assets/font.ttf", 32, 0);
        ALLEGRO_FONT* fonte2 = al_load_ttf_font("assets/game_over.ttf", 150, 0);
        ALLEGRO_FONT* sombra_fonte2 = al_load_ttf_font ("assets/game_over.ttf", 155, 0);
        ALLEGRO_SAMPLE *musica_tema = al_load_sample("assets/musicatema.ogg");
        ALLEGRO_SAMPLE_INSTANCE *inst_musica_tema = nullptr;
        inst_musica_tema = al_create_sample_instance(musica_tema);
        al_attach_sample_instance_to_mixer(inst_musica_tema, al_get_default_mixer());
        al_set_sample_instance_playmode(inst_musica_tema, ALLEGRO_PLAYMODE_LOOP);
        al_set_sample_instance_gain(inst_musica_tema, 0.8);

        al_register_event_source(fila_eventos, al_get_display_event_source(tela));
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());        
        
        al_start_timer(timer);

    do {  // o do-while pra reiniciar tudo

        // CONTROLES DE JOGO
        reiniciar = false;
        bool sair = false;
        bool tecla_espaco = false;
        EstadoDoJogo estado_atual = INICIO;

        const int NUM_TUBOS = 3;
        Tubo tubos[NUM_TUBOS] = { Tubo(LARGURA_TELA), Tubo(LARGURA_TELA + 350), Tubo(LARGURA_TELA + 700) };  // cria os tubos espaçados pra aparecer varios simultaneos (3)
        tubos[0].altura_abertura = ALTURA_TELA / 2;

        carregar_imagens_tubo();
        Fundo fundo;
        Jogador jogador;
        Pontos pontuacao;
        

        while (!sair) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            switch(evento.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                    sair = true;
                    reiniciar = false;
                    break;
                }

                case ALLEGRO_EVENT_KEY_DOWN : {

                    if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                        if (estado_atual == FIM_DE_JOGO) {
                            reiniciar = true; // quando aperta a tecla r reinicia o jogo (quando perde)
                            sair = true;// força a sair do loop pra reiniciar
                        }
                    }
                    
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                            reiniciar = false; // quando aperta a tecla r reinicia o jogo (quando perde)
                            sair = true;// força a sair do loop pra reiniciar
                    }

                    if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        if (estado_atual == INICIO || estado_atual == JOGANDO) {
                            tecla_espaco = true;
                        }
                    }

                    break;
                }
                
                // aplicação da lógica do jogo
                case ALLEGRO_EVENT_TIMER : {
                    // o que é isso??
                    int hitbox_largura = 36;
                    int hitbox_altura = 26;
                    int hitbox_x = 8;
                    int hitbox_y = 6;

                    if (estado_atual == INICIO) {
                
                        fundo.atualizar();

                        if (tecla_espaco) {
                            estado_atual = JOGANDO;
                            jogador.pular();
                        }
                    }
                    else if (estado_atual == JOGANDO) {
                        if (tecla_espaco) {
                            jogador.pular();
                            tecla_espaco = false;
                        }
                        
                        fundo.atualizar();
                        jogador.atualizar();
                
                    }

                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        // solução temporária para tubos e pontuação parar de atualizar depois do fim do jogo
                        if (estado_atual != FIM_DE_JOGO  && estado_atual != INICIO){
                            al_play_sample_instance(inst_musica_tema);
                            tubos[i].atualizar();
                            pontuacao.verificar(jogador.x, tubos[i].x);
                        }
                        // *
                        if (tubos[i].colide(jogador.x + hitbox_x, jogador.y + hitbox_y, hitbox_largura, hitbox_altura)) {
                            estado_atual = FIM_DE_JOGO;
                        }
                    }

                    if (jogador.y >= ALTURA_TELA - 38 || jogador.y < 0) {
                        estado_atual = FIM_DE_JOGO;
                    }

                    tecla_espaco = false;

                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    fundo.desenhar();
                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        tubos[i].desenhar();
                    }
                    jogador.desenhar();
                    pontuacao.atualizar();

                    if (estado_atual == INICIO) {
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "Pressione espaço para começar!");
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 100, ALLEGRO_ALIGN_CENTER, "Pressione esc para sair");
                    }
                    else if (estado_atual == FIM_DE_JOGO) {
                        al_stop_sample_instance(inst_musica_tema);
                        al_draw_text(fonte2, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                        al_draw_text(sombra_fonte2, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 70, ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 90, ALLEGRO_ALIGN_CENTER, "Pressione esc para sair");
                        
                    }

                    al_flip_display();
        
                    break;
                }

            }
        }

    } while (reiniciar);

        destruir_imagens_tubo();
        al_destroy_font(fonte);
        al_destroy_timer(timer);
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(tela);


    return 0;
}