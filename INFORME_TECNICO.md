# Informe tecnico - Medialunas Pro

## Objetivo del sistema

Medialunas Pro queda dividido en dos partes:

- Aplicacion de escritorio interna para la empresa.
- Web de pedidos para clientes mayoristas.

La aplicacion de escritorio se usa dentro de la empresa para administrar usuarios, produccion, logistica, pedidos, facturacion, stock e insumos. La web permite que el cliente cargue pedidos y luego consulte el estado, la facturacion y el detalle usando el ID de pedido.

## Requisitos para Linux

### Sistema base

Se necesita una distribucion Linux con herramientas de compilacion C++ y SQLite.

Paquetes recomendados en Debian, Ubuntu o Linux Mint:

```bash
sudo apt update
sudo apt install build-essential cmake sqlite3 libsqlite3-dev python3
```

Para compilar la app de escritorio tambien se necesita Raylib. Si Raylib ya esta instalado con soporte CMake, el proyecto compila con:

```bash
cmake --build build
```

Si no esta instalado, hay que instalar Raylib desde el gestor de paquetes de la distribucion o compilarlo desde fuente. En sistemas donde exista paquete:

```bash
sudo apt install libraylib-dev
```

Nota: el nombre del paquete puede variar segun la distribucion.

### Ejecutar la aplicacion de escritorio

Desde la raiz del proyecto:

```bash
./build/MedialunasPro
```

La aplicacion usa la base:

```text
baseDatos/medialunas.db
```

Se corrigio la conexion para que, aunque se ejecute desde la raiz o desde `build/`, busque la base correcta en `baseDatos/medialunas.db`.

### Ejecutar la web local de prueba

La version Python no necesita dependencias externas:

```bash
python3 web/server.py
```

Luego abrir:

```text
http://127.0.0.1:8080/
```

Esta web local lee y escribe la misma base SQLite que la aplicacion de escritorio:

```text
baseDatos/medialunas.db
```

### Ejecutar la version PHP

Se agrego una version PHP en:

```text
web_php/
```

Para probarla en Linux se necesita PHP con SQLite/PDO:

```bash
sudo apt install php php-cli php-sqlite3
```

Servidor local de prueba:

```bash
php -S 127.0.0.1:8081 -t web_php
```

Luego abrir:

```text
http://127.0.0.1:8081/
```

En esta maquina no se pudo probar PHP porque `php` no esta instalado, pero los archivos quedaron preparados para un entorno PHP con SQLite habilitado.

## Usuarios de prueba

La base incluye usuarios de prueba:

| Usuario | Contrasena | Rol | Sector |
| --- | --- | --- | --- |
| duenio | 1234 | Duenio | Admin |
| admin | 1234 | Admin | Admin |
| admin2 | 1234 | Admin | Admin |
| ventas | 1234 | Empleado | Ventas |
| produccion | 1234 | Empleado | Produccion |
| logistica | 1234 | Empleado | Logistica |

Roles usados:

- `1`: Duenio.
- `2`: Admin.
- `3`: Empleado.
- `4`: Cliente.

## Implementaciones agregadas

### 1. Cierre de sesion

En la aplicacion de escritorio el boton ahora dice `Cerrar sesion`. Antes decia `Salir`, lo que podia confundirse con cerrar el programa. Al presionarlo vuelve a la pantalla inicial y limpia los datos de la sesion.

Justificacion: el usuario necesita entender claramente que abandona su sesion, no necesariamente la aplicacion.

### 2. Permisos por rol y sector

Se agrego control de acceso basico:

- Duenio: ve todos los paneles.
- Admin: ve todos los paneles, puede administrar empleados, pero no asignar rol Duenio.
- Empleado: ve solo paneles de su sector.

Sectores actuales:

- Ventas.
- Produccion.
- Logistica.
- Admin.

Justificacion: evita que un empleado de logistica modifique informacion de produccion, o que ventas toque datos internos que no corresponden.

### 3. Flujo real de pedidos web

Se ajusto el modelo mental:

- El cliente no entra a la app de escritorio.
- El cliente carga el pedido desde la web.
- La app interna ve ese pedido en `Pedidos web`.
- El empleado actualiza solo el estado.
- La web permite consultar el estado y la factura con el ID de pedido.

Justificacion: separa correctamente el canal publico de clientes del sistema interno de la empresa.

### 4. Consulta web por ID de pedido

La web tiene una pantalla de consulta:

```text
/consulta
```

Tambien se agrego un formulario de consulta en la portada. El cliente ingresa su ID de pedido y ve:

