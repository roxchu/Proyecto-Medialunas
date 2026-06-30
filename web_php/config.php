<?php
$dbConfig = [
    'host' => getenv('MEDIALUNAS_DB_HOST') ?: 'pma.torga.com.ar',
    'port' => getenv('MEDIALUNAS_DB_PORT') ?: '3306',
    'name' => getenv('MEDIALUNAS_DB_NAME') ?: 's4_medialunasPro',
    'user' => getenv('MEDIALUNAS_DB_USER') ?: 'u4_G2Ci7RXpul',
    'pass' => getenv('MEDIALUNAS_DB_PASS') ?: 'o=1wT!plMiCJofM3fC4jc!=n',
];

$localConfig = __DIR__ . '/database.local.php';
if (is_file($localConfig)) {
    $dbConfig = array_merge($dbConfig, require $localConfig);
}

function db(): PDO {
    global $dbConfig;
    $dsn = 'mysql:host=' . $dbConfig['host'] . ';port=' . $dbConfig['port'] . ';dbname=' . $dbConfig['name'] . ';charset=utf8mb4';
    $pdo = new PDO($dsn, $dbConfig['user'], $dbConfig['pass']);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
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
