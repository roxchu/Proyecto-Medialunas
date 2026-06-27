from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse
import html
import sqlite3


ROOT = Path(__file__).resolve().parents[1]
DB_PATH = ROOT / "baseDatos" / "medialunas.db"


def db():
    con = sqlite3.connect(DB_PATH)
    con.row_factory = sqlite3.Row
    con.execute("PRAGMA foreign_keys = ON")
    con.execute("DELETE FROM detallePedido WHERE NOT EXISTS (SELECT 1 FROM pedidos WHERE pedidos.id = detallePedido.idPedido)")
    return con


def escape(value):
    return html.escape("" if value is None else str(value))


def page(title, body):
    return f"""<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>{escape(title)} - Medialunas Pro</title>
  <style>
    :root {{
      color-scheme: light;
      --fondo: #faf2e0;
      --panel: #fffaf0;
      --texto: #372b22;
      --primario: #c45c20;
      --oscuro: #7e4424;
      --suave: #e5d3b9;
      --verde: #48875c;
    }}
    * {{ box-sizing: border-box; }}
    body {{
      margin: 0;
      font-family: Arial, sans-serif;
      background: var(--fondo);
      color: var(--texto);
    }}
    header {{
      background: var(--oscuro);
      color: white;
      padding: 18px 24px;
    }}
    main {{
      width: min(1040px, calc(100% - 32px));
      margin: 24px auto 48px;
    }}
    nav {{
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
      margin-bottom: 18px;
    }}
    a, button {{
      background: var(--primario);
      color: white;
      border: 0;
      padding: 11px 15px;
      text-decoration: none;
      font-weight: 700;
      cursor: pointer;
    }}
    section {{
      background: var(--panel);
      border: 1px solid var(--suave);
      padding: 18px;
      margin-bottom: 18px;
    }}
    form {{
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 14px;
    }}
    label {{
      display: grid;
      gap: 6px;
      font-weight: 700;
    }}
    input, select {{
      width: 100%;
      padding: 10px;
      border: 1px solid #bba88f;
      background: white;
      color: var(--texto);
      font: inherit;
    }}
    .full {{ grid-column: 1 / -1; }}
    .ok {{ border-left: 6px solid var(--verde); }}
    .warn {{ border-left: 6px solid var(--primario); }}
    table {{
      width: 100%;
      border-collapse: collapse;
      background: white;
    }}
    th, td {{
      padding: 9px;
      border-bottom: 1px solid var(--suave);
      text-align: left;
    }}
    @media (max-width: 720px) {{
      form {{ grid-template-columns: 1fr; }}
    }}
  </style>
</head>
<body>
  <header><strong>Medialunas Pro</strong> | Pedidos web</header>
  <main>
    <nav>
      <a href="/">Nuevo pedido</a>
      <a href="/consulta">Consultar pedido</a>
    </nav>
    {body}
  </main>
</body>
</html>"""


def variedades_options():
    with db() as con:
        filas = con.execute(
            """
            SELECT v.id, v.nombreVariedad,
                   COALESCE((SELECT sv.precioUnitario
                             FROM stockVenta sv
                             WHERE sv.idVariedad = v.id
                             ORDER BY sv.id DESC
                             LIMIT 1), 0) AS precioDocena
            FROM variedad v
            ORDER BY v.nombreVariedad
            """
        ).fetchall()
    if not filas:
        return '<option value="">Primero cargar variedades en la app</option>'
    return "\n".join(
        f'<option value="{fila["id"]}">{escape(fila["nombreVariedad"])} - ${totalizar(fila["precioDocena"])} por docena</option>'
        for fila in filas
    )


def precio_docena(con, id_variedad):
    fila = con.execute(
        """
        SELECT precioUnitario
        FROM stockVenta
        WHERE idVariedad = ?
        ORDER BY id DESC
        LIMIT 1
        """,
        (id_variedad,),
    ).fetchone()
    return float(fila["precioUnitario"] or 0) if fila else 0.0


