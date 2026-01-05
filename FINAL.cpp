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

// --- ESTRUCTURAS ---

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
    string bando; // "Ladron" o "Policia"
    string nombre;
    int vida, vida_max;
    string state; // "Activo", "Capturado"
    int estacion;
    bool corrupto;
    int oro_lleva;
    int cap_oro;
    bool movido_este_turno;
    int items_asignados;

    Implemento* items;
    Personaje* next;

    Personaje() : next(nullptr), items(nullptr), corrupto(false), 
                  oro_lleva(0), state("Activo"), movido_este_turno(false),
                  cap_oro(3), vida(100), vida_max(100), items_asignados(0) {}
};

struct Estacion {
    int id;
    string nombre;
    int oro;
    int turnos_civil; // 0 significa que no hay civil

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
    Implemento* items_disponibles;
    Estacion* estaciones;
    EventoBitacora* log;

    int turno_actual;
    int oro_total_mapa;
    int oro_ladrones;
    int oro_recuperado_policia;
    int ladrones_totales;
    int ladrones_capturados;
    bool turno_de_ladrones;
    int dados[4];
    bool activo;

    Game() : personajes(nullptr), items_disponibles(nullptr), estaciones(nullptr), log(nullptr),
             turno_actual(1), oro_total_mapa(0), oro_ladrones(0), oro_recuperado_policia(0),
             ladrones_totales(0), ladrones_capturados(0),
             turno_de_ladrones(true), activo(false) {}
};

// --- FUNCIONES AUXILIARES ---

void guardar_evento(Game& game, string acc, string pers, string res) {
    EventoBitacora* nuevo = new EventoBitacora();
    nuevo->turno = game.turno_actual;
    nuevo->accion = acc;
    nuevo->personaje = pers;
    nuevo->resultado = res;
    nuevo->next = game.log;
    game.log = nuevo;
}

void limpiar_pantalla() {
    for(int i = 0; i < 30; i++) cout << endl;
}

// --- CARGA DE ARCHIVOS ---

bool cargar_datos(Game& game) {
    // Simulación de carga (debes mantener tus funciones ifstream aquí)
    game.oro_total_mapa = 20;
    return true;
}

// --- REGLAS SEGUNDA ENTREGA: SETUP MANUAL ---

void setup_manual(Game& game) {
    cout << "\n--- CONFIGURACION MANUAL DE PARTIDA ---\n";
    
    // 1. Elegir Estacion Inicial
    Personaje* p = game.personajes;
    while(p) {
        cout << "Elija estacion inicial para " << p->nombre << " (" << p->bando << "): ";
        cin >> p->estacion;
        
        // 2. Asignar hasta 3 items
        cout << "Asignar items a " << p->nombre << " (Max 3). Ingrese ID del item (0 para terminar): \n";
        for(int i = 0; i < 3; i++) {
            int id_item;
            cout << "Item " << i+1 << ": "; cin >> id_item;
            if(id_item == 0) break;
            // Aquí buscarías en game.items_disponibles y clonarías
            p->items_asignados++;
        }
        p = p->next;
    }
    
    // 3. Generar Civiles Aleatorios (Regla II.3)
    Estacion* e = game.estaciones;
    while(e) {
        if(rand() % 10 < 3) e->turnos_civil = (rand() % 5) + 3;
        e = e->next;
    }
}

// --- ACCIONES DETALLADAS ---

void accion_investigar(Game& game, Personaje* p) {
    cout << "[INVESTIGANDO] Buscando rastros de ladrones...\n";
    Personaje* temp = game.personajes;
    bool encontrado = false;
    while(temp) {
        if(temp->bando == "Ladron" && temp->estacion == p->estacion) {
            cout << "¡AVISTAMIENTO! Se ha detectado a " << temp->nombre << " en esta zona.\n";
            encontrado = true;
        }
        temp = temp->next;
    }
    if(!encontrado) cout << "No hay actividad sospechosa en la estacion " << p->estacion << ".\n";
}

void accion_interrogar(Game& game, Personaje* p) {
    Estacion* e = game.estaciones;
    while(e) {
        if(e->id == p->estacion) {
            if(e->turnos_civil > 0) {
                cout << "Civil interrogado: 'He visto oro en las estaciones conectadas a la " << e->id << "'.\n";
            } else {
                cout << "No hay nadie a quien interrogar aquí.\n";
            }
            return;
        }
        e = e->next;
    }
}

void accion_sabotear(Game& game, Personaje* p) {
    if(!p->corrupto) return;
    cout << "[SABOTAJE] Has ocultado pruebas. El oro recuperado bajará en el informe oficial.\n";
    game.oro_recuperado_policia = max(0, game.oro_recuperado_policia - 2);
    guardar_evento(game, "Sabotaje", p->nombre, "Mintió sobre el hallazgo de oro");
}

// --- CICLO DE TURNO ---

