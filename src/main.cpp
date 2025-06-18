#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>

#include "constants.h"
#include "fundo.hpp"
#include "tubo.hpp"
#include "jogador.hpp"
#include "pontos.hpp"
#include "cadastro.hpp"

enum EstadoDoJogo {MENU_JOGADORES, RANKING, INICIO, JOGANDO, FIM_DE_JOGO, REMOVER_JOGADOR};

void mostrar_tela_ranking(Cadastro& cadastro, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte_pequena, bool& sair, EstadoDoJogo& estado_atual, ALLEGRO_EVENT_QUEUE* fila_eventos) {
    bool sair_ranking = false;
    
    vector<Jogador> ranking = cadastro.obter_ranking();
    sort(ranking.begin(), ranking.end(), [](const Jogador& a, const Jogador& b) {
        return a.melhor_pontuacao > b.melhor_pontuacao;
    });

    while(!sair_ranking && !sair) {
        ALLEGRO_EVENT evento;
        while(al_get_next_event(fila_eventos, &evento)) {
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    sair_ranking = true;
                }
            }
        }
        al_clear_to_color(al_map_rgb(0,0,0));
         al_draw_text(fonte, al_map_rgb(255, 215, 0), 
                     LARGURA_TELA/2, 50, ALLEGRO_ALIGN_CENTER, 
                     "RANKING DOS JOGADORES");

        for (size_t i = 0; i < min((size_t)10, ranking.size()); i++) {
            const Jogador& j = ranking[i];
            int y = 120 + i * 40;
            
            if (i < 3) {
                ALLEGRO_COLOR cor_medalha;
                if (i == 0) cor_medalha = al_map_rgb(255, 215, 0);
                else if (i == 1) cor_medalha = al_map_rgb(192, 192, 192);
                else cor_medalha = al_map_rgb(205, 127, 50);
                
                al_draw_textf(fonte_pequena, cor_medalha, 100, y, 0, "%dº", i+1);
            } else {
                al_draw_textf(fonte_pequena, al_map_rgb(200, 200, 200), 100, y, 0, "%dº", i+1);
            }
            
            al_draw_textf(fonte_pequena, j.cor, 200, y, 0, "%s", j.apelido_unico.c_str());
            al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 
                          LARGURA_TELA - 100, y, ALLEGRO_ALIGN_RIGHT, 
                          "%d pontos", j.melhor_pontuacao);
        }

        al_draw_text(fonte_pequena, al_map_rgb(180, 180, 180),
                    LARGURA_TELA/2, ALTURA_TELA - 50, ALLEGRO_ALIGN_CENTER,
                    "Pressione ESC para voltar");

        al_flip_display();
    }
    estado_atual = MENU_JOGADORES;
}

