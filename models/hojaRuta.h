#ifndef hojaRuta_HR
#define hojaRuta_HR
#include <string>
using namespace std;

class hojaRuta{
    private:
    int idVehiculo;
    int idEmpleado;
    int idPedido;
    int ordenEntrega;

    public:
    hojaRuta();

    //getters
    int getIdVehiculo();
    int getIdEmpleado();
    int getIdPedido();
    int getOrdenEntrega();

    //setters
    void setIdVehiculo(int idVehiculo);
    void setIdEmpleado(int idEmpleado);
    void setIdPedido(int idPedido);
    void setOrdenEntrega(int ordenEntrega);
};
#endif

