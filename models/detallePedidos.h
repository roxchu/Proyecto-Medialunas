#ifndef DetallesPedidos_DP
#define DetallesPedidos_DP
#include <string> 
using namespace std;

class DetallesPedidos{
    private:
    int idPedido;
    int idVariedad;
    float cantidad;
    float precioUnitario;

    public:
    DetallesPedidos();

    //getters
    int getIdPedido();
    int getIdVariedad();
    float getCantidad();
    float getPrecioUnitario();

    //setters
    void setIdPedido(int idPedido);
    void setIdVariedad(int idVariedad);
    void setCantidad(float cantidad);
    void setPrecioUnitario(float precioUnitario);
};
#endif