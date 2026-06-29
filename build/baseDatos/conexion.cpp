#include "conexion.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>

struct ConfigBD {
    string host = "127.0.0.1";
    string usuario = "root";
    string clave = "";
    string base = "medialunas_pro";
    unsigned int puerto = 3306;
};

static string trim(const string& texto) {
    size_t inicio = 0;
    while (inicio < texto.size() && isspace((unsigned char)texto[inicio])) inicio++;
    size_t fin = texto.size();
    while (fin > inicio && isspace((unsigned char)texto[fin - 1])) fin--;
    return texto.substr(inicio, fin - inicio);
}

static string minusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char c) { return (char)tolower(c); });
    return texto;
}

static string env(const char* nombre, const string& defecto) {
    const char* valor = getenv(nombre);
    return valor && *valor ? string(valor) : defecto;
}

static ConfigBD leerConfig() {
    ConfigBD config;
    config.host = env("MEDIALUNAS_DB_HOST", config.host);
    config.usuario = env("MEDIALUNAS_DB_USER", config.usuario);
    config.clave = env("MEDIALUNAS_DB_PASS", config.clave);
    config.base = env("MEDIALUNAS_DB_NAME", config.base);
    config.puerto = (unsigned int)atoi(env("MEDIALUNAS_DB_PORT", to_string(config.puerto)).c_str());

    vector<string> rutas = {"baseDatos/db_config.ini", "../baseDatos/db_config.ini"};
    for (const string& ruta : rutas) {
        ifstream archivo(ruta);
        if (!archivo.good()) continue;

        string linea;
        while (getline(archivo, linea)) {
            linea = trim(linea);
            if (linea.empty() || linea[0] == '#') continue;
            size_t igual = linea.find('=');
            if (igual == string::npos) continue;
            string clave = trim(linea.substr(0, igual));
            string valor = trim(linea.substr(igual + 1));
            if (clave == "host") config.host = valor;
            if (clave == "usuario") config.usuario = valor;
            if (clave == "clave") config.clave = valor;
            if (clave == "base") config.base = valor;
            if (clave == "puerto") config.puerto = (unsigned int)atoi(valor.c_str());
        }
        break;
    }

    return config;
}

static bool empiezaCon(const string& texto, const string& prefijo) {
    return texto.rfind(prefijo, 0) == 0;
}

static string reemplazarTodos(string texto, const string& buscar, const string& reemplazo) {
    size_t pos = 0;
    while ((pos = texto.find(buscar, pos)) != string::npos) {
        texto.replace(pos, buscar.size(), reemplazo);
        pos += reemplazo.size();
    }
    return texto;
}

static string traducirSql(string sql) {
    sql = reemplazarTodos(sql, "date('now')", "CURDATE()");
    sql = reemplazarTodos(sql, "INSERT OR IGNORE", "INSERT IGNORE");
    sql = reemplazarTodos(sql, "insert or ignore", "INSERT IGNORE");
    return sql;
}

static string escapar(sqlite3* db, const string& valor) {
    string salida;
    salida.resize(valor.size() * 2 + 1);
    unsigned long largo = mysql_real_escape_string(db->mysql, salida.data(), valor.c_str(), valor.size());
    salida.resize(largo);
    return "'" + salida + "'";
}

static string aplicarParametros(sqlite3_stmt* stmt) {
    string sql = traducirSql(stmt->sql);
    string armado;
    int parametro = 0;
    bool enComilla = false;

    for (size_t i = 0; i < sql.size(); i++) {
        char c = sql[i];
        if (c == '\'' && (i == 0 || sql[i - 1] != '\\')) enComilla = !enComilla;
        if (c == '?' && !enComilla && parametro < (int)stmt->parametros.size()) {
            if (parametro < (int)stmt->parametrosNull.size() && stmt->parametrosNull[parametro]) {
                armado += "NULL";
                parametro++;
            } else {
                armado += escapar(stmt->db, stmt->parametros[parametro++]);
            }
        } else {
            armado += c;
        }
    }

    return armado;
}

static vector<string> separarSentencias(const string& sql) {
    vector<string> sentencias;
    string actual;
    bool enComilla = false;

    for (size_t i = 0; i < sql.size(); i++) {
        char c = sql[i];
        if (c == '\'' && (i == 0 || sql[i - 1] != '\\')) enComilla = !enComilla;
        if (c == ';' && !enComilla) {
            string limpia = trim(actual);
            if (!limpia.empty()) sentencias.push_back(limpia);
            actual.clear();
        } else {
            actual += c;
        }
    }

    string limpia = trim(actual);
    if (!limpia.empty()) sentencias.push_back(limpia);
    return sentencias;
}

