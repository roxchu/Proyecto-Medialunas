#include "conexion.h"

Conexion::Conexion(string nombreBD) {
    this->nombreBD = nombreBD;
    db = nullptr;
}

bool Conexion::conectar() {
    int resultado = sqlite3_open(nombreBD.c_str(), &db);
    if (resultado != SQLITE_OK) {
        cout << "Error al conectar BD" << endl;
        return false;
    }
    return true;
}

sqlite3* Conexion::getDB() {
    return db;
}

void Conexion::cerrar() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}