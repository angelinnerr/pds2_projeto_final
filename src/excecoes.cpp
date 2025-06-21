#include "excecoes.hpp"

ErroDeInicializacao::ErroDeInicializacao(const std::string& mensagem): std::runtime_error(mensagem) {}
