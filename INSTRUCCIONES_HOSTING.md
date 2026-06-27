# Medialunas Pro en hosting MySQL

## 1. Crear la base en el hosting

1. Entrar al panel del hosting.
2. Crear una base MySQL o MariaDB, por ejemplo `medialunas_pro`.
3. Crear un usuario de base de datos y asignarle permisos sobre esa base.
4. Anotar estos datos:
   - host
   - puerto, normalmente `3306`
   - nombre de la base
   - usuario
   - clave

## 2. Importar las tablas

1. Abrir phpMyAdmin o el importador SQL del hosting.
2. Seleccionar la base creada.
3. Importar el archivo `baseDatos/medialunas_mysql.sql`.
4. Si el hosting rechaza la parte de `CREATE VIEW`, volver a importar solo hasta los usuarios iniciales; la app y la web no dependen de esa vista para funcionar.

## 3. Configurar la web PHP

1. En el hosting, subir la carpeta `web_php`. En este proyecto quedo en `/medialunas/web_php`.
2. Crear dentro de `web_php` un archivo `database.local.php` usando `web_php/database.example.php` como modelo.
3. Completar los datos reales del hosting.
4. Probar `index.php` desde el navegador. Si la carpeta quedo como ahora, la URL termina en `/medialunas/web_php/index.php`.

## 4. Configurar la app C++

1. Crear `baseDatos/db_config.ini` usando `baseDatos/db_config.example.ini` como modelo.
2. Completar los mismos datos de la base del hosting.
3. Compilar:

```bash
cmake -S . -B build
cmake --build build
```

4. Ejecutar `build/MedialunasPro`.

Si la prueba de conexion devuelve `Access denied`, el host esta respondiendo pero la clave o los permisos del usuario MySQL no coinciden. Revisar que la clave pegada sea la del usuario de base de datos, no la clave del panel del hosting.

## 5. Comprobar que ambos miran la misma base

1. En la app C++, entrar con `admin` / `1234`.
2. Cargar al menos una variedad y un precio en stock publico.
3. Desde la web, crear un pedido.
4. Volver a la app y abrir `Pedidos web`, `Clientes web` o `Detalle pedidos`.
5. Si aparece el pedido nuevo, ya estan conectados al mismo host.
