#ifndef clienteDAO_H
#define clienteDAO_H
#include "DAO.h"
#include "../models/cliente.h"        // el modelo

class ClienteDAO : public DAO<Cliente>{
    public:
    ClienteDAO();
    //public: NombreClase();
};

#endif