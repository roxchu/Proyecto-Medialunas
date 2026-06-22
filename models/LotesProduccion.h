#ifndef LotesProduccion_LP
#define LotesProduccion_LP
#include <string>
using namespace std;

class LotesProduccion{
    private: 
    int id;
    string fecha;
    int idVariedad;
    float cantidadProducida;
    int idEmpleado;
    int idReceta;

    public:
    LotesProduccion();

    //getters
    int getId();
    string getFecha();
    int getIdVariedad();
    float getCantidadProducida();
    int getIdEmpleado();
    int getIdReceta();

    //setters
    void setId(int id);
    void setFecha(string fecha);
    void setIdVariedad(int idVariedad);
    void setCantidadProducida(float cantidadProducida);
    void setIdEmpleado(int idEmpleado);
    void setIdReceta(int idReceta);
};

#endif