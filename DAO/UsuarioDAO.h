//declarar a herencia de DAO padre. Tiene solo esta linea de código para cada tabla del DER
#ifndef UsuarioDAO_H
#define UsuarioDAO_H
#include "DAO.h"
#include "../models/usuario.h"

class UsuarioDAO : public DAO <Usuario>{
    public:
    UsuarioDAO();

};

#endif

