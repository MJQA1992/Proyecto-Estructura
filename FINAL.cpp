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
#include <algorithm>
#include <iomanip>

using namespace std;

// --- ESTRUCTURAS ORIGINALES Y NUEVAS ---

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
    string bando;      // "Ladron" o "Policia"
    string nombre;
    int vida, vida_max;
    string state;      // "Activo", "Capturado"
    int estacion;
    bool corrupto;     // Solo para policias
    int oro_lleva;
    int cap_oro;
    bool movido_este_turno;
    bool accion_realizada;

    Implemento* items;
    Personaje* next;

    Personaje() : next(nullptr), items(nullptr), corrupto(false), 
                  oro_lleva(0), state("Activo"), movido_este_turno(false),
                  accion_realizada(false), cap_oro(3), vida(100), vida_max(100) {}
};

struct Estacion {
    int id;
    string nombre;
    int oro;
    int turnos_civil; // Para la regla de civiles que desaparecen

    struct Link {
        int dest;
        int dist;
        Link* next;
        Link() : next(nullptr) {}
    };

    Link* conexiones;
    Estacion* next;
    Estacion() : next(nullptr), conexiones(nullptr), oro(0), turnos_civil(0) {}
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

    int turno_actual;
    int oro_total_mapa;
    int oro_ladrones;          // Oro que los ladrones han logrado "sacar"
    int oro_recuperado_policia; // Oro recuperado por agentes honestos
    int ladrones_totales;
    int ladrones_capturados;
    bool turno_de_ladrones;    // Alternancia entre Jugador 1 y Jugador 2
    int dados[4];
    bool activo;

    Game() : personajes(nullptr), items(nullptr), estaciones(nullptr), log(nullptr),
             turno_actual(1), oro_total_mapa(0), oro_ladrones(0), oro_recuperado_policia(0),
             ladrones_totales(0), ladrones_capturados(0),
             turno_de_ladrones(true), activo(false) {}
};

// --- FUNCIONES DE UTILIDAD Y BITACORA ---

void guardar_evento(Game& game, string acc, string pers, string res) {
    EventoBitacora* nuevo = new EventoBitacora();
    nuevo->turno = game.turno_actual;
    nuevo->accion = acc;
    nuevo->personaje = pers;
    nuevo->resultado = res;
    nuevo->next = game.log;
    game.log = nuevo;
}

// --- CARGA DE ARCHIVOS (TU LOGICA ORIGINAL MEJORADA) ---

bool cargar_items(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) return false;
    int cant; f >> cant; f.ignore();
    for (int i = 0; i < cant; i++) {
        string linea; getline(f, linea);
        Implemento* impl = new Implemento();
        getline(f, linea); impl->id = stoi(linea);
        getline(f, impl->tipo);
        getline(f, impl->nombre);
        getline(f, linea); impl->funcion = linea;
        getline(f, linea); impl->usos_max = 5; impl->usos_left = 5;
        getline(f, linea); impl->alcance = 1;
        impl->next = game.items;
        game.items = impl;
    }
    return true;
}

bool cargar_personajes(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) return false;
    int cant; f >> cant; f.ignore();
    for (int i = 0; i < cant; i++) {
        string linea; getline(f, linea);
        Personaje* p = new Personaje();
        getline(f, linea); p->id = stoi(linea);
        getline(f, p->bando);
        getline(f, p->nombre);
        getline(f, linea); // Ignorar items por ahora para asignacion manual
        if(p->bando == "Ladron") game.ladrones_totales++;
        p->next = game.personajes;
        game.personajes = p;
    }
    return true;
}

bool cargar_mapa(Game& game, const string& archivo) {
    ifstream f(archivo);
    if (!f.is_open()) return false;
    int cant; f >> cant; f.ignore();
    for (int i = 0; i < cant; i++) {
        string linea; getline(f, linea);
        Estacion* e = new Estacion();
        getline(f, linea); e->id = stoi(linea);
        getline(f, e->nombre);
        getline(f, linea); // Oro sera distribuido por sistema
        getline(f, linea);
        if (linea != "-") {
            stringstream ss(linea); string conn;
            while (getline(ss, conn, '|')) {
                size_t pos = conn.find(':');
                if (pos != string::npos) {
                    Estacion::Link* link = new Estacion::Link();
                    link->dest = stoi(conn.substr(0, pos));
                    link->dist = stoi(conn.substr(pos + 1));
                    link->next = e->conexiones;
                    e->conexiones = link;
                }
            }
        }
        e->next = game.estaciones;
        game.estaciones = e;
    }
    return true;
}

