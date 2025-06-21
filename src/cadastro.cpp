#include "cadastro.hpp"
#include <algorithm>
#include <fstream>
#include <allegro5/allegro_native_dialog.h>

RegistroJogador::RegistroJogador(std::string apelido, int ultima, int recorde, ALLEGRO_COLOR cor) :
    apelido(apelido), ultima_pontuacao(ultima), recorde(recorde), cor(cor) {}

Cadastro::Cadastro(const std::string& arquivo, ALLEGRO_FONT* fonte) :
    arquivo_dados(arquivo), fonte(fonte) {
    carregar_dados();
}

bool Cadastro::processar_tela_cadastro(ALLEGRO_EVENT_QUEUE* fila_eventos, 
                                     ALLEGRO_DISPLAY* display, 
                                     std::string& apelido_saida) {
    std::string apelido;
    bool concluido = false;
    
    while(!concluido) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return false;
        }
        else if(evento.type == ALLEGRO_EVENT_KEY_CHAR) {
            if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER && !apelido.empty()) {
                if(registrar_jogador(apelido)) {
                    apelido_saida = apelido;
                    return true;
                }
            }
            else if(evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && !apelido.empty()) {
                apelido.pop_back();
            }
            else if(evento.keyboard.unichar >= 32 && evento.keyboard.unichar <= 126) {
                if(apelido.size() < 15) {
                    apelido += (char)evento.keyboard.unichar;
                }
            }
        }

        // Renderização
        al_clear_to_color(al_map_rgb(0, 0, 50));
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 
                    al_get_display_width(display)/2, 
                    al_get_display_height(display)/3, 
                    ALLEGRO_ALIGN_CENTER, 
                    "Digite seu apelido (ENTER para confirmar):");
        
        al_draw_text(fonte, al_map_rgb(255, 255, 0), 
                    al_get_display_width(display)/2, 
                    al_get_display_height(display)/2, 
                    ALLEGRO_ALIGN_CENTER, 
                    apelido.c_str());
        
        al_flip_display();
    }
    return false;
}

bool Cadastro::registrar_jogador(const std::string& apelido) {
    if(apelido.empty()) return false;
    
    for(const auto& reg : registros) {
        if(reg.apelido == apelido) return false;
    }
    
    ALLEGRO_COLOR cor = al_map_rgb(
        rand() % 256,
        rand() % 256,
        rand() % 256
    );
    
    registros.emplace_back(apelido, 0, 0, cor);
    return salvar_dados();
}

bool Cadastro::registrar_pontuacao(const std::string& apelido, int pontos_jogo) {
    auto reg = buscar_registro(apelido);
    if(!reg) return false;
    
    reg->ultima_pontuacao = pontos_jogo;
    if(pontos_jogo > reg->recorde) {
        reg->recorde = pontos_jogo;
    }
    
    ordenar_ranking();
    return salvar_dados();
}

bool Cadastro::salvar_dados() {
    std::ofstream arquivo(arquivo_dados);
    if(!arquivo) return false;
    
    for(const auto& reg : registros) {
        arquivo << reg.apelido << " " 
                << reg.ultima_pontuacao << " " 
                << reg.recorde << " "
                << (int)reg.cor.r << " "
                << (int)reg.cor.g << " "
                << (int)reg.cor.b << "\n";
    }
    
    return true;
}

bool Cadastro::carregar_dados() {
    std::ifstream arquivo(arquivo_dados);
    if(!arquivo) return false;
    
    registros.clear();
    std::string apelido;
    int ultima, recorde, r, g, b;
    
    while(arquivo >> apelido >> ultima >> recorde >> r >> g >> b) {
        ALLEGRO_COLOR cor = al_map_rgb(r, g, b);
        registros.emplace_back(apelido, ultima, recorde, cor);
    }
    
    ordenar_ranking();
    return true;
}

void Cadastro::exibir_ranking(int x, int y, ALLEGRO_DISPLAY* display) const {
    if(!fonte) return;
    
    al_draw_text(fonte, al_map_rgb(255, 215, 0), 
                x, y, 
                ALLEGRO_ALIGN_CENTER, 
                "TOP 5 JOGADORES");
    
    y += 40;
    for(int i = 0; i < std::min(5, (int)registros.size()); ++i) {
        const auto& reg = registros[i];
        std::string texto = std::to_string(i+1) + ". " + reg.apelido + ": " + std::to_string(reg.recorde);
        
        al_draw_text(fonte, reg.cor, 
                    x, y, 
                    ALLEGRO_ALIGN_CENTER, 
                    texto.c_str());
        y += 30;
    }
}

RegistroJogador* Cadastro::buscar_registro(const std::string& apelido) {
    for(auto& reg : registros) {
        if(reg.apelido == apelido) {
            return &reg;
        }
    }
    return nullptr;
}

void Cadastro::ordenar_ranking() {
    std::sort(registros.begin(), registros.end(), 
        [](const RegistroJogador& a, const RegistroJogador& b) {
            return a.recorde > b.recorde;
        });
}