#ifndef CADASTRO_HPP
#define CADASTRO_HPP

#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class RegistroJogador {
public:
    std::string apelido;
    int ultima_pontuacao;
    int recorde;
    ALLEGRO_COLOR cor;
    
    RegistroJogador(std::string apelido = "", int ultima = 0, int recorde = 0, 
                   ALLEGRO_COLOR cor = al_map_rgb(255, 255, 255));
};

class Cadastro {
private:
    std::vector<RegistroJogador> registros;
    ALLEGRO_FONT* fonte;
    std::string arquivo_dados;

    RegistroJogador* buscar_registro(const std::string& apelido);
    void ordenar_ranking();

public:
    Cadastro(const std::string& arquivo, ALLEGRO_FONT* fonte);
    
    bool processar_tela_cadastro(ALLEGRO_EVENT_QUEUE* fila_eventos, 
                                ALLEGRO_DISPLAY* display, 
                                std::string& apelido_saida);
    bool registrar_jogador(const std::string& apelido);
    bool registrar_pontuacao(const std::string& apelido, int pontos_jogo);
    bool salvar_dados();
    bool carregar_dados();
    void exibir_ranking(int x, int y, ALLEGRO_DISPLAY* display) const;
};

#endif