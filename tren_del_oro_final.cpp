/*===================================================================
  PROYECTO: EL TREN DEL ORO
  ASIGNATURA: Algoritmos y Estructuras de Datos
  PROFESOR: Yisheng Leon Espinoza
===================================================================*/

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

using namespace std;

// ===================================================================
// ESTRUCTURAS DE DATOS PRINCIPALES
// ===================================================================

struct Implemento {
    int idImplemento;
    string tipoUsuario;
    string nombre;
    string funcion;
    int usoMaximo;
    int alcance;
    int usosRestantes;
    Implemento* siguiente;

    Implemento() : idImplemento(0), usoMaximo(0), alcance(0), usosRestantes(0), siguiente(nullptr) {}
};

struct Personaje {
    int idPersonaje;
    string bando;
    string nombre;
    int vidaActual;
    int vidaMaxima;
    string tipo;
    string estado;
    int estacionActual;
    bool esCorrupto;
    int oroTransportando;
    int capacidadOro;

    Implemento* implementos;
    Personaje* siguiente;

    Personaje() : idPersonaje(0), vidaActual(100), vidaMaxima(100),
                  estado("Activo"), estacionActual(1), esCorrupto(false),
                  oroTransportando(0), capacidadOro(0),
                  implementos(nullptr), siguiente(nullptr) {}
};

struct Estacion {
    int idEstacion;
    string nombre;
    int oroDisponible;

    struct Arista {
        int destino;
        int distancia;
        Arista* siguiente;
        Arista() : destino(0), distancia(0), siguiente(nullptr) {}
    };

    Arista* adyacentes;
    Estacion* siguiente;

    Estacion() : idEstacion(0), oroDisponible(0), adyacentes(nullptr), siguiente(nullptr) {}
};

struct RegistroBitacora {
    int turno;
    string accion;
    string personaje;
    string resultado;
    RegistroBitacora* siguiente;

    RegistroBitacora() : turno(0), siguiente(nullptr) {}
};

struct SistemaJuego {
    Personaje* listaPersonajes;
    Implemento* listaImplementos;
    Estacion* grafoEstaciones;
    RegistroBitacora* bitacora;

    int turnoActual;
    int totalOro;
    int oroRecuperado;
    int ladronesCapturados;
    int corruptosIdentificados;
    bool juegoEnCurso;
    string ganador;
    string faccionGanadora;
    int turnosJugados;

    SistemaJuego() : listaPersonajes(nullptr), listaImplementos(nullptr),
                     grafoEstaciones(nullptr), bitacora(nullptr),
                     turnoActual(0), totalOro(0), oroRecuperado(0),
                     ladronesCapturados(0), corruptosIdentificados(0),
                     juegoEnCurso(false), ganador(""), faccionGanadora(""),
                     turnosJugados(0) {}
};

// ===================================================================
// VALIDACIONES / ENTRADAS SEGURAS
// ===================================================================

bool validacionNumeros(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    return true;
}

int leerEnteroSeguro(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);

        // trim simple
        while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();

        if (!validacionNumeros(s)) {
            cout << "ERROR: Ingrese un numero valido.\n";
            continue;
        }

        try {
            return stoi(s);
        } catch (...) {
            cout << "ERROR: Numero fuera de rango.\n";
        }
    }
}

char leerSN(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        if (s.empty()) continue;
        char c = (char)toupper((unsigned char)s[0]);
        if (c == 'S' || c == 'N') return c;
        cout << "ERROR: Responda S o N.\n";
    }
}

// ===================================================================
// BITACORA
// ===================================================================

void registrarBitacora(SistemaJuego& sistema, const string& accion,
                       const string& personaje, const string& resultado) {
    RegistroBitacora* nuevoRegistro = new RegistroBitacora();
    nuevoRegistro->turno = sistema.turnoActual;
    nuevoRegistro->accion = accion;
    nuevoRegistro->personaje = personaje;
    nuevoRegistro->resultado = resultado;
    nuevoRegistro->siguiente = sistema.bitacora;
    sistema.bitacora = nuevoRegistro;
}

// ===================================================================
// AYUDANTE DE BUSQUEDA
// ===================================================================

