#include "variedadDAOimple.h"
#include "../baseDatos/conexion.h"

variedadDAOImpl::variedadDAOImpl() {}

void variedadDAOImpl::insertar(variedad obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO variedad (nombreVariedad, descripcion) VALUES ('"
                 + obj.getNombreVariedad() + "', '"
                 + obj.getDescripcion() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void variedadDAOImpl::eliminar(variedad obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM variedad WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

variedad variedadDAOImpl::buscarPorId(int id) {
    variedad obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM variedad WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombreVariedad((char*)sqlite3_column_text(stmt, 1));
        obj.setDescripcion((char*)sqlite3_column_text(stmt, 2));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<variedad> variedadDAOImpl::listar() {
    vector<variedad> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM variedad";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        variedad obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombreVariedad((char*)sqlite3_column_text(stmt, 1));
        obj.setDescripcion((char*)sqlite3_column_text(stmt, 2));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void variedadDAOImpl::actualizar(variedad obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE variedad SET nombreVariedad='" + obj.getNombreVariedad()
                 + "', descripcion='" + obj.getDescripcion()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}