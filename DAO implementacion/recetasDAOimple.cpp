#include "recetasDAOimple.h"
#include "../baseDatos/conexion.h"

recetasDAOImpl::recetasDAOImpl() {}

void recetasDAOImpl::insertar(Receta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO receta (nombreProducto, insumosNecesarios, cantidadEsperada, procedimiento) VALUES ('"
                 + obj.getNombreProducto() + "', "
                 + to_string(obj.getInsumosNecesarios()) + ", "
                 + to_string(obj.getCantidadEsperada()) + ", '"
                 + obj.getProcedimiento() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void recetasDAOImpl::eliminar(Receta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM receta WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Receta recetasDAOImpl::buscarPorId(int id) {
    Receta obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM receta WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombreProducto((char*)sqlite3_column_text(stmt, 1));
        obj.setInsumosNecesarios(sqlite3_column_int(stmt, 2));
        obj.setCantidadEsperada(sqlite3_column_double(stmt, 3));
        obj.setProcedimiento((char*)sqlite3_column_text(stmt, 4));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<Receta> recetasDAOImpl::listar() {
    vector<Receta> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM receta";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Receta obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombreProducto((char*)sqlite3_column_text(stmt, 1));
        obj.setInsumosNecesarios(sqlite3_column_int(stmt, 2));
        obj.setCantidadEsperada(sqlite3_column_double(stmt, 3));
        obj.setProcedimiento((char*)sqlite3_column_text(stmt, 4));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void recetasDAOImpl::actualizar(Receta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE receta SET nombreProducto='" + obj.getNombreProducto()
                 + "', insumosNecesarios=" + to_string(obj.getInsumosNecesarios())
                 + ", cantidadEsperada=" + to_string(obj.getCantidadEsperada())
                 + ", procedimiento='" + obj.getProcedimiento()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}