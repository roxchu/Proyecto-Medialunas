#include "hojaRutaDAOImple.h"
#include "../baseDatos/conexion.h"

hojaRutaDAOImpl::hojaRutaDAOImpl() {}

void hojaRutaDAOImpl::insertar(hojaRuta obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO hojaRuta (idVehiculo, idPedido, ordenEntrega, idEmpleado) VALUES ("
                 + to_string(obj.getIdVehiculo()) + ", "
                 + to_string(obj.getIdPedido()) + ", "
                 + to_string(obj.getOrdenEntrega()) + ", "
                 + to_string(obj.getIdEmpleado()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void hojaRutaDAOImpl::eliminar(hojaRuta obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM hojaRuta WHERE id = " + to_string(obj.getIdVehiculo());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

hojaRuta hojaRutaDAOImpl::buscarPorId(int id) {
    hojaRuta obj;
    Conexion con("medialunas.db");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM hojaRuta WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setIdVehiculo(sqlite3_column_int(stmt, 0));
        obj.setIdPedido(sqlite3_column_int(stmt, 1));
        obj.setOrdenEntrega(sqlite3_column_int(stmt, 2));
        obj.setIdEmpleado(sqlite3_column_int(stmt, 3));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<hojaRuta> hojaRutaDAOImpl::listar() {
    vector<hojaRuta> lista;
    Conexion con("medialunas.db");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM hojaRuta";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hojaRuta obj;
        obj.setIdVehiculo(sqlite3_column_int(stmt, 0));
        obj.setIdPedido(sqlite3_column_int(stmt, 1));
        obj.setOrdenEntrega(sqlite3_column_int(stmt, 2));
        obj.setIdEmpleado(sqlite3_column_int(stmt, 3));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void hojaRutaDAOImpl::actualizar(hojaRuta obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE hojaRuta SET idVehiculo=" + to_string(obj.getIdVehiculo())
                 + ", idPedido=" + to_string(obj.getIdPedido())
                 + ", ordenEntrega=" + to_string(obj.getOrdenEntrega())
                 + ", idEmpleado=" + to_string(obj.getIdEmpleado())
                 + " WHERE id=" + to_string(obj.getIdVehiculo());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}