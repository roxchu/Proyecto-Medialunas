#include "raylib.h"
#include "baseDatos/conexion.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

enum PantallaAuth { INICIO, LOGIN, REGISTRO, SISTEMA };

struct CampoTabla {
    string columna;
    string etiqueta;
    bool clave;
    bool autoIncremental;
    bool ocultar;
};

struct ConfigTabla {
    string titulo;
    string tabla;
    string sector;
    vector<CampoTabla> campos;
};

class CampoTexto {
private:
    Rectangle area;
    string etiqueta;
    string valor;
    bool activo;
    int maximo;

public:
    CampoTexto() {
        area = {0, 0, 0, 0};
        etiqueta = "";
        valor = "";
        activo = false;
        maximo = 60;
    }

    CampoTexto(string etiqueta, Rectangle area, int maximo = 60) {
        this->etiqueta = etiqueta;
        this->area = area;
        this->maximo = maximo;
        valor = "";
        activo = false;
    }

    void procesar(Vector2 mouse, bool editable = true) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        activo = editable && CheckCollisionPointRec(mouse, area);
    }

    if (!activo || !editable) return;

    int tecla = GetCharPressed();
    while (tecla > 0) {
        if (tecla >= 32 && tecla <= 126 && (int)valor.size() < maximo) {
            valor += (char)tecla;
        }
        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !valor.empty()) valor.pop_back();
    }

    void dibujar(Color borde, Color texto, bool ocultar = false, bool editable = true) {
        DrawText(etiqueta.c_str(), area.x, area.y - 20, 16, texto);
        DrawRectangleRec(area, editable ? WHITE : Color{239, 232, 220, 255});
        DrawRectangleLinesEx(area, activo ? 2 : 1, activo ? borde : Color{190, 180, 165, 255});

        string visible = ocultar ? string(valor.size(), '*') : valor;
        while (MeasureText(visible.c_str(), 16) > area.width - 18 && visible.size() > 1) {
            visible = visible.substr(1);
        }
        DrawText(visible.c_str(), area.x + 9, area.y + 9, 16, texto);
    }

    string getValor() const { return valor; }
    void setValor(string nuevoValor) { valor = nuevoValor; }
    void limpiar() { valor = ""; activo = false; }
};

class Boton {
private:
    Rectangle area;
    string texto;

public:
    Boton() {
        area = {0, 0, 0, 0};
        texto = "";
    }

    Boton(string texto, Rectangle area) {
        this->texto = texto;
        this->area = area;
    }

    bool presionado(Vector2 mouse) {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, area);
    }

    void dibujar(Color base, Color hover, Color textoColor, int fuente = 16) {
        Vector2 mouse = GetMousePosition();
        Color color = CheckCollisionPointRec(mouse, area) ? hover : base;
        DrawRectangleRec(area, color);
        DrawRectangleLinesEx(area, 1, Color{102, 75, 55, 255});
        int anchoTexto = MeasureText(texto.c_str(), fuente);
        DrawText(texto.c_str(), area.x + (area.width - anchoTexto) / 2, area.y + (area.height - fuente) / 2, fuente, textoColor);
    }
};

class MedialunasApp {
private:
    PantallaAuth pantallaAuth;
    vector<ConfigTabla> tablas;
    int tablaActual;
    vector<vector<string>> filas;
    vector<CampoTexto> formulario;
    vector<string> claveSeleccionada;
    int usuarioDNI;
    int usuarioCategoria;
    string usuarioNombre;
    string usuarioSector;
    string mensaje;
    int scrollTabla;
    int scrollMenu;

    CampoTexto loginCuenta;
    CampoTexto loginClave;
    CampoTexto regDNI;
    CampoTexto regNombre;
    CampoTexto regApellido;
    CampoTexto regEmail;
    CampoTexto regTelefono;
    CampoTexto regCuenta;
    CampoTexto regClave;

    Color fondo = {250, 242, 224, 255};
    Color panel = {255, 250, 240, 255};
    Color primario = {196, 92, 32, 255};
    Color primarioOscuro = {126, 68, 36, 255};
    Color texto = {55, 43, 34, 255};
    Color suave = {229, 211, 185, 255};
    Color verde = {72, 135, 92, 255};
    Color rojo = {180, 63, 58, 255};

public:
    MedialunasApp() {
        pantallaAuth = INICIO;
        tablaActual = 0;
        usuarioDNI = 0;
        usuarioCategoria = 0;
        usuarioSector = "";
        mensaje = "Bienvenido a Medialunas Pro";
        scrollTabla = 0;
        scrollMenu = 0;

        loginCuenta = CampoTexto("Usuario", {455, 285, 290, 38}, 40);
        loginClave = CampoTexto("Contrasena", {455, 355, 290, 38}, 40);
        regDNI = CampoTexto("DNI", {315, 210, 230, 38}, 12);
        regNombre = CampoTexto("Nombre", {575, 210, 230, 38}, 40);
        regApellido = CampoTexto("Apellido", {315, 280, 230, 38}, 40);
        regEmail = CampoTexto("Email", {575, 280, 230, 38}, 60);
        regTelefono = CampoTexto("Telefono", {315, 350, 230, 38}, 30);
        regCuenta = CampoTexto("Usuario", {575, 350, 230, 38}, 40);
        regClave = CampoTexto("Contrasena", {445, 420, 230, 38}, 40);

        configurarTablas();
    }

