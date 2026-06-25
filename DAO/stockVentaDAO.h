#ifndef stockVentaDAO_H
#define stockVentaDAO_H
#include "DAO.h"
#include "../models/stockVenta.h"

class StockVentaDAO : public DAO <StockVenta>{
    public:
    StockVentaDAO();
};

#endif