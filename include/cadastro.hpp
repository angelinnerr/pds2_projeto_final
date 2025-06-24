#ifndef CADASTRO_HPP
#define CADASTRO_HPP

#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Fundo;

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
    
    ~Cadastro();
    
    Cadastro(ALLEGRO_FONT* fonte, const std::string& arquivo, bool carregarImagens);
    
    bool processar_tela_cadastro(ALLEGRO_EVENT_QUEUE* fila_eventos, 
                                ALLEGRO_DISPLAY* display, 
                                std::string& apelido_saida, Fundo& fundo_do_jogo); 
    bool registrar_jogador(const std::string& apelido);
    bool registrar_pontuacao(const std::string& apelido, int pontos_jogo);
    bool salvar_dados();
    bool carregar_dados();
    void exibir_ranking(int x, int y, ALLEGRO_DISPLAY* display) const;
    void carregar_imagens();

    ALLEGRO_BITMAP* imagem_caixa_texto_1;
    ALLEGRO_BITMAP* imag_iniciar;
    ALLEGRO_BITMAP* imag_caixa;
    ALLEGRO_BITMAP* imag_cadastro;

    ALLEGRO_BITMAP* imagem_fundo_ranking;
    ALLEGRO_BITMAP* imagem_fundo_ranking2;
    ALLEGRO_BITMAP* imagem_caixa_instrucoes;
    ALLEGRO_BITMAP* imagem_botao1;

    ALLEGRO_BITMAP* getImagemCaixaInstrucoes() const;

    
};

#endif