    void ejecutar() {
        InitWindow(1200, 760, "Medialunas Pro - Gestion");
        SetTargetFPS(60);
        inicializarBase();

        while (!WindowShouldClose()) {
            Vector2 mouse = GetMousePosition();
            procesar(mouse);
            dibujar();
        }

        CloseWindow();
    }

private:
    void configurarTablas() {
    tablas = {
        {"Clientes web", "clientes", "Ventas", {{"id", "ID", true, true, false}, {"nombre", "Nombre", false, false, false}, {"CUIT", "CUIT", false, false, false}, {"direccion", "Direccion", false, false, false}, {"telefono", "Telefono", false, false, false}, {"metodoPago", "Metodo pago", false, false, false}, {"categoria", "Categoria", false, false, false}}},
        {"Pedidos web", "pedidos", "Comun", {{"id", "ID pedido", true, true, false}, {"idCliente", "ID cliente", false, false, false}, {"fechaPedido", "Fecha", false, false, false}, {"estado", "Estado (Pendiente/Entregado)", false, false, false}}},        {"Detalle pedidos", "detallePedido", "Ventas", {{"idPedido", "ID pedido", true, false, false}, {"idVariedad", "ID variedad", true, false, false}, {"cantidad", "Cantidad (docenas)", false, false, false}, {"precioUnitario", "Precio x unidad ($)", false, false, false}}},
        {"Facturacion", "facturacion", "Ventas", {{"direccion", "Direccion", false, false, false}, {"idPedido", "ID pedido", true, false, false}, {"idCliente", "ID cliente", false, false, false}, {"fecha", "Fecha", false, false, false}, {"idVariedad", "ID variedad", false, false, false}, {"cantidad", "Cantidad (docenas)", false, false, false}, {"precio", "Precio x unidad ($)", false, false, false}, {"metodoPago", "Metodo pago", false, false, false}}},
        {"Variedades", "variedad", "Produccion", {{"id", "ID", true, true, false}, {"nombreVariedad", "Nombre", false, false, false}, {"descripcion", "Descripcion", false, false, false}}},
        {"Stock publico", "stockVenta", "Ventas", {{"idVariedad", "ID variedad", true, false, false}, {"variedad", "Variedad", false, false, false}, {"stockDisponible", "Stock (unidades)", false, false, false}, {"precioUnitario", "Precio x docena ($)", false, false, false}}},
        {"Insumos", "insumos", "Produccion", {{"id", "ID", true, true, false}, {"nombre", "Nombre", false, false, false}, {"cantidadActual", "Cantidad actual", false, false, false}, {"unidadMedida", "Unidad (kg/l/etc)", false, false, false}, {"proveedor", "Proveedor", false, false, false}, {"stockMinimo", "Stock minimo", false, false, false}}},
        {"Detalles de produccion", "loteInsumos", "Produccion", {{"id", "ID", true, true, false}, {"idLoteProduccion", "ID lote prod.", false, false, false}, {"idReceta", "ID receta", false, false, false}, {"costoUnitario", "Costo x unidad", false, false, false}, {"observaciones", "Observaciones", false, false, false}}},
        {"Recetas", "receta", "Produccion", {{"id", "ID", true, true, false}, {"nombreProducto", "Producto", false, false, false}, {"cantidadEsperada", "Cant. esperada (unidades)", false, false, false}, {"procedimiento", "Procedimiento", false, false, false}}},
        {"Insumos por receta", "recetaInsumos", "Produccion", {{"idReceta", "ID receta", true, false, false}, {"idInsumo", "ID insumo", true, false, false}, {"cantidad", "Cantidad requerida", false, false, false}}},
        {"Lotes produccion", "lotesProduccion", "Produccion", {{"id", "ID", true, true, false}, {"fecha", "Fecha", false, false, false}, {"idVariedad", "ID variedad", false, false, false}, {"cantidadProducida", "Cantidad producida (unidades)", false, false, false}, {"idEmpleado", "ID empleado", false, false, false}, {"idReceta", "ID receta", false, false, false}}},
        {"Vehiculos", "vehiculos", "Logistica", {{"id", "ID", true, true, false}, {"patente", "Patente", false, false, false}, {"modelo", "Modelo", false, false, false}, {"estadoMantenimiento", "Estado mantenimiento", false, false, false}}},
        {"Hoja de ruta", "hojaRuta", "Logistica", {{"id", "ID", true, true, false}, {"idVehiculo", "ID vehiculo", false, false, false}, {"idPedido", "ID pedido", false, false, false}, {"ordenEntrega", "Orden entrega", false, false, false}, {"idEmpleado", "ID empleado", false, false, false}}},
        {"Usuarios", "usuario", "Admin", {{"DNI", "DNI", true, false, false}, {"nombre", "Nombre", false, false, false}, {"apellido", "Apellido", false, false, false}, {"direccion", "Direccion", false, false, false}, {"telefono", "Telefono", false, false, false}, {"email", "Email", false, false, false}, {"idCategoria", "Categoria (1=Duenio, 2=Admin, 3=Empleado)", false, false, false}, {"sector", "Sector (Ventas/Produccion/Logistica)", false, false, false}, {"cuenta", "Usuario", false, false, false}, {"clave", "Contrasena", false, false, true}}},
        {"Categorias usuario", "categoriaUsuario", "Admin", {{"id", "ID", true, true, false}, {"nombreCategoria", "Nombre categoria", false, false, false}}}
    };
 }

    bool esDuenio() { return usuarioCategoria == 1; }
    bool esAdmin() { return usuarioCategoria == 2; }
    bool esEmpleado() { return usuarioCategoria == 3; }

    bool puedeVerPanel(int indice) {
        if (esDuenio() || esAdmin()) return true;
        if (!esEmpleado()) return false;
        string sector = usuarioSector.empty() ? "Ventas" : usuarioSector;
        return tablas[indice].sector == sector || tablas[indice].sector == "Comun";
    }

    int primerPanelVisible() {
        for (int i = 0; i < (int)tablas.size(); i++) {
            if (puedeVerPanel(i)) return i;
        }
        return 0;
    }

    bool esPanelLectura() {
        string tabla = tablas[tablaActual].tabla;
        return tabla == "facturacion" || tabla == "stockVenta" || tabla == "clientes";
    }

    bool puedeCrear() {
        string tabla = tablas[tablaActual].tabla;
        if (tabla == "facturacion" || tabla == "clientes" || tabla == "pedidos") return false;
        if (tabla == "stockVenta") return esDuenio() || esAdmin() || 
                                        (esEmpleado() && usuarioSector == "Produccion");
        if (tabla == "variedad" || tabla == "usuario" || tabla == "categoriaUsuario") return esDuenio() || esAdmin();
        if (esDuenio() || esAdmin()) return true;
        return esEmpleado() && puedeVerPanel(tablaActual);
   }

    bool puedeActualizar() {
        string tabla = tablas[tablaActual].tabla;
        if (tabla == "facturacion" || tabla == "clientes") return false;
        if (tabla == "stockVenta") return esDuenio() || esAdmin() || 
                                        (esEmpleado() && usuarioSector == "Produccion");
        if (tabla == "variedad" || tabla == "usuario" || tabla == "categoriaUsuario") return esDuenio() || esAdmin();
        // Permitir actualizar pedidos a Admin, Dueño, Ventas y Logistica
        if (tabla == "pedidos") return esDuenio() || esAdmin() || 
                                    (esEmpleado() && (usuarioSector == "Ventas" || usuarioSector == "Logistica"));
        // Por defecto: Admin y Dueño siempre pueden, empleados si tienen acceso al panel
        if (esDuenio() || esAdmin()) return true;
        return esEmpleado() && puedeVerPanel(tablaActual);
    }

