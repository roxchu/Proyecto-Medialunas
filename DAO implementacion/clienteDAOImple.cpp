#include "clienteDAOImple.h"
#include "../baseDatos/conexion.h"

ClienteDAOImpl::ClienteDAOImpl() {}

void ClienteDAOImpl::insertar(Cliente obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO clientes (nombre, CUIT, direccion, telefono, metodoPago, categoria) VALUES ('"
                 + obj.getNombre() + "', '"
                 + obj.getCUIT() + "', '"
                 + obj.getDireccion() + "', '"
                 + obj.getTelefono() + "', '"
                 + obj.getMetodoPago() + "', '"
                 + obj.getCategoria() + "');";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void ClienteDAOImpl::eliminar(Cliente obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM clientes WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Cliente ClienteDAOImpl::buscarPorId(int id) {
    Cliente c;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return c;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM clientes WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        c.setId(sqlite3_column_int(stmt, 0));
        c.setNombre((char*)sqlite3_column_text(stmt, 1));
        c.setCUIT((char*)sqlite3_column_text(stmt, 2));
        c.setDireccion((char*)sqlite3_column_text(stmt, 3));
        c.setTelefono((char*)sqlite3_column_text(stmt, 4));
        c.setMetodoPago((char*)sqlite3_column_text(stmt, 5));
        c.setCategoria((char*)sqlite3_column_text(stmt, 6));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return c;
}

vector<Cliente> ClienteDAOImpl::listar() {
    vector<Cliente> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM clientes";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cliente c;
        c.setId(sqlite3_column_int(stmt, 0));
        c.setNombre((char*)sqlite3_column_text(stmt, 1));
        c.setCUIT((char*)sqlite3_column_text(stmt, 2));
        c.setDireccion((char*)sqlite3_column_text(stmt, 3));
        c.setTelefono((char*)sqlite3_column_text(stmt, 4));
        c.setMetodoPago((char*)sqlite3_column_text(stmt, 5));
        c.setCategoria((char*)sqlite3_column_text(stmt, 6));
        lista.push_back(c);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void ClienteDAOImpl::actualizar(Cliente obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE clientes SET nombre='" + obj.getNombre()
                 + "', CUIT='" + obj.getCUIT()
                 + "', direccion='" + obj.getDireccion()
                 + "', telefono='" + obj.getTelefono()
                 + "', metodoPago='" + obj.getMetodoPago()
                 + "', categoria='" + obj.getCategoria()
                 + "' WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}