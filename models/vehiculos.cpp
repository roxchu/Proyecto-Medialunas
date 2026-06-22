#include "vehiculos.h"

Vehiculos::Vehiculos(){}

//getters
int Vehiculos::getId(){return id;}
string Vehiculos::getPatente(){return patente;}
string Vehiculos::getModelo(){return modelo;}
string Vehiculos::getEstadoMantenimiento(){return estadoMantenimiento;}

//setters
void Vehiculos::setId(int id){this -> id = id;}
void Vehiculos::setPatente(string patente){this->patente = patente;}
void Vehiculos::setModelo(string modelo){this->modelo = modelo;}
void Vehiculos::setEstadoMantenimiento(string estadoMantenimiento){this->estadoMantenimiento = estadoMantenimiento;}
