# Manual Tecnico


| | |
|---|---|
| **Proyecto** | Analizador de comandos EXT2 |
| **Curso** | Manejo e Implementacion de Archivos (MIA) |
| **Practica** | 1 |
| **Carnet** | 201807092 |
| **Nombre del estudiante** | Eduardo Enmanuel Alejandro Zamora Gomez |
| **Fecha** | Agosto 2026 |

---

## 1. Objetivo

Desarrollar un analizador lexico y sintactico de los comandos del sistema de archivos EXT2. El analizador lee una linea de comandos, la separa en su comando y parametros, y valida que cumpla con la sintaxis definida. Todo se trabaja en memoria, no se crean archivos fisicos en el disco: la idea es que el analizador corrija bien y el alumno pueda ver el flujo de una sesion completa sin tocar el disco real.

## 2. Alcance

El sistema cubre 8 comandos:

1. `mkdisk`
2. `rmdisk`
3. `fdisk`
4. `mount`
5. `mkfs`
6. `mkusr`
7. `rmusr`
8. `mkfile`

Cada comando se valida a dos niveles:

- **Lexico**: la linea se separa en tokens respetando comillas y comentarios.
- **Sintactico**: se revisa que el comando exista, que los parametros sean validos, que no se repitan y que los valores tengan el formato correcto.

Al final de la validacion el comando se ejecuta de forma simulada: guarda el resultado en estructuras de memoria (discos, particiones, montajes, usuarios).

## 3. Arquitectura del sistema

El sistema se divide en dos partes que se comunican por una API REST:

```
+----------------+          +-------------------+          +----------------------+
|   FRONTEND     |  HTTP    |   API REST        |          |   BACKEND C++        |
|  React + Vite  | -------> |   cpp-httplib     | -------> |  analizador EXT2     |
|  puerto 3000   | <------- |   puerto 8080     | <------- |  estado en memoria   |
+----------------+          +-------------------+          +----------------------+
```

### 3.1 Frontend

Aplicacion web hecha con **React 18** y **Vite 5**. Muestra una interfaz tipo terminal:

- **BarraSuperior**: titulo y botones (Ejecutar, Limpiar, Reiniciar estado, Subir archivo).
- **AreaEntrada**: textarea donde se escriben los comandos (con Enter se ejecuta, con Shift+Enter se salta de linea).
- **PanelSalida**: panel dividido en dos, la Salida principal y el Registro de errores.

El frontend no analiza nada, solo envia cada linea al backend con fetch y pinta la respuesta con colores semanticos (verde para [OK], rojo para [ERROR], gris para comentarios, etc).

### 3.2 API REST

El backend ofrece 4 rutas:

| Metodo | Ruta | Descripcion |
|--------|------|-------------|
| GET | `/` | Comprueba que el servidor este arriba |
| POST | `/comando` | Recibe un JSON `{"comando":"..."}`, lo analiza y devuelve la salida |
| GET | `/estado` | Devuelve todo lo que hay en memoria (discos, montajes, usuarios) en JSON |
| POST | `/reset` | Limpia el estado para empezar una sesion desde cero |

### 3.3 Backend

Programa en **C++17** con dos modos de ejecucion:

- **Consola** (`analizador`): lee comandos por stdin usando `main.cpp`.
- **Servidor** (`servidor`): la misma logica pero expuesta por HTTP usando `server.cpp` y la libreria header-only `cpp-httplib`.

El servidor redirige `std::cout` a un buffer de string mientras analiza el comando y devuelve ese texto como respuesta, de esa forma el frontend muestra exactamente lo mismo que se veria en consola.

## 4. Estructuras de datos utilizadas

Todas estan definidas en `structs.h`.

### 4.1 `parametro`

Un parametro ya separado de la linea de comandos.

```cpp
struct parametro {
    std::string nombre; // ej: "size"
    std::string valor; // ej: "3000"
};
```

### 4.2 `particion`

Particion que se crea con `fdisk`.

```cpp
struct particion {
    char tipo; //'p' primaria, 'e' extendida, 'l' logica
    char fit; //'b' best, 'f' first, 'w' worst
    long long size; // tamaño en bytes
    std::string nombre;// nombre, unico dentro del disco
    int padre;// para logicas: posicion de la extendida que la contiene
};
```

