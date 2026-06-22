#ifndef Receta_R
#define Receta_R
#include <string>
using namespace std;

class Receta{
    private:
    int id;
    string nombreProducto;
    int insumosNecesarios;
    float cantidadEsperada;
    string procedimiento;

    public:
    Receta();

    //getters
    int getId();
    string getNombreProducto();
    int getInsumosNecesarios();
    float getCantidadEsperada();
    string getProcedimiento();

    //setters
    void setId(int id);
    void setNombreProducto(string nombreProducto);
    void setInsumosNecesarios(int insumosNecesarios);
    void setCantidadEsperada(float cantidadEsperada);
    void setProcedimiento(string procedimiento);
};
#endif
