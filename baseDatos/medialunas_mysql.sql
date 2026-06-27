CREATE TABLE IF NOT EXISTS categoriaUsuario (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombreCategoria VARCHAR(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS variedad (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombreVariedad VARCHAR(120) NOT NULL,
    descripcion TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS insumos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(120) NOT NULL,
    cantidadActual DECIMAL(12,2),
    unidadMedida VARCHAR(40),
    proveedor VARCHAR(120),
    stockMinimo DECIMAL(12,2) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS vehiculos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    patente VARCHAR(30) NOT NULL,
    modelo VARCHAR(120),
    estadoMantenimiento VARCHAR(120)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS receta (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombreProducto VARCHAR(120) NOT NULL,
    insumosNecesarios INT,
    cantidadEsperada DECIMAL(12,2),
    procedimiento TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS usuario (
    DNI INT PRIMARY KEY,
    nombre VARCHAR(120) NOT NULL,
    apellido VARCHAR(120) NOT NULL,
    direccion VARCHAR(180),
    telefono VARCHAR(80),
    email VARCHAR(150),
    contactoEmergencia VARCHAR(80),
    nombreCE VARCHAR(120),
    idCategoria INT,
    sector VARCHAR(80) DEFAULT 'Ventas',
    cuenta VARCHAR(80),
    clave VARCHAR(120),
    CONSTRAINT fk_usuario_categoria FOREIGN KEY (idCategoria) REFERENCES categoriaUsuario(id),
    UNIQUE KEY idx_usuario_cuenta (cuenta)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS clientes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(120) NOT NULL,
    CUIT VARCHAR(30),
    direccion VARCHAR(180),
    telefono VARCHAR(80),
    metodoPago VARCHAR(80),
    categoria VARCHAR(80)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS lotesProduccion (
    id INT AUTO_INCREMENT PRIMARY KEY,
    fecha DATE,
    idVariedad INT,
    cantidadProducida DECIMAL(12,2),
    idEmpleado INT,
    idReceta INT,
    CONSTRAINT fk_lotes_variedad FOREIGN KEY (idVariedad) REFERENCES variedad(id),
    CONSTRAINT fk_lotes_empleado FOREIGN KEY (idEmpleado) REFERENCES usuario(DNI),
    CONSTRAINT fk_lotes_receta FOREIGN KEY (idReceta) REFERENCES receta(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS stockVenta (
    id INT AUTO_INCREMENT PRIMARY KEY,
    idVariedad INT,
    stockDisponible DECIMAL(12,2),
    precioUnitario DECIMAL(12,2),
    idLote INT,
    CONSTRAINT fk_stock_variedad FOREIGN KEY (idVariedad) REFERENCES variedad(id),
    CONSTRAINT fk_stock_lote FOREIGN KEY (idLote) REFERENCES lotesProduccion(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS pedidos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    idCliente INT,
    fechaPedido DATE,
    estado VARCHAR(80),
    CONSTRAINT fk_pedidos_cliente FOREIGN KEY (idCliente) REFERENCES clientes(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS hojaRuta (
    id INT AUTO_INCREMENT PRIMARY KEY,
    idVehiculo INT,
    idPedido INT,
    ordenEntrega INT,
    idEmpleado INT,
    CONSTRAINT fk_hoja_vehiculo FOREIGN KEY (idVehiculo) REFERENCES vehiculos(id),
    CONSTRAINT fk_hoja_pedido FOREIGN KEY (idPedido) REFERENCES pedidos(id),
    CONSTRAINT fk_hoja_empleado FOREIGN KEY (idEmpleado) REFERENCES usuario(DNI)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS loteInsumos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    idLoteProduccion INT,
    idInsumo INT,
    cantidad DECIMAL(12,2),
    unidadMedida VARCHAR(40),
    observaciones TEXT,
    costoUnitario DECIMAL(12,2),
    stockMinimo DECIMAL(12,2) DEFAULT 0,
    CONSTRAINT fk_lote_insumos_lote FOREIGN KEY (idLoteProduccion) REFERENCES lotesProduccion(id),
    CONSTRAINT fk_lote_insumos_insumo FOREIGN KEY (idInsumo) REFERENCES insumos(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS recetaInsumos (
    idReceta INT,
    idInsumo INT,
    cantidad DECIMAL(12,2),
    PRIMARY KEY (idReceta, idInsumo),
    CONSTRAINT fk_receta_insumos_receta FOREIGN KEY (idReceta) REFERENCES receta(id),
    CONSTRAINT fk_receta_insumos_insumo FOREIGN KEY (idInsumo) REFERENCES insumos(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS detallePedido (
    idPedido INT,
    idVariedad INT,
    cantidad DECIMAL(12,2),
    precioUnitario DECIMAL(12,2),
    PRIMARY KEY (idPedido, idVariedad),
    CONSTRAINT fk_detalle_pedido FOREIGN KEY (idPedido) REFERENCES pedidos(id),
    CONSTRAINT fk_detalle_variedad FOREIGN KEY (idVariedad) REFERENCES variedad(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT IGNORE INTO categoriaUsuario (id, nombreCategoria) VALUES
(1, 'Duenio'),
(2, 'Admin'),
(3, 'Empleado'),
(4, 'Cliente');

INSERT IGNORE INTO usuario
(DNI, nombre, apellido, direccion, telefono, email, contactoEmergencia, nombreCE, idCategoria, sector, cuenta, clave)
VALUES
(1, 'Admin', 'General', '', '', 'admin@medialunas.local', '', '', 2, 'Admin', 'admin', '1234'),
(100, 'Duenio', 'Prueba', '', '', 'duenio@medialunas.local', '', '', 1, 'Admin', 'duenio', '1234'),
(101, 'Admin', 'Prueba', '', '', 'admin2@medialunas.local', '', '', 2, 'Admin', 'admin2', '1234'),
(201, 'Ventas', 'Prueba', '', '', 'ventas@medialunas.local', '', '', 3, 'Ventas', 'ventas', '1234'),
(202, 'Produccion', 'Prueba', '', '', 'produccion@medialunas.local', '', '', 3, 'Produccion', 'produccion', '1234'),
(203, 'Logistica', 'Prueba', '', '', 'logistica@medialunas.local', '', '', 3, 'Logistica', 'logistica', '1234');

DROP VIEW IF EXISTS facturacionWeb;
CREATE VIEW facturacionWeb AS
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
