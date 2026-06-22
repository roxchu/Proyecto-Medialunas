#ifndef UsuarioDAOImpl_H
#define UsuarioDAImpl_H
#include "UsuarioDAO.h"

class UsuarioDAOImpl : public UsuarioDAO{
public:
 UsuarioDAOImpl();
 void insertar (Usuario obj);
 void actualizar (Usuario obj);
 void eliminar (Usuario obj);
 Usuario buscarPorId(int id);
 vector<Usuario> listar();
};

#endif