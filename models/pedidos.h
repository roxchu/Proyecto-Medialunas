#ifndef pedidos_P
#define pedidos_P
#include <string>
using namespace std;

class Pedidos{
    private:
    int id;
    int idCliente;
    string fechaPedido;
    string Estado;

    public:
    Pedidos();

    //getters
    int getId();
    int getIdCliente();
    string getFechaPedido();
    string getEstado();

    //setters
    void setId(int id);
    void setIdCliente(int idCliente);
    void setFechaPedido(string fechaPedido);
    void setEstado(string Estado);
};
#endif

