#ifndef hojaRutaDAOImpl_H
#define hojaRutaDAOImpl_H
#include "../DAO/hojaRutaDAO.h"

class hojaRutaDAOImpl : public hojaRutaDAO{ 
public:
 hojaRutaDAOImpl();
 void insertar (hojaRuta obj);
 void actualizar (hojaRuta obj);
 void eliminar (hojaRuta obj);
 hojaRuta buscarPorId(int id);
 vector<hojaRuta> listar();
};

#endif