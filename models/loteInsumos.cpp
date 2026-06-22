#include "loteInsumos.h"

loteInsumos::loteInsumos(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int loteInsumos::getId(){return id;}
int loteInsumos::getIdLoteProduccion(){return idLoteProduccion;}
int loteInsumos::getIdInsumo(){return idInsumo;}
float loteInsumos::getCantidad(){return cantidad;}
string loteInsumos::getUnidadMedia(){return unidadMedida;}
string loteInsumos::getObservaciones(){return observaciones;}
float loteInsumos::getCostoUnitario(){return costoUnitario;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void loteInsumos::setId(int id){this->id = id;}
void loteInsumos::setIdLoteProduccion(int idLoteProduccion){this->idLoteProduccion = idLoteProduccion;}
void loteInsumos::setIdInsumo (int idInsumo){this->idInsumo = idInsumo;}
void loteInsumos::setCantidad(float cantidad){this->cantidad = cantidad;}
void loteInsumos::setUnidadMEdia(string unidadMedia){this->unidadMedida = unidadMedia;}
void loteInsumos::setObservaciones( string observaciones){this->observaciones = observaciones;}
void loteInsumos::setCostoUnitario(float costoUnitario){this->costoUnitario = costoUnitario;}