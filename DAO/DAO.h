#ifndef DAO_H
#define DAO_H
#include <vector>
using namespace std;

template <class T>
class DAO {         
public:
    virtual void insertar(T obj) = 0;
    virtual void actualizar(T obj) = 0;
    virtual void eliminar(T obj) = 0;
    virtual T buscarPorId(int id) = 0;
    virtual vector<T> listar() = 0;
};

#endif