void remover_jogador(Cadastro& cadastro, ALLEGRO_FONT* fonte,ALLEGRO_DISPLAY* tela,bool& sair, std::string& apelido_atual, EstadoDoJogo& estado_atual){
    char apelido[100] = {0};
    al_show_native_text_input_box(tela, "Remover Jogador",
                                 "Digite o apelido do jogador que deseja remover:", "", apelido, 100, 0);
    if (apelido[0] != '\0'){
        if(cadastro.remover_jogador(apelido)){
            if(apelido_atual == apelido) {
                apelido_atual.clear(); // Limpa o apelido atual se o jogador removido for o atual
            }
            al_show_native_message_box(tela, "Sucesso", "Jogador removido com sucesso!", "", nullptr, 0);
            cadastro.salvar_jogadores("cadastro.txt");
        }else {
            al_show_native_message_box(tela, "Erro", "Jogador não encontrado!", "", nullptr, 0);
        }
    }
    estado_atual = MENU_JOGADORES;
}
void mostrar_menu_jogadores(Cadastro& cadastro, ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* tela, bool& sair, std::string& apelido_atual, EstadoDoJogo& estado_atual) {
    bool sair_menu = false;
    int opcao_selecionada = 0;
    const int opcoes = 5;
    const char* opcoes_menu[opcoes] = {
        "1. Selecionar jogador existente",
        "2. Cadastrar novo jogador",
        "3. Remover jogador",
        "4. Ranking",
        "5. Sair do jogo"
    };
    while(!sair_menu && !sair){
        ALLEGRO_EVENT evento;
        while (al_get_next_event(fila_eventos, &evento)) {
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch (evento.keyboard.keycode){
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = (opcao_selecionada - 1 + opcoes) % opcoes;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = (opcao_selecionada + 1) % opcoes;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        sair_menu = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        sair = true;
                        return;
                }
            }
        }
        al_clear_to_color(al_map_rgb(50, 50, 80));
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 
                     LARGURA_TELA/2, 80, ALLEGRO_ALIGN_CENTER, 
                     "MENU PRINCIPAL");

        for (int i = 0; i < opcoes; i++) {
            ALLEGRO_COLOR cor = (i == opcao_selecionada) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
            al_draw_text(fonte, cor, LARGURA_TELA/2, 150 + i*40, 
                        ALLEGRO_ALIGN_CENTER, opcoes[i]);
        }

        if (!apelido_atual.empty()) {
            al_draw_textf(fonte, al_map_rgb(180, 180, 180), LARGURA_TELA/2, 400, 
                         ALLEGRO_ALIGN_CENTER, "Jogador atual: %s", apelido_atual.c_str());
        }

        al_flip_display();
    }

    if (!sair) {
        switch (opcao_selecionada) {
            case 0: {
                char apelido[100] = {0};
                al_show_native_text_input_box(tela, "Selecionar Jogador", 
                                            "Digite o apelido do jogador:", "", apelido, 100, 0);
                
                if (apelido[0] != '\0') {
                    if (cadastro.buscar_jogador(apelido)) {
                        apelido_atual = apelido;
                        estado_atual = INICIO;
                    } else {
                        al_show_native_message_box(tela, "Erro", "Jogador não encontrado", 
                                                 "Apelido não existe!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                }
                break;
            }
            case 1: {
                char apelido[100] = {0};
                al_show_native_text_input_box(tela, "Novo Jogador", 
                                            "Digite um apelido único:", "", apelido, 100, 0);
                
                if (apelido[0] != '\0') {
                    ALLEGRO_COLOR cor = al_map_rgb(rand()%256, rand()%256, rand()%256);
                    if (cadastro.cadastrar_jogador(apelido, "", cor)) {
                        apelido_atual = apelido;
                        estado_atual = INICIO;
                        al_show_native_message_box(tela, "Sucesso", "Jogador criado", 
                                                   "Jogador cadastrado com sucesso!", NULL, 0);
                    } else {
                        al_show_native_message_box(tela, "Erro", "Apelido já existe", 
                                                 "Escolha outro apelido!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                }
                break;
            }
            case 2:
                estado_atual = REMOVER_JOGADOR;
                break;
            case 3:
                estado_atual = RANKING;
                break;
            case 4:
                sair = true;
                break;
        }
    }
}
int main(){
    bool reiniciar = true;
    std::string apelido_atual;

    do{
        reiniciar = false;
        bool sair = false;
        bool tecla_espaco = false;
        EstadoDoJogo estado_atual = MENU_JOGADORES;

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
        al_init_native_dialog_addon();

        ALLEGRO_DISPLAY* tela = al_create_display(LARGURA_TELA, ALTURA_TELA); // cria a janela
        ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue(); // teclado e timer (eventos)
        ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
        ALLEGRO_FONT* fonte = al_load_ttf_font("assets/font.ttf", 32, 0);
        ALLEGRO_SAMPLE *musica_tema = al_load_sample("assets/musicatema.ogg");
        ALLEGRO_SAMPLE_INSTANCE *inst_musica_tema = nullptr;

        Cadastro cadastro (fonte);

        if(musica_tema){
            inst_musica_tema = al_create_sample_instance(musica_tema);
            al_attach_sample_instance_to_mixer(inst_musica_tema, al_get_default_mixer());
            al_set_sample_instance_playmode(inst_musica_tema, ALLEGRO_PLAYMODE_LOOP);
            al_set_sample_instance_gain(inst_musica_tema, 0.8);
            al_attach_sample_instance_to_mixer(inst_musica_tema, al_get_default_mixer());
            al_set_sample_instance_playmode(inst_musica_tema, ALLEGRO_PLAYMODE_LOOP);
            al_set_sample_instance_gain(inst_musica_tema, 0.8);
        }

        Fundo fundo;
        carregar_imagens_tubo();
        Jogador jogador;
        Pontos pontuacao;

        const int NUM_TUBOS = 3;
        Tubo tubos[NUM_TUBOS] = { Tubo(LARGURA_TELA), Tubo(LARGURA_TELA + 350), Tubo(LARGURA_TELA + 700) };  // cria os tubos espaçados pra aparecer varios simultaneos (3)
        tubos[0].altura_abertura = ALTURA_TELA / 2;

        al_register_event_source(fila_eventos, al_get_display_event_source(tela));
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());        
        
        al_start_timer(timer);

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
                    if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        if (estado_atual == INICIO || estado_atual == JOGANDO) {
                            tecla_espaco = true;
                        }
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                        if (estado_atual == FIM_DE_JOGO) {
                            reiniciar = true; // quando aperta a tecla r reinicia o jogo (quando perde)
                            sair = true;// força a sair do loop pra reiniciar
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

                    if (estado_atual == MENU_JOGADORES){
                        mostrar_menu_jogadores(cadastro, fonte, tela, sair, apelido_atual, estado_atual);
                    }
                    else if (estadp_atual == REMOVER_JOGADOR) {
                        remover_jogador(cadastro, fonte, tela, sair, apelido_atual, estado_atual);
                    }
                    else if (estado_atual == RANKING) {
                        mostrar_tela_ranking(cadastro, fonte, fonte, sair, estado_atual, fila_eventos);
                    }
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
                        if (estado_atual != FIM_DE_JOGO){
                            al_play_sample_instance(inst_musica_tema);
                            tubos[i].atualizar();
                            pontuacao.verificar(jogador.x, tubos[i].x);
                        }
                        // *
                        if (tubos[i].colide(jogador.x + hitbox_x, jogador.y + hitbox_y, hitbox_largura, hitbox_altura)) {
                            estado_atual = FIM_DE_JOGO;
                            if(apelido_atual.empty()){
                                cadastro.incrementar_pontuacao("Jogador Desconhecido", pontuacao.get_pontos());
                                cadastro.salvar_jogadores("cadastro.txt");
                            }
                        }
                    }

                    if (jogador.y >= ALTURA_TELA - 38 || jogador.y < 0) {
                        estado_atual = FIM_DE_JOGO;
                        if(apelido_atual.empty()){
                            cadastro.incrementar_pontuacao("Jogador Desconhecido", pontuacao.get_pontos());
                            cadastro.salvar_jogadores("cadastro.txt");
                        }
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
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "Pressione espaco para comecar!");
                    }
                    else if (estado_atual == FIM_DE_JOGO) {
                        if (inst_musica_tema) al_stop_sample_instance(inst_musica_tema);
                        al_draw_text(fonte, al_map_rgb(255, 0, 0), 
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 80, 
                                     ALLEGRO_ALIGN_CENTER, "GAME OVER");
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), 
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, 
                                     ALLEGRO_ALIGN_CENTER, "Pontuacao: %d", pontuacao.get_pontos());
                        
                        // Mostra ranking
                        vector<Jogador> ranking = cadastro.obter_ranking();
                        sort(ranking.begin(), ranking.end(), [](const Jogador& a, const Jogador& b) {
                            return a.melhor_pontuacao > b.melhor_pontuacao;
                        });
                        
                        al_draw_text(fonte_pequena, al_map_rgb(255, 215, 0), 
                                     LARGURA_TELA / 2, ALTURA_TELA / 2 + 20, 
                                     ALLEGRO_ALIGN_CENTER, "TOP 5 JOGADORES");
                        
                        for (int i = 0; i < min(5, (int)ranking.size()); ++i) {
                            const Jogador& j = ranking[i];
                            al_draw_textf(fonte_pequena, j.cor, LARGURA_TELA / 2, 
                                         ALTURA_TELA / 2 + 70 + (i * 30), 
                                         ALLEGRO_ALIGN_CENTER, "%d. %s - %d pts", 
                                         i+1, j.apelido_unico.c_str(), j.melhor_pontuacao);
                        }

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), 
                                     LARGURA_TELA / 2, ALTURA_TELA - 50, 
                                     ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                    }

                    al_flip_display();
                    break;
                }
            }
        }

        destruir_imagens_tubo();
        al_destroy_font(fonte);
        al_destroy_font(fonte_pequena);
        al_destroy_timer(timer);
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(tela);
        if (inst_musica_tema) {
            al_destroy_sample_instance(inst_musica_tema);
        }
        if (musica_tema) {
            al_destroy_sample(musica_tema);
        }

    } while (reiniciar);

    return 0;
}