#include "loteInsumosDAOImple.h"
#include "../baseDatos/conexion.h"

loteInsumosDAOImpl::loteInsumosDAOImpl() {}

void loteInsumosDAOImpl::insertar(loteInsumos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "INSERT INTO loteInsumos (idLoteProduccion, idInsumo, cantidad, unidadMedida, observaciones, costoUnitario) VALUES ("
                 + to_string(obj.getIdLoteProduccion()) + ", "
                 + to_string(obj.getIdInsumo()) + ", "
                 + to_string(obj.getCantidad()) + ", '"
                 + obj.getUnidadMedia() + "', '"
                 + obj.getObservaciones() + "', "
                 + to_string(obj.getCostoUnitario()) + ");";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

void loteInsumosDAOImpl::eliminar(loteInsumos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "DELETE FROM loteInsumos WHERE id = " + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}

loteInsumos loteInsumosDAOImpl::buscarPorId(int id) {
    loteInsumos obj;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return obj;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM loteInsumos WHERE id = " + to_string(id);
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdLoteProduccion(sqlite3_column_int(stmt, 1));
        obj.setIdInsumo(sqlite3_column_int(stmt, 2));
        obj.setCantidad(sqlite3_column_double(stmt, 3));
        obj.setUnidadMedia((char*)sqlite3_column_text(stmt, 4));
        obj.setObservaciones((char*)sqlite3_column_text(stmt, 5));
        obj.setCostoUnitario(sqlite3_column_double(stmt, 6));
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return obj;
}

vector<loteInsumos> loteInsumosDAOImpl::listar() {
    vector<loteInsumos> lista;
    Conexion con("medialunas_pro");
    if (!con.conectar()) return lista;
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM loteInsumos";
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        loteInsumos obj;
        obj.setId(sqlite3_column_int(stmt, 0));
        obj.setIdLoteProduccion(sqlite3_column_int(stmt, 1));
        obj.setIdInsumo(sqlite3_column_int(stmt, 2));
        obj.setCantidad(sqlite3_column_double(stmt, 3));
        obj.setUnidadMedia((char*)sqlite3_column_text(stmt, 4));
        obj.setObservaciones((char*)sqlite3_column_text(stmt, 5));
        obj.setCostoUnitario(sqlite3_column_double(stmt, 6));
        lista.push_back(obj);
    }
    sqlite3_finalize(stmt);
    con.cerrar();
    return lista;
}

void loteInsumosDAOImpl::actualizar(loteInsumos obj) {
    Conexion con("medialunas_pro");
    con.conectar();
    sqlite3_stmt *stmt;
    string sql = "UPDATE loteInsumos SET idLoteProduccion=" + to_string(obj.getIdLoteProduccion())
                 + ", idInsumo=" + to_string(obj.getIdInsumo())
                 + ", cantidad=" + to_string(obj.getCantidad())
                 + ", unidadMedida='" + obj.getUnidadMedia()
                 + "', observaciones='" + obj.getObservaciones()
                 + "', costoUnitario=" + to_string(obj.getCostoUnitario())
                 + " WHERE id=" + to_string(obj.getId());
    sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    con.cerrar();
}