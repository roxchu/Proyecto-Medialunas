# Medialunas Pro

Sistema de gestion de medialunas hecho en C++ con POO, raylib y base central MySQL/MariaDB.

## Estructura

- `models/`: clases del dominio, por ejemplo `Cliente`, `Pedidos`, `Insumos`, `StockVenta` y `variedad`.
- `DAO/`: clases DAO abstractas que heredan de `DAO<T>`.
- `DAO implementacion/`: clases que implementan insertar, actualizar, eliminar, buscar y listar.
- `baseDatos/`: conexion C++ a MySQL/MariaDB, esquema SQL y archivos de configuracion.
- `web_php/`: web de pedidos y consulta conectada a la misma base MySQL/MariaDB.
- `main.cpp`: punto de entrada del programa.
- `InterfazGrafica.h` y `InterfazGrafica.cpp`: interfaz grafica con raylib.

## Base de datos

El esquema principal esta en:

```text
baseDatos/medialunas_mysql.sql
```

La app de escritorio y la web PHP deben apuntar a la misma base para compartir clientes, pedidos, stock y facturacion.

## Configuracion local

Crear el archivo:

```text
baseDatos/db_config.ini
```

usando como modelo:

```text
baseDatos/db_config.example.ini
```

Ejemplo:

```ini
host=127.0.0.1
puerto=3306
base=medialunas_pro
usuario=root
clave=
```

La web PHP usa:

```text
web_php/database.local.php
```

tomando como modelo:

```text
web_php/database.example.php
```

## Acceso

Usuario de prueba:

```text
admin
```

Contrasena:

```text
1234
```

## Compilacion con CMake en Linux

Se necesita tener instalado:

- Un compilador C++ compatible con C++17.
- CMake.
- raylib.
- MySQL/MariaDB server o acceso a una base remota.
- Libreria cliente MariaDB/MySQL para C/C++ (`libmariadb-dev` en Debian/Ubuntu).

Paquetes orientativos:

```bash
sudo apt update
sudo apt install build-essential cmake mariadb-server libmariadb-dev pkg-config
```

Comandos:

```bash
cmake -S . -B build
cmake --build build
```

El ejecutable copia la carpeta `baseDatos` al directorio de salida para encontrar `db_config.ini` y el esquema SQL.

## Web PHP

Para probar la web local:

```bash
sudo apt install php php-cli php-mysql
php -S 127.0.0.1:8081 -t web_php
```

Luego abrir:

```text
http://127.0.0.1:8081/
```
