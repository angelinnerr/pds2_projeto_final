# ifndef PONTOS_HPP
# define PONTOS_HPP

#include <string>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>

class Pontos{
    public:
        Pontos(); // construtor

        static ALLEGRO_FONT* fonte_score;
        static ALLEGRO_FONT* sombra_fonte;

        static ALLEGRO_FONT* fonte_game_over;
        static ALLEGRO_FONT* sombra_game_over;

        void carrega_fontes();
        void verificar(int posicaoJogador, int posicaoTubo);
        void atualizar();
        void game_over();
        void reset();
        int getScore() const; 

        
    private:
        int score;
};



# endif