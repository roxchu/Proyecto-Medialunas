<?php
require __DIR__ . '/config.php';

$pdo = db();
$variedades = $pdo->query("
    SELECT v.id, v.nombreVariedad,
           COALESCE((SELECT sv.precioUnitario
                     FROM stockVenta sv
                     WHERE sv.idVariedad = v.id
                     ORDER BY sv.id DESC
                     LIMIT 1), 0) AS precioDocena
    FROM variedad v
    ORDER BY v.nombreVariedad
")->fetchAll(PDO::FETCH_ASSOC);

ob_start();
?>
<section>
  <h1>Nuevo pedido mayorista</h1>
  <form method="post" action="pedido.php">
    <label>Nombre
      <input name="nombre" required>
    </label>
    <label>CUIT
      <input name="cuit">
    </label>
    <label class="full">Direccion
      <input name="direccion" required>
    </label>
    <label>Telefono
      <input name="telefono">
    </label>
    <label>Metodo de pago
      <select name="metodo_pago">
        <option>Efectivo</option>
        <option>Transferencia</option>
        <option>Tarjeta</option>
      </select>
    </label>
    <label>Variedad
      <select name="id_variedad" required>
        <?php if (!$variedades): ?>
          <option value="">Primero cargar variedades en la app</option>
        <?php endif; ?>
        <?php foreach ($variedades as $variedad): ?>
          <option value="<?= h($variedad['id']) ?>">
            <?= h($variedad['nombreVariedad']) ?> - $<?= money($variedad['precioDocena']) ?> por docena
          </option>
        <?php endforeach; ?>
      </select>
    </label>
    <label>Cantidad de docenas
      <input name="cantidad" type="number" min="1" step="1" required>
    </label>
    <p class="full">El precio se calcula automaticamente segun el precio por docena cargado por la empresa.</p>
    <div class="full">
      <button type="submit">Enviar pedido</button>
    </div>
  </form>
</section>
<section>
  <h2>Ya hiciste un pedido?</h2>
  <form method="get" action="pedido.php">
    <label>ID de pedido
      <input name="id" type="number" min="1" required>
    </label>
    <div>
      <button type="submit">Consultar estado y factura</button>
    </div>
  </form>
</section>
<?php
layout('Nuevo pedido', ob_get_clean());
?>
