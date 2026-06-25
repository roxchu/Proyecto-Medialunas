cmake_minimum_required(VERSION 3.16)
project(MedialunasProPOO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MedialunasPro
    main.cpp
    InterfazGrafica.cpp
    baseDatos/conexion.cpp
    models/cliente.cpp
    models/insumos.cpp
    models/pedidos.cpp
    models/stockVenta.cpp
    models/variedad.cpp
    DAO/clienteDAO.cpp
    DAO/insumosDAO.cpp
    DAO/pedidosDAO.cpp
    DAO/stockVentaDAO.cpp
    DAO/variedadDAO.cpp
    "DAO implementacion/clienteDAOImple.cpp"
    "DAO implementacion/insumosDAOImple.cpp"
    "DAO implementacion/pedidosDAOimple.cpp"
    "DAO implementacion/stockVentaDAOimple.cpp"
    "DAO implementacion/variedadDAOimple.cpp"
)

target_include_directories(MedialunasPro PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/baseDatos
    ${CMAKE_CURRENT_SOURCE_DIR}/DAO
    "${CMAKE_CURRENT_SOURCE_DIR}/DAO implementacion"
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

find_package(raylib CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

target_link_libraries(MedialunasPro PRIVATE raylib SQLite::SQLite3)

add_custom_command(TARGET MedialunasPro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/baseDatos
    $<TARGET_FILE_DIR:MedialunasPro>/baseDatos
)
