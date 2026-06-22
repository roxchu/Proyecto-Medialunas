#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
using namespace std;

class Cliente {
private:
    int id;
    string nombre;
    string CUIT;
    string direccion;
    string telefono;
    string metodoPago;
    string categoria;

    public:
    Cliente();

    // Getters
    int getId();
    string getNombre();
    string getCUIT();
    string getDireccion();
    string getTelefono();
    string getMetodoPago();
    string getCategoria();

    // Setters
    void setId(int id);
    void setNombre(string nombre);
    void setCUIT(string CUIT);
    void setDireccion(string direccion);
    void setTelefono(string telefono);
    void setMetodoPago(string metodoPago);
    void setCategoria(string categoria);
};

//lo creas aca y en el.cpp lo desarrollas, con los getters y setters, constructores, etc. Los parametros no estan acá estan el e .cpp

#endif