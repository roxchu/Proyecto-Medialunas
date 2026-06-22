#ifndef hojaRutaDAOImpl_H
#define hojaRutaDAOImpl_H
#include "hojaRutaDAO.h"

class hojaRutaDAO : public hojaRutaDAO{ 
public:
 hojaRutaDAO();
 void insertar (hojaRuta obj);
 void actualizar (hojaRuta obj);
 void eliminar (hojaRuta obj);
 hojaRuta buscarPorId(int id);
 vector<hojaRuta> listar();
};

#endif