#ifndef Usuario_U
#define Usuario_U
#include <string>
using namespace std;

class Usuario{
    private:
    int DNI;
    string nombre;
    string apellido;
    string direccion;
    string email;
    string telefono;
    string contactoEmergencia;
    string nombreCE;
    int idCategoria;

    public:
    Usuario();

    //getters
    int getDNI();
    string getNombre();
    string getApellido();
    string getDireccion();
    string getEmail();
    string getTelefono();
    string getContactoEmergencia();
    string getNombreCE();
    int getIdCategoria();   

    //setters
    void setDNI(int DNI);
    void setNombre(string nombre);
    void setApellido(string apellido);
    void setDireccion(string direccion);
    void setEmail(string email);
    void setTelefono(string telefono);
    void setContactoEmergencia(string contactoEmergencia);
    void setNombreCE(string nombreCE);
    void setIdCategoria(int idCategoria);
};
#endif