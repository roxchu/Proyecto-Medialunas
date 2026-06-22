#include "cliente.h"

Cliente::Cliente() {}  // constructor vacío

// Getters
int Cliente::getId() { return id; }
string Cliente::getNombre() { return nombre; }
string Cliente::getCUIT() { return CUIT; }
string Cliente::getDireccion() { return direccion; }
string Cliente::getTelefono() { return telefono; }
string Cliente::getMetodoPago() { return metodoPago; }
string Cliente::getCategoria() { return categoria; }

// Setters
void Cliente::setId(int id) { this->id = id; }
void Cliente::setNombre(string nombre) { this->nombre = nombre; }
void Cliente::setCUIT(string CUIT) { this->CUIT = CUIT; }
void Cliente::setDireccion(string direccion) { this->direccion = direccion; }
void Cliente::setTelefono(string telefono) { this->telefono = telefono; }
void Cliente::setMetodoPago(string metodoPago) { this->metodoPago = metodoPago; }
void Cliente::setCategoria(string categoria) { this->categoria = categoria; }