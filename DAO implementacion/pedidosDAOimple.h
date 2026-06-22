#ifndef pedidosDAOImpl_H
#define pedidosDAOImpl_H
#include "pedidosDAO.h"

class PedidosDAOImpl : public PedidosDAO{
public:
 PedidosDAOImpl();
 void insertar (Pedidos obj);
 void actualizar (Pedidos obj);
 void eliminar (Pedidos obj);
 Pedidos buscarPorId(int id);
 vector<Pedidos> listar();
};

#endif