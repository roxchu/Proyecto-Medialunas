#ifndef detallePedidosDAOImple_H
#define detallePedidosDAOImple_H
#include "detallePedidosDAO.h"

class DetallesPedidosDAOImpl : public DetallesPedidosDAO{
public: 
 DetallesPedidosDAOImpl();
 void insertar (DetallesPedidos obj);
 void actualizar (DetallesPedidos obj);
 void eliminar (DetallesPedidos obj);
 DetallesPedidos buscarPorId(int id);
 vector<DetallesPedidos> listar ();
};

#endif
