#include "receta.h"

Receta::Receta(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int Receta::getId(){return id;}
string Receta::getNombreProducto(){return nombreProducto;}
int Receta::getInsumosNecesarios(){return insumosNecesarios;}
float Receta::getCantidadEsperada(){return cantidadEsperada;}
string Receta::getProcedimiento(){return procedimiento;}


//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void Receta::setId(int id){this->id = id;}
void Receta::setNombreProducto(string nombreProducto){this->nombreProducto = nombreProducto;}
void Receta::setInsumosNecesarios(int insumosNecesarios){this->insumosNecesarios = insumosNecesarios;}
void Receta::setCantidadEsperada(float cantidadEsperada){this->cantidadEsperada = cantidadEsperada;}
void Receta::setProcedimiento(string procedimiento){this->procedimiento = procedimiento;}