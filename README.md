# Medialunas Pro

Sistema de gestión de medialunas hecho en C++ con POO y raylib para la interfaz gráfica, con una base de datos central MySQL/MariaDB compartida con una web PHP de pedidos para clientes mayoristas.

## Estructura del proyecto

- `models/`: clases del dominio (`Cliente`, `Pedidos`, `Insumos`, `StockVenta`, `Variedad`, `Usuario`, etc.).
- `DAO/`: clases DAO abstractas que heredan de `DAO<T>`.
- `DAO implementacion/`: clases que implementan insertar, actualizar, eliminar, buscar y listar contra MySQL/MariaDB.
- `baseDatos/`: conexión C++ a MySQL/MariaDB, esquema SQL y archivos de configuración.
- `web_php/`: web de pedidos y consulta, conectada a la misma base.
- `InterfazGrafica.h` / `InterfazGrafica.cpp`: interfaz gráfica con raylib (paneles, login, tablas CRUD).
- `main.cpp`: punto de entrada del programa.

## Requisitos del programa

- Sistema Linux (probado en Debian/Ubuntu/Linux Mint).
- Compilador C++ compatible con C++17.
- CMake.
- Servidor MySQL/MariaDB propio, o acceso a una base remota (host, puerto, usuario y clave).
- Acceso a PHP (CLI y módulo `pdo_mysql`) si también se va a correr la web de pedidos.

## Librerías necesarias

| Librería | Uso | Paquete en Debian/Ubuntu |
| --- | --- | --- |
| raylib | Interfaz gráfica de la app de escritorio | `libraylib-dev` (el nombre puede variar según distro) |
| MariaDB/MySQL Connector C (`mariadb/mysql.h`) | Conexión de la app C++ a la base de datos (`mysql_init`, `mysql_real_connect`, `mysql_query`, etc.) | `libmariadb-dev` |
| MariaDB/MySQL server | Motor de base de datos (local o remoto) | `mariadb-server` |
| PHP CLI + PDO MySQL | Ejecutar y probar la web de pedidos | `php php-cli php-mysql` |
| CMake / build-essential | Compilar el proyecto | `cmake build-essential pkg-config` |

