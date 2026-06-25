#include "raylib.h"
#include "DAO implementacion/clienteDAOImple.h"
#include "DAO implementacion/insumosDAOImple.h"
#include "DAO implementacion/pedidosDAOimple.h"
#include "DAO implementacion/stockVentaDAOimple.h"
#include "DAO implementacion/variedadDAOimple.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

enum Pantalla {
    LOGIN,
    CLIENTES,
    PEDIDOS,
    VARIEDADES,
    INSUMOS,
    STOCK
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
        maximo = 40;
    }

    CampoTexto(string etiqueta, Rectangle area, int maximo = 40) {
        this->etiqueta = etiqueta;
        this->area = area;
        this->maximo = maximo;
        valor = "";
        activo = false;
    }

    void procesar(Vector2 mouse) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            activo = CheckCollisionPointRec(mouse, area);
        }

        if (!activo) return;

        int tecla = GetCharPressed();
        while (tecla > 0) {
            if (tecla >= 32 && tecla <= 126 && (int)valor.size() < maximo) {
                valor += (char)tecla;
            }
            tecla = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !valor.empty()) {
            valor.pop_back();
        }
    }

    void dibujar(Color borde, Color texto, bool ocultar = false) {
        DrawText(etiqueta.c_str(), area.x, area.y - 22, 18, texto);
        DrawRectangleRec(area, WHITE);
        DrawRectangleLinesEx(area, activo ? 2 : 1, activo ? borde : Color{190, 180, 165, 255});

        string visible = ocultar ? string(valor.size(), '*') : valor;
        if (visible.size() > 28) {
            visible = visible.substr(visible.size() - 28);
        }
        DrawText(visible.c_str(), area.x + 10, area.y + 10, 18, texto);
    }

    string getValor() { return valor; }
    int getEntero() { return valor.empty() ? 0 : atoi(valor.c_str()); }
    float getDecimal() { return valor.empty() ? 0.0f : (float)atof(valor.c_str()); }
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

    void dibujar(Color base, Color hover, Color textoColor) {
        Vector2 mouse = GetMousePosition();
        Color color = CheckCollisionPointRec(mouse, area) ? hover : base;
        DrawRectangleRec(area, color);
        DrawRectangleLinesEx(area, 1, Color{100, 70, 45, 255});
        int anchoTexto = MeasureText(texto.c_str(), 18);
        DrawText(texto.c_str(), area.x + (area.width - anchoTexto) / 2, area.y + 11, 18, textoColor);
    }
};

class MedialunasApp {
private:
    Pantalla pantalla;
    ClienteDAOImpl clienteDAO;
    PedidosDAOImpl pedidosDAO;
    variedadDAOImpl variedadDAO;
    InsumosDAOImpl insumosDAO;
    stockVentaDAOImpl stockDAO;

    vector<Cliente> clientes;
    vector<Pedidos> pedidos;
    vector<variedad> variedades;
    vector<Insumos> insumos;
    vector<StockVenta> stocks;

    CampoTexto loginUsuario;
    CampoTexto loginClave;
    vector<CampoTexto> formulario;
    int idSeleccionado;
    string mensaje;
    int scroll;

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
        pantalla = LOGIN;
        loginUsuario = CampoTexto("Usuario", {310, 250, 280, 40}, 24);
        loginClave = CampoTexto("Contrasena", {310, 325, 280, 40}, 24);
        idSeleccionado = 0;
        mensaje = "Ingrese con admin / 1234";
        scroll = 0;
    }

    void ejecutar() {
        InitWindow(1100, 700, "Medialunas Pro - Gestion");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            Vector2 mouse = GetMousePosition();
            procesar(mouse);
            dibujar();
        }

        CloseWindow();
    }

