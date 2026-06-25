#ifndef insumosDAOImpl_H
#define insumosDAOImpl_H
#include "../DAO/insumosDAO.h"

class InsumosDAOImpl : public InsumosDAO{
public:
 InsumosDAOImpl();
 void insertar (Insumos obj);
 void actualizar (Insumos obj);
 void eliminar (Insumos obj);
 Insumos buscarPorId(int id);
 vector<Insumos> listar();
};

#endif;