void gestionar_civiles(Game& game) {
    Estacion* e = game.estaciones;
    while(e) {
        if(e->turnos_civil > 0) e->turnos_civil--;
        e = e->next;
    }
}

bool verificar_victoria_final(Game& game) {
    float perc_oro = (float)game.oro_ladrones / game.oro_total_mapa;
    float perc_captura = (float)game.ladrones_capturados / (game.ladrones_totales > 0 ? game.ladrones_totales : 1);

    if (perc_oro > 0.70) { cout << "VICTORIA SOMBRAS\n"; return true; }
    if (perc_captura >= 0.80 && (float)game.oro_recuperado_policia/game.oro_total_mapa >= 0.90) {
        cout << "VICTORIA POLICIA HONESTA\n"; return true;
    }
    return false;
}

void jugar_turno_bando(Game& game) {
    limpiar_pantalla();
    cout << "--- TURNO DE " << (game.turno_de_ladrones ? "LADRONES" : "POLICIA") << " ---" << endl;
    
    if(game.turno_de_ladrones) {
        cout << "[SECRETO] Agentes Corruptos: ";
        Personaje* c = game.personajes;
        while(c) { if(c->corrupto) cout << c->nombre << " "; c = c->next; }
        cout << endl;
    }

    // Regla III.1: Lanzar 4 dados
    int dados_usados = 0;
    int frecuencias[7] = {0};
    for(int i=0; i<4; i++) {
        game.dados[i] = (rand() % 6) + 1;
        frecuencias[game.dados[i]]++;
    }

    // Doble-dobles (Regla III.1.2)
    int pares = 0;
    for(int i=1; i<=6; i++) if(frecuencias[i] >= 2) pares++;
    
    while(dados_usados < 4) {
        cout << "\nDados disponibles: ";
        for(int i=0; i<4; i++) if(game.dados[i] != 0) cout << "[" << game.dados[i] << "] ";
        
        cout << "\nElija un dado para usar (1-4, o 0 para pasar): ";
        int d_idx; cin >> d_idx;
        if(d_idx == 0) break;
        d_idx--;

        cout << "Elija personaje para mover:\n";
        Personaje* p = game.personajes;
        int idx = 1;
        while(p) {
            bool mi_bando = (game.turno_de_ladrones && p->bando == "Ladron") || (!game.turno_de_ladrones && p->bando == "Policia");
            if(mi_bando && p->state == "Activo" && !p->movido_este_turno) 
                cout << idx << ". " << p->nombre << endl;
            p = p->next; idx++;
        }

        int p_sel; cin >> p_sel;
        p = game.personajes;
        for(int j=1; j<p_sel; j++) p = p->next;

        // MOVIMIENTO
        int pasos = game.dados[d_idx];
        for(int s=0; s<pasos; s++) {
            cout << "Paso " << s+1 << "/" << pasos << ". Estacion actual: " << p->estacion << ". Destino: ";
            cin >> p->estacion;
            // Lógica de recoger oro (Regla III.Fase Acción)
            Estacion* est = game.estaciones;
            while(est) {
                if(est->id == p->estacion && est->oro > 0) {
                   if(p->bando == "Ladron") { game.oro_ladrones++; est->oro--; }
                }
                est = est->next;
            }
        }
        
        // ACCION (Regla III.Fase Acción)
        cout << "Accion: 1.Investigar 2.Interrogar 3.Capturar 4.Sabotear: ";
        int acc; cin >> acc;
        if(acc == 1) accion_investigar(game, p);
        if(acc == 2) accion_interrogar(game, p);
        if(acc == 4 && p->corrupto) accion_sabotear(game, p);

        p->movido_este_turno = true;
        game.dados[d_idx] = 0;
        dados_usados++;
    }

    gestionar_civiles(game);
    if(verificar_victoria_final(game)) game.activo = false;
    game.turno_de_ladrones = !game.turno_de_ladrones;
    game.turno_actual++;
}

// --- MAIN ---

int main() {
    srand(time(0));
    Game game;
    
    // Carga de datos base
    cargar_datos(game);
    
    // Mock de personajes para prueba
    Personaje* p1 = new Personaje(); p1->nombre = "Ladron1"; p1->bando = "Ladron";
    Personaje* p2 = new Personaje(); p2->nombre = "Policia1"; p2->bando = "Policia";
    Personaje* p3 = new Personaje(); p3->nombre = "Policia2"; p3->bando = "Policia"; p3->corrupto = true;
    p1->next = p2; p2->next = p3;
    game.personajes = p1; game.ladrones_totales = 1;

    // Mock de estaciones
    Estacion* e1 = new Estacion(); e1->id = 1; e1->oro = 10;
    Estacion* e2 = new Estacion(); e2->id = 2; e2->oro = 5;
    e1->next = e2; game.estaciones = e1;

    setup_manual(game);
    game.activo = true;

    while(game.activo) {
        jugar_turno_bando(game);
        if(game.turno_actual > 50) break;
    }

    return 0;
}
