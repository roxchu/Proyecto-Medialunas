#ifndef Variedad_V
#define Variedad_V
#include <string>
using namespace std;

class variedad{
    private:
    int id;
    string nombreVariedad;
    string descripcion;

    public:
    variedad();

    //getters
    int getId();
    string getNombreVariedad();
    string getDescripcion();

    //setters
    void setId(int id);
    void setNombreVariedad(string nombreVariedad);
    void setDescripcion(string descripcion);
};
#endif
