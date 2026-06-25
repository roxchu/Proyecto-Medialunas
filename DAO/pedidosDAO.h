#ifndef pedidosDAO_H
#define pedidosDAO_H
#include "DAO.h"
#include "../models/pedidos.h"

class PedidosDAO : public DAO<Pedidos>{
    public:
    PedidosDAO();

};

#endif