// --- LOGICA DE DADOS Y REGLAS DE ENTREGA 2 ---

void lanzar_dados(Game& game) {
    int frecuencias[7] = {0};
    int pares = 0;
    cout << "\n[DADOS] Lanzando 4 dados para " << (game.turno_de_ladrones ? "LADRONES" : "POLICIA") << "... ";
    for(int i = 0; i < 4; i++) {
        game.dados[i] = (rand() % 6) + 1;
        cout << "[" << game.dados[i] << "] ";
        frecuencias[game.dados[i]]++;
    }
    for(int i = 1; i <= 6; i++) if(frecuencias[i] >= 2) pares += (frecuencias[i] / 2);
    if(pares >= 2) cout << "\n¡DOS DOBLES! Se otorga un lanzamiento extra tras esta fase.";
    cout << endl;
}

bool verificar_victoria(Game& game) {
    float perc_oro_ladron = (float)game.oro_ladrones / (game.oro_total_mapa > 0 ? game.oro_total_mapa : 1);
    
    // Victoria Ladrones: > 70% del oro (Regla I.Bando 1)
    if (perc_oro_ladron > 0.70) {
        cout << "\n!!! VICTORIA: LOS LADRONES ESCAPARON CON EL BOTIN !!!\n";
        return true;
    }
    // Victoria Honestos: >=80% capturas Y >=90% oro recuperado (Regla I.Bando 2)
    float perc_captura = (float)game.ladrones_capturados / (game.ladrones_totales > 0 ? game.ladrones_totales : 1);
    float perc_recuperado = (float)game.oro_recuperado_policia / (game.oro_total_mapa > 0 ? game.oro_total_mapa : 1);
    if (perc_captura >= 0.80 && perc_recuperado >= 0.90) {
        cout << "\n!!! VICTORIA: LA POLICIA HONESTA HA IMPUESTO JUSTICIA !!!\n";
        return true;
    }
    // Victoria Corruptos: Escape entre 40% y 60% (Regla I.Bando 2.2)
    if (!game.activo && perc_oro_ladron >= 0.40 && perc_oro_ladron <= 0.60) {
        cout << "\n!!! VICTORIA: LOS POLICIAS CORRUPTOS SE HAN ENRIQUECIDO !!!\n";
        return true;
    }
    return false;
}

// --- MECANICAS DE TURNO ---

void ejecutar_movimiento(Game& game, Personaje* p, int pasos) {
    cout << "\n--- Moviendo a " << p->nombre << " (" << pasos << " pasos) ---" << endl;
    for(int i = 0; i < pasos; i++) {
        cout << "Paso " << i+1 << "/" << pasos << ". Estacion actual: " << p->estacion << endl;
        cout << "Conexiones: ";
        Estacion* e = game.estaciones;
        while(e) {
            if(e->id == p->estacion) {
                Estacion::Link* l = e->conexiones;
                while(l) { cout << "[" << l->dest << "] "; l = l->next; }
                break;
            }
            e = e->next;
        }
        cout << "\nElija destino: ";
        int dest; cin >> dest;
        p->estacion = dest;

        // Interaccion automatica con Oro (Regla III.Fase Acción)
        e = game.estaciones;
        while(e) {
            if(e->id == p->estacion && e->oro > 0) {
                if(p->bando == "Ladron") {
                    int t = min(e->oro, p->cap_oro - p->oro_lleva);
                    p->oro_lleva += t; e->oro -= t; game.oro_ladrones += t;
                    cout << "¡Oro recogido! Llevas: " << p->oro_lleva << endl;
                } else if (!p->corrupto) {
                    game.oro_recuperado_policia += e->oro; e->oro = 0;
                    cout << "¡Agente honesto ha recuperado el oro para evidencia!" << endl;
                }
            }
            e = e->next;
        }
    }
    p->movido_este_turno = true;
}