int sqlite3_open(const char*, sqlite3** db) {
    *db = new sqlite3{mysql_init(nullptr), ""};
    if (!(*db)->mysql) {
        (*db)->ultimoError = "No se pudo iniciar MariaDB";
        return SQLITE_ERROR;
    }

    ConfigBD config = leerConfig();
    mysql_options((*db)->mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    if (!mysql_real_connect((*db)->mysql, config.host.c_str(), config.usuario.c_str(), config.clave.c_str(),
                            config.base.c_str(), config.puerto, nullptr, 0)) {
        (*db)->ultimoError = mysql_error((*db)->mysql);
        return SQLITE_ERROR;
    }

    mysql_query((*db)->mysql, "SET NAMES utf8mb4");
    return SQLITE_OK;
}

int sqlite3_close(sqlite3* db) {
    if (!db) return SQLITE_OK;
    if (db->mysql) mysql_close(db->mysql);
    delete db;
    return SQLITE_OK;
}

int sqlite3_prepare_v2(sqlite3* db, const char* sql, int, sqlite3_stmt** stmt, const char**) {
    if (!db || !db->mysql || !sql) return SQLITE_ERROR;
    *stmt = new sqlite3_stmt{db, sql, {}, {}, {}, {}, 0, false, false, ""};
    return SQLITE_OK;
}

int sqlite3_bind_text(sqlite3_stmt* stmt, int posicion, const char* valor, int, void*) {
    if (!stmt || posicion <= 0) return SQLITE_ERROR;
    if ((int)stmt->parametros.size() < posicion) stmt->parametros.resize(posicion);
    if ((int)stmt->parametrosNull.size() < posicion) stmt->parametrosNull.resize(posicion, false);
    stmt->parametros[posicion - 1] = valor ? valor : "";
    stmt->parametrosNull[posicion - 1] = false;
    return SQLITE_OK;
}

int sqlite3_bind_null(sqlite3_stmt* stmt, int posicion) {
    if (!stmt || posicion <= 0) return SQLITE_ERROR;
    if ((int)stmt->parametros.size() < posicion) stmt->parametros.resize(posicion);
    if ((int)stmt->parametrosNull.size() < posicion) stmt->parametrosNull.resize(posicion, false);
    stmt->parametros[posicion - 1] = "";
    stmt->parametrosNull[posicion - 1] = true;
    return SQLITE_OK;
}

int sqlite3_step(sqlite3_stmt* stmt) {
    if (!stmt || !stmt->db || !stmt->db->mysql) return SQLITE_ERROR;

    if (!stmt->ejecutado) {
        string sql = aplicarParametros(stmt);
        string limpia = minusculas(trim(sql));
        stmt->consulta = empiezaCon(limpia, "select") || empiezaCon(limpia, "show");

        if (mysql_query(stmt->db->mysql, sql.c_str()) != 0) {
            stmt->error = mysql_error(stmt->db->mysql);
            stmt->db->ultimoError = stmt->error;
            return SQLITE_ERROR;
        }

        if (stmt->consulta) {
            MYSQL_RES* resultado = mysql_store_result(stmt->db->mysql);
            if (!resultado) {
                stmt->error = mysql_error(stmt->db->mysql);
                stmt->db->ultimoError = stmt->error;
                return SQLITE_ERROR;
            }

            int columnas = mysql_num_fields(resultado);
            MYSQL_ROW fila;
            unsigned long* largos;
            while ((fila = mysql_fetch_row(resultado))) {
                largos = mysql_fetch_lengths(resultado);
                vector<string> valores;
                for (int i = 0; i < columnas; i++) {
                    valores.push_back(fila[i] ? string(fila[i], largos[i]) : "");
                }
                stmt->filas.push_back(valores);
            }
            mysql_free_result(resultado);
        }

        stmt->ejecutado = true;
    }

    if (!stmt->consulta) return SQLITE_DONE;
    if (stmt->indiceFila >= stmt->filas.size()) return SQLITE_DONE;
    stmt->filaActual = stmt->filas[stmt->indiceFila++];
    return SQLITE_ROW;
}

int sqlite3_finalize(sqlite3_stmt* stmt) {
    delete stmt;
    return SQLITE_OK;
}

const unsigned char* sqlite3_column_text(sqlite3_stmt* stmt, int columna) {
    static const unsigned char vacio[] = "";
    if (!stmt || columna < 0 || columna >= (int)stmt->filaActual.size()) return vacio;
    return (const unsigned char*)stmt->filaActual[columna].c_str();
}

int sqlite3_column_int(sqlite3_stmt* stmt, int columna) {
    return atoi((const char*)sqlite3_column_text(stmt, columna));
}

double sqlite3_column_double(sqlite3_stmt* stmt, int columna) {
    return atof((const char*)sqlite3_column_text(stmt, columna));
}

int sqlite3_exec(sqlite3* db, const char* sql, int (*)(void*, int, char**, char**), void*, char** errmsg) {
    if (!db || !db->mysql || !sql) return SQLITE_ERROR;

    for (string sentencia : separarSentencias(traducirSql(sql))) {
        string limpia = minusculas(trim(sentencia));
        if (limpia.empty() || empiezaCon(limpia, "pragma ")) continue;

        if (mysql_query(db->mysql, sentencia.c_str()) != 0) {
            db->ultimoError = mysql_error(db->mysql);
            if (errmsg) *errmsg = (char*)db->ultimoError.c_str();
            return SQLITE_ERROR;
        }

        MYSQL_RES* resultado = mysql_store_result(db->mysql);
        if (resultado) mysql_free_result(resultado);
    }

    return SQLITE_OK;
}

const char* sqlite3_errmsg(sqlite3* db) {
    if (!db) return "Conexion no inicializada";
    if (!db->ultimoError.empty()) return db->ultimoError.c_str();
    return mysql_error(db->mysql);
}

Conexion::Conexion(string nombreBD) {
    this->nombreBD = nombreBD;
    db = nullptr;
}

bool Conexion::conectar() {
    int resultado = sqlite3_open(nombreBD.c_str(), &db);
    if (resultado != SQLITE_OK) {
        cout << "Error al conectar BD: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

sqlite3* Conexion::getDB() {
    return db;
}

void Conexion::cerrar() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}
