#include "usuario.h"

Usuario::Usuario(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int Usuario::getDNI(){return DNI;}
string Usuario::getNombre(){return nombre;}
string Usuario::getApellido(){return apellido;}
string Usuario::getDireccion(){return direccion;}
string Usuario::getEmail(){return email;}
string Usuario::getTelefono(){return telefono;}
string Usuario::getContactoEmergencia(){return contactoEmergencia;}
string Usuario::getNombreCE(){return nombreCE;}
int Usuario::getIdCategoria(){return idCategoria;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void Usuario::setDNI(int DNI){this->DNI = DNI;}
void Usuario::setNombre(string nombre){this->nombre = nombre;}
void Usuario::setApellido(string apellido){this->apellido = apellido;}
void Usuario::setDireccion(string direccion){this->direccion = direccion;}
void Usuario::setEmail(string email){this->email = email;}
void Usuario::setTelefono(string telefono){this->telefono = telefono;}
void Usuario::setContactoEmergencia(string contactoEmergencia){this->contactoEmergencia = contactoEmergencia;}
void Usuario::setNombreCE(string nombreCE){this->nombreCE = nombreCE;}
void Usuario::setIdCategoria(int idCategoria){this->idCategoria = idCategoria;}