- Estado.
- Cliente.
- Direccion.
- Fecha.
- Metodo de pago.
- Detalle de variedades.
- Cantidad en docenas.
- Precio por docena.
- Total.

Justificacion: el cliente necesita poder revisar su pedido sin llamar a la empresa.

### 5. Cantidades por docena

La web ahora pide `Cantidad de docenas`, no unidades sueltas.

Justificacion: el negocio se planteo como venta mayorista, por lo que la unidad comercial real es la docena.

### 6. Precio calculado por el sistema

El cliente ya no ingresa precio. La web toma el precio desde:

```text
stockVenta.precioUnitario
```

segun la variedad elegida.

Justificacion: el precio lo define la empresa. Permitir que el cliente escriba el precio rompe la facturacion y permite pedidos inconsistentes.

### 7. Facturacion

Se agrego una vista de facturacion que combina:

- Cliente.
- Pedido.
- Detalle del pedido.
- Variedad.
- Metodo de pago.

La factura muestra:

- Direccion.
- ID pedido.
- ID cliente.
- Fecha.
- ID variedad.
- Cantidad.
- Precio.
- Metodo de pago.

Justificacion: la facturacion necesita datos de varias tablas, no corresponde cargarla manualmente campo por campo.

### 8. Stock publico

El panel `Stock publico` se trata como panel de consulta. El stock se calcula segun produccion menos ventas registradas.

Justificacion: el stock visible al publico depende de movimientos internos y pedidos, no deberia cargarse arbitrariamente desde la web.

### 9. Alertas de minimo en insumos

Se agrego `stockMinimo` en insumos y lotes de insumos. Las filas bajo minimo se resaltan en rojo suave.

Justificacion: produccion necesita detectar rapidamente faltantes o niveles bajos.

### 10. Recetas con varios insumos

Se agrego la tabla intermedia:

```text
recetaInsumos
```

Campos:

- `idReceta`
- `idInsumo`
- `cantidad`

Justificacion: una receta puede requerir muchos insumos, y un insumo puede aparecer en muchas recetas. Esto es una relacion muchos-a-muchos.

### 11. Web Python local

Se creo:

```text
web/server.py
```

Sirve para probar rapido sin instalar frameworks. Usa `http.server`, `sqlite3` y HTML/CSS embebido.

Justificacion: permite validar el flujo web de pedidos sin depender todavia de hosting, Apache, Nginx o PHP.

### 12. Web PHP preparada para dominio/hosting

Se creo:

```text
web_php/config.php
web_php/index.php
web_php/pedido.php
web_php/consulta.php
web_php/styles.css
```

Justificacion: PHP + HTML + CSS es mas parecido al despliegue tradicional en hosting compartido con dominio.

## Sobre dominio y base de datos

Para una entrega local, escritorio y web pueden compartir:

```text
baseDatos/medialunas.db
```

Pero para un dominio real hay un punto importante: una pagina web publicada en internet no puede conectarse directamente a la SQLite que esta en la PC de la empresa, salvo que esa PC se exponga como servidor, lo cual no es recomendable para un proyecto basico.

Opciones posibles:

### Opcion A: Hosting con PHP y SQLite

Se sube `web_php/` y una copia de `medialunas.db` al hosting.

Ventaja:

- Simple.
- Barato.
- Compatible con muchos hostings.

Problema:

- La app de escritorio local no estaria usando automaticamente esa misma base si sigue en la PC de la empresa.

### Opcion B: Hosting/VPS con base central

La web y la app de escritorio se conectan a un servidor central.

Ventaja:

- Web y escritorio comparten datos reales.
- Mas escalable.

Problema:

- Requiere configurar servidor, seguridad y posiblemente migrar de SQLite a MySQL/PostgreSQL.

### Opcion C: API intermedia

La web habla con una API, y la app de escritorio tambien. La base queda centralizada.

Ventaja:

- Es la arquitectura mas ordenada.
- Permite controlar permisos, seguridad y validaciones.

Problema:

- Requiere mas desarrollo.

## Recomendacion

Para la materia/proyecto actual:

1. Mantener la app de escritorio con SQLite.
2. Usar la web local Python para demostrar integracion inmediata.
3. Presentar la version PHP como camino para dominio/hosting.
4. Explicar que para produccion real conviene servidor central o API.

Para una version real:

1. Comprar dominio.
2. Contratar hosting o VPS.
3. Migrar datos a una base central.
4. Hacer que la app de escritorio consuma esa base o una API.
5. Proteger credenciales, backups y acceso por roles.
