#include "stockVentaDAOimple.h"
#include "../baseDatos/conexion.h"

stockVentaDAOImpl::stockVentaDAOImpl() {}

void stockVentaDAOImpl::insertar(StockVenta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO stockVenta (idVariedad, stockDisponible, precioUnitario, idLote) VALUES ("
                 + to_string(obj.getIdVariedad()) + ", "
                 + to_string(obj.getStockDisponible()) + ", "
                 + to_string(obj.getPrecioUnitario()) + ", "
                 + to_string(obj.getIdLote()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void stockVentaDAOImpl::eliminar(StockVenta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM stockVenta WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

StockVenta stockVentaDAOImpl::buscarPorId(int id) {
    StockVenta obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM stockVenta WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdVariedad(sqlite3_column_int(stmt, 1));
        obj.setStockDisponible(sqlite3_column_double(stmt, 2));
        obj.setPrecioUnitario(sqlite3_column_double(stmt, 3));
        obj.setIdLote(sqlite3_column_int(stmt, 4));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<StockVenta> stockVentaDAOImpl::listar() {
    vector<StockVenta> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM stockVenta";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        StockVenta obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdVariedad(sqlite3_column_int(stmt, 1));
        obj.setStockDisponible(sqlite3_column_double(stmt, 2));
        obj.setPrecioUnitario(sqlite3_column_double(stmt, 3));
        obj.setIdLote(sqlite3_column_int(stmt, 4));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void stockVentaDAOImpl::actualizar(StockVenta obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE stockVenta SET idVariedad=" + to_string(obj.getIdVariedad())
                 + ", stockDisponible=" + to_string(obj.getStockDisponible())
                 + ", precioUnitario=" + to_string(obj.getPrecioUnitario())
                 + ", idLote=" + to_string(obj.getIdLote())
                 + " WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}