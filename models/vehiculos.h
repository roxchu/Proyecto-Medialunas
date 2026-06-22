#ifndef Vehiculos_T
#define Vehiculos_T
#include <string>
using namespace std;

class Vehiculos{
    private:
    int id;
    string patente;
    string modelo;
    string estadoMantenimiento;

    public:
    Vehiculos();

    //getters
    int getId();
    string getPatente();
    string getModelo();
    string getEstadoMantenimiento();

    //setters
    void setId(int id);
    void setPatente(string patente);
    void setModelo(string modelo);
    void setEstadoMantenimiento(string estadoMantenimiento);
};
#endif