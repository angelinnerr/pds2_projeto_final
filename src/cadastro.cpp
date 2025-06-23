#include "cadastro.hpp"
#include "constants.h"
#include "inicializador.hpp"
#include <algorithm>
#include "fundo.hpp"
#include <fstream>
#include <allegro5/allegro_native_dialog.h>

RegistroJogador::RegistroJogador(std::string apelido, int ultima, int recorde, ALLEGRO_COLOR cor) :
    apelido(apelido), ultima_pontuacao(ultima), recorde(recorde), cor(cor) {}

Cadastro::Cadastro(ALLEGRO_FONT* fonte, const std::string& arquivo) :
    fonte(fonte), arquivo_dados(arquivo), imagem_caixa_texto_1(nullptr), imagem_fundo_ranking(nullptr), imagem_fundo_ranking2(nullptr),imagem_caixa_instrucoes(nullptr),imagem_botao1(nullptr)  {
    carregar_dados();

    imagem_caixa_texto_1 = al_load_bitmap(MOLDURA_APELIDO); 
    verificarInicializacao(imagem_caixa_texto_1, "imagem da caixa de texto do cadastro");

    imag_cadastro = al_load_bitmap(PLACA_CADASTRO);
    verificarInicializacao(imag_cadastro, "imagem da placa do cadastro");

    imag_caixa = al_load_bitmap(CAIXA_CADASTRO);
    verificarInicializacao(imag_caixa, "imagem da caixa do cadastro");

    imag_iniciar = al_load_bitmap(BOTAO_INICIAR);
    verificarInicializacao(imag_iniciar, "imagem iniciar");

    imagem_fundo_ranking = al_load_bitmap(MOLDURA_RANKING);
    verificarInicializacao(imagem_fundo_ranking, "imagem de fundo do ranking");

    imagem_fundo_ranking2 = al_load_bitmap(QUADRO_CAD); 
    verificarInicializacao(imagem_fundo_ranking2, "imagem 2 do fundo do ranking");

    imagem_caixa_instrucoes = al_load_bitmap(FUNDO_TEXTOS);
    verificarInicializacao(imagem_caixa_instrucoes, "imagem da caixa de instruções");

    imagem_botao1 = al_load_bitmap(NUMERO_UM);
    verificarInicializacao(imagem_botao1, "imagem do botão 1");
}

bool Cadastro::processar_tela_cadastro(ALLEGRO_EVENT_QUEUE* fila_eventos, 
                                     ALLEGRO_DISPLAY* display, 
                                     std::string& apelido_saida, Fundo& fundo_do_jogo) {
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
        fundo_do_jogo.atualizar(); 
        fundo_do_jogo.desenhar();

        al_draw_bitmap(imagem_caixa_texto_1, 55, 195, 0); 
        al_draw_bitmap(imag_cadastro, 230, 50, 0);
        al_draw_bitmap(imag_caixa, 280, 295, 0);
        al_draw_bitmap(imag_iniciar, 500, 380, 0);

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
    
    //  se a pessoa já está cadastrada
    RegistroJogador* reg = buscar_registro(apelido);
    if (reg != nullptr) {
        return true; // Jogador já existe, não cria de novo.
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
        unsigned char r, g, b;
        al_unmap_rgb(reg.cor, &r, &g, &b);
        arquivo << reg.apelido << " "
                << reg.ultima_pontuacao << " "
                << reg.recorde << " "
                << (int)r << " "
                << (int)g << " "
                << (int)b << "\n";
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

    if (imagem_fundo_ranking) { //
    
        float pos_x_imagem = x - al_get_bitmap_width(imagem_fundo_ranking) / 2; 
        float pos_y_imagem = y - 30; 

        al_draw_bitmap(imagem_fundo_ranking2, pos_x_imagem - 210 , pos_y_imagem - 170, 0);

        al_draw_bitmap(imagem_fundo_ranking, pos_x_imagem, pos_y_imagem, 0); 

        al_draw_bitmap(imag_caixa, pos_x_imagem + 45 , pos_y_imagem + 25, 0);

    }
    
    al_draw_text(fonte, al_map_rgb(255, 215, 0), 
                x, y, 
                ALLEGRO_ALIGN_CENTER, 
                "TOP 5 JOGADORES");
    
    y += 55;
    for(int i = 0; i < std::min(5, (int)registros.size()); ++i) {
        const auto& reg = registros[i];
        std::string texto = std::to_string(i+1) + ". " + reg.apelido + ": " + std::to_string(reg.recorde);
        
        al_draw_text(fonte, reg.cor, 
                    x, y, 
                    ALLEGRO_ALIGN_CENTER, 
                    texto.c_str());
        y += 25;
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

ALLEGRO_BITMAP* Cadastro::getImagemCaixaInstrucoes() const {
    return imagem_caixa_instrucoes; // para imagens das caixas dos textos
}

Cadastro::~Cadastro() {
    if (imagem_caixa_texto_1) al_destroy_bitmap(imagem_caixa_texto_1);
    if (imag_cadastro) al_destroy_bitmap(imag_cadastro);
    if (imag_caixa) al_destroy_bitmap(imag_caixa);
    if (imag_iniciar) al_destroy_bitmap(imag_iniciar);
    if (imagem_fundo_ranking) al_destroy_bitmap(imagem_fundo_ranking);
    if (imagem_fundo_ranking2) al_destroy_bitmap(imagem_fundo_ranking2);
    if (imagem_caixa_instrucoes) al_destroy_bitmap(imagem_caixa_instrucoes);
    if (imagem_botao1) al_destroy_bitmap(imagem_botao1);
}
