-- Base de datos: Medialunas Pro
PRAGMA foreign_keys = ON;

-- Tablas independientes
CREATE TABLE IF NOT EXISTS categoriaUsuario (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombreCategoria TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS variedad (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombreVariedad TEXT NOT NULL,
    descripcion TEXT
);

CREATE TABLE IF NOT EXISTS insumos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    cantidadActual REAL,
    unidadMedida TEXT,
    proveedor TEXT,
    stockMinimo REAL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS vehiculos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    patente TEXT NOT NULL,
    modelo TEXT,
    estadoMantenimiento TEXT
);

CREATE TABLE IF NOT EXISTS receta (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombreProducto TEXT NOT NULL,
    insumosNecesarios INTEGER,
    cantidadEsperada REAL,
    procedimiento TEXT
);

-- Tablas que dependen de otras
CREATE TABLE IF NOT EXISTS usuario (
    DNI INTEGER PRIMARY KEY,
    nombre TEXT NOT NULL,
    apellido TEXT NOT NULL,
    direccion TEXT,
    telefono TEXT,
    email TEXT,
    contactoEmergencia TEXT,
    nombreCE TEXT,
    idCategoria INTEGER,
    sector TEXT DEFAULT 'Ventas',
    FOREIGN KEY (idCategoria) REFERENCES categoriaUsuario(id)
);

CREATE TABLE IF NOT EXISTS clientes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    CUIT TEXT,
    direccion TEXT,
    telefono TEXT,
    metodoPago TEXT,
    categoria TEXT
);

CREATE TABLE IF NOT EXISTS lotesProduccion (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    fecha TEXT,
    idVariedad INTEGER,
    cantidadProducida REAL,
    idEmpleado INTEGER,
    idReceta INTEGER,
    FOREIGN KEY (idVariedad) REFERENCES variedad(id),
    FOREIGN KEY (idEmpleado) REFERENCES usuario(DNI),
    FOREIGN KEY (idReceta) REFERENCES receta(id)
);

CREATE TABLE IF NOT EXISTS stockVenta (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idVariedad INTEGER,
    stockDisponible REAL,
    precioUnitario REAL,
    idLote INTEGER,
    FOREIGN KEY (idVariedad) REFERENCES variedad(id),
    FOREIGN KEY (idLote) REFERENCES lotesProduccion(id)
);

CREATE TABLE IF NOT EXISTS pedidos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idCliente INTEGER,
    fechaPedido TEXT,
    estado TEXT,
    FOREIGN KEY (idCliente) REFERENCES clientes(id)
);

CREATE TABLE IF NOT EXISTS hojaRuta (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idVehiculo INTEGER,
    idPedido INTEGER,
    ordenEntrega INTEGER,
    idEmpleado INTEGER,
    FOREIGN KEY (idVehiculo) REFERENCES vehiculos(id),
    FOREIGN KEY (idPedido) REFERENCES pedidos(id),
    FOREIGN KEY (idEmpleado) REFERENCES usuario(DNI)
);

CREATE TABLE IF NOT EXISTS loteInsumos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idLoteProduccion INTEGER,
    idInsumo INTEGER,
    cantidad REAL,
    unidadMedida TEXT,
    observaciones TEXT,
    costoUnitario REAL,
    stockMinimo REAL DEFAULT 0,
    FOREIGN KEY (idLoteProduccion) REFERENCES lotesProduccion(id),
    FOREIGN KEY (idInsumo) REFERENCES insumos(id)
);

CREATE TABLE IF NOT EXISTS recetaInsumos (
    idReceta INTEGER,
    idInsumo INTEGER,
    cantidad REAL,
    PRIMARY KEY (idReceta, idInsumo),
    FOREIGN KEY (idReceta) REFERENCES receta(id),
    FOREIGN KEY (idInsumo) REFERENCES insumos(id)
);

CREATE TABLE IF NOT EXISTS detallePedido (
    idPedido INTEGER,
    idVariedad INTEGER,
    cantidad REAL,
    precioUnitario REAL,
    PRIMARY KEY (idPedido, idVariedad),
    FOREIGN KEY (idPedido) REFERENCES pedidos(id),
    FOREIGN KEY (idVariedad) REFERENCES variedad(id)
);

CREATE VIEW IF NOT EXISTS facturacionWeb AS
SELECT
    c.direccion AS direccion,
    p.id AS idPedido,
    p.idCliente AS idCliente,
    p.fechaPedido AS fecha,
    d.idVariedad AS idVariedad,
    d.cantidad AS cantidad,
    d.precioUnitario AS precio,
    c.metodoPago AS metodoPago
FROM pedidos p
LEFT JOIN clientes c ON c.id = p.idCliente
LEFT JOIN detallePedido d ON d.idPedido = p.id;
