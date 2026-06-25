<?php
require __DIR__ . '/config.php';

ob_start();
?>
<section>
  <h1>Consultar pedido</h1>
  <form method="get" action="pedido.php">
    <label>ID pedido
      <input name="id" type="number" min="1" required>
    </label>
    <div>
      <button type="submit">Ver estado y factura</button>
    </div>
  </form>
</section>
<?php
layout('Consultar pedido', ob_get_clean());
?>
