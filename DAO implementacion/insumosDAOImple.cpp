#include "insumosDAOImple.h"
#include "../baseDatos/conexion.h"

InsumosDAOImpl::InsumosDAOImpl() {}

void InsumosDAOImpl::insertar(Insumos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO insumos (nombre, cantidadActual, unidadMedida, proveedor) VALUES ('"
                 + obj.getNombre() + "', "
                 + to_string(obj.getCantidadActual()) + ", '"
                 + obj.getUnidadMedia() + "', '"
                 + obj.getProveedor() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void InsumosDAOImpl::eliminar(Insumos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM insumos WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Insumos InsumosDAOImpl::buscarPorId(int id) {
    Insumos obj;
    Conexion con("medialunas.db");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM insumos WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombre((char*)sqlite3_column_text(stmt, 1));
        obj.setCantidadActual(sqlite3_column_double(stmt, 2));
        obj.setUnidadMedia((char*)sqlite3_column_text(stmt, 3));
        obj.setProveedor((char*)sqlite3_column_text(stmt, 4));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<Insumos> InsumosDAOImpl::listar() {
    vector<Insumos> lista;
    Conexion con("medialunas.db");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM insumos";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Insumos obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setNombre((char*)sqlite3_column_text(stmt, 1));
        obj.setCantidadActual(sqlite3_column_double(stmt, 2));
        obj.setUnidadMedia((char*)sqlite3_column_text(stmt, 3));
        obj.setProveedor((char*)sqlite3_column_text(stmt, 4));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void InsumosDAOImpl::actualizar(Insumos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE insumos SET nombre='" + obj.getNombre()
                 + "', cantidadActual=" + to_string(obj.getCantidadActual())
                 + ", unidadMedida='" + obj.getUnidadMedia()
                 + "', proveedor='" + obj.getProveedor()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}