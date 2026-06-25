#ifndef clienteDAOImpl_H
#define clienteDAOImpl_H
#include "../DAO/clienteDAO.h"

class ClienteDAOImpl : public ClienteDAO {
public:
    ClienteDAOImpl();
    void insertar(Cliente obj);
    void actualizar(Cliente obj);
    void eliminar(Cliente obj);
    Cliente buscarPorId(int id);
    vector<Cliente> listar();
};

#endif