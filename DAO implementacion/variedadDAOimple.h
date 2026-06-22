#ifndef variedadDAOImpl_H
#define variedadDAOImpl_H
#include "variedadDAO.h"

class variedadDAOImpl : public variedadDAO{ 
public:
 variedadDAOImpl();
 void insertar (variedad obj);
 void actualizar (variedad obj);
 void eliminar (variedad obj);
 variedad buscarPorId(int id);
 vector<variedad> listar();
};

#endif