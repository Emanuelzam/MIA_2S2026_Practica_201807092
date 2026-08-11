#include <iostream>
#include <sstream>
#include <string>

#include "libs/httplib.h"

#include "analizador.h"
#include "structs.h"

//Inicio de la api rest donde el frontend se comunica con el backend
//Los comandos llegan en un POST y el estado de la sesion
//se devuelve en el GET /estado

//ejecuta un comando y devuelve todo lo que el analizador imprimio por consola
static std::string ejecutarConSalida(const std::string &linea) {
    std::ostringstream buffer;
    std::streambuf *original = std::cout.rdbuf(buffer.rdbuf());
    ejecutarComando(linea);
    std::cout.rdbuf(original);
    return buffer.str();
}

//escapa los caracteres que rompen el formato json
static std::string escaparJson(const std::string &texto) {
    std::string resultado;
    for (char c : texto) {
        if (c == '"') {
            resultado += "\\\"";
        } else if (c == '\\') {
            resultado += "\\\\";
        } else if (c == '\n') {
            resultado += "\\n";
        } else if (c == '\r') {
            resultado += "\\r";
        } else if (c == '\t') {
            resultado += "\\t";
        } else {
            resultado += c;
        }
    }
    return resultado;
}

//el frontend manda el cuerpo de la peticion como json:
//{"comando":"mkdisk -size=5 -path=\"/tmp/Disco1.mia\""}
//aqui el valor trae las comillas escapadas (\") si el comando
//lleva rutas con espacios, asi que hay que leer hasta la comilla
//que no tenga una barra atras
static std::string extraerComando(const std::string &body) {
    std::string clave = "\"comando\"";
    size_t pos = body.find(clave);
    if (pos == std::string::npos) {
        return "";
    }
    pos = body.find(':', pos + clave.size());
    if (pos == std::string::npos) {
        return "";
    }
    size_t ini = body.find('"', pos);
    if (ini == std::string::npos) {
        return "";
    }

    std::string valor;
    bool escapado = false;
    for (size_t i = ini + 1; i < body.size(); ++i) {
        char c = body[i];
        if (escapado) {
            // convierto las secuencias mas comunes y dejo las
            // comillas escapadas como comilla normal
            if (c == 'n') {
                valor += '\n';
            } else if (c == 't') {
                valor += '\t';
            } else if (c == 'r') {
                valor += '\r';
            } else {
                valor += c;
            }
            escapado = false;
        } else if (c == '\\') {
            escapado = true;
        } else if (c == '"') {
            break; // es el cierre del string json
        } else {
            valor += c;
        }
    }
    return valor;
}

//arma el json con todo el estado que se lleva en memoria
static std::string estadoAJson() {
    std::string json = "{";

    //Discos con sus particiones
    json += "\"discos\":[";
    for (size_t i = 0; i < estado.discos.size(); ++i) {
        disco &d = estado.discos[i];
        json += "{\"path\":\"" + escaparJson(d.path) + "\",";
        json += "\"size\":" + std::to_string(d.size) + ",";
        json += "\"fit\":\"" + std::string(1, d.fit) + "\",";
        json += "\"letra\":\"" + d.letra + "\",";
        json += "\"particiones\":[";
        for (size_t j = 0; j < d.partes.size(); ++j) {
            particion &p = d.partes[j];
            json += "{\"nombre\":\"" + escaparJson(p.nombre) + "\",";
            json += "\"tipo\":\"" + std::string(1, p.tipo) + "\",";
            json += "\"fit\":\"" + std::string(1, p.fit) + "\",";
            json += "\"size\":" + std::to_string(p.size) + ",";
            json += "\"padre\":" + std::to_string(p.padre) + "}";
            if (j + 1 < d.partes.size()) {
                json += ",";
            }
        }
        json += "]}";
        if (i + 1 < estado.discos.size()) {
            json += ",";
        }
    }
    json += "],";

    //Particiones montadas
    json += "\"montajes\":[";
    for (size_t i = 0; i < estado.montajes.size(); ++i) {
        montaje &m = estado.montajes[i];
        json += "{\"id\":\"" + escaparJson(m.id) + "\",";
        json += "\"path\":\"" + escaparJson(m.path) + "\",";
        json += "\"nombre\":\"" + escaparJson(m.nombre) + "\",";
        json += "\"tipo\":\"" + std::string(1, m.tipo) + "\",";
        json += "\"formateada\":";
        json += m.formateada ? "true" : "false";
        json += "}";
        if (i + 1 < estado.montajes.size()) {
            json += ",";
        }
    }
    json += "],";

    //Usuarios creados
    json += "\"usuarios\":[";
    for (size_t i = 0; i < estado.usuarios.size(); ++i) {
        usuario &u = estado.usuarios[i];
        json += "{\"nombre\":\"" + escaparJson(u.nombre) + "\",";
        json += "\"pass\":\"" + escaparJson(u.pass) + "\",";
        json += "\"grupo\":\"" + escaparJson(u.grupo) + "\"}";
        if (i + 1 < estado.usuarios.size()) {
            json += ",";
        }
    }
    json += "]";

    json += "}";
    return json;
}

int main() {
    httplib::Server svr;

    //el frontend corre en otro puerto cualquier
    //origen pueda llamar al servidor (CORS)
    svr.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        //peticiones no necesitan pasar por el resto
        if (req.method == "OPTIONS") {
            res.status = 204;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    //ruta para saber que el server esta corriendo
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("API analizador EXT2 corriendo", "text/plain");
    });

    //recibe un comando, lo analiza y devuelve la salida
    svr.Post("/comando", [](const httplib::Request &req, httplib::Response &res) {
        std::string comando = extraerComando(req.body);
        if (comando.empty()) {
            res.set_content("{\"ok\":false,\"salida\":\"[ERROR] el cuerpo debe ser un json con el campo comando\"}",
                            "application/json");
            return;
        }
        std::string salida = ejecutarConSalida(comando);
        res.set_content("{\"ok\":true,\"salida\":\"" + escaparJson(salida) + "\"}", "application/json");
    });

    //devuelve lo que se tiene en memoria (discos, montajes, usuarios)
    svr.Get("/estado", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(estadoAJson(), "application/json");
    });

    //limpia todo el estado para correr un archivo de pruebas desde cero
    //sin tener que reiniciar el servidor
    svr.Post("/reset", [](const httplib::Request &, httplib::Response &res) {
        estado.discos.clear();
        estado.montajes.clear();
        estado.usuarios.clear();
        res.set_content("{\"ok\":true,\"salida\":\"[OK] estado reiniciado\"}", "application/json");
    });

    std::cout << "Servidor corriendo en http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}