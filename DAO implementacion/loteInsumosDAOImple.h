#ifndef loteInsumosDAOImpl_H
#define loteInsumosDAOImpl_H
#include "loteInsumosDAO.h"

class loteInsumosDAOImpl : public loteInsumosDAO{
public:
 loteInsumosDAOImpl();
 void insertar (loteInsumos obj);
 void actualizar (loteInsumos obj);
 void eliminar (loteInsumos obj);
 loteInsumos buscarPorId(int id);
 vector<loteInsumos> listar();
};

#endif