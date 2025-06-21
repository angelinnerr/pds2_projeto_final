#ifndef EXCECOES_HPP
#define EXCECOES

#include <stdexcept>
#include <string>

class ErroDeInicializacao : public std::runtime_error {
    public:
        ErroDeInicializacao(const std::string& mensagem);
};

#endif