Estacion* buscarEstacion(SistemaJuego& sistema, int idEstacion) {
    Estacion* e = sistema.grafoEstaciones;
    while (e) {
        if (e->idEstacion == idEstacion) return e;
        e = e->siguiente;
    }
    return nullptr;
}

// ===================================================================
// CARGA DE ARCHIVOS
// ===================================================================

bool cargarImplementos(SistemaJuego& sistema, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir archivo de implementos: " << nombreArchivo << endl;
        return false;
    }

    int cantidad;
    archivo >> cantidad;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea); // separador "---" o linea vacia

        Implemento* nuevoImpl = new Implemento();

        getline(archivo, linea);
        nuevoImpl->idImplemento = stoi(linea);

        getline(archivo, nuevoImpl->tipoUsuario);
        getline(archivo, nuevoImpl->nombre);

        getline(archivo, linea);
        size_t pos = linea.find(": ");
        if (pos != string::npos) {
            nuevoImpl->funcion = linea.substr(pos + 2);
        }

        getline(archivo, linea);
        pos = linea.find(": ");
        if (pos != string::npos) {
            nuevoImpl->usoMaximo = stoi(linea.substr(pos + 2));
            nuevoImpl->usosRestantes = nuevoImpl->usoMaximo;
        }

        getline(archivo, linea);
        pos = linea.find(": ");
        if (pos != string::npos) {
            nuevoImpl->alcance = stoi(linea.substr(pos + 2));
        }

        nuevoImpl->siguiente = sistema.listaImplementos;
        sistema.listaImplementos = nuevoImpl;
    }

    archivo.close();
    cout << "[OK] Implementos cargados exitosamente (" << cantidad << " items)" << endl;
    return true;
}

bool cargarPersonajes(SistemaJuego& sistema, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir archivo de personajes: " << nombreArchivo << endl;
        return false;
    }

    int cantidad;
    archivo >> cantidad;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea); // separador "---" o vacia

        Personaje* nuevoPerso = new Personaje();

        getline(archivo, linea);
        nuevoPerso->idPersonaje = stoi(linea);

        getline(archivo, nuevoPerso->bando);
        getline(archivo, nuevoPerso->nombre);

        getline(archivo, linea);
        if (linea != "-") {
            stringstream ss(linea);
            string idStr;
            while (getline(ss, idStr, '|')) {
                int idImpl = stoi(idStr);
                Implemento* impl = sistema.listaImplementos;
                while (impl) {
                    if (impl->idImplemento == idImpl) {
                        Implemento* copia = new Implemento(*impl);
                        copia->siguiente = nuevoPerso->implementos;
                        nuevoPerso->implementos = copia;
                        break;
                    }
                    impl = impl->siguiente;
                }
            }
        }

        nuevoPerso->estado = "Activo";
        nuevoPerso->vidaMaxima = 100;
        nuevoPerso->vidaActual = 100;
        nuevoPerso->estacionActual = 1;
        nuevoPerso->capacidadOro = (nuevoPerso->bando == "Ladron") ? 5 : 1;
        nuevoPerso->esCorrupto = false;

        nuevoPerso->siguiente = sistema.listaPersonajes;
        sistema.listaPersonajes = nuevoPerso;
    }

    archivo.close();
    cout << "[OK] Personajes cargados exitosamente (" << cantidad << " personajes)" << endl;
    return true;
}

bool cargarMapa(SistemaJuego& sistema, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir archivo de mapa: " << nombreArchivo << endl;
        return false;
    }

    int cantidad;
    archivo >> cantidad;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea); // separador "---" o vacia

        Estacion* nuevaEst = new Estacion();

        getline(archivo, linea);
        nuevaEst->idEstacion = stoi(linea);

        getline(archivo, nuevaEst->nombre);

        getline(archivo, linea); // "-"
        nuevaEst->oroDisponible = 0;

        getline(archivo, linea); // conexiones "5:3|12:6" o "-"
        if (linea != "-") {
            stringstream ss(linea);
            string conexion;
            while (getline(ss, conexion, '|')) {
                size_t pos = conexion.find(':');
                if (pos != string::npos) {
                    int destino = stoi(conexion.substr(0, pos));
                    int distancia = stoi(conexion.substr(pos + 1));

                    Estacion::Arista* arista = new Estacion::Arista();
                    arista->destino = destino;
                    arista->distancia = distancia;
                    arista->siguiente = nuevaEst->adyacentes;
                    nuevaEst->adyacentes = arista;
                }
            }
        }

        nuevaEst->siguiente = sistema.grafoEstaciones;
        sistema.grafoEstaciones = nuevaEst;
    }

    archivo.close();
    cout << "[OK] Mapa cargado exitosamente (" << cantidad << " estaciones)" << endl;
    return true;
}