### 4.3 `disco`

Disco simulado que se crea con `mkdisk`.

```cpp
struct disco {
    long long size; // tamaño en bytes
    char fit; // fit por defecto del disco
    std::string path; // ruta del disco
    std::string letra; // letra para el id del mount (A, B, C...)
    std::vector<particion> partes; // particiones del disco
};
```

### 4.4 `montaje`

Una particion que ya se monto con `mount`.

```cpp
struct montaje {
    std::string id; // ej: "921A"
    std::string path; // disco al que pertenece
    std::string nombre; // nombre de la particion
    int numParticion; // posicion de la particion en el disco
    char tipo; // tipo de particion
    bool formateada = false; // si ya se le hizo mkfs
};
```

### 4.5 `usuario`

Usuario creado con `mkusr`.

```cpp
struct usuario {
    std::string nombre;
    std::string pass;
    std::string grupo;
};
```

### 4.6 `estadoSistema`

Junta todo el estado de la sesion. Es una variable global `estado` definida en `analizador.cpp`.

```cpp
struct estadoSistema {
    std::vector<disco> discos; // discos creados
    std::vector<montaje> montajes;// particiones montadas
    std::vector<usuario> usuarios;// usuarios creados
};
```

## 5. Modulos del backend

| Archivo | Funcion |
|---------|---------|
| `main.cpp` | Modo consola: lee lineas y llama `ejecutarComando`, sale con `exit` |
| `analizador.cpp/.h` | Tokenizador, analizador lexico/sintactico y despacho de comandos. Define las globales `estado` y `CARNET` |
| `utils.cpp/.h` | Funciones de apoyo: pasar a minusculas, buscar disco/montaje/usuario, contar y obtener parametros, detectar parametros invalidos o repetidos, convertir string a entero |
| `mkdisk.cpp/.h` | Analiza el comando `mkdisk` |
| `rmdisk.cpp/.h` | Analiza el comando `rmdisk` |
| `fdisk.cpp/.h` | Analiza el comando `fdisk` |
| `mount.cpp/.h` | Analiza el comando `mount` |
| `mkfs.cpp/.h` | Analiza el comando `mkfs` |
| `mkusr.cpp/.h` | Analiza el comando `mkusr` |
| `rmusr.cpp/.h` | Analiza el comando `rmusr` |
| `mkfile.cpp/.h` | Analiza el comando `mkfile` |
| `server.cpp` | API REST sobre cpp-httplib, manda los comandos al analizador |
| `libs/httplib.h` | Libreria header-only del servidor HTTP |
| `Makefile` | Compila los dos ejecutables |

## 6. Analisis lexico

Lo hace la funcion `tokenizar` en `analizador.cpp`:

1. Recorre la linea caracter por caracter.
2. Un `#` fuera de comillas inicia un comentario, se ignora el resto de la linea.
3. Las comillas `"` permiten meter espacios dentro de un parametro, y no se guardan en el token.
4. Los espacios fuera de comillas separan tokens.
5. Si al final quedo una comilla abierta, marca `comillas_sin_cerrar` para reportar el error.

## 7. Analisis sintactico

La funcion `analizarLinea` revisa la forma general `comando -param=valor ...`:

- El primer token es el comando y se pasa a minusculas.
- El comando no puede empezar con guion.
- Todo lo demas debe empezar con `-`.
- Un parametro puede ser `-nombre=valor` o solo `-nombre` (flag, lo usa `-r` de mkfile).
- El nombre del parametro no distingue mayusculas.

Luego `ejecutarComando` manda los parametros al analizador de cada comando, o reporta "comando no existe".

## 8. Comandos implementados

Convenciones: los parametros se escriben como `-parametro=valor`, el orden no importa y los nombres no distinguen mayusculas.

### 8.1 `mkdisk` — crear un disco

| Parametro | Obligatorio | Valor por defecto | Valores |
|-----------|:-----------:|:-----------------:|---------|
| `-size` | si | - | entero mayor que 0 |
| `-path` | si | - | ruta del disco |
| `-fit` | no | `ff` | `bf`, `ff`, `wf` |
| `-unit` | no | `m` | `k`, `m` |

El tamaño se convierte a bytes y se guarda en memoria. Si ya existe un disco en la misma ruta se reporta error.