Instalación orientativa de todo lo anterior:

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config mariadb-server libmariadb-dev php php-cli php-mysql
sudo apt install libraylib-dev
```

## Base de datos

El esquema principal está en:

```text
baseDatos/medialunas_mysql.sql
```

La app de escritorio y la web PHP apuntan a la misma base para compartir clientes, pedidos, stock y facturación.

### Configuración de la app C++

Crear `baseDatos/db_config.ini` usando como modelo `baseDatos/db_config.example.ini`:

```ini
host=127.0.0.1
puerto=3306
base=medialunas_pro
usuario=root
clave=
```

### Configuración de la web PHP

La conexión a la base de datos se configura exclusivamente mediante variables de entorno, definidas en el panel del hosting donde se publique la web:

```text
MEDIALUNAS_DB_HOST
MEDIALUNAS_DB_PORT
MEDIALUNAS_DB_NAME
MEDIALUNAS_DB_USER
MEDIALUNAS_DB_PASS
```

## Compilación

```bash
cmake -S . -B build
cmake --build build
./build/MedialunasPro
```

El ejecutable copia la carpeta `baseDatos` al directorio de salida para encontrar `db_config.ini` y el esquema SQL.

## Acceso y roles

El login de la app de escritorio devuelve una categoría de usuario y, si corresponde, un sector:

| Categoría | Rol | Alcance |
| --- | --- | --- |
| 1 | Dueño | Acceso total a todos los paneles, sin restricciones de sector |
| 2 | Admin | Acceso total a todos los paneles, sin restricciones de sector |
| 3 | Empleado | Solo ve los paneles de su sector (`Ventas`, `Produccion` o `Logistica`) más los paneles marcados como `Comun` |

Usuarios de prueba:

| Usuario | Contraseña | Categoría | Sector |
| --- | --- | --- | --- |
| duenio | 1234 | Dueño | Admin |
| admin | 1234 | Admin | Admin |
| admin2 | 1234 | Admin | Admin |
| ventas | 1234 | Empleado | Ventas |
| produccion | 1234 | Empleado | Produccion |
| logistica | 1234 | Empleado | Logistica |

## Paneles del sistema y acciones CRUD

Cada panel corresponde a una tabla de la base de datos. El acceso a cada panel y las acciones disponibles (crear, leer, actualizar, eliminar) dependen del rol y, para empleados, del sector asignado. Dueño y Admin tienen acceso total salvo en los paneles marcados explícitamente como solo lectura.

### Clientes web (`clientes`) — sector Ventas

Campos: ID, Nombre, CUIT, Dirección, Teléfono, Método de pago, Categoría.

- **Leer:** sí, todos los roles con acceso al panel.
- **Crear / Actualizar / Eliminar:** no disponible desde la app. Los clientes se generan automáticamente desde la web de pedidos; este panel es **solo lectura**.

### Pedidos web (`pedidos`) — sector Comun (visible para todos los sectores)

Campos: ID pedido, ID cliente, Fecha, Estado (Pendiente/Entregado).

- **Leer:** sí, todos los roles.
- **Crear:** no disponible desde la app (los pedidos se generan desde la web de pedidos).
- **Actualizar:** Dueño, Admin, y empleados de sector Ventas o Logística (por ejemplo, para cambiar el estado a "Entregado").
- **Eliminar:** no disponible desde ningún rol.

### Detalle pedidos (`detallePedido`) — sector Ventas

Campos: ID pedido, ID variedad, Cantidad (docenas), Precio x unidad.

- **Leer:** Dueño, Admin, empleados de Ventas.
- **Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Ventas.

### Facturación (`facturacion`) — sector Ventas

Campos: Dirección, ID pedido, ID cliente, Fecha, ID variedad, Cantidad, Precio x unidad, Método de pago.

- **Leer:** Dueño, Admin, empleados de Ventas. Panel **solo lectura**: no admite crear, actualizar ni eliminar desde la app (se completa a partir de pedidos y detalle de pedidos).

### Variedades (`variedad`) — sector Producción

Campos: ID, Nombre, Descripción.

- **Leer:** Dueño, Admin, empleados de Producción.
- **Crear / Actualizar:** solo Dueño y Admin.
- **Eliminar:** solo Dueño y Admin.

### Stock público (`stockVenta`) — sector Ventas

Campos: ID variedad, Variedad, Stock disponible (unidades), Precio x docena.

- **Leer:** Dueño, Admin, empleados de Ventas.
- **Crear / Actualizar:** Dueño, Admin, y también empleados de Producción (quienes generan el stock).
- **Eliminar:** solo Dueño y Admin.

### Insumos (`insumos`) — sector Producción

Campos: ID, Nombre, Cantidad actual, Unidad de medida, Proveedor, Stock mínimo.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Producción.

### Detalles de producción (`loteInsumos`) — sector Producción

Campos: ID, ID lote de producción, ID receta, Costo unitario, Observaciones.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Producción.

### Recetas (`receta`) — sector Producción

Campos: ID, Producto, Cantidad esperada (unidades), Procedimiento.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Producción.

### Insumos por receta (`recetaInsumos`) — sector Producción

Campos: ID receta, ID insumo, Cantidad requerida.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Producción.

### Lotes de producción (`lotesProduccion`) — sector Producción

Campos: ID, Fecha, ID variedad, Cantidad producida (unidades), ID empleado, ID receta.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Producción.

### Vehículos (`vehiculos`) — sector Logística

Campos: ID, Patente, Modelo, Estado de mantenimiento.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Logística.

### Hoja de ruta (`hojaRuta`) — sector Logística

Campos: ID, ID vehículo, ID pedido, Orden de entrega, ID empleado.

- **Leer / Crear / Actualizar / Eliminar:** Dueño, Admin, empleados de Logística.

### Usuarios (`usuario`) — solo Admin/Dueño

Campos: DNI, Nombre, Apellido, Dirección, Teléfono, Email, Categoría (1=Dueño, 2=Admin, 3=Empleado), Sector (Ventas/Producción/Logística), Usuario, Contraseña.

- **Leer / Crear / Actualizar / Eliminar:** solo Dueño y Admin. No visible ni accesible para empleados.

### Categorías usuario (`categoriaUsuario`) — solo Admin/Dueño

Campos: ID, Nombre de categoría.

- **Leer / Crear / Actualizar / Eliminar:** solo Dueño y Admin.

## Resumen de permisos por sector (empleados)

| Sector del empleado | Paneles visibles |
| --- | --- |
| Ventas | Clientes web (lectura), Pedidos web (lectura/actualización), Detalle pedidos (CRUD completo), Facturación (lectura), Stock público (lectura/creación/actualización) |
| Producción | Variedades (lectura), Stock público (lectura/creación/actualización), Insumos (CRUD completo), Detalles de producción (CRUD completo), Recetas (CRUD completo), Insumos por receta (CRUD completo), Lotes de producción (CRUD completo) |
| Logística | Pedidos web (lectura/actualización), Vehículos (CRUD completo), Hoja de ruta (CRUD completo) |

Dueño y Admin ven y administran todos los paneles, incluidos Usuarios y Categorías de usuario, que están vedados para empleados de cualquier sector.

## Web de pedidos (clientes mayoristas)

La web está publicada en:

```text
https://nt.cavernservices.org/medialunas/web_php
```

La web PHP (`web_php/`) permite que el cliente cargue un pedido (`index.php`/`pedido.php`) y luego lo consulte por su ID (`consulta.php`), generando filas en `clientes`, `pedidos` y `detallePedido` que después aparecen en los paneles de la app de escritorio.

## Comprobación de integración

1. Entrar en la app C++ con `admin` / `1234`.
2. Cargar al menos una variedad y un precio en stock público.
3. Abrir la web PHP y crear un pedido.
4. Volver a la app y abrir `Pedidos web`, `Clientes web` o `Detalle pedidos`.
5. Si el pedido aparece, escritorio y web están usando la misma base MySQL/MariaDB.