#ifndef vehiculosDAOImpl_H
#define vehiculosDAOImpl_H
#include "../DAO/vehiculosDAO.h"

class VehiculosDAOImpl : public VehiculosDAO {
public:
 VehiculosDAOImpl();
 void insertar (Vehiculos obj);
 void actualizar (Vehiculos obj);
 void eliminar (Vehiculos obj);
 Vehiculos buscarPorId(int id);
 vector<Vehiculos> listar();
};

#endif