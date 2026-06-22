#ifndef Insumos_I
#define Insumos_I
#include <string>
using namespace std;

class Insumos{
    private:
    int id;
    string nombre;
    float cantidadActual;
    string unidadMedia;
    string proveedor;

    public:
    Insumos();

    //getters
    int getId();
    string getNombre();
    float getCantidadActual();
    string getUnidadMedia();
    string getProveedor();

    //setters
    void setId(int id);
    void setNombre(string nombre);
    void setCantidadActual( float cantidadActual);
    void setUnidadMedia(string unidadMedia);
    void setProveedor(string proveedor);
};

#endif