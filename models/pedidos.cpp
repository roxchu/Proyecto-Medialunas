#include "pedidos.h"

Pedidos::Pedidos(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int Pedidos::getId(){return id;}
int Pedidos::getIdCliente(){return idCliente;}
string Pedidos::getFechaPedido(){return fechaPedido;}
string Pedidos::getEstado(){return Estado;}


//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void Pedidos::setId(int id){this->id = id;}
void Pedidos::setIdCliente(int idCliente){this->idCliente = idCliente;}
void Pedidos::setFechaPedido(string fechaPedido){this->fechaPedido = fechaPedido;}
void Pedidos::setEstado(string Estado){this->Estado = Estado;}
