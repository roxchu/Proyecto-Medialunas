#ifndef loteInsumos_LI
#define loteInsumos_LI
#include <string>
using namespace std;

class loteInsumos{
    private:
    int id;
    int idLoteProduccion;
    int idInsumo;
    float cantidad;
    string unidadMedida;
    string observaciones;
    float costoUnitario;

    public:
    loteInsumos();

    //getters
    int getId();
    int getIdLoteProduccion();
    int getIdInsumo();
    float getCantidad();
    string getUnidadMedia();
    string getObservaciones();
    float getCostoUnitario();

    //setters
    void setId(int id);
    void setIdLoteProduccion(int idLoteProduccion);
    void setIdInsumo (int idInsumo);
    void setCantidad(float cantidad);
    void setUnidadMedia(string unidadMedia);
    void setObservaciones( string observaciones);
    void setCostoUnitario(float costoUnitario);

};
#endif