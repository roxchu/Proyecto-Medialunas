#include "UsuarioDAOImple.h"
#include "../baseDatos/conexion.h"

UsuarioDAOImpl::UsuarioDAOImpl() {}

void UsuarioDAOImpl::insertar(Usuario obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO usuario (DNI, nombre, apellido, direccion, telefono, email, contactoEmergencia, nombreCE, idCategoria) VALUES ("
                 + to_string(obj.getDNI()) + ", '"
                 + obj.getNombre() + "', '"
                 + obj.getApellido() + "', '"
                 + obj.getDireccion() + "', '"
                 + obj.getTelefono() + "', '"
                 + obj.getEmail() + "', '"
                 + obj.getContactoEmergencia() + "', '"
                 + obj.getNombreCE() + "', "
                 + to_string(obj.getIdCategoria()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void UsuarioDAOImpl::eliminar(Usuario obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM usuario WHERE DNI = " + to_string(obj.getDNI());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

Usuario UsuarioDAOImpl::buscarPorId(int dni) {
    Usuario obj;
    Conexion con("medialunas.db");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM usuario WHERE DNI = " + to_string(dni);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setDNI(sqlite3_column_int(stmt, 0));
        obj.setNombre((char*)sqlite3_column_text(stmt, 1));
        obj.setApellido((char*)sqlite3_column_text(stmt, 2));
        obj.setDireccion((char*)sqlite3_column_text(stmt, 3));
        obj.setTelefono((char*)sqlite3_column_text(stmt, 4));
        obj.setEmail((char*)sqlite3_column_text(stmt, 5));
        obj.setContactoEmergencia((char*)sqlite3_column_text(stmt, 6));
        obj.setNombreCE((char*)sqlite3_column_text(stmt, 7));
        obj.setIdCategoria(sqlite3_column_int(stmt, 8));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<Usuario> UsuarioDAOImpl::listar() {
    vector<Usuario> lista;
    Conexion con("medialunas.db");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM usuario";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Usuario obj;
        obj.setDNI(sqlite3_column_int(stmt, 0));
        obj.setNombre((char*)sqlite3_column_text(stmt, 1));
        obj.setApellido((char*)sqlite3_column_text(stmt, 2));
        obj.setDireccion((char*)sqlite3_column_text(stmt, 3));
        obj.setTelefono((char*)sqlite3_column_text(stmt, 4));
        obj.setEmail((char*)sqlite3_column_text(stmt, 5));
        obj.setContactoEmergencia((char*)sqlite3_column_text(stmt, 6));
        obj.setNombreCE((char*)sqlite3_column_text(stmt, 7));
        obj.setIdCategoria(sqlite3_column_int(stmt, 8));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void UsuarioDAOImpl::actualizar(Usuario obj) {
    Conexion con("medialunas.db");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE usuario SET nombre='" + obj.getNombre()
                 + "', apellido='" + obj.getApellido()
                 + "', direccion='" + obj.getDireccion()
                 + "', telefono='" + obj.getTelefono()
                 + "', email='" + obj.getEmail()
                 + "', contactoEmergencia='" + obj.getContactoEmergencia()
                 + "', nombreCE='" + obj.getNombreCE()
                 + "', idCategoria=" + to_string(obj.getIdCategoria())
                 + " WHERE DNI=" + to_string(obj.getDNI());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}