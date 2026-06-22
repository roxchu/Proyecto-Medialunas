#ifndef StockVenta_SV
#define StockVenta_SV
#include <string> 
using namespace std;

class StockVenta{
    private:
    int id;
    int idVariedad;
    float stockDisponible;
    float precioUnitario;
    int idLote;

    public:
    StockVenta();

    //getters
    int getId();
    int getIdVariedad();
    float getStockDisponible();
    float getPrecioUnitario();
    int getIdLote();

    //setters
    void setId(int id);
    void setIdVariedad(int idVariedad);
    void setStockDisponible(float stockDisponible);
    void setPrecioUnitario(float precioUnitario);
    void setIdLote(int idLote);
};

#endif