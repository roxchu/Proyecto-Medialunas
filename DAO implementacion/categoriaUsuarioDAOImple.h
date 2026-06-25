#ifndef categoriaUsuarioDAOImpl_H
#define categoriaUsuarioDAOImpl_H
#include "../DAO/categoriaUsuarioDAO.h"

class categoriaUsuarioDAOImpl : public categoriaUsuarioDAO{
public:
 categoriaUsuarioDAOImpl();
 void insertar(categoriaUsuario obj);
 void actualizar(categoriaUsuario obj);
 void eliminar(categoriaUsuario obj);
 categoriaUsuario buscarPorId(int id);
 vector<categoriaUsuario> listar();
};

#endif