def factura_html(id_pedido):
    try:
        id_pedido = int(id_pedido)
    except (TypeError, ValueError):
        return page("Pedido invalido", '<section class="warn">Ingrese un ID de pedido valido.</section>')

    with db() as con:
        pedido = con.execute(
            """
            SELECT p.id, p.idCliente, p.fechaPedido, p.estado, c.nombre, c.direccion, c.metodoPago
            FROM pedidos p
            LEFT JOIN clientes c ON c.id = p.idCliente
            WHERE p.id = ?
            """,
            (id_pedido,),
        ).fetchone()
        detalles = con.execute(
            """
            SELECT d.idVariedad, v.nombreVariedad, d.cantidad, d.precioUnitario,
                   d.cantidad * d.precioUnitario AS subtotal
            FROM detallePedido d
            LEFT JOIN variedad v ON v.id = d.idVariedad
            WHERE d.idPedido = ?
            ORDER BY d.idVariedad
            """,
            (id_pedido,),
        ).fetchall()

    if not pedido:
        return page("Pedido no encontrado", '<section class="warn">No encontramos ese pedido.</section>')

    total = sum(float(row["subtotal"] or 0) for row in detalles)
    filas = "".join(
        f"<tr><td>{escape(row['idVariedad'])}</td><td>{escape(row['nombreVariedad'])}</td>"
        f"<td>{escape(row['cantidad'])} docenas</td><td>${escape(row['precioUnitario'])} / docena</td>"
        f"<td>${totalizar(row['subtotal'])}</td></tr>"
        for row in detalles
    )
    if not filas:
        filas = '<tr><td colspan="5">Todavia no hay detalle cargado para este pedido.</td></tr>'

    return page(
        "Factura",
        f"""
        <section class="ok">
          <h1>Pedido #{escape(pedido['id'])}</h1>
          <p><strong>Estado:</strong> {escape(pedido['estado'])}</p>
          <p><strong>Cliente:</strong> {escape(pedido['nombre'])} | ID {escape(pedido['idCliente'])}</p>
          <p><strong>Direccion:</strong> {escape(pedido['direccion'])}</p>
          <p><strong>Fecha:</strong> {escape(pedido['fechaPedido'])}</p>
          <p><strong>Metodo de pago:</strong> {escape(pedido['metodoPago'])}</p>
        </section>
        <section>
          <h2>Detalle</h2>
          <table>
            <thead><tr><th>ID variedad</th><th>Variedad</th><th>Cantidad</th><th>Precio por docena</th><th>Subtotal</th></tr></thead>
            <tbody>{filas}</tbody>
            <tfoot><tr><th colspan="4">Total</th><th>${totalizar(total)}</th></tr></tfoot>
          </table>
        </section>
        <section>
          <h2>Consultar otro pedido</h2>
          <form method="get" action="/pedido">
            <label>ID de pedido<input name="id" type="number" min="1" required></label>
            <div><button type="submit">Ver estado y factura</button></div>
          </form>
        </section>
        """,
    )


def totalizar(value):
    return f"{float(value or 0):.2f}"


