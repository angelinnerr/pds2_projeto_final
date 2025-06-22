#ifndef CONSTANTS_H
#define CONSTANTS_H


// ------------- funções gerais ------------

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
const int GRAVIDADE = 1;
const int FORCA_PULO = -12;
extern const char* const DADOS_JOGADORES;

// ------------------ tubo ----------------
extern const char* const TUBO_BAIXO;
extern const char* const TUBO_CIMA;
const int NUM_TUBOS = 3;
const int ESPACO_HORIZONTAL_ENTRE_TUBOS = 350;
const int ESPACO_ENTRE_TUBOS = 200;
const int LARGURA_TUBO = 110;
const int ALTURA_TUBO = 580;
const int VELOCIDADE_TUBO = 2;

// ------------------ fundo ------------------------

extern const char* const FUNDO_CADASTRO;
extern const char* const FUNDO_JOGO;
const int VELOCIDADE_FUNDO = 2;

// ----------- elementos do cadastro ----------------

extern const char* const PLACA_CADASTRO;
extern const char* const MOLDURA_APELIDO;
extern const char* const BOTAO_INICIAR;
extern const char* const QUADRO_CAD;
extern const char* const CAIXA_CADASTRO;
extern const char* const MOLDURA_RANKING;
extern const char* const FUNDO_TEXTOS;

extern const char* const NUMERO_UM;
extern const char* const NUMERO_DOIS;
extern const char* const NUMERO_TRES;

// -------------- pássaro ----------------------------

extern const char* const IMAGEM_BIRD;

// ---------------- música -----------------------------

extern const char* const MUSICA; 

// ----------- fontes -----------------

extern const char* const FONTE_GAME_OVER;
extern const char* const FONTE_BASE;








#endif