### 8.2 `rmdisk` — eliminar un disco

Solo acepta `-path`. Si el disco no existe se da por eliminado (no se reporta error), si existe se quita del vector de discos.

### 8.3 `fdisk` — crear una particion

| Parametro | Obligatorio | Valor por defecto | Valores |
|-----------|:-----------:|:-----------------:|---------|
| `-size` | si | - | entero mayor que 0 |
| `-path` | si | - | disco donde vive la particion |
| `-name` | si | - | nombre unico dentro del disco |
| `-type` | no | `p` | `p`, `e`, `l` |
| `-unit` | no | `k` | `b`, `k`, `m` |
| `-fit` | no | `ff` | `bf`, `ff`, `wf` |

Reglas que se validan:

- A lo mucho 4 particiones entre primarias y extendidas.
- Solo puede existir una particion extendida.
- Las logicas deben estar dentro de una extendida y caber en su espacio.
- Cada particion debe caber en el espacio libre del disco.
- El nombre no se puede repetir en el mismo disco.

Si el disco no existe se crea uno simulado del tamaño de la particion, para que las pruebas de catedra no fallen por el orden de los comandos.

### 8.4 `mount` — montar una particion

Acepta `-path` y `-name` (obligatorios). Monta la particion y le genera un id con el formato:

```
[ultimos 2 digitos del carnet][numero de montaje en el disco][letra del disco]
```

Ejemplo con carnet 201807092: `921A`, `921B`, `921C`.

A cada disco se le asigna una letra en el orden en que se monta por primera vez. Montar dos veces la misma particion es error. Si el disco o la particion no existen se crean simulados para poder montar igual.

### 8.5 `mkfs` — formatear una particion

| Parametro | Obligatorio | Valor por defecto |
|-----------|:-----------:|:-----------------:|
| `-id` | si | - |
| `-type` | no | `full` |

El unico valor aceptado para `-type` es `full` (en esta practica no se implementa fast). Marca como `formateada` la particion montada. Si el id no corresponde a ninguna particion montada se simula el formateo igual.

### 8.6 `mkusr` — crear un usuario

| Parametro | Obligatorio | Restricciones |
|-----------|:-----------:|---------------|
| `-user` | si | maximo 10 caracteres |
| `-pass` | si | maximo 10 caracteres |
| `-grp` | si | maximo 10 caracteres |

El usuario no puede repetirse.

### 8.7 `rmusr` — eliminar un usuario

Solo acepta `-user`. Debe existir el usuario para poder borrarlo.

### 8.8 `mkfile` — crear un archivo

| Parametro | Obligatorio | Descripcion |
|-----------|:-----------:|-------------|
| `-path` | si | ruta del archivo |
| `-r` | no | flag, no lleva valor; permite crear archivos en rutas intermedias |
| `-size` | no | entero no negativo (0 permitido) |
| `-cont` | no | ruta de un archivo del cual copiar contenido |

Solo se valida, no se escribe nada en disco.

## 9. Manejo de errores

Todas las salidas del analizador usan prefijos para que el frontend las pinte:

- `[OK]` — el comando se acepto y se ejecuto de forma simulada.
- `[ERROR]` — el comando fallo (parametro invalido, faltante, repetido, comillas sin cerrar, etc).

El prefijo `[ERROR]` es el que el frontend usa para separar las lineas hacia el Registro de errores.

## 10. Compilacion y ejecucion

Desde la carpeta `Backend`:

```bash
make analizador   # compila el ejecutable de consola
make servidor     # compila el ejecutable de la API (requiere -lpthread)
make clean        # borra los ejecutables
```

Ejecutar el analizador de consola:

```bash
./analizador
```

Ejecutar el servidor:

```bash
./servidor
```

Levantar el frontend (desde la carpeta `Frontend`, requiere Node 22):

```bash
npm install
npm run dev      # corre en http://localhost:3000
```

## 11. Limitaciones

- El estado solo vive en memoria; al apagar el servidor o la consola se pierde todo.
- No se crean ni se leen archivos fisicos (.mia), solo se valida la sintaxis de los comandos.
- No se simula el arranque, superbloque, inodos ni bitmap.
- Los ids de mount usan el formato del carnet (`921A`), no el formato clasico de 5 caracteres.

---
