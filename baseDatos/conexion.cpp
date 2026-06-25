#include "conexion.h"
#include <fstream>

static bool existeArchivo(const string& ruta) {
    ifstream archivo(ruta);
    return archivo.good();
}

Conexion::Conexion(string nombreBD) {
    this->nombreBD = nombreBD;
    db = nullptr;
}

bool Conexion::conectar() {
    string ruta = nombreBD;
    if (nombreBD == "medialunas.db") {
        if (existeArchivo("baseDatos/medialunas.db")) {
            ruta = "baseDatos/medialunas.db";
        } else if (existeArchivo("../baseDatos/medialunas.db")) {
            ruta = "../baseDatos/medialunas.db";
        }
    }

    int resultado = sqlite3_open(ruta.c_str(), &db);
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
