#ifndef FUNDO_HPP
#define FUNDO_HPP

#include <string>
#include <allegro5/allegro.h>

using namespace std;

class Fundo {
private:
    ALLEGRO_BITMAP* imagem;
    int posicao_x;

public:
    Fundo();
    ~Fundo();
    void atualizar();
    void desenhar();
    void carregar_imagem(const string& caminho_imagem); 
};

#endif