    bool puedeEliminar() {
        string tabla = tablas[tablaActual].tabla;
        if (tabla == "facturacion" || tabla == "clientes" || tabla == "pedidos") return false;
        if (tabla == "stockVenta") return esDuenio() || esAdmin();
        if (tabla == "usuario" || tabla == "categoriaUsuario" || tabla == "variedad") return esDuenio() || esAdmin();
        if (esDuenio() || esAdmin()) return true;
        return esEmpleado() && puedeVerPanel(tablaActual);
    }

    void procesar(Vector2 mouse) {
        if (pantallaAuth == INICIO) {
            Boton iniciar("Tengo cuenta", {420, 370, 170, 44});
            Boton crear("Crear cuenta", {610, 370, 170, 44});
            if (iniciar.presionado(mouse)) pantallaAuth = LOGIN;
            if (crear.presionado(mouse)) pantallaAuth = REGISTRO;
            return;
        }

        if (pantallaAuth == LOGIN) {
            procesarLogin(mouse);
            return;
        }

        if (pantallaAuth == REGISTRO) {
            procesarRegistro(mouse);
            return;
        }

        procesarSistema(mouse);
    }

    void procesarLogin(Vector2 mouse) {
        loginCuenta.procesar(mouse);
        loginClave.procesar(mouse);
        Boton ingresar("Ingresar", {500, 430, 200, 42});
        Boton volver("Volver", {500, 485, 200, 38});

        if (volver.presionado(mouse)) pantallaAuth = INICIO;
        if (ingresar.presionado(mouse) || IsKeyPressed(KEY_ENTER)) iniciarSesion();
    }

    void procesarRegistro(Vector2 mouse) {
        regDNI.procesar(mouse);
        regNombre.procesar(mouse);
        regApellido.procesar(mouse);
        regEmail.procesar(mouse);
        regTelefono.procesar(mouse);
        regCuenta.procesar(mouse);
        regClave.procesar(mouse);

        Boton crear("Crear usuario", {445, 500, 230, 42});
        Boton volver("Volver", {445, 555, 230, 38});
        if (volver.presionado(mouse)) pantallaAuth = INICIO;
        if (crear.presionado(mouse) || IsKeyPressed(KEY_ENTER)) registrarUsuario();
    }

    void procesarSistema(Vector2 mouse) {
        float rueda = GetMouseWheelMove();
        if (rueda != 0 && mouse.x < 205) scrollMenu = max(0, scrollMenu - (int)rueda * 26);
        if (rueda != 0 && mouse.x >= 220) scrollTabla = max(0, scrollTabla - (int)rueda * 28);

        int visible = 0;
        for (int i = 0; i < (int)tablas.size(); i++) {
            if (!puedeVerPanel(i)) continue;
            Boton boton(tablas[i].titulo, {18, 108.0f + visible * 43.0f - scrollMenu, 172, 34});
            if (boton.presionado(mouse)) cambiarTabla(i);
            visible++;
        }

        const ConfigTabla& config = tablas[tablaActual];
        for (int i = 0; i < (int)formulario.size(); i++) {
            bool editable = campoEditable(config.campos[i]);
            formulario[i].procesar(mouse, editable);
        }

        Boton nuevo("Crear", {220, 292, 115, 38});
        Boton guardar(claveSeleccionada.empty() ? "Guardar" : "Actualizar", {350, 292, 125, 38});
        Boton eliminar("Borrar", {490, 292, 125, 38});
        Boton salir("Cerrar sesion", {1035, 292, 135, 38});
        
        if (nuevo.presionado(mouse)) {
            if (puedeCrear()) limpiarFormulario();
            else mensaje = "No tenes permiso para crear en este panel";
        }
        if (guardar.presionado(mouse)) {
            if ((claveSeleccionada.empty() && puedeCrear()) || (!claveSeleccionada.empty() && puedeActualizar())) guardarRegistro();
            else mensaje = "No tenes permiso para guardar en este panel";
        }
        if (eliminar.presionado(mouse)) {
            if (puedeEliminar()) eliminarRegistro();
            else mensaje = "No tenes permiso para borrar en este panel";
        }
        if (salir.presionado(mouse)) cerrarSesion();

        procesarSeleccion(mouse);
    }

    void cambiarTabla(int indice) {
        if (!puedeVerPanel(indice)) {
            mensaje = "No tenes acceso a ese sector";
            return;
        }
        tablaActual = indice;
        scrollTabla = 0;
        configurarFormulario();
        cargarDatos();
        mensaje = "Panel: " + tablas[tablaActual].titulo;
    }

    void configurarFormulario() {
        formulario.clear();
        const ConfigTabla& config = tablas[tablaActual];
        
        // Especial para usuarios: 2 columnas grandes
        // Para otros: 4 columnas normales
        int columnas = (config.tabla == "usuario") ? 2 : 4;
        float ancho = (config.tabla == "usuario") ? 380.0f : 210.0f;
        float pasoX = (config.tabla == "usuario") ? 410.0f : 232.0f;
        
        for (int i = 0; i < (int)config.campos.size(); i++) {
            int col = i % columnas;
            int fila = i / columnas;
            formulario.push_back(CampoTexto(config.campos[i].etiqueta, {220.0f + col * pasoX, 105.0f + fila * 62.0f, ancho, 34}, 90));
        }
        claveSeleccionada.clear();
    } 

    bool campoEditable(const CampoTabla& campo) {
        string tabla = tablas[tablaActual].tabla;
        if (tabla == "facturacion" || tabla == "clientes") return false;
        
        // Permitir editar precioUnitario y stockDisponible en stockVenta
        if (tabla == "stockVenta") {
            if (campo.columna == "precioUnitario" || campo.columna == "stockDisponible") {
                return esDuenio() || esAdmin() || 
                    (esEmpleado() && usuarioSector == "Produccion");
            }
            return false;
        }
        
        // Permitir editar SOLO estado en pedidos para Ventas y Logistica
        if (tabla == "pedidos") {
            return !claveSeleccionada.empty() && campo.columna == "estado" &&
                (esDuenio() || esAdmin() || 
                    (esEmpleado() && (usuarioSector == "Ventas" || usuarioSector == "Logistica")));
        }
        
        if (claveSeleccionada.empty() && !puedeCrear()) return false;
        if (!claveSeleccionada.empty() && !puedeActualizar()) return false;
        if (claveSeleccionada.empty()) return !campo.autoIncremental;
        return !campo.clave;
    }

