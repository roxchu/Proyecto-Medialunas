# Informe tecnico - Medialunas Pro

## Objetivo del sistema

Medialunas Pro queda dividido en dos partes:

- Aplicacion de escritorio interna para la empresa.
- Web PHP de pedidos para clientes mayoristas.

Ambas partes trabajan contra una base central MySQL/MariaDB. La aplicacion de escritorio administra usuarios, produccion, logistica, pedidos, facturacion, stock e insumos. La web permite que el cliente cargue pedidos y luego consulte estado, facturacion y detalle usando el ID de pedido.

## Justificacion de migracion a MySQL/MariaDB

SQLite es muy practico para una aplicacion local porque guarda todo en un archivo y casi no requiere administracion. El problema aparece cuando el sistema deja de ser solo local: una web publicada y una app de escritorio necesitan compartir la misma informacion en tiempo real.

MySQL/MariaDB conviene en este proyecto porque:

- Centraliza los datos en un servidor accesible por la app C++ y por la web PHP.
- Permite varios usuarios y procesos conectados al mismo tiempo.
- Encaja mejor con hosting tradicional, phpMyAdmin, backups y usuarios con permisos.
- Evita copiar archivos `.db` entre la PC de la empresa y el hosting.
- Permite que los pedidos cargados desde la web aparezcan en la app interna sin sincronizaciones manuales.

Lecturas utiles para justificar el cambio:

- SQLite - Appropriate Uses For SQLite: https://www.sqlite.org/whentouse.html
- MySQL 8.4 C API Developer Guide: https://dev.mysql.com/doc/c-api/8.4/en/
- MySQL C API Basic Interface: https://dev.mysql.com/doc/c-api/8.4/en/c-api-basic-interface-usage.html
- PHP PDO MySQL: https://www.php.net/manual/en/ref.pdo-mysql.php

## C++ con MySQL en Linux

Si, C++ funciona con MySQL/MariaDB en Linux. En este proyecto se usa la libreria cliente MariaDB/MySQL mediante:

```cpp
#include <mariadb/mysql.h>
```

La conexion real esta en:

```text
baseDatos/conexion.cpp
baseDatos/conexion.h
```

El proyecto mantiene nombres compatibles con `sqlite3_*` para no reescribir todos los DAO de golpe, pero internamente llama a funciones MySQL/MariaDB como `mysql_init`, `mysql_real_connect`, `mysql_query`, `mysql_store_result`, `mysql_fetch_row` y `mysql_close`.

## Requisitos para Linux

Paquetes recomendados en Debian, Ubuntu o Linux Mint:

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config mariadb-server libmariadb-dev php php-cli php-mysql
```

Tambien se necesita raylib. En sistemas donde exista paquete:

```bash
sudo apt install libraylib-dev
```

Nota: el nombre del paquete puede variar segun la distribucion.

## Crear e importar la base

1. Iniciar MySQL/MariaDB.
2. Crear la base:

```sql
CREATE DATABASE medialunas_pro CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

3. Importar el esquema:

```bash
mysql -u root -p medialunas_pro < baseDatos/medialunas_mysql.sql
```

El archivo principal de esquema es:

```text
baseDatos/medialunas_mysql.sql
```

## Configurar la aplicacion C++

Crear:

```text
baseDatos/db_config.ini
```

tomando como base:

```text
baseDatos/db_config.example.ini
```

Ejemplo local:

```ini
host=127.0.0.1
puerto=3306
base=medialunas_pro
usuario=root
clave=
```

Tambien se pueden usar variables de entorno:

```text
MEDIALUNAS_DB_HOST
MEDIALUNAS_DB_PORT
MEDIALUNAS_DB_NAME
MEDIALUNAS_DB_USER
MEDIALUNAS_DB_PASS
```

## Compilar y ejecutar la app C++

```bash
cmake -S . -B build
cmake --build build
./build/MedialunasPro
```

Usuarios de prueba:

| Usuario | Contrasena | Rol | Sector |
| --- | --- | --- | --- |
| duenio | 1234 | Duenio | Admin |
| admin | 1234 | Admin | Admin |
| admin2 | 1234 | Admin | Admin |
| ventas | 1234 | Empleado | Ventas |
| produccion | 1234 | Empleado | Produccion |
| logistica | 1234 | Empleado | Logistica |

## Configurar la web PHP

Crear:

```text
web_php/database.local.php
```

tomando como base:

```text
web_php/database.example.php
```

Ejemplo:

```php
<?php
return [
    'host' => '127.0.0.1',
    'port' => '3306',
    'name' => 'medialunas_pro',
    'user' => 'root',
    'pass' => '',
];
```

Servidor local de prueba:

```bash
php -S 127.0.0.1:8081 -t web_php
```

Luego abrir:

```text
http://127.0.0.1:8081/
```

## Comprobacion de integracion

1. Entrar en la app C++ con `admin` / `1234`.
2. Cargar al menos una variedad y un precio en stock publico.
3. Abrir la web PHP.
4. Crear un pedido.
5. Volver a la app y abrir `Pedidos web`, `Clientes web` o `Detalle pedidos`.
6. Si el pedido aparece, escritorio y web estan usando la misma base MySQL/MariaDB.

## Paso a paso de migracion

1. Instalar MySQL/MariaDB y la libreria cliente de desarrollo.
2. Crear la base `medialunas_pro`.
3. Importar `baseDatos/medialunas_mysql.sql`.
4. Configurar `baseDatos/db_config.ini` para la app C++.
5. Configurar `web_php/database.local.php` para la web.
6. Compilar con CMake.
7. Probar login en la app C++.
8. Probar alta de pedido desde la web PHP.
9. Verificar que el pedido aparezca en la app C++.
10. Eliminar el uso operativo de SQLite y mantener MySQL/MariaDB como fuente unica de datos.

## Archivos principales

- `baseDatos/conexion.cpp`: conexion C++ a MySQL/MariaDB.
- `baseDatos/conexion.h`: definiciones usadas por los DAO.
- `baseDatos/medialunas_mysql.sql`: esquema e inserts iniciales.
- `baseDatos/db_config.example.ini`: modelo de configuracion para C++.
- `web_php/config.php`: conexion PHP con PDO MySQL.
- `web_php/database.example.php`: modelo de configuracion para PHP.
- `INSTRUCCIONES_HOSTING.md`: guia corta para hosting.

## Pendientes recomendados

- Renombrar progresivamente las funciones `sqlite3_*` de compatibilidad a nombres propios del proyecto, por ejemplo `db_prepare`, `db_step` y `db_column_text`.
- Migrar cualquier dato real que exista en archivos `.db` antiguos hacia MySQL.
- Crear un usuario MySQL con permisos limitados en vez de usar `root`.
- Agregar backups periodicos de la base.
- Revisar consultas armadas por concatenacion y reemplazarlas por prepared statements reales.
