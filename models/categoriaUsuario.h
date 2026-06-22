#ifndef categoriaUsuario_CA
#define categoriaUsuario_CA
#include <string>
using namespace std;

class categoriaUsuario{
    private: 
    int id;
    string nombreCategoria;

    public:
    categoriaUsuario(); //método constructor

    //getters
    int getId();
    string getNombreCategoria();

    //setters
    void setId(int id);
    void setNombreCategoria(string nombreCategoria);
};
#endif

