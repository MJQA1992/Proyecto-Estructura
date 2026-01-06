/*
  PROYECTO: EL TREN DEL ORO - ENTREGA FINAL
  Asignatura: Algoritmos y Estructuras de Datos
  Universidad: UCAB
  Profesor: Yisheng Leon Espinoza

  Descripcion:
  Sistema de juego policias vs ladrones en una red de metro.
  Usa grafos para el mapa, listas enlazadas para personajes e implementos.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// estructuras para almacenar los datos del juego

struct Implemento {
    int id;
    string tipo;
    string nombre;
    string funcion;
    int usos_max;
    int alcance;
    int usos_left;
    Implemento* next;

    Implemento() : next(nullptr) {}
};

struct Personaje {
    int id;
    string bando;
    string nombre;
    int vida;
    int vida_max;
    string type;
    string state;
    int estacion;
    bool corrupto;
    int oro_lleva;
    int cap_oro;

    Implemento* items;
    Personaje* next;

    Personaje() : next(nullptr), items(nullptr),
                  corrupto(false), oro_lleva(0) {}
};

struct Estacion {
    int id;
    string nombre;
    int oro;

    struct Link {
        int dest;
        int dist;
        Link* next;
        Link() : next(nullptr) {}
    };

    Link* conexiones;
    Estacion* next;

    Estacion() : next(nullptr), conexiones(nullptr), oro(0) {}
};

struct EventoBitacora {
    int turno;
    string accion;
    string personaje;
    string resultado;
    EventoBitacora* next;

    EventoBitacora() : next(nullptr) {}
};

struct Game {
    Personaje* personajes;
    Implemento* items;
    Estacion* estaciones;
    EventoBitacora* log;

    int turno;
    int oro_total;
    int oro_obtenido;
    int ladrones_capturados;
    int corruptos_encontrados;
    bool activo;
    string ganador;
    string faccion_gana;
    int turnos_totales;

    Game() : personajes(nullptr), items(nullptr),
             estaciones(nullptr), log(nullptr),
             turno(0), oro_total(0), oro_obtenido(0),
             ladrones_capturados(0), corruptos_encontrados(0),
             activo(false), ganador(""), faccion_gana(""),
             turnos_totales(0) {}
};

// funciones auxiliares para validar entrada

bool es_numero(const string& s) {
    if (s.empty()) return false;
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

bool es_decimal(const string& s) {
    if (s.empty()) return false;
    bool hay_punto = false;
    for (char c : s) {
        if (c == '.') {
            if (hay_punto) return false;
            hay_punto = true;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void guardar_evento(Game& game, const string& accion,
                    const string& personaje, const string& resultado) {
    EventoBitacora* nuevo = new EventoBitacora();
    nuevo->turno = game.turno;
    nuevo->accion = accion;
    nuevo->personaje = personaje;
    nuevo->resultado = resultado;
    nuevo->next = game.log;
    game.log = nuevo;
}

// carga de archivos

bool cargar_items(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) {
        cout << "ERROR: No se pudo abrir " << archivo << endl;
        return false;
    }

    int cant;
    f >> cant;
    f.ignore();

    for (int i = 0; i < cant; i++) {
        string linea;
        getline(f, linea);

        Implemento* impl = new Implemento();

        getline(f, linea);
        impl->id = stoi(linea);

        getline(f, impl->tipo);
        getline(f, impl->nombre);

        getline(f, linea);
        size_t pos = linea.find(": ");
        if (pos != string::npos) {
            impl->funcion = linea.substr(pos + 2);
        }

        getline(f, linea);
        pos = linea.find(": ");
        if (pos != string::npos) {
            impl->usos_max = stoi(linea.substr(pos + 2));
            impl->usos_left = impl->usos_max;
        }

        getline(f, linea);
        pos = linea.find(": ");
        if (pos != string::npos) {
            impl->alcance = stoi(linea.substr(pos + 2));
        }

        impl->next = game.items;
        game.items = impl;
    }

    f.close();
    cout << "[OK] Items cargados (" << cant << ")" << endl;
    return true;
}

bool cargar_personajes(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) {
        cout << "ERROR: No se pudo abrir " << archivo << endl;
        return false;
    }

    int cant;
    f >> cant;
    f.ignore();

    for (int i = 0; i < cant; i++) {
        string linea;
        getline(f, linea);

        Personaje* p = new Personaje();

        getline(f, linea);
        p->id = stoi(linea);

        getline(f, p->bando);
        getline(f, p->nombre);

        getline(f, linea);
        if (linea != "-") {
            stringstream ss(linea);
            string id_str;
            while (getline(ss, id_str, '|')) {
                int id_item = stoi(id_str);
                Implemento* impl = game.items;
                while (impl) {
                    if (impl->id == id_item) {
                        Implemento* copia = new Implemento(*impl);
                        copia->next = p->items;
                        p->items = copia;
                        break;
                    }
                    impl = impl->next;
                }
            }
        }

        p->state = "Activo";
        p->vida_max = 100;
        p->vida = 100;
        p->estacion = 1;
        p->cap_oro = (p->bando == "Ladron") ? 5 : 1;
        p->corrupto = false;

        p->next = game.personajes;
        game.personajes = p;
    }

    f.close();
    cout << "[OK] Personajes cargados (" << cant << ")" << endl;
    return true;
}

bool cargar_mapa(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) {
        cout << "ERROR: No se pudo abrir " << archivo << endl;
        return false;
    }

    int cant;
    f >> cant;
    f.ignore();

    for (int i = 0; i < cant; i++) {
        string linea;
        getline(f, linea);

        Estacion* e = new Estacion();

        getline(f, linea);
        e->id = stoi(linea);

        getline(f, e->nombre);

        getline(f, linea);
        e->oro = 0;

        getline(f, linea);
        if (linea != "-") {
            stringstream ss(linea);
            string conn;
            while (getline(ss, conn, '|')) {
                size_t pos = conn.find(':');
                if (pos != string::npos) {
                    int dest = stoi(conn.substr(0, pos));
                    int dist = stoi(conn.substr(pos + 1));

                    Estacion::Link* link = new Estacion::Link();
                    link->dest = dest;
                    link->dist = dist;
                    link->next = e->conexiones;
                    e->conexiones = link;
                }
            }
        }

        e->next = game.estaciones;
        game.estaciones = e;
    }

    f.close();
    cout << "[OK] Mapa cargado (" << cant << " estaciones)" << endl;
    return true;
}

// inicializacion del juego

void init_game(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  INICIALIZANDO EL TREN DEL ORO\n";
    cout << string(60, '=') << endl;

    if (!cargar_items(game, "accesorios.tren")) return;
    if (!cargar_personajes(game, "personajes.tren")) return;
    if (!cargar_mapa(game, "Mapa.tren")) return;

    cout << "\n[SISTEMA] Buscando policias corruptos..." << endl;
    Personaje* polis[100];
    int n_polis = 0;

    Personaje* tmp = game.personajes;
    while (tmp) {
        if (tmp->bando == "Policia") {
            polis[n_polis++] = tmp;
        }
        tmp = tmp->next;
    }

    if (n_polis >= 2) {
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < 2; i++) {
            int idx = rand() % n_polis;
            polis[idx]->corrupto = true;
        }
    }

    cout << "[SISTEMA] Distribuyendo oro..." << endl;
    game.oro_total = 20;

    Estacion* e = game.estaciones;
    int oro_left = game.oro_total;

    while (e && oro_left > 0) {
        int cantidad = rand() % 4;
        if (cantidad > oro_left) cantidad = oro_left;

        e->oro = cantidad;
        oro_left -= cantidad;
        e = e->next;
    }

    cout << "\n[OK] LISTO\n";
    cout << "  Oro distribuido: " << game.oro_total << endl;
}

// condiciones de victoria

bool chequear_victoria(Game& game) {
    if (game.oro_obtenido >= 15) {
        game.ganador = "LADRONES";
        game.faccion_gana = "LADRONES";
        return true;
    }

    if (game.ladrones_capturados >= 5) {
        game.ganador = "POLICIAS_HONESTOS";
        game.faccion_gana = "POLICIAS HONESTOS";
        return true;
    }

    int oro_escape = game.oro_total - game.oro_obtenido;
    if (oro_escape >= 8 && oro_escape <= 12) {
        game.ganador = "POLICIAS_CORRUPTOS";
        game.faccion_gana = "POLICIAS CORRUPTOS";
        return true;
    }

    return false;
}

void mostrar_victoria(Game& game) {
    cout << "\n\n" << string(60, '=') << endl;
    cout << "  *** JUEGO TERMINADO ***\n";
    cout << string(60, '=') << "\n" << endl;

    if (game.ganador == "LADRONES") {
        cout << "  !!! LOS LADRONES GANARON !!!\n\n";
        cout << "  Oro recolectado: " << game.oro_obtenido << "/20\n";
        cout << "  Porcentaje: " << (game.oro_obtenido * 100 / game.oro_total) << "%\n";
    } else if (game.ganador == "POLICIAS_HONESTOS") {
        cout << "  !!! POLICIAS HONESTOS GANARON !!!\n\n";
        cout << "  Ladrones capturados: " << game.ladrones_capturados << "\n";
        cout << "  Oro recuperado: " << game.oro_obtenido << "/" << game.oro_total << "\n";
    } else if (game.ganador == "POLICIAS_CORRUPTOS") {
        cout << "  !!! POLICIAS CORRUPTOS GANARON !!!\n\n";
        int escape = game.oro_total - game.oro_obtenido;
        cout << "  Oro que escapo: " << escape << "/20\n";
        cout << "  Porcentaje: " << (escape * 100 / game.oro_total) << "%\n";
    }

    cout << "\n  Turnos jugados: " << game.turno << "\n";
    cout << "  Corruptos identificados: " << game.corruptos_encontrados << "\n";
    cout << "\n" << string(60, '=') << endl;
    cout << "  Presione ENTER para ver el historial...\n";
    cout << string(60, '=') << endl;
    cin.ignore();
}

void mostrar_log(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  HISTORIAL DEL JUEGO\n";
    cout << string(60, '=') << "\n" << endl;

    if (!game.log) {
        cout << "No hay eventos registrados.\n";
        return;
    }

    // invertir log
    EventoBitacora* eventos[1000];
    int cnt = 0;
    EventoBitacora* tmp = game.log;
    while (tmp) {
        eventos[cnt++] = tmp;
        tmp = tmp->next;
    }

    cout << setw(8) << "Turno" << setw(20) << "Personaje"
         << setw(25) << "Accion" << setw(30) << "Resultado" << endl;
    cout << string(90, '-') << endl;

    for (int i = cnt - 1; i >= 0; i--) {
        cout << setw(8) << eventos[i]->turno
             << setw(20) << eventos[i]->personaje
             << setw(25) << eventos[i]->accion
             << setw(30) << eventos[i]->resultado << endl;
    }

    cout << "\nPresione ENTER para volver...\n";
    cin.ignore();
}

// mostrar datos

void listar_personajes(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  PERSONAJES\n";
    cout << string(60, '=') << "\n" << endl;

    Personaje* tmp = game.personajes;
    if (!tmp) {
        cout << "Sin personajes.\n";
        return;
    }

    cout << setw(5) << "ID" << setw(15) << "Nombre"
         << setw(12) << "Bando" << setw(15) << "Estado"
         << setw(12) << "Vida" << setw(12) << "Estacion" << endl;
    cout << string(75, '-') << endl;

    while (tmp) {
        cout << setw(5) << tmp->id
             << setw(15) << tmp->nombre
             << setw(12) << tmp->bando
             << setw(15) << tmp->state
             << setw(12) << tmp->vida << "/" << tmp->vida_max
             << setw(12) << tmp->estacion << endl;
        tmp = tmp->next;
    }
    cout << string(75, '-') << endl;
}

void listar_items(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  ITEMS\n";
    cout << string(60, '=') << "\n" << endl;

    Implemento* tmp = game.items;
    if (!tmp) {
        cout << "Sin items.\n";
        return;
    }

    cout << setw(5) << "ID" << setw(20) << "Nombre"
         << setw(15) << "Tipo" << setw(20) << "Funcion"
         << setw(10) << "Usos" << setw(10) << "Alcance" << endl;
    cout << string(80, '-') << endl;

    while (tmp) {
        cout << setw(5) << tmp->id
             << setw(20) << tmp->nombre
             << setw(15) << tmp->tipo
             << setw(20) << tmp->funcion
             << setw(10) << tmp->usos_left
             << setw(10) << tmp->alcance << endl;
        tmp = tmp->next;
    }
    cout << string(80, '-') << endl;
}

void listar_mapa(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  MAPA\n";
    cout << string(60, '=') << "\n" << endl;

    Estacion* tmp = game.estaciones;
    if (!tmp) {
        cout << "Sin estaciones.\n";
        return;
    }

    cout << setw(5) << "ID" << setw(25) << "Nombre"
         << setw(12) << "Oro" << setw(20) << "Conexiones" << endl;
    cout << string(75, '-') << endl;

    while (tmp) {
        cout << setw(5) << tmp->id
             << setw(25) << tmp->nombre
             << setw(12) << tmp->oro << setw(5) << "[";
        Estacion::Link* link = tmp->conexiones;
        bool first = true;
        while (link) {
            if (!first) cout << ", ";
            cout << link->dest;
            first = false;
            link = link->next;
        }
        cout << "]" << endl;
        tmp = tmp->next;
    }
    cout << string(75, '-') << endl;
}

// gameplay

void mostrar_estado(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  TURNO " << game.turno << "\n";
    cout << string(60, '=') << "\n" << endl;
    cout << "Oro total: " << game.oro_total << "\n";
    cout << "Oro obtenido (Ladrones): " << game.oro_obtenido << "/15 PARA GANAR\n";
    cout << "Oro escapado: " << (game.oro_total - game.oro_obtenido) << "\n";
    cout << "Ladrones capturados (Policias): " << game.ladrones_capturados << "/5 PARA GANAR\n";
    cout << "Corruptos encontrados: " << game.corruptos_encontrados << "\n";
}

void listar_activos(Game& game) {
    cout << "\n--- PERSONAJES ACTIVOS ---\n";
    Personaje* tmp = game.personajes;
    int cont = 0;

    while (tmp) {
        if (tmp->state == "Activo") {
            cout << cont + 1 << ". " << tmp->nombre
                 << " (" << tmp->bando << ") - Est "
                 << tmp->estacion << " - Vida: "
                 << tmp->vida << "/" << tmp->vida_max;
            if (tmp->corrupto) cout << " [CORRUPTO]";
            if (tmp->oro_lleva > 0) cout << " [LLEVA " << tmp->oro_lleva << " ORO]";
            cout << endl;
            cont++;
        }
        tmp = tmp->next;
    }
}

Personaje* elegir_personaje(Game& game) {
    listar_activos(game);

    cout << "\nElige personaje (numero): ";
    int op;

    if (!(cin >> op)) {
        cout << "ERROR: numero invalido.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return nullptr;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Personaje* tmp = game.personajes;
    int cont = 0;

    while (tmp) {
        if (tmp->state == "Activo") {
            cont++;
            if (cont == op) {
                return tmp;
            }
        }
        tmp = tmp->next;
    }

    cout << "ERROR: Personaje no encontrado.\n";
    return nullptr;
}

void mostrar_conexiones(Game& game, int id_est) {
    Estacion* est = game.estaciones;

    while (est) {
        if (est->id == id_est) {
            cout << "\n--- ESTACIONES CONECTADAS ---\n";
            Estacion::Link* link = est->conexiones;
            int cont = 1;

            if (!link) {
                cout << "Sin conexiones.\n";
                return;
            }

            while (link) {
                Estacion* dest = game.estaciones;
                while (dest) {
                    if (dest->id == link->dest) {
                        cout << cont << ". Est " << link->dest
                             << " (" << dest->nombre << ") - Dist: "
                             << link->dist << " - Oro: "
                             << dest->oro << endl;
                        cont++;
                        break;
                    }
                    dest = dest->next;
                }
                link = link->next;
            }
            return;
        }
        est = est->next;
    }
}

void jugar_turno(Game& game) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  TURNO\n";
    cout << string(60, '=') << endl;

    Personaje* pers = elegir_personaje(game);
    if (!pers) {
        cout << "Turno saltado.\n";
        return;
    }

    cout << "\nPersonaje: " << pers->nombre << endl;
    cout << "Ubicacion: Estacion " << pers->estacion << endl;

    mostrar_conexiones(game, pers->estacion);

    cout << "\nElige estacion destino (numero): ";
    int op;

    if (!(cin >> op)) {
        cout << "ERROR: numero invalido.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Estacion* est_act = game.estaciones;
    while (est_act) {
        if (est_act->id == pers->estacion) {
            Estacion::Link* link = est_act->conexiones;
            int cont = 1;

            while (link) {
                if (cont == op) {
                    pers->estacion = link->dest;
                    cout << "\n[OK] " << pers->nombre << " se movio a Estacion "
                         << link->dest << endl;

                    string res = "Movimiento a estacion " + to_string(link->dest);
                    guardar_evento(game, "Movimiento", pers->nombre, res);

                    Estacion* dest = game.estaciones;
                    while (dest) {
                        if (dest->id == link->dest) {
                            if (dest->oro > 0 && pers->bando == "Ladron") {
                                int toma = min(dest->oro,
                                              pers->cap_oro - pers->oro_lleva);
                                pers->oro_lleva += toma;
                                dest->oro -= toma;
                                cout << "[OK] " << pers->nombre << " tomo "
                                     << toma << " oro!\n";

                                game.oro_obtenido += toma;

                                string res_oro = "Recolecto " + to_string(toma) + " oro";
                                guardar_evento(game, "Recoleccion", pers->nombre, res_oro);
                            }
                            break;
                        }
                        dest = dest->next;
                    }
                    return;
                }
                cont++;
                link = link->next;
            }
            break;
        }
        est_act = est_act->next;
    }

    cout << "ERROR: Estacion no disponible.\n";
}

void juego_activo(Game& game) {
    game.activo = true;
    game.turno = 1;
    int op = -1;

    while (game.activo) {
        if (chequear_victoria(game)) {
            game.turnos_totales = game.turno;
            mostrar_victoria(game);
            mostrar_log(game);
            game.activo = false;
            break;
        }

        mostrar_estado(game);

        cout << "\n" << string(60, '=') << endl;
        cout << "  MENU\n";
        cout << string(60, '=') << endl;
        cout << "\n1. Jugar Turno\n";
        cout << "2. Ver Personajes\n";
        cout << "3. Ver Mapa\n";
        cout << "4. Ver Estado\n";
        cout << "5. Ver Historial\n";
        cout << "6. Salir\n";
        cout << "\nOpcion (1-6): ";

        if (!(cin >> op)) {
            cout << "ERROR: numero invalido.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (op) {
            case 1:
                jugar_turno(game);
                game.turno++;
                break;
            case 2:
                listar_personajes(game);
                break;
            case 3:
                listar_mapa(game);
                break;
            case 4:
                mostrar_estado(game);
                break;
            case 5:
                mostrar_log(game);
                break;
            case 6:
                game.activo = false;
                cout << "\n[OK] Saliendo...\n";
                break;
            default:
                cout << "ERROR: opcion invalida.\n";
        }
    }
}

// menu principal

void menu_principal(Game& game) {
    int op = -1;

    do {
        cout << "\n" << string(60, '=') << endl;
        cout << "  EL TREN DEL ORO\n";
        cout << string(60, '=') << endl;
        cout << "\n1. Ver Personajes\n";
        cout << "2. Ver Items\n";
        cout << "3. Ver Mapa\n";
        cout << "4. Iniciar Juego\n";
        cout << "5. Salir\n";
        cout << "\nOpcion (1-5): ";

        if (!(cin >> op)) {
            cout << "ERROR: numero invalido.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            op = -1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (op) {
            case 1:
                listar_personajes(game);
                break;
            case 2:
                listar_items(game);
                break;
            case 3:
                listar_mapa(game);
                break;
            case 4:
                juego_activo(game);
                break;
            case 5:
                cout << "\n[OK] Adios!\n\n";
                break;
            default:
                cout << "ERROR: opcion invalida.\n";
        }
    } while (op != 5);
}

// main

int main() {
    Game game;

    init_game(game);
    menu_principal(game);

    return 0;
}
