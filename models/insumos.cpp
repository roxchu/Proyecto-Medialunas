#include "insumos.h"

Insumos::Insumos(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int Insumos::getId(){return id;}
string Insumos::getNombre(){return nombre;}
float Insumos::getCantidadActual(){return cantidadActual;}
string Insumos::getUnidadMedia(){return unidadMedia;}
string Insumos::getProveedor(){return proveedor;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void Insumos::setId(int id){this->id = id;}
void Insumos::setNombre(string nombre){this->nombre = nombre;}
void Insumos::setCantidadActual( float cantidadActual){this->cantidadActual = cantidadActual;}
void Insumos::setUnidadMedia(string unidadMedia){this->unidadMedia = unidadMedia;}
void Insumos::setProveedor(string proveedor){this->proveedor = proveedor;}