// ===================================================================
// POSICIONAMIENTO INICIAL
// ===================================================================

void posicionarPersonajesInicio(SistemaJuego& sistema) {
    cout << "\n=== POSICIONAMIENTO INICIAL ===\n";
    cout << "Ingrese la estacion inicial (ID) para cada personaje.\n";
    cout << "Puede ver los nombres desde 'Ver Mapa'.\n\n";

    Personaje* p = sistema.listaPersonajes;
    while (p) {
        bool ok = false;
        while (!ok) {
            cout << "Personaje [" << p->idPersonaje << "] " << p->nombre
                 << " (" << p->bando << ") -> Estacion inicial: ";

            int idEst;
            if (!(cin >> idEst)) {
                cout << "ERROR: Ingrese un numero valido.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Estacion* e = buscarEstacion(sistema, idEst);
            if (!e) {
                cout << "ERROR: Estacion no existe.\n";
                continue;
            }

            p->estacionActual = idEst;
            ok = true;
            cout << "[OK] Asignado a estacion " << idEst << " (" << e->nombre << ")\n\n";
        }
        p = p->siguiente;
    }

    cout << "[OK] Posicionamiento completado.\n";
}

// ===================================================================
// INICIALIZACION
// ===================================================================

void inicializarSistema(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    INICIALIZANDO SISTEMA - EL TREN DEL ORO" << endl;
    cout << "============================================================" << endl;

    if (!cargarImplementos(sistema, "accesorios.tren")) return;
    if (!cargarPersonajes(sistema, "personajes.tren")) return;
    if (!cargarMapa(sistema, "Mapa.tren")) return;

    cout << "\n[SISTEMA] Identificando agentes infiltrados..." << endl;

    vector<Personaje*> policias;
    Personaje* temp = sistema.listaPersonajes;
    while (temp) {
        if (temp->bando == "Policia") policias.push_back(temp);
        temp = temp->siguiente;
    }

    if (policias.size() >= 2) {
        srand(static_cast<unsigned>(time(nullptr)));
        random_shuffle(policias.begin(), policias.end());
        policias[0]->esCorrupto = true;
        policias[1]->esCorrupto = true;
    }

    cout << "[SISTEMA] Distribuyendo oro en estaciones..." << endl;
    sistema.totalOro = 20;

    Estacion* est = sistema.grafoEstaciones;
    int oroRestante = sistema.totalOro;

    while (est && oroRestante > 0) {
        int cantidad = rand() % 4;
        if (cantidad > oroRestante) cantidad = oroRestante;

        est->oroDisponible = cantidad;
        oroRestante -= cantidad;
        est = est->siguiente;
    }

    cout << "\n[OK] SISTEMA INICIALIZADO CORRECTAMENTE" << endl;
    cout << "  - Total de oro distribuido: " << sistema.totalOro << " lingotes" << endl;
}

// ===================================================================
// VICTORIA
// ===================================================================

bool verificarVictoria(SistemaJuego& sistema) {
    if (sistema.oroRecuperado >= 15) {
        sistema.ganador = "LADRONES";
        sistema.faccionGanadora = "LADRONES";
        return true;
    }

    if (sistema.ladronesCapturados >= 5) {
        sistema.ganador = "POLICIAS_HONESTOS";
        sistema.faccionGanadora = "POLICIAS HONESTOS";
        return true;
    }

    int oroEscapado = sistema.totalOro - sistema.oroRecuperado;
    if (oroEscapado >= 8 && oroEscapado <= 12) {
        sistema.ganador = "POLICIAS_CORRUPTOS";
        sistema.faccionGanadora = "POLICIAS CORRUPTOS";
        return true;
    }

    return false;
}

void mostrarPantallaVictoria(SistemaJuego& sistema) {
    cout << "\n\n" << endl;
    cout << "============================================================" << endl;
    cout << "                  *** JUEGO TERMINADO ***" << endl;
    cout << "============================================================" << endl;
    cout << "\n";

    if (sistema.ganador == "LADRONES") {
        cout << "    !!! LOS LADRONES HAN GANADO !!!" << endl;
        cout << "\n    Lograron recolectar: " << sistema.oroRecuperado << "/20 lingotes de oro" << endl;
        cout << "    Porcentaje: " << (sistema.oroRecuperado * 100 / sistema.totalOro) << "%" << endl;
    } else if (sistema.ganador == "POLICIAS_HONESTOS") {
        cout << "    !!! LOS POLICIAS HONESTOS HAN GANADO !!!" << endl;
        cout << "\n    Ladrones capturados: " << sistema.ladronesCapturados << endl;
        cout << "    Oro recuperado: " << sistema.oroRecuperado << "/" << sistema.totalOro << endl;
        cout << "    Porcentaje recuperado: " << (sistema.oroRecuperado * 100 / sistema.totalOro) << "%" << endl;
    } else if (sistema.ganador == "POLICIAS_CORRUPTOS") {
        cout << "    !!! LOS POLICIAS CORRUPTOS HAN GANADO !!!" << endl;
        int oroEscapado = sistema.totalOro - sistema.oroRecuperado;
        cout << "\n    Oro que logro escapar: " << oroEscapado << "/20 lingotes" << endl;
        cout << "    Porcentaje de escape: " << (oroEscapado * 100 / sistema.totalOro) << "%" << endl;
        cout << "    Rango ganable: 40-60% (8-12 lingotes) - [CUMPLIDO]" << endl;
    }

    cout << "\n    Turnos jugados: " << sistema.turnoActual << endl;
    cout << "    Corruptos identificados: " << sistema.corruptosIdentificados << endl;

    cout << "\n============================================================" << endl;
    cout << "              Presione ENTER para ver bitacora..." << endl;
    cout << "============================================================" << endl;
    cin.ignore();
}

void mostrarBitacora(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    BITACORA DEL JUEGO - HISTORIAL COMPLETO" << endl;
    cout << "============================================================" << endl;

    if (!sistema.bitacora) {
        cout << "\nNo hay registros en la bitacora." << endl;
        return;
    }

    RegistroBitacora* registros[1000];
    int contador = 0;
    RegistroBitacora* temp = sistema.bitacora;
    while (temp && contador < 1000) {
        registros[contador++] = temp;
        temp = temp->siguiente;
    }

    cout << "\nTurno | Personaje | Accion | Resultado\n";
    cout << "-----------------------------------------------\n";

    for (int i = contador - 1; i >= 0; i--) {
        cout << registros[i]->turno << " | "
             << registros[i]->personaje << " | "
             << registros[i]->accion << " | "
             << registros[i]->resultado << endl;
    }

    cout << "\nPresione ENTER para volver al menu..." << endl;
    cin.ignore();
}

// ===================================================================
// CONSULTA / DISPLAY
// ===================================================================

void mostrarPersonajes(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    LISTA DE PERSONAJES DISPONIBLES" << endl;
    cout << "============================================================" << endl;

    Personaje* temp = sistema.listaPersonajes;
    if (!temp) {
        cout << "No hay personajes cargados." << endl;
        return;
    }

    while (temp) {
        cout << "ID: " << temp->idPersonaje
             << " | " << temp->nombre
             << " | Bando: " << temp->bando
             << " | Estado: " << temp->estado
             << " | Vida: " << temp->vidaActual << "/" << temp->vidaMaxima
             << " | Estacion: " << temp->estacionActual;
        if (temp->bando == "Policia" && temp->esCorrupto) cout << " [CORRUPTO]";
        if (temp->oroTransportando > 0) cout << " [ORO:" << temp->oroTransportando << "]";
        cout << endl;

        temp = temp->siguiente;
    }
}

void mostrarImplementos(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    LISTA DE IMPLEMENTOS DISPONIBLES" << endl;
    cout << "============================================================" << endl;

    Implemento* temp = sistema.listaImplementos;
    if (!temp) {
        cout << "No hay implementos cargados." << endl;
        return;
    }

    while (temp) {
        cout << "ID: " << temp->idImplemento
             << " | " << temp->nombre
             << " | Usuario: " << temp->tipoUsuario
             << " | Funcion: " << temp->funcion
             << " | Usos: " << temp->usosRestantes
             << " | Alcance: " << temp->alcance << endl;

        temp = temp->siguiente;
    }
}

void mostrarMapa(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    MAPA - RED DE METRO" << endl;
    cout << "============================================================" << endl;

    Estacion* temp = sistema.grafoEstaciones;
    if (!temp) {
        cout << "No hay estaciones cargadas." << endl;
        return;
    }

    while (temp) {
        cout << "Estacion " << temp->idEstacion << " - " << temp->nombre
             << " | Oro: " << temp->oroDisponible << " | Conexiones: ";

        Estacion::Arista* arista = temp->adyacentes;
        if (!arista) {
            cout << "-";
        } else {
            bool primera = true;
            while (arista) {
                if (!primera) cout << " | ";
                cout << arista->destino << ":" << arista->distancia;
                primera = false;
                arista = arista->siguiente;
            }
        }
        cout << endl;

        temp = temp->siguiente;
    }
}

// ===================================================================
// JUEGO
// ===================================================================

void mostrarEstadoPartida(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    ESTADO DE LA PARTIDA - TURNO " << sistema.turnoActual << endl;
    cout << "============================================================" << endl;
    cout << "\nOro Total en Juego: " << sistema.totalOro << " lingotes" << endl;
    cout << "Oro Recuperado (Ladrones): " << sistema.oroRecuperado << "/15 PARA GANAR" << endl;
    cout << "Oro Escapado: " << (sistema.totalOro - sistema.oroRecuperado) << " lingotes" << endl;
    cout << "Ladrones Capturados (Policias): " << sistema.ladronesCapturados << "/5 PARA GANAR" << endl;
    cout << "Corruptos Identificados: " << sistema.corruptosIdentificados << endl;
}

void mostrarPersonajesActivos(SistemaJuego& sistema) {
    cout << "\n--- PERSONAJES ACTIVOS ---" << endl;
    Personaje* temp = sistema.listaPersonajes;
    int contador = 0;

    while (temp) {
        if (temp->estado == "Activo") {
            cout << contador + 1 << ". " << temp->nombre
                 << " (" << temp->bando << ") - Estacion "
                 << temp->estacionActual << " - Vida: "
                 << temp->vidaActual << "/" << temp->vidaMaxima;
            if (temp->bando == "Policia" && temp->esCorrupto) cout << " [CORRUPTO]";
            if (temp->oroTransportando > 0) cout << " [LLEVA " << temp->oroTransportando << " ORO]";
            cout << endl;
            contador++;
        }
        temp = temp->siguiente;
    }
}

Personaje* seleccionarPersonaje(SistemaJuego& sistema) {
    mostrarPersonajesActivos(sistema);

    cout << "\nSeleccione numero de personaje a mover: ";
    int opcion;

    if (!(cin >> opcion)) {
        cout << "ERROR: Ingrese un numero valido." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return nullptr;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Personaje* temp = sistema.listaPersonajes;
    int contador = 0;

    while (temp) {
        if (temp->estado == "Activo") {
            contador++;
            if (contador == opcion) {
                return temp;
            }
        }
        temp = temp->siguiente;
    }

    cout << "ERROR: Personaje no encontrado." << endl;
    return nullptr;
}

int contarAdyacentes(Estacion* est) {
    int c = 0;
    Estacion::Arista* a = est ? est->adyacentes : nullptr;
    while (a) { c++; a = a->siguiente; }
    return c;
}

Estacion::Arista* obtenerAristaPorIndice(Estacion* est, int idx1based) {
    if (!est || idx1based <= 0) return nullptr;
    int c = 1;
    Estacion::Arista* a = est->adyacentes;
    while (a) {
        if (c == idx1based) return a;
        c++;
        a = a->siguiente;
    }
    return nullptr;
}

void mostrarEstacionesConexas(SistemaJuego& sistema, int idEstacionActual) {
    Estacion* est = buscarEstacion(sistema, idEstacionActual);

    while (est) {
        if (est->idEstacion == idEstacionActual) {
            cout << "\n--- ESTACIONES CONECTADAS ---" << endl;
            Estacion::Arista* arista = est->adyacentes;
            int contador = 1;

            if (!arista) {
                cout << "No hay estaciones conectadas." << endl;
                return;
            }

            while (arista) {
                Estacion* destino = buscarEstacion(sistema, arista->destino);
                if (destino) {
                    cout << contador << ". Estacion " << arista->destino
                         << " (" << destino->nombre << ") - Distancia: "
                         << arista->distancia << " - Oro: "
                         << destino->oroDisponible << endl;
                } else {
                    cout << contador << ". Estacion " << arista->destino
                         << " (Desconocida) - Distancia: " << arista->distancia << endl;
                }
                contador++;
                arista = arista->siguiente;
            }
            return;
        }
        est = est->siguiente;
    }
}

void aplicarRecoleccionSiCorresponde(SistemaJuego& sistema, Personaje* personaje) {
    if (!personaje) return;
    Estacion* est = buscarEstacion(sistema, personaje->estacionActual);
    if (!est) return;

    if (personaje->bando == "Ladron" && est->oroDisponible > 0) {
        int espacio = personaje->capacidadOro - personaje->oroTransportando;
        if (espacio <= 0) {
            cout << "[INFO] No tiene capacidad para cargar mas oro." << endl;
            return;
        }

        int oroTomado = min(est->oroDisponible, espacio);
        personaje->oroTransportando += oroTomado;
        est->oroDisponible -= oroTomado;
        sistema.oroRecuperado += oroTomado;

        cout << "[OK] " << personaje->nombre << " tomo " << oroTomado << " lingotes de oro!" << endl;

        string resultado_oro = "Recolecto " + to_string(oroTomado) + " oro";
        registrarBitacora(sistema, "Recoleccion", personaje->nombre, resultado_oro);
    }
}

void turnoJugador(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    TURNO INTERACTIVO" << endl;
    cout << "============================================================" << endl;

    Personaje* personaje = seleccionarPersonaje(sistema);
    if (!personaje) {
        cout << "Turno saltado." << endl;
        return;
    }

    // Pedir distancia maxima del turno (entero)
    int distanciaMax = -1;
    while (distanciaMax < 0) {
        distanciaMax = leerEnteroSeguro("\nIngrese la distancia maxima que puede recorrer este turno (entero >= 0): ");
        if (distanciaMax < 0) cout << "ERROR: La distancia no puede ser negativa.\n";
    }

    int distanciaRestante = distanciaMax;

    cout << "\nPersonaje seleccionado: " << personaje->nombre << endl;
    cout << "Ubicacion actual: Estacion " << personaje->estacionActual << endl;
    cout << "Distancia disponible este turno: " << distanciaRestante << endl;

    // Permitir múltiples movimientos mientras haya distancia y el usuario quiera
    while (true) {
        if (distanciaRestante <= 0) {
            cout << "\n[INFO] Distancia agotada. Fin del turno.\n";
            return;
        }

        Estacion* estActual = buscarEstacion(sistema, personaje->estacionActual);
        if (!estActual) {
            cout << "ERROR: Estacion actual invalida.\n";
            return;
        }

        int totalOps = contarAdyacentes(estActual);
        if (totalOps == 0) {
            cout << "\n[INFO] No hay conexiones desde esta estacion. Fin del turno.\n";
            return;
        }

        mostrarEstacionesConexas(sistema, personaje->estacionActual);

        int opcion = -1;
        while (true) {
            opcion = leerEnteroSeguro("\nSeleccione estacion destino (numero de la lista): ");
            if (opcion < 1 || opcion > totalOps) {
                cout << "ERROR: Opcion fuera de rango (1-" << totalOps << ").\n";
                continue;
            }
            break;
        }

        Estacion::Arista* aristaElegida = obtenerAristaPorIndice(estActual, opcion);
        if (!aristaElegida) {
            cout << "ERROR: Conexion invalida.\n";
            continue;
        }

        // VALIDACION PRINCIPAL: distancia del arco vs distancia restante
        if (aristaElegida->distancia > distanciaRestante) {
            cout << "\n[ERROR] No puedes moverte a esa estacion.\n";
            cout << "Distancia requerida: " << aristaElegida->distancia << "\n";
            cout << "Distancia disponible: " << distanciaRestante << "\n";
            cout << "Elige otra conexion.\n";
            continue;
        }

        // Ejecutar movimiento
        personaje->estacionActual = aristaElegida->destino;
        distanciaRestante -= aristaElegida->distancia;

        cout << "\n[OK] " << personaje->nombre << " se movio a Estacion "
             << aristaElegida->destino << endl;
        cout << "[INFO] Distancia consumida: " << aristaElegida->distancia
             << " | Distancia restante: " << distanciaRestante << endl;

        registrarBitacora(sistema, "Movimiento", personaje->nombre,
                          "Movimiento a estacion " + to_string(aristaElegida->destino) +
                          " (dist " + to_string(aristaElegida->distancia) + ")");

        // Recoleccion automática si es ladrón y hay oro
        aplicarRecoleccionSiCorresponde(sistema, personaje);

        // Preguntar si quiere seguir moviendo
        char seguir = leerSN("\nDesea seguir moviendose en este turno? (S/N): ");
        if (seguir == 'N') return;
    }
}

void juegoInteractivo(SistemaJuego& sistema) {
    sistema.juegoEnCurso = true;
    sistema.turnoActual = 1;
    int opcionMenu = -1;

    char resp = leerSN("\nDesea posicionar manualmente los personajes antes de iniciar? (S/N): ");
    if (resp == 'S') {
        posicionarPersonajesInicio(sistema);
    }

    while (sistema.juegoEnCurso) {
        if (verificarVictoria(sistema)) {
            sistema.turnosJugados = sistema.turnoActual;
            mostrarPantallaVictoria(sistema);
            mostrarBitacora(sistema);
            sistema.juegoEnCurso = false;
            break;
        }

        mostrarEstadoPartida(sistema);

        cout << "\n============================================================" << endl;
        cout << "    MENU DE JUEGO" << endl;
        cout << "============================================================" << endl;
        cout << "\n1. Jugar Turno" << endl;
        cout << "2. Ver Personajes" << endl;
        cout << "3. Ver Mapa" << endl;
        cout << "4. Ver Estadisticas" << endl;
        cout << "5. Ver Bitacora" << endl;
        cout << "6. Terminar Partida" << endl;
        cout << "\nSeleccione opcion (1-6): ";

        if (!(cin >> opcionMenu)) {
            cout << "ERROR: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcionMenu) {
            case 1:
                turnoJugador(sistema);
                sistema.turnoActual++;
                break;
            case 2:
                mostrarPersonajes(sistema);
                break;
            case 3:
                mostrarMapa(sistema);
                break;
            case 4:
                mostrarEstadoPartida(sistema);
                break;
            case 5:
                mostrarBitacora(sistema);
                break;
            case 6:
                sistema.juegoEnCurso = false;
                cout << "\n[OK] Partida terminada. Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "ERROR: Opcion invalida. Intente nuevamente." << endl;
        }
    }
}

// ===================================================================
// MENU PRINCIPAL
// ===================================================================

void mostrarMenuPrincipal(SistemaJuego& sistema) {
    int opcion = -1;

    do {
        cout << "\n============================================================" << endl;
        cout << "    MENU PRINCIPAL - EL TREN DEL ORO" << endl;
        cout << "============================================================" << endl;
        cout << "\n1. Ver Personajes" << endl;
        cout << "2. Ver Implementos" << endl;
        cout << "3. Ver Mapa (Estaciones)" << endl;
        cout << "4. Iniciar Partida" << endl;
        cout << "5. Salir" << endl;
        cout << "\nSeleccione opcion (1-5): ";

        if (!(cin >> opcion)) {
            cout << "ERROR: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1:
                mostrarPersonajes(sistema);
                break;
            case 2:
                mostrarImplementos(sistema);
                break;
            case 3:
                mostrarMapa(sistema);
                break;
            case 4:
                juegoInteractivo(sistema);
                break;
            case 5:
                cout << "\n[OK] Programa terminado. Gracias por jugar!" << endl;
                break;
            default:
                cout << "ERROR: Opcion invalida. Intente nuevamente." << endl;
        }
    } while (opcion != 5);
}

// ===================================================================
// MAIN
// ===================================================================

int main() {
    SistemaJuego sistema;

    inicializarSistema(sistema);
    mostrarMenuPrincipal(sistema);

    return 0;
}
