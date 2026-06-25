#ifndef stockVentaDAOImpl_H
#define stockVentaDAOImpl_H
#include "../DAO/stockVentaDAO.h"

class stockVentaDAOImpl : public StockVentaDAO{ 
public:
 stockVentaDAOImpl();
 void insertar (StockVenta obj);
 void actualizar (StockVenta obj);
 void eliminar (StockVenta obj);
 StockVenta buscarPorId(int id);
 vector<StockVenta> listar();
};

#endif