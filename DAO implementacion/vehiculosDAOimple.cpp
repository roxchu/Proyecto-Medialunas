#include "vehiculosDAOimple.h"
#include "../baseDatos/conexion.h"

VehiculosDAOImpl::VehiculosDAOImpl() {}

void VehiculosDAOImpl::insertar(Vehiculos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO vehiculos (patente, modelo, estadoMantenimiento) VALUES ('"
                 + obj.getPatente() + "', '"
                 + obj.getModelo() + "', '"
                 + obj.getEstadoMantenimiento() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void VehiculosDAOImpl::eliminar(Vehiculos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM vehiculos WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Vehiculos VehiculosDAOImpl::buscarPorId(int id) {
    Vehiculos obj;
    Conexion con("medialunas.db");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM vehiculos WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setPatente((char*)sqlite3_column_text(stmt, 1));
        obj.setModelo((char*)sqlite3_column_text(stmt, 2));
        obj.setEstadoMantenimiento((char*)sqlite3_column_text(stmt, 3));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<Vehiculos> VehiculosDAOImpl::listar() {
    vector<Vehiculos> lista;
    Conexion con("medialunas.db");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM vehiculos";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Vehiculos obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setPatente((char*)sqlite3_column_text(stmt, 1));
        obj.setModelo((char*)sqlite3_column_text(stmt, 2));
        obj.setEstadoMantenimiento((char*)sqlite3_column_text(stmt, 3));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void VehiculosDAOImpl::actualizar(Vehiculos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE vehiculos SET patente='" + obj.getPatente()
                 + "', modelo='" + obj.getModelo()
                 + "', estadoMantenimiento='" + obj.getEstadoMantenimiento()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}