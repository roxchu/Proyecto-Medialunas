#include "categoriaUsuario.h"

categoriaUsuario::categoriaUsuario() {}  // constructor vacío

//getters
int categoriaUsuario::getId(){return id;}
string categoriaUsuario::getNombreCategoria(){return nombreCategoria;}

//setters
void categoriaUsuario::setId(int id){this->id = id;}
void categoriaUsuario::setNombreCategoria(string nombreCategoria){this->nombreCategoria = nombreCategoria;}