    void procesarSeleccion(Vector2 mouse) {
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
        if (mouse.x < 220 || mouse.x > 1140 || mouse.y < 345 || mouse.y > 682) return;

        int fila = ((int)mouse.y - 381 + scrollTabla) / 32;
        if (fila < 0 || fila >= (int)filas.size()) return;

        claveSeleccionada.clear();
        const ConfigTabla& config = tablas[tablaActual];
        for (int i = 0; i < (int)config.campos.size(); i++) {
            string valor = filas[fila][i];

            // Convertir fecha yyyy-mm-dd a dd/mm/yyyy para que pase la validación
            string col = config.campos[i].columna;
            if ((col == "fecha" || col == "fechaPedido") &&
                valor.size() == 10 && valor[4] == '-' && valor[7] == '-') {
                valor = valor.substr(8, 2) + "/" + valor.substr(5, 2) + "/" + valor.substr(0, 4);
            }

            formulario[i].setValor(valor);
            if (config.campos[i].clave) claveSeleccionada.push_back(filas[fila][i]);
        }
        mensaje = "Registro seleccionado";
    }

    void limpiarFormulario() {
        for (CampoTexto& campo : formulario) campo.limpiar();
        claveSeleccionada.clear();
        mensaje = "Formulario listo para nuevo registro";
    }

    void cargarDatos() {
        filas.clear();
        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }

