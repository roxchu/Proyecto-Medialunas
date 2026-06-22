#include "hojaRuta.h"

hojaRuta::hojaRuta(){}

//getters
//tipo funcion + nombre clase ::getMétodo() {return variable;}
int hojaRuta::getIdVehiculo(){return idVehiculo;}
int hojaRuta::getIdEmpleado(){return idEmpleado;}
int hojaRuta::getIdPedido(){return idPedido;}
int hojaRuta::getOrdenEntrega(){return ordenEntrega;}

//setters
//void + nombre clase::setMétodo(variable por parametro){this->variable=variable;}
void hojaRuta::setIdVehiculo(int idVehiculo){this->idVehiculo = idVehiculo;}
void hojaRuta::setIdEmpleado(int idEmpleado){this->idEmpleado = idEmpleado;}
void hojaRuta::setIdPedido(int idPedido){this->idPedido = idPedido;}
void hojaRuta::setOrdenEntrega(int ordenEntrega){this ->ordenEntrega = ordenEntrega;}