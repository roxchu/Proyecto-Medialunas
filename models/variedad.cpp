#include "variedad.h"

variedad::variedad(){}

//getters
int variedad::getId(){return id;}
string variedad::getNombreVariedad(){return nombreVariedad;}
string variedad::getDescripcion(){return descripcion;}

//setters
void variedad::setId(int id){this->id = id;}
void variedad::setNombreVariedad(string nombreVariedad){this->nombreVariedad = nombreVariedad;}
void variedad::setDescripcion(string descripcion){this->descripcion = descripcion;}