#ifndef categoriaUsuarioDAO_H
#define categoriaUsuarioDAO_H
#include "DAO.h"
#include "../models/categoriaUsuario.h"        // el modelo

class categoriaUsuarioDAO : public DAO<categoriaUsuario> {
    public:
    categoriaUsuarioDAO();
};

#endif