        const ConfigTabla& config = tablas[tablaActual];
        string sql = consultaListado(config);

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                vector<string> fila;
                for (int i = 0; i < (int)config.campos.size(); i++) {
                    const unsigned char* valor = sqlite3_column_text(stmt, i);
                    fila.push_back(valor ? (const char*)valor : "");
                }
                filas.push_back(fila);
            }
        } else {
            mensaje = "No se pudo listar " + config.titulo;
        }
        sqlite3_finalize(stmt);
        con.cerrar();
    }

    string consultaListado(const ConfigTabla& config) {
        if (config.tabla == "facturacion") {
            return "SELECT c.direccion, p.id, p.idCliente, p.fechaPedido, d.idVariedad, d.cantidad, d.precioUnitario, c.metodoPago "
                   "FROM pedidos p "
                   "LEFT JOIN clientes c ON c.id = p.idCliente "
                   "LEFT JOIN detallePedido d ON d.idPedido = p.id "
                   "ORDER BY p.id";
        }

        if (config.tabla == "stockVenta") {
            return "SELECT sv.idVariedad, v.nombreVariedad, "
                   "COALESCE((SELECT SUM(lp.cantidadProducida) FROM lotesProduccion lp WHERE lp.idVariedad = sv.idVariedad), 0) - "
                   "COALESCE((SELECT SUM(dp.cantidad * 12) FROM detallePedido dp WHERE dp.idVariedad = sv.idVariedad), 0), "
                   "sv.precioUnitario "
                   "FROM stockVenta sv "
                   "LEFT JOIN variedad v ON v.id = sv.idVariedad "
                   "WHERE sv.id = (SELECT MAX(sv2.id) FROM stockVenta sv2 WHERE sv2.idVariedad = sv.idVariedad) "
                   "ORDER BY sv.idVariedad";
        }

        string sql = "SELECT ";
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (i > 0) sql += ", ";
            sql += config.campos[i].columna;
        }
        sql += " FROM " + config.tabla + " ORDER BY 1";
        return sql;
    }

    void guardarRegistro() {
        if (!validarReglasDelPanel()) return;
        if (claveSeleccionada.empty()) insertarRegistro();
        else actualizarRegistro();
        cargarDatos();
    }

    bool validarReglasDelPanel() {
    const ConfigTabla& config = tablas[tablaActual];
    for (int i = 0; i < (int)config.campos.size(); i++) {
        string valor = trim(formulario[i].getValor());
        
        // Validar tipos según el campo
        if (!valor.empty()) {
            string columna = config.campos[i].columna;
            
            // Validar fechas (formato dd/mm/yyyy)
            if (columna == "fecha" || columna == "fechaPedido") {
                if (!esFechaValida(valor)) {
                    mensaje = "Fecha inválida. Use formato dd/mm/yyyy";
                    return false;
                }
            }
            // Validar números
            else if (esCampoNumerico(columna) && !esNumero(valor)) {
                mensaje = config.campos[i].etiqueta + " debe ser numerico";
                return false;
            }
        }
        
        if (valor.empty() && config.campos[i].clave && !config.campos[i].autoIncremental) {
            mensaje = "Complete " + config.campos[i].etiqueta;
            return false;
        }
    }

    if (!validarRelaciones(config)) return false;

    if (config.tabla != "usuario") return true;

    int indiceCategoria = indiceCampo("idCategoria");
    int categoriaNueva = indiceCategoria >= 0 ? atoi(formulario[indiceCategoria].getValor().c_str()) : 3;
    int categoriaActual = categoriaSeleccionadaUsuario();

    if (esAdmin()) {
        if (categoriaNueva == 1) {
            mensaje = "Un admin no puede asignar rol Duenio";
            return false;
        }
        if (!claveSeleccionada.empty() && categoriaActual != 3) {
            mensaje = "Un admin solo puede cambiar usuarios empleados";
            return false;
        }
    }

    return true;
    }

    bool validarRelaciones(const ConfigTabla& config) {
        if (!validarRelacion(config, "idPedido", "pedidos", "id", "pedido")) return false;
        if (!validarRelacion(config, "idCliente", "clientes", "id", "cliente")) return false;
        if (!validarRelacion(config, "idVariedad", "variedad", "id", "variedad")) return false;
        if (!validarRelacion(config, "idInsumo", "insumos", "id", "insumo")) return false;
        if (!validarRelacion(config, "idReceta", "receta", "id", "receta")) return false;  // ✅ ESTA LÍNEA YA ESTABA
        if (!validarRelacion(config, "idLote", "lotesProduccion", "id", "lote de produccion")) return false;
        if (!validarRelacion(config, "idLoteProduccion", "lotesProduccion", "id", "lote de produccion")) return false;
        if (!validarRelacion(config, "idVehiculo", "vehiculos", "id", "vehiculo")) return false;
        if (!validarRelacion(config, "idEmpleado", "usuario", "DNI", "empleado")) return false;
        if (!validarRelacion(config, "idCategoria", "categoriaUsuario", "id", "categoria")) return false;
        return true;
    }

    bool validarRelacion(const ConfigTabla& config, const string& columna, const string& tablaRef, const string& columnaRef, const string& nombre) {
        int indice = indiceCampo(columna);
        if (indice < 0) return true;
        string valor = trim(formulario[indice].getValor());
        if (valor.empty()) return true;
        if (!existeValor(tablaRef, columnaRef, valor)) {
            mensaje = "No existe " + nombre + " con ID " + valor;
            return false;
        }
        return true;
    }

    bool existeValor(const string& tabla, const string& columna, const string& valor) {
        Conexion con("medialunas_pro");
        if (!con.conectar()) return false;
        string sql = "SELECT 1 FROM " + tabla + " WHERE " + columna + "=? LIMIT 1";
        sqlite3_stmt* stmt = nullptr;
        bool existe = false;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            bindTexto(stmt, 1, valor);
            existe = sqlite3_step(stmt) == SQLITE_ROW;
        }
        sqlite3_finalize(stmt);
        con.cerrar();
        return existe;
    }

    string trim(const string& valor) {
        size_t inicio = 0;
        while (inicio < valor.size() && isspace((unsigned char)valor[inicio])) inicio++;
        size_t fin = valor.size();
        while (fin > inicio && isspace((unsigned char)valor[fin - 1])) fin--;
        return valor.substr(inicio, fin - inicio);
    }

    bool esNumero(const string& valor) {
        bool digito = false;
        bool punto = false;
        for (int i = 0; i < (int)valor.size(); i++) {
            char c = valor[i];
            if ((c == '-' || c == '+') && i == 0) continue;
            if (c == '.' || c == ',') {
                if (punto) return false;
                punto = true;
                continue;
            }
            if (!isdigit((unsigned char)c)) return false;
            digito = true;
        }
        return digito;
    }

    bool esFechaValida(const string& fecha) {
        if (fecha.size() != 10) return false;
        if (fecha[2] != '/' || fecha[5] != '/') return false;
        
        string dia = fecha.substr(0, 2);
        string mes = fecha.substr(3, 2);
        string anio = fecha.substr(6, 4);
        
        // Verificar que sean números
        for (char c : dia) if (!isdigit(c)) return false;
        for (char c : mes) if (!isdigit(c)) return false;
        for (char c : anio) if (!isdigit(c)) return false;
        
        int d = atoi(dia.c_str());
        int m = atoi(mes.c_str());
        int a = atoi(anio.c_str());
        
        if (m < 1 || m > 12) return false;
        if (d < 1 || d > 31) return false;
        if (a < 1900 || a > 2100) return false;
        
        return true;
    }

    bool esCampoNumerico(const string& columna) {
    return columna == "DNI" || columna == "id" || columna == "idCliente" || columna == "idPedido" ||
           columna == "idVariedad" || columna == "idInsumo" || columna == "idReceta" ||
           columna == "idLote" || columna == "idLoteProduccion" || columna == "idVehiculo" ||
           columna == "idEmpleado" || columna == "idCategoria" ||
           columna == "precioUnitario" || columna == "costoUnitario" || columna == "ordenEntrega";
    }

    bool aceptaNull(const string& columna) {
        return columna != "DNI" && columna != "idPedido" && columna != "idVariedad" &&
               columna != "idReceta" && columna != "idInsumo" && esCampoNumerico(columna);
    }

    int indiceCampo(const string& columna) {
        const ConfigTabla& config = tablas[tablaActual];
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (config.campos[i].columna == columna) return i;
        }
        return -1;
    }

    int categoriaSeleccionadaUsuario() {
        if (filas.empty() || claveSeleccionada.empty()) return 3;
        const ConfigTabla& config = tablas[tablaActual];
        int indiceCategoria = indiceCampo("idCategoria");
        if (indiceCategoria < 0) return 3;
        for (const vector<string>& fila : filas) {
            if (coincideClave(fila, config)) return atoi(fila[indiceCategoria].c_str());
        }
        return 3;
    }

    void insertarRegistro() {
        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        const ConfigTabla& config = tablas[tablaActual];
        vector<int> indices;
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (!config.campos[i].autoIncremental) indices.push_back(i);
        }

        string sql = "INSERT INTO " + config.tabla + " (";
        for (int i = 0; i < (int)indices.size(); i++) {
            if (i > 0) sql += ", ";
            sql += config.campos[indices[i]].columna;
        }
        sql += ") VALUES (";
        for (int i = 0; i < (int)indices.size(); i++) {
            if (i > 0) sql += ", ";
            sql += "?";
        }
        sql += ")";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            for (int i = 0; i < (int)indices.size(); i++) bindValor(stmt, i + 1, config.campos[indices[i]], formulario[indices[i]].getValor());
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                // NUEVO: Si es un lote de producción, crear stock automáticamente
                if (config.tabla == "lotesProduccion") {
                    crearStockAutomatico(con.getDB());
                }
                limpiarFormulario();
                mensaje = "Registro guardado";
            } else {
                mensaje = "No se pudo guardar: " + string(sqlite3_errmsg(con.getDB()));
            }
        } else {
            mensaje = "Error preparando guardado: " + string(sqlite3_errmsg(con.getDB()));
        }
        sqlite3_finalize(stmt);
        mensaje = "Stock creado. Ir a 'Stock publico' para asignar precio por docena";
        con.cerrar();
    }

    void crearStockAutomatico(sqlite3* db) {
        // Obtener el último lote creado
        string sqlUltimoLote = "SELECT id, idVariedad, cantidadProducida FROM lotesProduccion ORDER BY id DESC LIMIT 1";
        sqlite3_stmt* stmtLote = nullptr;
        
        int idLote = 0;
        int idVariedad = 0;
        string cantidad = "0";
        
        if (sqlite3_prepare_v2(db, sqlUltimoLote.c_str(), -1, &stmtLote, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmtLote) == SQLITE_ROW) {
                idLote = sqlite3_column_int(stmtLote, 0);
                idVariedad = sqlite3_column_int(stmtLote, 1);
                const unsigned char* cant = sqlite3_column_text(stmtLote, 2);
                cantidad = cant ? (const char*)cant : "0";
            }
        }
        sqlite3_finalize(stmtLote);
        
        if (idLote == 0 || idVariedad == 0) return;
        
        // Verificar si ya existe stock para esta variedad
        string sqlExiste = "SELECT id FROM stockVenta WHERE idVariedad = ?";
        sqlite3_stmt* stmtExiste = nullptr;
        bool yaExiste = false;
        
        if (sqlite3_prepare_v2(db, sqlExiste.c_str(), -1, &stmtExiste, NULL) == SQLITE_OK) {
            bindTexto(stmtExiste, 1, to_string(idVariedad));
            yaExiste = sqlite3_step(stmtExiste) == SQLITE_ROW;
        }
        sqlite3_finalize(stmtExiste);
        
        if (yaExiste) {
            // Si ya existe, solo actualizar cantidad y lote
            string sqlUpdate = "UPDATE stockVenta SET stockDisponible = ?, idLote = ? WHERE idVariedad = ?";
            sqlite3_stmt* stmtUpdate = nullptr;
            if (sqlite3_prepare_v2(db, sqlUpdate.c_str(), -1, &stmtUpdate, NULL) == SQLITE_OK) {
                bindTexto(stmtUpdate, 1, cantidad);
                bindTexto(stmtUpdate, 2, to_string(idLote));
                bindTexto(stmtUpdate, 3, to_string(idVariedad));
                sqlite3_step(stmtUpdate);
            }
            sqlite3_finalize(stmtUpdate);
        } else {
            // Si no existe, crear nuevo registro con precio inicial en 0
            string sqlInsert = "INSERT INTO stockVenta (idVariedad, stockDisponible, precioUnitario, idLote) VALUES (?, ?, 0, ?)";
            sqlite3_stmt* stmtInsert = nullptr;
            if (sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmtInsert, NULL) == SQLITE_OK) {
                bindTexto(stmtInsert, 1, to_string(idVariedad));
                bindTexto(stmtInsert, 2, cantidad);
                bindTexto(stmtInsert, 3, to_string(idLote));
                sqlite3_step(stmtInsert);
            }
            sqlite3_finalize(stmtInsert);
        } 
    }

    void actualizarRegistro() {
        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        const ConfigTabla& config = tablas[tablaActual];
        vector<int> editables;
        vector<int> claves;
        
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (config.campos[i].clave) {
                claves.push_back(i);
            } else if (config.campos[i].columna != "variedad") {  // Excluir campos que vienen de JOINs
                editables.push_back(i);
            }
        }

        string sql = "UPDATE " + config.tabla + " SET ";
        for (int i = 0; i < (int)editables.size(); i++) {
            if (i > 0) sql += ", ";
            sql += config.campos[editables[i]].columna + "=?";
        }
        sql += " WHERE " + condicionClaves(config);

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            int pos = 1;
            for (int indice : editables) {
                bindValor(stmt, pos++, config.campos[indice], formulario[indice].getValor());
            }
            for (string clave : claveSeleccionada) {
                bindTexto(stmt, pos++, clave);
            }
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                limpiarFormulario();
                mensaje = "Registro actualizado";
            } else {
                mensaje = "No se pudo actualizar: " + string(sqlite3_errmsg(con.getDB()));
            }
        } else {
            mensaje = "Error preparando actualizacion: " + string(sqlite3_errmsg(con.getDB()));
        }
        sqlite3_finalize(stmt);
        con.cerrar();
    }
    void eliminarRegistro() {
        if (claveSeleccionada.empty()) {
            mensaje = "Seleccione un registro para eliminar";
            return;
        }

        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        const ConfigTabla& config = tablas[tablaActual];
        string sql = "DELETE FROM " + config.tabla + " WHERE " + condicionClaves(config);

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            for (int i = 0; i < (int)claveSeleccionada.size(); i++) bindTexto(stmt, i + 1, claveSeleccionada[i]);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                limpiarFormulario();
                mensaje = "Registro eliminado";
            } else {
                mensaje = "No se pudo eliminar: " + string(sqlite3_errmsg(con.getDB()));
            }
        } else {
            mensaje = "Error preparando eliminacion: " + string(sqlite3_errmsg(con.getDB()));
        }
        sqlite3_finalize(stmt);
        con.cerrar();
        cargarDatos();
    }

    void cargarRegistroSeleccionado() {
        Conexion con("medialunas_pro");
        if (!con.conectar()) return;
        const ConfigTabla& config = tablas[tablaActual];
        string sql = "SELECT * FROM " + config.tabla;
        if (config.tabla == "stockVenta") sql = "SELECT s.*, v.nombreVariedad as variedad FROM stockVenta s LEFT JOIN variedad v ON s.idVariedad = v.id";
        sql += " WHERE " + condicionClaves(config);

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                for (int i = 0; i < (int)config.campos.size(); i++) {
                    string valor;
                    const unsigned char* raw = sqlite3_column_text(stmt, i);
                    if (raw) {
                        valor = (const char*)raw;
                        string col = config.campos[i].columna;
                        if ((col == "fecha" || col == "fechaPedido") &&
                            valor.size() == 10 && valor[4] == '-' && valor[7] == '-') {
                            valor = valor.substr(8, 2) + "/" + valor.substr(5, 2) + "/" + valor.substr(0, 4);
                        }
                    }
                    formulario[i].setValor(valor);
                }
            }
        }
        sqlite3_finalize(stmt);
        con.cerrar();
    }

    string condicionClaves(const ConfigTabla& config) {
        string condicion = "";
        int pos = 0;
        for (const CampoTabla& campo : config.campos) {
            if (!campo.clave) continue;
            if (pos > 0) condicion += " AND ";
            condicion += campo.columna + "=?";
            pos++;
        }
        return condicion;
    }

    void bindTexto(sqlite3_stmt* stmt, int posicion, const string& valor) {
        sqlite3_bind_text(stmt, posicion, valor.c_str(), -1, SQLITE_TRANSIENT);
    }

    void bindValor(sqlite3_stmt* stmt, int posicion, const CampoTabla& campo, const string& valor) {
    string limpio = trim(valor);
    
    // Convertir fecha dd/mm/yyyy a yyyy-mm-dd para MySQL
    if ((campo.columna == "fecha" || campo.columna == "fechaPedido") && !limpio.empty()) {
        if (limpio.size() == 10 && limpio[2] == '/' && limpio[5] == '/') {
            string dia = limpio.substr(0, 2);
            string mes = limpio.substr(3, 2);
            string anio = limpio.substr(6, 4);
            limpio = anio + "-" + mes + "-" + dia;
        }
    }
    
    if (limpio.empty() && aceptaNull(campo.columna)) {
        sqlite3_bind_null(stmt, posicion);
        return;
    }
    sqlite3_bind_text(stmt, posicion, limpio.c_str(), -1, SQLITE_TRANSIENT);
    }

    void iniciarSesion() {
        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        string sql = "SELECT DNI, nombre, idCategoria, sector FROM usuario WHERE cuenta=? AND clave=?";
        sqlite3_stmt* stmt = nullptr;
        bool ok = false;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            bindTexto(stmt, 1, loginCuenta.getValor());
            bindTexto(stmt, 2, loginClave.getValor());
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                usuarioDNI = sqlite3_column_int(stmt, 0);
                usuarioNombre = textoColumna(stmt, 1);
                usuarioCategoria = sqlite3_column_int(stmt, 2);
                usuarioSector = textoColumna(stmt, 3);
                if (usuarioSector.empty()) usuarioSector = "Ventas";
                ok = true;
            }
        }
        sqlite3_finalize(stmt);
        con.cerrar();

        if (ok) {
            pantallaAuth = SISTEMA;
            tablaActual = primerPanelVisible();
            configurarFormulario();
            cargarDatos();
            mensaje = "Sesion iniciada";
        } else {
            mensaje = "Usuario o contrasena incorrectos";
        }
    }

    void registrarUsuario() {
        if (regDNI.getValor().empty() || regNombre.getValor().empty() || regApellido.getValor().empty() || regCuenta.getValor().empty() || regClave.getValor().empty()) {
            mensaje = "Complete DNI, nombre, apellido, usuario y contrasena";
            return;
        }

        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        string sql = "INSERT INTO usuario (DNI, nombre, apellido, direccion, telefono, email, contactoEmergencia, nombreCE, idCategoria, sector, cuenta, clave) VALUES (?, ?, ?, '', ?, ?, '', '', 3, 'Ventas', ?, ?)";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(con.getDB(), sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            bindTexto(stmt, 1, regDNI.getValor());
            bindTexto(stmt, 2, regNombre.getValor());
            bindTexto(stmt, 3, regApellido.getValor());
            bindTexto(stmt, 4, regTelefono.getValor());
            bindTexto(stmt, 5, regEmail.getValor());
            bindTexto(stmt, 6, regCuenta.getValor());
            bindTexto(stmt, 7, regClave.getValor());
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                mensaje = "Usuario creado como empleado. Ya puede iniciar sesion";
                limpiarRegistro();
                pantallaAuth = LOGIN;
            } else {
                mensaje = "No se pudo crear usuario: " + string(sqlite3_errmsg(con.getDB()));
            }
        } else {
            mensaje = "Error creando usuario: " + string(sqlite3_errmsg(con.getDB()));
        }
        sqlite3_finalize(stmt);
        con.cerrar();
    }

    void limpiarRegistro() {
        regDNI.limpiar();
        regNombre.limpiar();
        regApellido.limpiar();
        regEmail.limpiar();
        regTelefono.limpiar();
        regCuenta.limpiar();
        regClave.limpiar();
    }

    string textoColumna(sqlite3_stmt* stmt, int columna) {
        const unsigned char* valor = sqlite3_column_text(stmt, columna);
        return valor ? (const char*)valor : "";
    }

    void cerrarSesion() {
        pantallaAuth = INICIO;
        usuarioDNI = 0;
        usuarioCategoria = 0;
        usuarioNombre = "";
        usuarioSector = "";
        tablaActual = 0;
        filas.clear();
        formulario.clear();
        claveSeleccionada.clear();
        scrollTabla = 0;
        scrollMenu = 0;
        loginCuenta.limpiar();
        loginClave.limpiar();
        mensaje = "Sesion cerrada";
    }

    void inicializarBase() {
        Conexion con("medialunas_pro");
        if (!con.conectar()) {
            mensaje = "No se pudo conectar a la base de datos";
            return;
        }
        ejecutarSqlArchivo(con.getDB());
        sqlite3_exec(con.getDB(), "DELETE FROM detallePedido WHERE NOT EXISTS (SELECT 1 FROM pedidos WHERE pedidos.id = detallePedido.idPedido)", NULL, NULL, NULL);
        con.cerrar();
    }

    void ejecutarSqlArchivo(sqlite3* db) {
        vector<string> rutas = {"baseDatos/medialunas_mysql.sql", "../baseDatos/medialunas_mysql.sql"};
        for (string ruta : rutas) {
            ifstream archivo(ruta);
            if (!archivo.good()) continue;
            stringstream buffer;
            buffer << archivo.rdbuf();
            sqlite3_exec(db, buffer.str().c_str(), NULL, NULL, NULL);
            return;
        }
    }

    void dibujar() {
        BeginDrawing();
        ClearBackground(fondo);
        if (pantallaAuth == INICIO) dibujarInicio();
        if (pantallaAuth == LOGIN) dibujarLogin();
        if (pantallaAuth == REGISTRO) dibujarRegistro();
        if (pantallaAuth == SISTEMA) dibujarSistema();
        EndDrawing();
    }

    void dibujarInicio() {
        DrawText("Medialunas Pro", 395, 150, 48, primario);
        DrawText("Bienvenido", 490, 220, 28, texto);
        DrawText("Tenes cuenta?", 442, 318, 20, texto);
        DrawText("No tenes cuenta?", 612, 318, 20, texto);
        Boton iniciar("Inicio de sesion", {420, 370, 170, 44});
        Boton crear("Crear una", {610, 370, 170, 44});
        iniciar.dibujar(primario, primarioOscuro, WHITE);
        crear.dibujar(verde, Color{54, 114, 76, 255}, WHITE);
        DrawText(mensaje.c_str(), 430, 455, 18, primarioOscuro);
    }

    void dibujarLogin() {
        DrawText("Inicio de sesion", 420, 170, 38, primario);
        DrawRectangle(385, 235, 430, 320, panel);
        DrawRectangleLinesEx({385, 235, 430, 320}, 1, suave);
        loginCuenta.dibujar(primario, texto);
        loginClave.dibujar(primario, texto, true);
        Boton ingresar("Ingresar", {500, 430, 200, 42});
        Boton volver("Volver", {500, 485, 200, 38});
        ingresar.dibujar(primario, primarioOscuro, WHITE);
        volver.dibujar(suave, Color{215, 190, 155, 255}, texto);
        DrawText(mensaje.c_str(), 430, 590, 18, mensaje.find("incorrect") != string::npos ? rojo : primarioOscuro);
    }

    void dibujarRegistro() {
        DrawText("Crear cuenta de empleado", 350, 130, 36, primario);
        DrawRectangle(280, 175, 560, 430, panel);
        DrawRectangleLinesEx({280, 175, 560, 430}, 1, suave);
        regDNI.dibujar(primario, texto);
        regNombre.dibujar(primario, texto);
        regApellido.dibujar(primario, texto);
        regEmail.dibujar(primario, texto);
        regTelefono.dibujar(primario, texto);
        regCuenta.dibujar(primario, texto);
        regClave.dibujar(primario, texto, true);
        Boton crear("Crear usuario", {445, 500, 230, 42});
        Boton volver("Volver", {445, 555, 230, 38});
        crear.dibujar(verde, Color{54, 114, 76, 255}, WHITE);
        volver.dibujar(suave, Color{215, 190, 155, 255}, texto);
        DrawText(mensaje.c_str(), 305, 640, 18, mensaje.find("No se") != string::npos ? rojo : primarioOscuro);
    }

    void dibujarSistema() {
        dibujarBarraLateral();
        dibujarEncabezado();
        dibujarFormulario();
        dibujarTabla();
        dibujarAcciones();
    }

    void dibujarBarraLateral() {
        DrawRectangle(0, 0, 205, 760, primarioOscuro);
        DrawText("Medialunas", 18, 24, 23, WHITE);
        DrawText("Pro", 18, 52, 23, Color{255, 219, 166, 255});

        BeginScissorMode(0, 92, 205, 590);
        int visible = 0;
        for (int i = 0; i < (int)tablas.size(); i++) {
            if (!puedeVerPanel(i)) continue;
            bool activo = i == tablaActual;
            Boton boton(tablas[i].titulo, {18, 108.0f + visible * 43.0f - scrollMenu, 172, 34});
            boton.dibujar(activo ? primario : Color{151, 91, 55, 255}, primario, WHITE, 14);
            visible++;
        }
        EndScissorMode();
    }

    void dibujarEncabezado() {
        DrawText(tablas[tablaActual].titulo.c_str(), 220, 28, 30, texto);
        string usuario = "Usuario: " + (usuarioNombre.empty() ? loginCuenta.getValor() : usuarioNombre) + " | Rol " + to_string(usuarioCategoria) + " | Sector " + usuarioSector;
        DrawText(usuario.c_str(), 220, 62, 17, primarioOscuro);
        DrawText(mensaje.c_str(), 690, 62, 17, primarioOscuro);
    }

    void dibujarFormulario() {
        const ConfigTabla& config = tablas[tablaActual];
        for (int i = 0; i < (int)formulario.size(); i++) {
            formulario[i].dibujar(primario, texto, config.campos[i].ocultar, campoEditable(config.campos[i]));
        }
    }

    void dibujarTabla() {
        DrawRectangle(220, 345, 920, 280, panel);  // Cambié de 338 a 280 (más pequeña)
        DrawRectangleLinesEx({220, 345, 920, 280}, 1, suave);
        DrawRectangle(220, 345, 920, 34, suave);

        const ConfigTabla& config = tablas[tablaActual];
        int ancho = max(70, 900 / max(1, (int)config.campos.size()));
        for (int i = 0; i < (int)config.campos.size(); i++) {
            DrawText(config.campos[i].etiqueta.c_str(), 230 + i * ancho, 355, 13, texto);
        }

        BeginScissorMode(220, 381, 920, 244);  // Cambié de 299 a 244
        for (int f = 0; f < (int)filas.size(); f++) {
            int y = 381 + f * 32 - scrollTabla;
            bool seleccionada = !claveSeleccionada.empty() && coincideClave(filas[f], config);
            if (filaEnMinimo(filas[f], config)) DrawRectangle(220, y, 920, 30, Color{255, 214, 205, 255});
            if (seleccionada) DrawRectangle(220, y, 920, 30, Color{255, 229, 190, 255});
            DrawLine(220, y + 30, 1140, y + 30, suave);
            for (int c = 0; c < (int)filas[f].size(); c++) {
                string valor = filas[f][c];
                if (config.campos[c].ocultar && !valor.empty()) valor = "********";
                while (MeasureText(valor.c_str(), 13) > ancho - 10 && valor.size() > 1) valor.pop_back();
                DrawText(valor.c_str(), 230 + c * ancho, y + 8, 13, texto);
            }
        }
        EndScissorMode();
    }

    bool filaEnMinimo(const vector<string>& fila, const ConfigTabla& config) {
        if (config.tabla != "insumos" && config.tabla != "loteInsumos") return false;
        int cantidad = -1;
        int minimo = -1;
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (config.campos[i].columna == "cantidad" || config.campos[i].columna == "cantidadActual") cantidad = i;
            if (config.campos[i].columna == "stockMinimo") minimo = i;
        }
        if (cantidad < 0 || minimo < 0 || cantidad >= (int)fila.size() || minimo >= (int)fila.size()) return false;
        return atof(fila[minimo].c_str()) > 0 && atof(fila[cantidad].c_str()) <= atof(fila[minimo].c_str());
    }

    bool coincideClave(const vector<string>& fila, const ConfigTabla& config) {
        int pos = 0;
        for (int i = 0; i < (int)config.campos.size(); i++) {
            if (!config.campos[i].clave) continue;
            if (pos >= (int)claveSeleccionada.size() || fila[i] != claveSeleccionada[pos]) return false;
            pos++;
        }
        return true;
    }

    void dibujarAcciones() {
        Boton nuevo("Crear", {220, 292, 115, 38});
        Boton guardar(claveSeleccionada.empty() ? "Guardar" : "Actualizar", {350, 292, 125, 38});
        Boton eliminar("Borrar", {490, 292, 125, 38});
        Boton salir("Cerrar sesion", {1035, 292, 135, 38});
        bool crear = puedeCrear();
        bool guardarOk = claveSeleccionada.empty() ? puedeCrear() : puedeActualizar();
        bool borrar = puedeEliminar();
        nuevo.dibujar(crear ? suave : Color{190, 184, 176, 255}, crear ? Color{215, 190, 155, 255} : Color{190, 184, 176, 255}, texto);
        guardar.dibujar(guardarOk ? verde : Color{150, 156, 150, 255}, guardarOk ? Color{54, 114, 76, 255} : Color{150, 156, 150, 255}, WHITE);
        eliminar.dibujar(borrar ? rojo : Color{158, 145, 142, 255}, borrar ? Color{144, 44, 42, 255} : Color{158, 145, 142, 255}, WHITE);
        salir.dibujar(primarioOscuro, primario, WHITE);
    }
};

void ejecutarInterfazGrafica() {
    MedialunasApp app;
    app.ejecutar();
}