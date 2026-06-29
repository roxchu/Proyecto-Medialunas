#include "detallePedidosDAOImple.h"
#include "../baseDatos/conexion.h"

DetallesPedidosDAOImpl::DetallesPedidosDAOImpl() {}

void DetallesPedidosDAOImpl::insertar(DetallesPedidos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO detallePedido (idPedido, idVariedad, cantidad, precioUnitario) VALUES ("
                 + to_string(obj.getIdPedido()) + ", "
                 + to_string(obj.getIdVariedad()) + ", "
                 + to_string(obj.getCantidad()) + ", "
                 + to_string(obj.getPrecioUnitario()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void DetallesPedidosDAOImpl::eliminar(DetallesPedidos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM detallePedido WHERE idPedido = " + to_string(obj.getIdPedido());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

DetallesPedidos DetallesPedidosDAOImpl::buscarPorId(int id) {
    DetallesPedidos obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM detallePedido WHERE idPedido = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setIdPedido(sqlite3_column_int(stmt, 0));
        obj.setIdVariedad(sqlite3_column_int(stmt, 1));
        obj.setCantidad(sqlite3_column_double(stmt, 2));
        obj.setPrecioUnitario(sqlite3_column_double(stmt, 3));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<DetallesPedidos> DetallesPedidosDAOImpl::listar() {
    vector<DetallesPedidos> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM detallePedido";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        DetallesPedidos obj;
        obj.setIdPedido(sqlite3_column_int(stmt, 0));
        obj.setIdVariedad(sqlite3_column_int(stmt, 1));
        obj.setCantidad(sqlite3_column_double(stmt, 2));
        obj.setPrecioUnitario(sqlite3_column_double(stmt, 3));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void DetallesPedidosDAOImpl::actualizar(DetallesPedidos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE detallePedido SET idVariedad=" + to_string(obj.getIdVariedad())
                 + ", cantidad=" + to_string(obj.getCantidad())
                 + ", precioUnitario=" + to_string(obj.getPrecioUnitario())
                 + " WHERE idPedido=" + to_string(obj.getIdPedido());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}