void fase_accion_principal(Game& game, Personaje* p) {
    cout << "\nACCION PRINCIPAL PARA " << p->nombre << ":\n";
    if (p->bando == "Policia") {
        cout << "1. Investigar (Ver pistas)\n2. Capturar (Intentar arresto en estacion)\n";
        if (p->corrupto) cout << "3. Sabotear (Mentir en el informe)\n";
    } else {
        cout << "1. Rastrear (Buscar oro adyacente)\n2. Esconder (Ocultar oro en la estacion)\n";
    }
    cout << "Elija: ";
    int op; cin >> op;
    
    if(p->bando == "Policia" && op == 2) {
        Personaje* target = game.personajes;
        while(target) {
            if(target->bando == "Ladron" && target->estacion == p->estacion && target->state == "Activo") {
                target->state = "Capturado";
                game.ladrones_capturados++;
                cout << "¡" << target->nombre << " ha sido arrestado!\n";
                guardar_evento(game, "Captura", p->nombre, "Arrestó a " + target->nombre);
                break;
            }
            target = target->next;
        }
    }
    p->accion_realizada = true;
}

// --- CONTROLADOR GENERAL ---

void jugar_fase_bando(Game& game) {
    cout << "\n========================================";
    cout << "\nTURNO " << game.turno_actual << ": " << (game.turno_de_ladrones ? "LADRONES" : "POLICIA");
    cout << "\n========================================\n";

    if (game.turno_de_ladrones) {
        cout << "[SISTEMA] Identidades corruptas conocidas: ";
        Personaje* c = game.personajes;
        while(c) { if(c->corrupto) cout << "[" << c->nombre << "] "; c = c->next; }
        cout << "\n";
    }

    lanzar_dados(game);

    for(int i = 0; i < 4; i++) {
        cout << "\nDado [" << game.dados[i] << "]. Seleccione personaje para este dado:\n";
        Personaje* p = game.personajes;
        int idx = 1;
        while(p) {
            bool mi_bando = (game.turno_de_ladrones && p->bando == "Ladron") || (!game.turno_de_ladrones && p->bando == "Policia");
            if(mi_bando && p->state == "Activo" && !p->movido_este_turno) {
                cout << idx << ". " << p->nombre << " (Est " << p->estacion << ")\n";
            }
            p = p->next; idx++;
        }
        cout << "Seleccion (0 para omitir dado): ";
        int sel; cin >> sel;
        if(sel > 0) {
            p = game.personajes;
            for(int j = 1; j < sel; j++) p = p->next;
            ejecutar_movimiento(game, p, game.dados[i]);
            fase_accion_principal(game, p);
        }
    }

    // Limpieza de banderas para el siguiente turno de esta faccion
    Personaje* r = game.personajes;
    while(r) { r->movido_este_turno = false; r->accion_realizada = false; r = r->next; }
    
    if(verificar_victoria(game)) game.activo = false;
    game.turno_de_ladrones = !game.turno_de_ladrones;
    game.turno_actual++;
}

void init_setup(Game& game) {
    srand(time(0));
    cout << "--- CARGANDO CONFIGURACION DEL TREN DEL ORO ---\n";
    cargar_items(game, "accesorios.tren");
    cargar_personajes(game, "personajes.tren");
    cargar_mapa(game, "Mapa.tren");

    // Asignacion de Corruptos (Regla II.1)
    int n_polis = 0;
    Personaje* p = game.personajes;
    while(p) { if(p->bando == "Policia") n_polis++; p = p->next; }
    
    if(n_polis >= 2) {
        int c1 = rand() % n_polis, c2 = rand() % n_polis;
        while(c1 == c2) c2 = rand() % n_polis;
        int curr = 0; p = game.personajes;
        while(p) {
            if(p->bando == "Policia") {
                if(curr == c1 || curr == c2) p->corrupto = true;
                curr++;
            }
            p = p->next;
        }
    }

    // Distribucion de Oro
    game.oro_total_mapa = 20;
    Estacion* e = game.estaciones;
    while(e) { e->oro = rand() % 5; e = e->next; }

    game.activo = true;
    cout << "[OK] Sistema listo. Comienza la partida.\n";
}

int main() {
    Game game;
    init_setup(game);

    while(game.activo) {
        jugar_fase_bando(game);
        if(game.turno_actual > 100) break; // Seguridad
    }

    // Mostrar historial al final
    cout << "\n--- FIN DE LA PARTIDA. BITACORA DE EVENTOS ---\n";
    EventoBitacora* log = game.log;
    while(log) {
        cout << "Turno " << log->turno << " | " << log->personaje << " | " << log->accion << " -> " << log->resultado << endl;
        log = log->next;
    }

    return 0;
}
