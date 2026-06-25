# Medialunas Pro

Sistema de gestion de medialunas hecho en C++ con POO, SQLite3 y raylib.

## Estructura

- `models/`: clases del dominio, por ejemplo `Cliente`, `Pedidos`, `Insumos`, `StockVenta` y `variedad`.
- `DAO/`: clases DAO abstractas que heredan de `DAO<T>`.
- `DAO implementacion/`: clases que implementan insertar, actualizar, eliminar, buscar y listar usando SQLite.
- `baseDatos/`: conexion y base `medialunas.db`.
- `main.cpp`: punto de entrada del programa.
- `InterfazGrafica.h` y `InterfazGrafica.cpp`: interfaz grafica con raylib.

## Acceso

Usuario de prueba:

```text
admin
```

Contrasena:

```text
1234
```

## Modulos de la interfaz

- Clientes
- Pedidos
- Variedades
- Insumos
- Stock

Cada modulo permite listar registros, seleccionar una fila, crear un registro nuevo, guardar cambios y eliminar.

## Compilacion con CMake

Se necesita tener instalado:

- Un compilador C++ compatible con C++17.
- raylib.
- SQLite3.
- CMake.

Comandos:

```bash
cmake -S . -B build
cmake --build build
```

El ejecutable copia la carpeta `baseDatos` al directorio de salida para que la aplicacion encuentre `medialunas.db`.
