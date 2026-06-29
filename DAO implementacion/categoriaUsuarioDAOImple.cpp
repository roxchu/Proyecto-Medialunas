#include "categoriaUsuarioDAOImple.h"
#include "../baseDatos/conexion.h"

categoriaUsuarioDAOImpl::categoriaUsuarioDAOImpl() {}

void categoriaUsuarioDAOImpl::insertar(categoriaUsuario cu) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO categoriaUsuario (nombreCategoria) VALUES ('"
                 + cu.getNombreCategoria() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void categoriaUsuarioDAOImpl::eliminar(categoriaUsuario cu) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM categoriaUsuario WHERE id = " + to_string(cu.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

categoriaUsuario categoriaUsuarioDAOImpl::buscarPorId(int id) {
    categoriaUsuario cu;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return cu;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM [TABLA] WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cu.setId(sqlite3_column_int(stmt, 0));
        cu.setNombreCategoria((char*)sqlite3_column_text(stmt, 1));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return cu;
}

vector<categoriaUsuario> categoriaUsuarioDAOImpl::listar() {
    vector<categoriaUsuario> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM categoriaUsuario";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        categoriaUsuario cu;
        cu.setId(sqlite3_column_int(stmt, 0));
        cu.setNombreCategoria((char*)sqlite3_column_text(stmt, 1));
        lista.push_back(cu);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void categoriaUsuarioDAOImpl::actualizar(categoriaUsuario cu) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE categoriaUsuario SET nombreCategoria='" + cu.getNombreCategoria()
             + "' WHERE id=" + to_string(cu.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}