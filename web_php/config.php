<?php
$dbPath = __DIR__ . '/../baseDatos/medialunas.db';

function db(): PDO {
    global $dbPath;
    $pdo = new PDO('sqlite:' . $dbPath);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $pdo->exec('PRAGMA foreign_keys = ON');
    $pdo->exec('DELETE FROM detallePedido WHERE NOT EXISTS (SELECT 1 FROM pedidos WHERE pedidos.id = detallePedido.idPedido)');
    return $pdo;
}

function h($value): string {
    return htmlspecialchars((string)($value ?? ''), ENT_QUOTES, 'UTF-8');
}

function money($value): string {
    return number_format((float)($value ?? 0), 2, ',', '.');
}

function layout(string $title, string $content): void {
    echo '<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>' . h($title) . ' - Medialunas Pro</title>
  <link rel="stylesheet" href="styles.css">
</head>
<body>
  <header><strong>Medialunas Pro</strong> | Pedidos web</header>
  <main>
    <nav>
      <a href="index.php">Nuevo pedido</a>
      <a href="consulta.php">Consultar pedido</a>
    </nav>
    ' . $content . '
  </main>
</body>
</html>';
}
?>
