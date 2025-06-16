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

enum EstadoDoJogo {INICIO, JOGANDO, FIM_DE_JOGO};

int main() {
    bool reiniciar = false; // reiniciar se apertar R; ta fora e antes dos controles de jogo pra reiniciar tudo

    do {  // o do-while pra reiniciar tudo
        
        reiniciar = false;

        srand(time(0));
        al_init();
        al_install_keyboard();
        al_init_image_addon();
        al_init_font_addon();
        al_init_ttf_addon();
        al_init_primitives_addon();

        ALLEGRO_DISPLAY* tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
        ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
        ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
        ALLEGRO_FONT* fonte = al_load_ttf_font("font.ttf", 32, 0);
        ALLEGRO_SAMPLE *musica_tema = al_load_sample("assets/musicatema.ogg");
        ALLEGRO_SAMPLE_INSTANCE *inst_musica_tema = nullptr;
        inst_musica_tema = al_create_sample_instance(musica_tema);
        al_attach_sample_instance_to_mixer(inst_musica_tema, al_get_default_mixer());
        al_set_sample_instance_playmode(inst_musica_tema, ALLEGRO_PLAYMODE_LOOP);
        al_set_sample_instance_gain(inst_musica_tema, 0.8);

        Fundo fundo;
        carregar_imagens_tubo();
        Jogador jogador;

        const int NUM_TUBOS = 3;
        Tubo tubos[NUM_TUBOS] = { Tubo(LARGURA_TELA), Tubo(LARGURA_TELA + 350), Tubo(LARGURA_TELA + 700) };  // cria os tubos espaçados pra aparecer varios simultaneos (3)
        tubos[0].altura_abertura = ALTURA_TELA / 2;

        al_register_event_source(fila_eventos, al_get_display_event_source(tela));
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());
        
         // controles do jogo

        bool sair = false;
        bool tecla_espaco = false;
        EstadoDoJogo estado_atual = INICIO;
        
        al_start_timer(timer);

        while (!sair) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true; // sai do loop quando fecha a janela
                reiniciar = false;
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    if (estado_atual == INICIO || estado_atual == JOGANDO) {
                        tecla_espaco = true;
                    }
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                    if (estado_atual == FIM_DE_JOGO) {
                        reiniciar = true; // quando aperta a tecla r reinicia o jogo (quando perde)
                        sair = true;/// força a sair do loop pra reiniciarr
                    }
                }
            }
            else if (evento.type == ALLEGRO_EVENT_TIMER){
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
                    }
                    
            fundo.atualizar();
            jogador.atualizar();
            
                }

                    for (int i = 0; i < NUM_TUBOS; ++i) {
                        tubos[i].atualizar();
                        if (tubos[i].colide(jogador.x + hitbox_x, jogador.y + hitbox_y, hitbox_largura, hitbox_altura)) {
                            estado_atual = FIM_DE_JOGO;
                        }
                    }

                    if (jogador.y >= ALTURA_TELA - 38 || jogador.y < 0) {
                        estado_atual = FIM_DE_JOGO;
                    }
                }
                
            tecla_espaco = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            fundo.desenhar();
                
                for (int i = 0; i < NUM_TUBOS; ++i) {
                    tubos[i].desenhar();
                }

            jogador.desenhar();

                if (estado_atual == INICIO) {
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "Pressione espaco para comecar!");
                }
                else if (estado_atual == FIM_DE_JOGO) {
                    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 20, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                }

                al_flip_display();
        }

        destruir_imagens_tubo();
        al_destroy_font(fonte);
        al_destroy_timer(timer);
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(tela);

    } while (reiniciar);

    return 0;
}