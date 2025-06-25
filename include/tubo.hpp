#ifndef TUBO_HPP
#define TUBO_HPP

#include <allegro5/allegro.h>

class Tubo {
public:
    int x;
    int altura_abertura;

    static ALLEGRO_BITMAP* imagem_cima;
    static ALLEGRO_BITMAP* imagem_baixo;

    static ALLEGRO_BITMAP* tubo_colorido_cima;
    static ALLEGRO_BITMAP* tubo_colorido_baixo;

    static bool usar_tubo_colorido;
    static void set_usar_imagens_coloridas(bool usar);

    Tubo(int posicao_x);
    void atualizar();
    void desenhar();
    bool colide(int jogador_x, int jogador_y, int jogador_largura, int jogador_altura);
    bool estaForaDaTela();
    void resetar(int nova_posicao_x);
    int getAbertura();
};

void carregar_imagens_tubo();
void carregar_tubo_colorido();
void destruir_imagens_tubo();

#endif