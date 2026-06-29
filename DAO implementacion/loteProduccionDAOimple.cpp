#include "loteProduccionDAOimple.h"
#include "../baseDatos/conexion.h"

lotesProduccionDAOImpl::lotesProduccionDAOImpl() {}

void lotesProduccionDAOImpl::insertar(LotesProduccion obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO lotesProduccion (fecha, idVariedad, cantidadProducida, idEmpleado, idReceta) VALUES ('"
                 + obj.getFecha() + "', "
                 + to_string(obj.getIdVariedad()) + ", "
                 + to_string(obj.getCantidadProducida()) + ", "
                 + to_string(obj.getIdEmpleado()) + ", "
                 + to_string(obj.getIdReceta()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void lotesProduccionDAOImpl::eliminar(LotesProduccion obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM lotesProduccion WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

LotesProduccion lotesProduccionDAOImpl::buscarPorId(int id) {
    LotesProduccion obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM lotesProduccion WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setFecha((char*)sqlite3_column_text(stmt, 1));
        obj.setIdVariedad(sqlite3_column_int(stmt, 2));
        obj.setCantidadProducida(sqlite3_column_double(stmt, 3));
        obj.setIdEmpleado(sqlite3_column_int(stmt, 4));
        obj.setIdReceta(sqlite3_column_int(stmt, 5));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<LotesProduccion> lotesProduccionDAOImpl::listar() {
    vector<LotesProduccion> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM lotesProduccion";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LotesProduccion obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setFecha((char*)sqlite3_column_text(stmt, 1));
        obj.setIdVariedad(sqlite3_column_int(stmt, 2));
        obj.setCantidadProducida(sqlite3_column_double(stmt, 3));
        obj.setIdEmpleado(sqlite3_column_int(stmt, 4));
        obj.setIdReceta(sqlite3_column_int(stmt, 5));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void lotesProduccionDAOImpl::actualizar(LotesProduccion obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE lotesProduccion SET fecha='" + obj.getFecha()
                 + "', idVariedad=" + to_string(obj.getIdVariedad())
                 + ", cantidadProducida=" + to_string(obj.getCantidadProducida())
                 + ", idEmpleado=" + to_string(obj.getIdEmpleado())
                 + ", idReceta=" + to_string(obj.getIdReceta())
                 + " WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}