class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        ruta = urlparse(self.path).path
        if ruta == "/":
            self.html(self.formulario_pedido())
            return
        if ruta == "/consulta":
            self.html(self.formulario_consulta())
            return
        if ruta == "/pedido":
            params = parse_qs(urlparse(self.path).query)
            self.html(factura_html(params.get("id", [""])[0]))
            return
        self.send_error(404)

    def do_POST(self):
        ruta = urlparse(self.path).path
        length = int(self.headers.get("Content-Length", "0"))
        data = parse_qs(self.rfile.read(length).decode("utf-8"))
        if ruta == "/pedido":
            self.crear_pedido(data)
            return
        self.send_error(404)

    def formulario_pedido(self):
        return page(
            "Nuevo pedido",
            f"""
            <section>
              <h1>Nuevo pedido</h1>
              <form method="post" action="/pedido">
                <label>Nombre<input name="nombre" required></label>
                <label>CUIT<input name="cuit"></label>
                <label class="full">Direccion<input name="direccion" required></label>
                <label>Telefono<input name="telefono"></label>
                <label>Metodo de pago
                  <select name="metodo_pago">
                    <option>Efectivo</option>
                    <option>Transferencia</option>
                    <option>Tarjeta</option>
                  </select>
                </label>
                <label>Variedad<select name="id_variedad" required>{variedades_options()}</select></label>
                <label>Cantidad de docenas<input name="cantidad" type="number" min="1" step="1" required></label>
                <p class="full">El precio se calcula automaticamente segun el precio por docena cargado por la empresa.</p>
                <div class="full"><button type="submit">Enviar pedido</button></div>
              </form>
            </section>
            <section>
              <h2>Ya hiciste un pedido?</h2>
              <form method="get" action="/pedido">
                <label>ID de pedido<input name="id" type="number" min="1" required></label>
                <div><button type="submit">Consultar estado y factura</button></div>
              </form>
            </section>
            """,
        )

    def formulario_consulta(self):
        return page(
            "Consultar pedido",
            """
            <section>
              <h1>Consultar pedido</h1>
              <form method="get" action="/pedido">
                <label>ID pedido<input name="id" type="number" min="1" required></label>
                <div><button type="submit">Ver estado y factura</button></div>
              </form>
            </section>
            """,
        )

    def crear_pedido(self, data):
        def campo(nombre):
            return data.get(nombre, [""])[0].strip()

        try:
            id_variedad = int(campo("id_variedad"))
            cantidad_docenas = int(campo("cantidad"))
        except ValueError:
            self.html(page("Error", '<section class="warn">La variedad y la cantidad deben ser validas.</section>'))
            return

        if cantidad_docenas <= 0:
            self.html(page("Error", '<section class="warn">La cantidad debe ser de al menos 1 docena.</section>'))
            return

        try:
            with db() as con:
                precio = precio_docena(con, id_variedad)
                if precio <= 0:
                    self.html(page("Error", '<section class="warn">Esta variedad todavia no tiene precio por docena cargado en Stock publico.</section>'))
                    return

                cursor = con.cursor()
                cursor.execute(
                    """
                    INSERT INTO clientes (nombre, CUIT, direccion, telefono, metodoPago, categoria)
                    VALUES (?, ?, ?, ?, ?, 'Web')
                    """,
                    (campo("nombre"), campo("cuit"), campo("direccion"), campo("telefono"), campo("metodo_pago")),
                )
                id_cliente = cursor.lastrowid
                cursor.execute(
                    "INSERT INTO pedidos (idCliente, fechaPedido, estado) VALUES (?, date('now'), 'Pendiente')",
                    (id_cliente,),
                )
                id_pedido = cursor.lastrowid
                cursor.execute(
                    "INSERT INTO detallePedido (idPedido, idVariedad, cantidad, precioUnitario) VALUES (?, ?, ?, ?)",
                    (id_pedido, id_variedad, cantidad_docenas, precio),
                )
                con.commit()
        except sqlite3.Error as error:
            self.html(
                page(
                    "Error",
                    f'<section class="warn">No se pudo crear el pedido: {escape(error)}</section>',
                )
            )
            return

        self.html(
            page(
                "Pedido creado",
                f"""
                <section class="ok">
                  <h1>Pedido recibido</h1>
                  <p>Tu ID de pedido es <strong>{id_pedido}</strong>.</p>
                  <p>Con ese numero podes consultar el estado y la factura.</p>
                  <a href="/pedido?id={id_pedido}">Ver mi pedido</a>
                  <a href="/consulta">Consultar otro pedido</a>
                </section>
                """,
            )
        )

    def html(self, body):
        encoded = body.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(encoded)))
        self.end_headers()
        self.wfile.write(encoded)

    def log_message(self, fmt, *args):
        print("%s - %s" % (self.address_string(), fmt % args))


if __name__ == "__main__":
    server = HTTPServer(("127.0.0.1", 8080), Handler)
    print("Web de pedidos en http://127.0.0.1:8080")
    server.serve_forever()
