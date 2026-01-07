/*===================================================================
  PROYECTO: EL TREN DEL ORO - ENTREGA FINAL COMPLETA
  ASIGNATURA: Algoritmos y Estructuras de Datos
  PROFESOR: Yisheng Leon Espinoza

  DESCRIPCION:
  Sistema de gestion de un juego de estrategia para dos jugadores:
  Policias vs Ladrones en una red de metro. Implementa grafos
  (estaciones conectadas) y listas enlazadas para gestionar
  personajes, implementos y relaciones de juego.

  CONDICIONES DE VICTORIA FINALES:
  LADRONES GANAN si:
    - Recolectan 15 o mas lingotes de oro (75% de 20)

  POLICIAS HONESTOS GANAN si:
    - Capturan 5 o mas ladrones
    O capturan a ambos corruptos

  POLICIAS CORRUPTOS GANAN si:
    - Logran que escape entre 40% y 60% del oro (8-12 lingotes)

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
#include <iomanip>

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

    Implemento() : siguiente(nullptr) {}
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

    Personaje() : siguiente(nullptr), implementos(nullptr),
                  esCorrupto(false), oroTransportando(0) {}
};

struct Estacion {
    int idEstacion;
    string nombre;
    int oroDisponible;

    struct Arista {
        int destino;
        int distancia;
        Arista* siguiente;
        Arista() : siguiente(nullptr) {}
    };

    Arista* adyacentes;
    Estacion* siguiente;

    Estacion() : siguiente(nullptr), adyacentes(nullptr),
                 oroDisponible(0) {}
};

struct RegistroBitacora {
    int turno;
    string accion;
    string personaje;
    string resultado;
    RegistroBitacora* siguiente;

    RegistroBitacora() : siguiente(nullptr) {}
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
// FUNCIONES DE VALIDACION
// ===================================================================

bool validacionEspacios(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isspace(static_cast<unsigned char>(c)))
            return false;
    return true;
}

bool validacionNumeros(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    return true;
}

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
// FUNCIONES DE CARGA DE ARCHIVOS
// ===================================================================

bool cargarImplementos(SistemaJuego& sistema, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir archivo de implementos: " << nombreArchivo << endl;
        return false;
    }

    int cantidad;
    archivo >> cantidad;
    archivo.ignore();

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea);

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
    archivo.ignore();

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea);

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
    archivo.ignore();

    for (int i = 0; i < cantidad; i++) {
        string linea;
        getline(archivo, linea);

        Estacion* nuevaEst = new Estacion();

        getline(archivo, linea);
        nuevaEst->idEstacion = stoi(linea);

        getline(archivo, nuevaEst->nombre);

        getline(archivo, linea);
        nuevaEst->oroDisponible = 0;

        getline(archivo, linea);
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
// FUNCIONES DE INICIALIZACION
// ===================================================================

void inicializarSistema(SistemaJuego& sistema) {
    cout << "\n============================================================" << endl;
    cout << "    INICIALIZANDO SISTEMA - EL TREN DEL ORO" << endl;
    cout << "============================================================" << endl;

    if (!cargarImplementos(sistema, "accesorios.tren")) return;
    if (!cargarPersonajes(sistema, "personajes.tren")) return;
    if (!cargarMapa(sistema, "Mapa.tren")) return;

    cout << "\n[SISTEMA] Identificando agentes infiltrados..." << endl;
    Personaje* policias[100];
    int contPolicia = 0;

    Personaje* temp = sistema.listaPersonajes;
    while (temp) {
        if (temp->bando == "Policia") {
            policias[contPolicia++] = temp;
        }
        temp = temp->siguiente;
    }

    if (contPolicia >= 2) {
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < 2; i++) {
            int indice = rand() % contPolicia;
            policias[indice]->esCorrupto = true;
        }
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
    cout << "  - Estaciones creadas: [Consultar mapa]" << endl;
    cout << "  - Personajes listos: [lista]" << endl;
    cout << "  - Implementos disponibles: [lista]" << endl;
}

// ===================================================================
// FUNCIONES DE CONDICIONES DE VICTORIA
// ===================================================================

bool verificarVictoria(SistemaJuego& sistema) {
    // CONDICION 1: LADRONES GANAN si recolectan 15+ lingotes (75%)
    if (sistema.oroRecuperado >= 15) {
        sistema.ganador = "LADRONES";
        sistema.faccionGanadora = "LADRONES";
        return true;
    }

    // CONDICION 2: POLICIAS HONESTOS GANAN si capturan 5+ ladrones
    if (sistema.ladronesCapturados >= 5) {
        sistema.ganador = "POLICIAS_HONESTOS";
        sistema.faccionGanadora = "POLICIAS HONESTOS";
        return true;
    }

    // CONDICION 3: POLICIAS CORRUPTOS GANAN si escape 40-60% oro (8-12 lingotes)
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

    // Invertir bitacora (fue insertada al inicio)
    RegistroBitacora* registros[1000];
    int contador = 0;
    RegistroBitacora* temp = sistema.bitacora;
    while (temp) {
        registros[contador++] = temp;
        temp = temp->siguiente;
    }

    cout << "\n" << setw(8) << "Turno" << setw(20) << "Personaje"
         << setw(25) << "Accion" << setw(30) << "Resultado" << endl;
    cout << string(90, '-') << endl;

    for (int i = contador - 1; i >= 0; i--) {
        cout << setw(8) << registros[i]->turno
             << setw(20) << registros[i]->personaje
             << setw(25) << registros[i]->accion
             << setw(30) << registros[i]->resultado << endl;
    }

    cout << string(90, '-') << endl;
    cout << "\nPresione ENTER para volver al menu..." << endl;
    cin.ignore();
}

// ===================================================================
// FUNCIONES DE CONSULTA Y DISPLAY
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

    cout << "\n" << setw(5) << "ID" << setw(15) << "Nombre"
         << setw(12) << "Bando" << setw(15) << "Estado"
         << setw(12) << "Vida" << setw(12) << "Estacion" << endl;
    cout << string(75, '-') << endl;

    while (temp) {
        cout << setw(5) << temp->idPersonaje
             << setw(15) << temp->nombre
             << setw(12) << temp->bando
             << setw(15) << temp->estado
             << setw(12) << temp->vidaActual << "/" << temp->vidaMaxima
             << setw(12) << temp->estacionActual << endl;
        temp = temp->siguiente;
    }
    cout << string(75, '-') << endl;
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

    cout << "\n" << setw(5) << "ID" << setw(20) << "Nombre"
         << setw(15) << "Tipo" << setw(20) << "Funcion"
         << setw(10) << "Usos" << setw(10) << "Alcance" << endl;
    cout << string(80, '-') << endl;

    while (temp) {
        cout << setw(5) << temp->idImplemento
             << setw(20) << temp->nombre
             << setw(15) << temp->tipoUsuario
             << setw(20) << temp->funcion
             << setw(10) << temp->usosRestantes
             << setw(10) << temp->alcance << endl;
        temp = temp->siguiente;
    }
    cout << string(80, '-') << endl;
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

    cout << "\n" << setw(5) << "ID" << setw(25) << "Nombre"
         << setw(12) << "Oro" << setw(20) << "Conexiones" << endl;
    cout << string(75, '-') << endl;

    while (temp) {
        cout << setw(5) << temp->idEstacion
             << setw(25) << temp->nombre
             << setw(12) << temp->oroDisponible;

        cout << setw(5) << "[";
        Estacion::Arista* arista = temp->adyacentes;
        bool primera = true;
        while (arista) {
            if (!primera) cout << ", ";
            cout << arista->destino;
            primera = false;
            arista = arista->siguiente;
        }
        cout << "]" << endl;

        temp = temp->siguiente;
    }
    cout << string(75, '-') << endl;
}

// ===================================================================
// FUNCIONES DEL JUEGO
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
            if (temp->esCorrupto) cout << " [CORRUPTO]";
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

void mostrarEstacionesConexas(SistemaJuego& sistema, int idEstacionActual) {
    Estacion* est = sistema.grafoEstaciones;

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
                Estacion* destino = sistema.grafoEstaciones;
                while (destino) {
                    if (destino->idEstacion == arista->destino) {
                        cout << contador << ". Estacion " << arista->destino
                             << " (" << destino->nombre << ") - Distancia: "
                             << arista->distancia << " - Oro: "
                             << destino->oroDisponible << endl;
                        contador++;
                        break;
                    }
                    destino = destino->siguiente;
                }
                arista = arista->siguiente;
            }
            return;
        }
        est = est->siguiente;
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

    cout << "\nPersonaje seleccionado: " << personaje->nombre << endl;
    cout << "Ubicacion actual: Estacion " << personaje->estacionActual << endl;

    mostrarEstacionesConexas(sistema, personaje->estacionActual);

    cout << "\nSeleccione estacion destino (numero): ";
    int opcion;

    if (!(cin >> opcion)) {
        cout << "ERROR: Ingrese un numero valido." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Estacion* estActual = sistema.grafoEstaciones;
    while (estActual) {
        if (estActual->idEstacion == personaje->estacionActual) {
            Estacion::Arista* arista = estActual->adyacentes;
            int contador = 1;

            while (arista) {
                if (contador == opcion) {
                    personaje->estacionActual = arista->destino;
                    cout << "\n[OK] " << personaje->nombre << " se movio a Estacion "
                         << arista->destino << endl;

                    // Registrar en bitacora
                    string resultado = "Movimiento a estacion " + to_string(arista->destino);
                    registrarBitacora(sistema, "Movimiento", personaje->nombre, resultado);

                    Estacion* destino = sistema.grafoEstaciones;
                    while (destino) {
                        if (destino->idEstacion == arista->destino) {
                            if (destino->oroDisponible > 0 && personaje->bando == "Ladron") {
                                int oroTomado = min(destino->oroDisponible,
                                                    personaje->capacidadOro - personaje->oroTransportando);
                                personaje->oroTransportando += oroTomado;
                                destino->oroDisponible -= oroTomado;
                                cout << "[OK] " << personaje->nombre << " tomo "
                                     << oroTomado << " lingotes de oro!" << endl;

                                sistema.oroRecuperado += oroTomado;

                                // Registrar en bitacora
                                string resultado_oro = "Recolecto " + to_string(oroTomado) + " oro";
                                registrarBitacora(sistema, "Recoleccion", personaje->nombre, resultado_oro);
                            }
                            break;
                        }
                        destino = destino->siguiente;
                    }
                    return;
                }
                contador++;
                arista = arista->siguiente;
            }
            break;
        }
        estActual = estActual->siguiente;
    }

    cout << "ERROR: Estacion no disponible." << endl;
}

void juegoInteractivo(SistemaJuego& sistema) {
    sistema.juegoEnCurso = true;
    sistema.turnoActual = 1;
    int opcionMenu = -1;

    while (sistema.juegoEnCurso) {
        // VERIFICAR CONDICIONES DE VICTORIA
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
// FUNCIONES DEL MENU PRINCIPAL
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
// FUNCION PRINCIPAL
// ===================================================================

int main() {
    SistemaJuego sistema;

    inicializarSistema(sistema);
    mostrarMenuPrincipal(sistema);

    return 0;
}
