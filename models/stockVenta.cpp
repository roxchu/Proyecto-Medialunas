#include "stockVenta.h"

StockVenta::StockVenta(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int StockVenta::getId(){return id;}
int StockVenta::getIdVariedad(){return idVariedad;}
float StockVenta::getStockDisponible(){return stockDisponible;}
float StockVenta::getPrecioUnitario(){return precioUnitario;}
int StockVenta::getIdLote(){return idLote;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void StockVenta::setId(int id){this->id = id;}
void StockVenta::setIdVariedad(int idVariedad){this->idVariedad = idVariedad;}
void StockVenta::setStockDisponible(float stockDisponible){this->stockDisponible = stockDisponible;}
void StockVenta::setPrecioUnitario(float precioUnitario){this->precioUnitario = precioUnitario;}
void StockVenta::setIdLote(int idLote){this->idLote = idLote;}