#ifndef CONEXION_H
#define CONEXION_H
#include <iostream>
#include <sqlite3.h>
using namespace std;

class Conexion {
private:
    sqlite3 *db;
    string nombreBD;

public:
    Conexion(string nombreBD);
    bool conectar();
    sqlite3* getDB();
    void cerrar();
};

#endif