private:
    void procesar(Vector2 mouse) {
        if (pantalla == LOGIN) {
            procesarLogin(mouse);
            return;
        }

        if (GetMouseWheelMove() != 0) {
            scroll -= (int)GetMouseWheelMove() * 24;
            scroll = max(0, scroll);
        }

        procesarMenu(mouse);
        for (CampoTexto& campo : formulario) campo.procesar(mouse);
        procesarAcciones(mouse);
    }

    void procesarLogin(Vector2 mouse) {
        loginUsuario.procesar(mouse);
        loginClave.procesar(mouse);
        Boton ingresar("Ingresar", {350, 405, 200, 42});

        if (ingresar.presionado(mouse) || IsKeyPressed(KEY_ENTER)) {
            if (loginUsuario.getValor() == "admin" && loginClave.getValor() == "1234") {
                cambiarPantalla(CLIENTES);
                mensaje = "Sesion iniciada";
            } else {
                mensaje = "Usuario o contrasena incorrectos";
            }
        }
    }

    void procesarMenu(Vector2 mouse) {
        vector<Pantalla> pantallas = {CLIENTES, PEDIDOS, VARIEDADES, INSUMOS, STOCK};
        vector<string> nombres = {"Clientes", "Pedidos", "Variedades", "Insumos", "Stock"};

        for (int i = 0; i < (int)pantallas.size(); i++) {
            Boton boton(nombres[i], {22, 118.0f + i * 52.0f, 150, 40});
            if (boton.presionado(mouse)) cambiarPantalla(pantallas[i]);
        }
    }

    void procesarAcciones(Vector2 mouse) {
        Boton nuevo("Nuevo", {220, 606, 120, 40});
        Boton guardar("Guardar", {355, 606, 120, 40});
        Boton eliminar("Eliminar", {490, 606, 120, 40});

        if (nuevo.presionado(mouse)) limpiarFormulario();
        if (guardar.presionado(mouse)) guardarActual();
        if (eliminar.presionado(mouse)) eliminarActual();

        procesarSeleccion(mouse);
    }

    void procesarSeleccion(Vector2 mouse) {
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
        if (mouse.x < 220 || mouse.x > 1035 || mouse.y < 235 || mouse.y > 570) return;

        int fila = ((int)mouse.y - 235 + scroll) / 34;
        if (pantalla == CLIENTES && fila >= 0 && fila < (int)clientes.size()) cargarCliente(clientes[fila]);
        if (pantalla == PEDIDOS && fila >= 0 && fila < (int)pedidos.size()) cargarPedido(pedidos[fila]);
        if (pantalla == VARIEDADES && fila >= 0 && fila < (int)variedades.size()) cargarVariedad(variedades[fila]);
        if (pantalla == INSUMOS && fila >= 0 && fila < (int)insumos.size()) cargarInsumo(insumos[fila]);
        if (pantalla == STOCK && fila >= 0 && fila < (int)stocks.size()) cargarStock(stocks[fila]);
    }

    void cambiarPantalla(Pantalla nuevaPantalla) {
        pantalla = nuevaPantalla;
        idSeleccionado = 0;
        scroll = 0;
        configurarFormulario();
        cargarDatos();
    }

    void configurarFormulario() {
        formulario.clear();
        float x = 220;
        float y = 98;
        float w = 250;
        float h = 38;

        if (pantalla == CLIENTES) {
            formulario.push_back(CampoTexto("Nombre", {x, y, w, h}));
            formulario.push_back(CampoTexto("CUIT", {x + 270, y, w, h}));
            formulario.push_back(CampoTexto("Direccion", {x, y + 70, w, h}));
            formulario.push_back(CampoTexto("Telefono", {x + 270, y + 70, w, h}));
            formulario.push_back(CampoTexto("Metodo de pago", {x + 540, y, w, h}));
            formulario.push_back(CampoTexto("Categoria", {x + 540, y + 70, w, h}));
        }

        if (pantalla == PEDIDOS) {
            formulario.push_back(CampoTexto("ID cliente", {x, y, w, h}, 8));
            formulario.push_back(CampoTexto("Fecha", {x + 270, y, w, h}, 12));
            formulario.push_back(CampoTexto("Estado", {x + 540, y, w, h}));
        }

        if (pantalla == VARIEDADES) {
            formulario.push_back(CampoTexto("Nombre variedad", {x, y, w, h}));
            formulario.push_back(CampoTexto("Descripcion", {x + 270, y, 520, h}, 80));
        }

        if (pantalla == INSUMOS) {
            formulario.push_back(CampoTexto("Nombre", {x, y, w, h}));
            formulario.push_back(CampoTexto("Cantidad", {x + 270, y, w, h}, 12));
            formulario.push_back(CampoTexto("Unidad", {x + 540, y, w, h}));
            formulario.push_back(CampoTexto("Proveedor", {x, y + 70, w, h}));
        }

        if (pantalla == STOCK) {
            formulario.push_back(CampoTexto("ID variedad", {x, y, w, h}, 8));
            formulario.push_back(CampoTexto("Stock disponible", {x + 270, y, w, h}, 12));
            formulario.push_back(CampoTexto("Precio unitario", {x + 540, y, w, h}, 12));
            formulario.push_back(CampoTexto("ID lote", {x, y + 70, w, h}, 8));
        }
    }

    void cargarDatos() {
        clientes.clear();
        pedidos.clear();
        variedades.clear();
        insumos.clear();
        stocks.clear();

        if (pantalla == CLIENTES) clientes = clienteDAO.listar();
        if (pantalla == PEDIDOS) pedidos = pedidosDAO.listar();
        if (pantalla == VARIEDADES) variedades = variedadDAO.listar();
        if (pantalla == INSUMOS) insumos = insumosDAO.listar();
        if (pantalla == STOCK) stocks = stockDAO.listar();
    }

    void limpiarFormulario() {
        idSeleccionado = 0;
        for (CampoTexto& campo : formulario) campo.limpiar();
        mensaje = "Formulario limpio";
    }

    void guardarActual() {
        if (pantalla == CLIENTES) guardarCliente();
        if (pantalla == PEDIDOS) guardarPedido();
        if (pantalla == VARIEDADES) guardarVariedad();
        if (pantalla == INSUMOS) guardarInsumo();
        if (pantalla == STOCK) guardarStock();
        cargarDatos();
    }

    void eliminarActual() {
        if (idSeleccionado == 0) {
            mensaje = "Seleccione un registro para eliminar";
            return;
        }

        if (pantalla == CLIENTES) {
            Cliente obj;
            obj.setId(idSeleccionado);
            clienteDAO.eliminar(obj);
        }
        if (pantalla == PEDIDOS) {
            Pedidos obj;
            obj.setId(idSeleccionado);
            pedidosDAO.eliminar(obj);
        }
        if (pantalla == VARIEDADES) {
            variedad obj;
            obj.setId(idSeleccionado);
            variedadDAO.eliminar(obj);
        }
        if (pantalla == INSUMOS) {
            Insumos obj;
            obj.setId(idSeleccionado);
            insumosDAO.eliminar(obj);
        }
        if (pantalla == STOCK) {
            StockVenta obj;
            obj.setId(idSeleccionado);
            stockDAO.eliminar(obj);
        }

        limpiarFormulario();
        cargarDatos();
        mensaje = "Registro eliminado";
    }

    void guardarCliente() {
        Cliente obj;
        obj.setId(idSeleccionado);
        obj.setNombre(formulario[0].getValor());
        obj.setCUIT(formulario[1].getValor());
        obj.setDireccion(formulario[2].getValor());
        obj.setTelefono(formulario[3].getValor());
        obj.setMetodoPago(formulario[4].getValor());
        obj.setCategoria(formulario[5].getValor());
        idSeleccionado == 0 ? clienteDAO.insertar(obj) : clienteDAO.actualizar(obj);
        mensaje = "Cliente guardado";
    }

    void guardarPedido() {
        Pedidos obj;
        obj.setId(idSeleccionado);
        obj.setIdCliente(formulario[0].getEntero());
        obj.setFechaPedido(formulario[1].getValor());
        obj.setEstado(formulario[2].getValor());
        idSeleccionado == 0 ? pedidosDAO.insertar(obj) : pedidosDAO.actualizar(obj);
        mensaje = "Pedido guardado";
    }

    void guardarVariedad() {
        variedad obj;
        obj.setId(idSeleccionado);
        obj.setNombreVariedad(formulario[0].getValor());
        obj.setDescripcion(formulario[1].getValor());
        idSeleccionado == 0 ? variedadDAO.insertar(obj) : variedadDAO.actualizar(obj);
        mensaje = "Variedad guardada";
    }

    void guardarInsumo() {
        Insumos obj;
        obj.setId(idSeleccionado);
        obj.setNombre(formulario[0].getValor());
        obj.setCantidadActual(formulario[1].getDecimal());
        obj.setUnidadMedia(formulario[2].getValor());
        obj.setProveedor(formulario[3].getValor());
        idSeleccionado == 0 ? insumosDAO.insertar(obj) : insumosDAO.actualizar(obj);
        mensaje = "Insumo guardado";
    }

    void guardarStock() {
        StockVenta obj;
        obj.setId(idSeleccionado);
        obj.setIdVariedad(formulario[0].getEntero());
        obj.setStockDisponible(formulario[1].getDecimal());
        obj.setPrecioUnitario(formulario[2].getDecimal());
        obj.setIdLote(formulario[3].getEntero());
        idSeleccionado == 0 ? stockDAO.insertar(obj) : stockDAO.actualizar(obj);
        mensaje = "Stock guardado";
    }

    void cargarCliente(Cliente obj) {
        idSeleccionado = obj.getId();
        formulario[0].setValor(obj.getNombre());
        formulario[1].setValor(obj.getCUIT());
        formulario[2].setValor(obj.getDireccion());
        formulario[3].setValor(obj.getTelefono());
        formulario[4].setValor(obj.getMetodoPago());
        formulario[5].setValor(obj.getCategoria());
        mensaje = "Cliente seleccionado";
    }

    void cargarPedido(Pedidos obj) {
        idSeleccionado = obj.getId();
        formulario[0].setValor(to_string(obj.getIdCliente()));
        formulario[1].setValor(obj.getFechaPedido());
        formulario[2].setValor(obj.getEstado());
        mensaje = "Pedido seleccionado";
    }

    void cargarVariedad(variedad obj) {
        idSeleccionado = obj.getId();
        formulario[0].setValor(obj.getNombreVariedad());
        formulario[1].setValor(obj.getDescripcion());
        mensaje = "Variedad seleccionada";
    }

    void cargarInsumo(Insumos obj) {
        idSeleccionado = obj.getId();
        formulario[0].setValor(obj.getNombre());
        formulario[1].setValor(decimalAString(obj.getCantidadActual()));
        formulario[2].setValor(obj.getUnidadMedia());
        formulario[3].setValor(obj.getProveedor());
        mensaje = "Insumo seleccionado";
    }

    void cargarStock(StockVenta obj) {
        idSeleccionado = obj.getId();
        formulario[0].setValor(to_string(obj.getIdVariedad()));
        formulario[1].setValor(decimalAString(obj.getStockDisponible()));
        formulario[2].setValor(decimalAString(obj.getPrecioUnitario()));
        formulario[3].setValor(to_string(obj.getIdLote()));
        mensaje = "Stock seleccionado";
    }

    string decimalAString(float valor) {
        ostringstream salida;
        salida.precision(2);
        salida << fixed << valor;
        return salida.str();
    }

    void dibujar() {
        BeginDrawing();
        ClearBackground(fondo);

        if (pantalla == LOGIN) dibujarLogin();
        else dibujarSistema();

        EndDrawing();
    }

    void dibujarLogin() {
        DrawText("Medialunas Pro", 300, 105, 46, primario);
        DrawText("Sistema de gestion de medialunas", 300, 160, 20, texto);
        DrawRectangle(250, 200, 400, 300, panel);
        DrawRectangleLinesEx({250, 200, 400, 300}, 1, suave);

        loginUsuario.dibujar(primario, texto);
        loginClave.dibujar(primario, texto, true);

        Boton ingresar("Ingresar", {350, 405, 200, 42});
        ingresar.dibujar(primario, primarioOscuro, WHITE);
        DrawText(mensaje.c_str(), 315, 465, 18, mensaje.find("incorrect") != string::npos ? rojo : primarioOscuro);
    }

    void dibujarSistema() {
        dibujarBarraLateral();
        dibujarEncabezado();
        dibujarFormulario();
        dibujarTabla();
        dibujarBotonesAccion();
    }

    void dibujarBarraLateral() {
        DrawRectangle(0, 0, 190, 700, primarioOscuro);
        DrawText("Medialunas", 22, 28, 24, WHITE);
        DrawText("Pro", 22, 56, 24, Color{255, 219, 166, 255});

        vector<Pantalla> pantallas = {CLIENTES, PEDIDOS, VARIEDADES, INSUMOS, STOCK};
        vector<string> nombres = {"Clientes", "Pedidos", "Variedades", "Insumos", "Stock"};

        for (int i = 0; i < (int)pantallas.size(); i++) {
            bool activo = pantalla == pantallas[i];
            Boton boton(nombres[i], {22, 118.0f + i * 52.0f, 150, 40});
            boton.dibujar(activo ? primario : Color{151, 91, 55, 255}, primario, WHITE);
        }
    }

    void dibujarEncabezado() {
        DrawText(tituloPantalla().c_str(), 220, 28, 30, texto);
        string seleccionado = idSeleccionado == 0 ? "Nuevo registro" : "Editando ID " + to_string(idSeleccionado);
        DrawText(seleccionado.c_str(), 220, 62, 18, primarioOscuro);
        DrawText(mensaje.c_str(), 720, 62, 18, primarioOscuro);
    }

    void dibujarFormulario() {
        for (CampoTexto& campo : formulario) campo.dibujar(primario, texto);
    }

    void dibujarBotonesAccion() {
        Boton nuevo("Nuevo", {220, 606, 120, 40});
        Boton guardar("Guardar", {355, 606, 120, 40});
        Boton eliminar("Eliminar", {490, 606, 120, 40});
        nuevo.dibujar(suave, Color{215, 190, 155, 255}, texto);
        guardar.dibujar(verde, Color{54, 114, 76, 255}, WHITE);
        eliminar.dibujar(rojo, Color{144, 44, 42, 255}, WHITE);
    }

    void dibujarTabla() {
        DrawRectangle(220, 198, 815, 382, panel);
        DrawRectangleLinesEx({220, 198, 815, 382}, 1, suave);
        dibujarEncabezadoTabla();

        BeginScissorMode(220, 235, 815, 335);
        if (pantalla == CLIENTES) dibujarClientes();
        if (pantalla == PEDIDOS) dibujarPedidos();
        if (pantalla == VARIEDADES) dibujarVariedades();
        if (pantalla == INSUMOS) dibujarInsumos();
        if (pantalla == STOCK) dibujarStock();
        EndScissorMode();
    }

    void dibujarEncabezadoTabla() {
        DrawRectangle(220, 198, 815, 34, suave);
        if (pantalla == CLIENTES) dibujarColumnas({"ID", "Nombre", "CUIT", "Telefono", "Pago", "Categoria"});
        if (pantalla == PEDIDOS) dibujarColumnas({"ID", "Cliente", "Fecha", "Estado"});
        if (pantalla == VARIEDADES) dibujarColumnas({"ID", "Nombre", "Descripcion"});
        if (pantalla == INSUMOS) dibujarColumnas({"ID", "Nombre", "Cantidad", "Unidad", "Proveedor"});
        if (pantalla == STOCK) dibujarColumnas({"ID", "Variedad", "Stock", "Precio", "Lote"});
    }

    void dibujarColumnas(vector<string> columnas) {
        int x = 230;
        for (string col : columnas) {
            DrawText(col.c_str(), x, 207, 16, texto);
            x += 135;
        }
    }

    void dibujarFilaBase(int y, int id) {
        if (id == idSeleccionado) DrawRectangle(220, y, 815, 32, Color{255, 229, 190, 255});
        DrawLine(220, y + 32, 1035, y + 32, suave);
    }

    void dibujarClientes() {
        for (int i = 0; i < (int)clientes.size(); i++) {
            int y = 235 + i * 34 - scroll;
            Cliente c = clientes[i];
            dibujarFilaBase(y, c.getId());
            DrawText(to_string(c.getId()).c_str(), 230, y + 8, 16, texto);
            DrawText(c.getNombre().substr(0, 18).c_str(), 365, y + 8, 16, texto);
            DrawText(c.getCUIT().substr(0, 15).c_str(), 500, y + 8, 16, texto);
            DrawText(c.getTelefono().substr(0, 15).c_str(), 635, y + 8, 16, texto);
            DrawText(c.getMetodoPago().substr(0, 14).c_str(), 770, y + 8, 16, texto);
            DrawText(c.getCategoria().substr(0, 14).c_str(), 905, y + 8, 16, texto);
        }
    }

    void dibujarPedidos() {
        for (int i = 0; i < (int)pedidos.size(); i++) {
            int y = 235 + i * 34 - scroll;
            Pedidos p = pedidos[i];
            dibujarFilaBase(y, p.getId());
            DrawText(to_string(p.getId()).c_str(), 230, y + 8, 16, texto);
            DrawText(to_string(p.getIdCliente()).c_str(), 365, y + 8, 16, texto);
            DrawText(p.getFechaPedido().c_str(), 500, y + 8, 16, texto);
            DrawText(p.getEstado().substr(0, 20).c_str(), 635, y + 8, 16, texto);
        }
    }

    void dibujarVariedades() {
        for (int i = 0; i < (int)variedades.size(); i++) {
            int y = 235 + i * 34 - scroll;
            variedad v = variedades[i];
            dibujarFilaBase(y, v.getId());
            DrawText(to_string(v.getId()).c_str(), 230, y + 8, 16, texto);
            DrawText(v.getNombreVariedad().substr(0, 22).c_str(), 365, y + 8, 16, texto);
            DrawText(v.getDescripcion().substr(0, 46).c_str(), 500, y + 8, 16, texto);
        }
    }

    void dibujarInsumos() {
        for (int i = 0; i < (int)insumos.size(); i++) {
            int y = 235 + i * 34 - scroll;
            Insumos in = insumos[i];
            dibujarFilaBase(y, in.getId());
            DrawText(to_string(in.getId()).c_str(), 230, y + 8, 16, texto);
            DrawText(in.getNombre().substr(0, 18).c_str(), 365, y + 8, 16, texto);
            DrawText(decimalAString(in.getCantidadActual()).c_str(), 500, y + 8, 16, texto);
            DrawText(in.getUnidadMedia().substr(0, 12).c_str(), 635, y + 8, 16, texto);
            DrawText(in.getProveedor().substr(0, 22).c_str(), 770, y + 8, 16, texto);
        }
    }

    void dibujarStock() {
        for (int i = 0; i < (int)stocks.size(); i++) {
            int y = 235 + i * 34 - scroll;
            StockVenta s = stocks[i];
            dibujarFilaBase(y, s.getId());
            DrawText(to_string(s.getId()).c_str(), 230, y + 8, 16, texto);
            DrawText(to_string(s.getIdVariedad()).c_str(), 365, y + 8, 16, texto);
            DrawText(decimalAString(s.getStockDisponible()).c_str(), 500, y + 8, 16, texto);
            DrawText(decimalAString(s.getPrecioUnitario()).c_str(), 635, y + 8, 16, texto);
            DrawText(to_string(s.getIdLote()).c_str(), 770, y + 8, 16, texto);
        }
    }

    string tituloPantalla() {
        if (pantalla == CLIENTES) return "Gestion de clientes";
        if (pantalla == PEDIDOS) return "Gestion de pedidos";
        if (pantalla == VARIEDADES) return "Variedades de medialunas";
        if (pantalla == INSUMOS) return "Control de insumos";
        if (pantalla == STOCK) return "Stock para venta";
        return "";
    }
};

void ejecutarInterfazGrafica() {
    MedialunasApp app;
    app.ejecutar();
}
