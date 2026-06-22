#include "pedidosDAOimple.h"
#include "../baseDatos/conexion.h"

PedidosDAOImpl::PedidosDAOImpl() {}

void PedidosDAOImpl::insertar(Pedidos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO pedidos (idCliente, fechaPedido, estado) VALUES ("
                 + to_string(obj.getIdCliente()) + ", '"
                 + obj.getFechaPedido() + "', '"
                 + obj.getEstado() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void PedidosDAOImpl::eliminar(Pedidos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM pedidos WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Pedidos PedidosDAOImpl::buscarPorId(int id) {
    Pedidos obj;
    Conexion con("medialunas.db");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM pedidos WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdCliente(sqlite3_column_int(stmt, 1));
        obj.setFechaPedido((char*)sqlite3_column_text(stmt, 2));
        obj.setEstado((char*)sqlite3_column_text(stmt, 3));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<Pedidos> PedidosDAOImpl::listar() {
    vector<Pedidos> lista;
    Conexion con("medialunas.db");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM pedidos";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Pedidos obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdCliente(sqlite3_column_int(stmt, 1));
        obj.setFechaPedido((char*)sqlite3_column_text(stmt, 2));
        obj.setEstado((char*)sqlite3_column_text(stmt, 3));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void PedidosDAOImpl::actualizar(Pedidos obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE pedidos SET idCliente=" + to_string(obj.getIdCliente())
                 + ", fechaPedido='" + obj.getFechaPedido()
                 + "', estado='" + obj.getEstado()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}