#ifndef lotesProduccionDAOImpl_H
#define lotesProduccionDAOImpl_H
#include "../DAO/lotesProduccionDAO.h"

class lotesProduccionDAOImpl : public LotesProduccionDAO{
public:
 lotesProduccionDAOImpl();
 void insertar (LotesProduccion obj);
 void actualizar (LotesProduccion obj);
 void eliminar (LotesProduccion obj);
 LotesProduccion buscarPorId(int id);
 vector<LotesProduccion> listar();
};

#endif