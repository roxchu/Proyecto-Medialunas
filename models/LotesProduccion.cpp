#include "LotesProduccion.h"

LotesProduccion::LotesProduccion(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int LotesProduccion::getId(){return id;}
string LotesProduccion::getFecha(){return fecha;}
int LotesProduccion::getIdVariedad(){return idVariedad;}
float LotesProduccion::getCantidadProducida(){return cantidadProducida;}
int LotesProduccion::getIdEmpleado(){return idEmpleado;}
int LotesProduccion::getIdReceta(){return idReceta;}


//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void LotesProduccion::setId(int id){this->id = id;}
void LotesProduccion::setFecha(string fecha){this->fecha = fecha;}
void LotesProduccion::setIdVariedad(int idVariedad){this->idVariedad = idVariedad;}
void LotesProduccion::setCantidadProducida(float cantidadProducida){this->cantidadProducida = cantidadProducida;}
void LotesProduccion::setIdEmpleado(int idEmpleado){this->idEmpleado = idEmpleado;}
void LotesProduccion::setIdReceta(int idReceta){this->idReceta = idReceta;}