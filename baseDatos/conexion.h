#ifndef CONEXION_H
#define CONEXION_H

#include <mariadb/mysql.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_ROW 100
#define SQLITE_DONE 101
#define SQLITE_TRANSIENT nullptr

struct sqlite3 {
    MYSQL* mysql;
    string ultimoError;
};

struct sqlite3_stmt {
    sqlite3* db;
    string sql;
    vector<string> parametros;
    vector<bool> parametrosNull;
    vector<vector<string>> filas;
    vector<string> filaActual;
    size_t indiceFila;
    bool ejecutado;
    bool consulta;
    string error;
};

int sqlite3_open(const char* nombreBD, sqlite3** db);
int sqlite3_close(sqlite3* db);
int sqlite3_prepare_v2(sqlite3* db, const char* sql, int bytes, sqlite3_stmt** stmt, const char** tail);
int sqlite3_step(sqlite3_stmt* stmt);
int sqlite3_finalize(sqlite3_stmt* stmt);
int sqlite3_bind_text(sqlite3_stmt* stmt, int posicion, const char* valor, int bytes, void* destructor);
int sqlite3_bind_null(sqlite3_stmt* stmt, int posicion);
const unsigned char* sqlite3_column_text(sqlite3_stmt* stmt, int columna);
int sqlite3_column_int(sqlite3_stmt* stmt, int columna);
double sqlite3_column_double(sqlite3_stmt* stmt, int columna);
int sqlite3_exec(sqlite3* db, const char* sql, int (*callback)(void*, int, char**, char**), void* data, char** errmsg);
const char* sqlite3_errmsg(sqlite3* db);

class Conexion {
private:
    sqlite3 *db;
    string nombreBD;

public:
    Conexion(string nombreBD);
    bool conectar();
    sqlite3* getDB();
    void cerrar();
};

#endif
