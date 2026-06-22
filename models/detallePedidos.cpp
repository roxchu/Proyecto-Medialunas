#include "detallePedidos.h" 

DetallesPedidos::DetallesPedidos(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}

int DetallesPedidos::getIdPedido(){return idPedido;}
int DetallesPedidos::getIdVariedad(){return idVariedad;}
float DetallesPedidos::getCantidad(){return cantidad;}
float DetallesPedidos::getPrecioUnitario(){return precioUnitario;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void DetallesPedidos::setIdPedido(int idPedido) { this->idPedido = idPedido; } 
void DetallesPedidos::setIdVariedad(int idVariedad) {this->idVariedad = idVariedad; }
void DetallesPedidos::setCantidad(float cantidad) {this->cantidad = cantidad; }
void DetallesPedidos::setPrecioUnitario(float precioUnitario) {this->precioUnitario = precioUnitario;}