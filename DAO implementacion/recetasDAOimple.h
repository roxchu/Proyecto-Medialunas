#ifndef recetasDAOImpl_H
#define recetasDAOImpl_H
#include "recetasDAO.h"

class recetasDAOImpl : public RecetaDAO{
public:
 recetasDAOImpl();
 void insertar (Receta obj);
 void actualizar (Receta obj);
 void eliminar (Receta obj);
 Receta buscarPorId(int id);
 vector<Receta> listar();
};

#endif