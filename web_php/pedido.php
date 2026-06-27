<?php
require __DIR__ . '/config.php';

function precioDocena(PDO $pdo, int $idVariedad): float {
    $stmt = $pdo->prepare("
        SELECT precioUnitario
        FROM stockVenta
        WHERE idVariedad = ?
        ORDER BY id DESC
        LIMIT 1
    ");
    $stmt->execute([$idVariedad]);
    $precio = $stmt->fetchColumn();
    return $precio === false ? 0.0 : (float)$precio;
}

function factura(PDO $pdo, int $idPedido): string {
    if ($idPedido <= 0) {
        return '<section class="warn">Ingrese un ID de pedido valido.</section>';
    }

    $stmt = $pdo->prepare("
        SELECT p.id, p.idCliente, p.fechaPedido, p.estado,
               c.nombre, c.direccion, c.metodoPago
        FROM pedidos p
        LEFT JOIN clientes c ON c.id = p.idCliente
        WHERE p.id = ?
    ");
    $stmt->execute([$idPedido]);
    $pedido = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$pedido) {
        return '<section class="warn">No encontramos ese pedido.</section>';
    }

    $stmt = $pdo->prepare("
        SELECT d.idVariedad, v.nombreVariedad, d.cantidad, d.precioUnitario,
               d.cantidad * d.precioUnitario AS subtotal
        FROM detallePedido d
        LEFT JOIN variedad v ON v.id = d.idVariedad
        WHERE d.idPedido = ?
        ORDER BY d.idVariedad
    ");
    $stmt->execute([$idPedido]);
    $detalles = $stmt->fetchAll(PDO::FETCH_ASSOC);

    $total = 0;
    $rows = '';
    foreach ($detalles as $detalle) {
        $total += (float)$detalle['subtotal'];
        $rows .= '<tr>
            <td>' . h($detalle['idVariedad']) . '</td>
            <td>' . h($detalle['nombreVariedad']) . '</td>
            <td>' . h($detalle['cantidad']) . ' docenas</td>
            <td>$' . money($detalle['precioUnitario']) . ' / docena</td>
            <td>$' . money($detalle['subtotal']) . '</td>
        </tr>';
    }

    if ($rows === '') {
        $rows = '<tr><td colspan="5">Todavia no hay detalle cargado para este pedido.</td></tr>';
    }

    return '<section class="ok">
        <h1>Pedido #' . h($pedido['id']) . '</h1>
        <p><strong>Estado:</strong> ' . h($pedido['estado']) . '</p>
        <p><strong>Cliente:</strong> ' . h($pedido['nombre']) . ' | ID ' . h($pedido['idCliente']) . '</p>
        <p><strong>Direccion:</strong> ' . h($pedido['direccion']) . '</p>
        <p><strong>Fecha:</strong> ' . h($pedido['fechaPedido']) . '</p>
        <p><strong>Metodo de pago:</strong> ' . h($pedido['metodoPago']) . '</p>
    </section>
    <section>
        <h2>Detalle</h2>
        <table>
            <thead><tr><th>ID variedad</th><th>Variedad</th><th>Cantidad</th><th>Precio por docena</th><th>Subtotal</th></tr></thead>
            <tbody>' . $rows . '</tbody>
            <tfoot><tr><th colspan="4">Total</th><th>$' . money($total) . '</th></tr></tfoot>
        </table>
    </section>
    <section>
        <h2>Consultar otro pedido</h2>
        <form method="get" action="pedido.php">
            <label>ID de pedido
                <input name="id" type="number" min="1" required>
            </label>
            <div>
                <button type="submit">Ver estado y factura</button>
            </div>
        </form>
    </section>';
}

$pdo = db();

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    $idPedido = (int)($_GET['id'] ?? 0);
    layout('Pedido', factura($pdo, $idPedido));
    exit;
}

$nombre = trim($_POST['nombre'] ?? '');
$cuit = trim($_POST['cuit'] ?? '');
$direccion = trim($_POST['direccion'] ?? '');
$telefono = trim($_POST['telefono'] ?? '');
$metodoPago = trim($_POST['metodo_pago'] ?? '');
$idVariedad = (int)($_POST['id_variedad'] ?? 0);
$cantidadDocenas = (int)($_POST['cantidad'] ?? 0);

if ($nombre === '' || $direccion === '' || $idVariedad <= 0 || $cantidadDocenas <= 0) {
    layout('Error', '<section class="warn">Complete los datos obligatorios y use una cantidad valida de docenas.</section>');
    exit;
}

try {
    $precio = precioDocena($pdo, $idVariedad);
    if ($precio <= 0) {
        layout('Error', '<section class="warn">Esta variedad todavia no tiene precio por docena cargado en Stock publico.</section>');
        exit;
    }

    $pdo->beginTransaction();

    $stmt = $pdo->prepare("
        INSERT INTO clientes (nombre, CUIT, direccion, telefono, metodoPago, categoria)
        VALUES (?, ?, ?, ?, ?, 'Web')
    ");
    $stmt->execute([$nombre, $cuit, $direccion, $telefono, $metodoPago]);
    $idCliente = (int)$pdo->lastInsertId();

    $stmt = $pdo->prepare("INSERT INTO pedidos (idCliente, fechaPedido, estado) VALUES (?, CURDATE(), 'Pendiente')");
    $stmt->execute([$idCliente]);
    $idPedido = (int)$pdo->lastInsertId();

    $stmt = $pdo->prepare("INSERT INTO detallePedido (idPedido, idVariedad, cantidad, precioUnitario) VALUES (?, ?, ?, ?)");
    $stmt->execute([$idPedido, $idVariedad, $cantidadDocenas, $precio]);

    $pdo->commit();

    layout('Pedido creado', '<section class="ok">
        <h1>Pedido recibido</h1>
        <p>Tu ID de pedido es <strong>' . h($idPedido) . '</strong>.</p>
        <p>Con ese numero podes consultar el estado y la factura.</p>
        <a href="pedido.php?id=' . h($idPedido) . '">Ver mi pedido</a>
        <a href="consulta.php">Consultar otro pedido</a>
    </section>');
} catch (Throwable $error) {
    if ($pdo->inTransaction()) {
        $pdo->rollBack();
    }
    layout('Error', '<section class="warn">No se pudo crear el pedido: ' . h($error->getMessage()) . '</section>');
}
?>
