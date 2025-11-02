#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// Estructura PERSONAJES
struct Personaje {
    int identificador ;
    string clase;
    string faccion;
    string nombre;
    string estado;
    int vida;
    Personaje *siguiente;
};

// Estructura IMPLEMENTOS
struct Implemento {
    int idImplemento;
    string nombre;
    int usosRestantes; 
    Implemento *siguiente;
};

// Estructura ESTACIONES
struct Estacion {
    int idEstacion;
    string nombre;
    int valijasOro; 
    Estacion *siguiente;
};

//Funcion para revisar si es constituida solo por espacios en blancos
bool validacionEspacios(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isspace(static_cast<unsigned char>(c)))
            return false;
    return true;
}

//Funcion para revisar si es constituida solo de numeros
bool validacionNumeros(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    return true;
}

// Función auxiliar para forzar un break en las validaciones de strings
bool validarCampoString(const string& valor, const string& nombre_campo) {
    if (valor.empty()) {
        cout << "ERROR: El campo " << nombre_campo << " no puede estar vacio.\n";
    } else if (validacionEspacios(valor)) {
        cout << "ERROR. El campo " << nombre_campo << " no puede contener solo espacios.\n";
    } else if (validacionNumeros(valor)) {
        cout << "ERROR. El campo " << nombre_campo << " no puede ser solo de numeros.\n";
    } else {
        return true; 
    }
    return false; 
}

// Personajes
bool buscarPersonajePorID(Personaje* cabeza, int id) {
    Personaje* temp = cabeza;
    while (temp != nullptr) {
        if (temp->identificador == id) {
            return true;
        }
        temp = temp->siguiente;
    }
    return false;
}

// Implementos
bool buscarImplementoPorID(Implemento* cabeza, int id) {
    Implemento* temp = cabeza;
    while (temp != nullptr) {
        if (temp->idImplemento == id) {
            return true;
        }
        temp = temp->siguiente;
    }
    return false;
}

// Estaciones
bool buscarEstacionPorID(Estacion* cabeza, int id) {
    Estacion* temp = cabeza;
    while (temp != nullptr) {
        if (temp->idEstacion == id) {
            return true;
        }
        temp = temp->siguiente;
    }
    return false;
}

// Agregar personajes
void agregarPersonaje (Personaje*& cabeza ) {

    Personaje *nuevoPtr = new Personaje();
    nuevoPtr->siguiente = nullptr;

    // Validación ID 
    do {
        cout << "\nIntroduzca el ID del personaje: " << endl;
        nuevoPtr->identificador = -1;
        
        if (!(cin >> nuevoPtr->identificador)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. El ID debe ser un numero entero." << endl;
        } 
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (buscarPersonajePorID(cabeza, nuevoPtr->identificador)) {
                cout << "ERROR. El ID " << nuevoPtr->identificador << " ya existe." << endl;
            } 
            else if (nuevoPtr->identificador <= 0) {
                 cout << "ERROR. El ID debe ser un numero positivo." << endl;
            }
            else {
                cout << "El ID introducido es: " << nuevoPtr->identificador << endl;
                break;
            }
        }
    } while( true);

    // Validacion CLASE
    do {
        cout << "Introduzca la CLASE del personaje: " << endl;
        getline(cin, nuevoPtr->clase);
        if (validarCampoString(nuevoPtr->clase, "CLASE")) {
            cout << "La CLASE introducida es: " << nuevoPtr->clase << endl;
            break;
        }
    } while (true);

    // Validacion FACCION
    do {
        cout << "Introduzca la FACCION del personaje: " << endl;
        getline(cin, nuevoPtr->faccion);
        if (validarCampoString(nuevoPtr->faccion, "FACCION")) {
            cout << "La FACCION introducida es: " << nuevoPtr->faccion << endl;
            break;
        }
    } while (true);

    // Validacion NOMBRE
    do {
        cout << "Introduzca el NOMBRE del personaje: " << endl;
        getline(cin, nuevoPtr->nombre);
        if (validarCampoString(nuevoPtr->nombre, "NOMBRE")) {
            cout << "El NOMBRE introducido es: " << nuevoPtr->nombre << endl;
            break;
        }
    } while (true);

    // VALIDACIÓN ESTADO
    do {
        cout << "Introduzca el ESTADO del personaje: " << endl;
        getline(cin, nuevoPtr->estado);
        if (validarCampoString(nuevoPtr->estado, "ESTADO")) {
            cout << "El ESTADO introducido es: " << nuevoPtr->estado << endl;
            break;
        }
    } while (true);

    //Valdiacion VIDA
    do {
        cout << "Introduzca los PUNTOS DE VIDA del personaje: " << endl;
        if (!(cin >> nuevoPtr->vida)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout <<"ERROR. Entrada de PUNTOS DE VIDA no valido." << endl;
        }
        else if (nuevoPtr->vida < 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. Los PUNTOS DE VIDA no pueden ser negativos." << endl;
        }
        else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Los PUNTOS DE VIDA introducidos son: " << nuevoPtr->vida << endl;
            break;
        }
    }while( true);

    nuevoPtr->siguiente = cabeza;
    cabeza = nuevoPtr;
};

// Mostrar lista
void mostrarLista(Personaje*& cabeza){

    Personaje* temp = cabeza;
    if (temp == nullptr){
        cout << "Lista Vacia." << endl;
    }
    else {
        cout << "\n//******LISTA DE PERSONAJES*******//" << endl;
        while (temp != nullptr ){
            cout << "ID: " << temp->identificador << "\nCLASE: " << temp->clase <<  "\nFACCION: "<<  temp->faccion << "\nNOMBRE: " << temp->nombre << "\nESTADO: " << temp->estado << "\nPUNTOS DE VIDA: " << temp->vida << "\n//*************************************//\n" << endl;
            temp = temp->siguiente;
        };
        cout << "Lista mostrada completamente." << endl;
    }
};

// Borrar toda la lista
void borrarLista(Personaje*& cabeza) {
    Personaje* actual = cabeza;
    while (actual != nullptr) {
        Personaje* temp = actual;
        actual = actual->siguiente;
        cout << "Borrando al personaje: " << temp->nombre << " (ID: " << temp->identificador << ")." << endl ;
        delete temp;
    }
    cout << "Lista Vacia." << endl;
    cabeza = nullptr;
}

// Selección genérica de personajes
int seleccion(Personaje*& cabeza) {
    Personaje* temp = cabeza;
    if (temp == nullptr){
        cout << "No hay personajes disponibles." << endl;
        return -1; 
    }

    int id_contador = 1;
    int seleccion;

    cout << "\n//******SELECCION DE PERSONAJE*******//" << endl;
    while (temp != nullptr) {
        cout << id_contador << ". " << temp->nombre << " (ID: " << temp->identificador << ")\n";
        temp = temp->siguiente;
        id_contador++;
    }

    do{
          cout << "Seleccione un personaje :\n";
          if (cin.fail() || !(cin >> seleccion)) {
              cin.clear(); 
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "ERROR. Debe ingresar un numero valido.\n";
          }
          else if (seleccion < 1 || seleccion >= id_contador) {
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "ERROR. Seleccion fuera de rango.\n";
          }
          else {
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              break;
            }
        } while (true);

      return seleccion;
}

// Borrar personaje individual
void personajeEliminar(Personaje*& cabeza, int seleccion) {
    if (seleccion == -1) return; 

    Personaje* actualPtr = cabeza;
    Personaje* previoPtr = nullptr;
    int id_contador = 1;

    while (actualPtr != nullptr && id_contador < seleccion) {
        previoPtr = actualPtr;
        actualPtr = actualPtr->siguiente;
        id_contador++;
    }

    if (actualPtr == nullptr) {
        cout << "Error. No se pudo encontrar el personaje." << endl;
        return;
    }

    if (previoPtr == nullptr) { 
        cabeza = actualPtr->siguiente;
        }
    else{ 
        previoPtr->siguiente = actualPtr->siguiente;
        };

    cout << "Eliminando al personaje:  " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;
    delete actualPtr;
}

// Modificar personaje
void personajeModificar(Personaje*& cabeza, int seleccion) {

    if (seleccion == -1) { cout << "Operacion cancelada." << endl; return; }

    Personaje* actualPtr = cabeza;
    int id_contador = 1;

    // Buscar el nodo
    while (actualPtr != nullptr && id_contador < seleccion) {
        actualPtr = actualPtr->siguiente;
        id_contador++;
    }

    if (actualPtr == nullptr) { cout << "Error. No se pudo encontrar el personaje para modificar." << endl; return; }

    int campo;
    
    do {
        cout << "\n//******MODIFICAR PERSONAJE*******//" << endl;
        cout << "Personaje a modificar: " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;
        cout <<"Campos a modificar: " << endl;
        cout << "1. ID del personaje" << endl;
        cout << "2. Clase del personaje" << endl;
        cout << "3. Faccion del personaje " << endl;
        cout << "4. Nombre del personaje" << endl;
        cout << "5. Estado del personaje "<< endl;
        cout << "6. Puntos de vida del personaje "<< endl;
        cout << "7. CANCELAR modificacion." << endl;
        cout << "Seleccione el campo (1-7): ";

        if (!(cin >> campo)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. Debe ingresar un numero valido.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break; 
    } while (true);
    
    string nuevo_valor_str;
    int nuevo_valor_int;

    switch (campo) {
        case 1: { // Modificar ID
            do {
                cout << "Indique el nuevo ID: " << endl;
                if (!(cin >> nuevo_valor_int)) { 
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout <<"ERROR. El ID debe ser un numero entero." << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (nuevo_valor_int <= 0) {
                        cout << "ERROR. El ID debe ser un numero positivo." << endl;
                    } 
                    else if (nuevo_valor_int != actualPtr->identificador && 
                             buscarPersonajePorID(cabeza, nuevo_valor_int)) {
                        cout << "ERROR. El ID " << nuevo_valor_int << " ya existe. Introduzca uno diferente." << endl;
                    } 
                    else {
                        actualPtr->identificador = nuevo_valor_int;
                        cout << "ID modificado exitosamente a: " << actualPtr->identificador << endl;
                        break;
                    }
                }
            } while (true);
            break;
        }

        case 2: case 3: case 4: case 5: { // Modificar String 
            string campo_nombre = (campo == 2) ? "CLASE" : (campo == 3) ? "FACCION" : (campo == 4) ? "NOMBRE" : "ESTADO";
            string* campo_ptr = (campo == 2) ? &actualPtr->clase : (campo == 3) ? &actualPtr->faccion : (campo == 4) ? &actualPtr->nombre : &actualPtr->estado;
            
            do {
                cout << "Indique la nueva " << campo_nombre << ": " << endl;
                getline(cin, nuevo_valor_str);

                if (validarCampoString(nuevo_valor_str, campo_nombre)) {
                    *campo_ptr = nuevo_valor_str;
                    cout << "La " << campo_nombre << " modificada es: " << *campo_ptr << endl;
                    break;
                }
            } while (true);
            break;
        }

        case 6: { // Modificar Puntos de VIDA
            do {
                cout << "Indique los nuevos puntos de vida: " << endl;
                if (!(cin >> nuevo_valor_int)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout <<"ERROR. Entrada de PUNTOS DE VIDA no valida (debe ser un numero entero)." << endl;
                }
                else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (nuevo_valor_int < 0) {
                        cout << "ERROR. Los PUNTOS DE VIDA no pueden ser negativos." << endl;
                    } else {
                        actualPtr->vida = nuevo_valor_int;
                        cout << "Los PUNTOS DE VIDA modificados son: " << actualPtr->vida << endl;
                        break;
                    }
                }
            } while (true);
            break;
        }
        case 7:
            cout << "Modificacion cancelada." << endl;
            break;
        default:
            cout << "Opción inválida. No se realizó ninguna modificación." << endl;
            break;
    }
}

void agregarImplemento(Implemento*& cabeza) {
    Implemento *nuevoPtr = new Implemento();
    nuevoPtr->siguiente = nullptr;
    do {
        cout << "\nIntroduzca el ID del Implemento: " << endl;
        nuevoPtr->idImplemento = -1;
        if (!(cin >> nuevoPtr->idImplemento)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. El ID debe ser un numero entero." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (buscarImplementoPorID(cabeza, nuevoPtr->idImplemento)) {
                cout << "ERROR. El ID " << nuevoPtr->idImplemento << " ya existe." << endl;
            } else if (nuevoPtr->idImplemento <= 0) {
                 cout << "ERROR. El ID debe ser un numero positivo." << endl;
            } else { cout << "El ID introducido es: " << nuevoPtr->idImplemento << endl; break; }
        }
    } while( true);

    do {
        cout << "Introduzca el NOMBRE del Implemento: " << endl;
        getline(cin, nuevoPtr->nombre);
        if (validarCampoString(nuevoPtr->nombre, "NOMBRE")) {
            cout << "El NOMBRE introducido es: " << nuevoPtr->nombre << endl;
            break;
        }
    } while (true);

    do {
        cout << "Introduzca los USOS RESTANTES/DURABILIDAD (entero): " << endl;
        if (!(cin >> nuevoPtr->usosRestantes)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout <<"ERROR. La durabilidad debe ser un numero entero." << endl;
        } else if (nuevoPtr->usosRestantes < 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. Los USOS no pueden ser negativos." << endl;
        } else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Los USOS RESTANTES introducidos son: " << nuevoPtr->usosRestantes << endl;
            break;
        }
    }while( true);

    nuevoPtr->siguiente = cabeza;
    cabeza = nuevoPtr;
};

void mostrarListaImplementos(Implemento*& cabeza){
    Implemento* temp = cabeza;
    if (temp == nullptr){
        cout << "Lista de Implementos Vacia." << endl;
    } else {
        cout << "\n//******LISTA DE IMPLEMENTOS*******//" << endl;
        while (temp != nullptr ){
            cout << "ID: " << temp->idImplemento << "\nNOMBRE: " << temp->nombre << "\nUSOS RESTANTES: " << temp->usosRestantes << "\n//*************************************//\n" << endl;
            temp = temp->siguiente;
        };
        cout << "Lista mostrada completamente." << endl;
    }
}

int seleccionImplemento(Implemento*& cabeza) {
    Implemento* temp = cabeza;
    if (temp == nullptr){ cout << "No hay implementos disponibles." << endl; return -1; }
    int id_contador = 1;
    int seleccion;
    cout << "\n//******SELECCION DE IMPLEMENTO*******//" << endl;
    while (temp != nullptr) {
        cout << id_contador << ". " << temp->nombre << " (ID: " << temp->idImplemento << ")\n";
        temp = temp->siguiente;
        id_contador++;
    }
    do{
          cout << "Seleccione un implemento:\n";
          if (cin.fail() || !(cin >> seleccion)) {
             cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
             cout << "ERROR. Debe ingresar un numero valido.\n";
          } else if (seleccion < 1 || seleccion >= id_contador) {
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "ERROR. Seleccion fuera de rango.\n";
          } else {
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              break;
            }
        } while (true);
      return seleccion;
}

void implementarModificar(Implemento*& cabeza, int seleccion) {

    if (seleccion == -1) return;
    Implemento* actualPtr = cabeza;
    int id_contador = 1;

    while (actualPtr != nullptr && id_contador < seleccion) { actualPtr = actualPtr->siguiente; id_contador++; }
    if (actualPtr == nullptr) { cout << "Error. No se pudo encontrar el implemento." << endl; return; }

    int campo;
    do {
        cout << "\n//******MODIFICAR IMPLEMENTO*******//" << endl;
        cout << "Implemento a modificar: " << actualPtr->nombre << " (ID: " << actualPtr->idImplemento << ")." << endl;
        cout <<"Campos a modificar: " << endl;
        cout << "1. ID del Implemento" << endl;
        cout << "2. Nombre del Implemento" << endl;
        cout << "3. Usos Restantes/Durabilidad "<< endl;
        cout << "4. CANCELAR modificacion." << endl;
        cout << "Seleccione el campo (1-4): ";

        if (!(cin >> campo)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "ERROR. Debe ingresar un numero valido.\n"; continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); break;
    } while (true);

    string nuevo_valor_str;
    int nuevo_valor_int;

    switch (campo) {
        case 1: { // Modificar ID
            do {
                cout << "Indique el nuevo ID: " << endl;
                if (!(cin >> nuevo_valor_int)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout <<"ERROR. El ID debe ser un numero entero." << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (nuevo_valor_int <= 0) { cout << "ERROR. El ID debe ser un numero positivo." << endl; } 
                    else if (nuevo_valor_int != actualPtr->idImplemento && buscarImplementoPorID(cabeza, nuevo_valor_int)) {
                        cout << "ERROR. El ID " << nuevo_valor_int << " ya existe. Introduzca uno diferente." << endl;
                    } else { actualPtr->idImplemento = nuevo_valor_int; cout << "ID modificado exitosamente a: " << actualPtr->idImplemento << endl; break; }
                }
            } while (true); break;
        }
        case 2: { // Modificar NOMBRE
            do {
                cout << "Indique el nuevo nombre: " << endl;
                getline(cin, nuevo_valor_str);
                if (validarCampoString(nuevo_valor_str, "NOMBRE")) {
                    actualPtr->nombre = nuevo_valor_str; cout << "El NOMBRE modificado es: " << actualPtr->nombre << endl; break;
                }
            } while (true); break;
        }
        case 3: { // Modificar USOS RESTANTES
            do {
                cout << "Indique los nuevos usos restantes: " << endl;
                if (!(cin >> nuevo_valor_int)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout <<"ERROR. Debe ser un numero entero." << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (nuevo_valor_int < 0) { cout << "ERROR. Los usos no pueden ser negativos." << endl; } 
                    else { actualPtr->usosRestantes = nuevo_valor_int; cout << "Los USOS RESTANTES modificados son: " << actualPtr->usosRestantes << endl; break; }
                }
            } while (true); break;
        }
        case 4: cout << "Modificacion cancelada." << endl; break;
        default: cout << "Opción inválida. No se realizó ninguna modificación." << endl; break;
    }
}

void implementoEliminar(Implemento*& cabeza, int seleccion) {
    if (seleccion == -1) return;
    Implemento* actualPtr = cabeza;
    Implemento* previoPtr = nullptr;
    int id_contador = 1;

    while (actualPtr != nullptr && id_contador < seleccion) { previoPtr = actualPtr; actualPtr = actualPtr->siguiente; id_contador++; }
    if (actualPtr == nullptr) { cout << "Error. No se pudo encontrar el implemento." << endl; return; }
    if (previoPtr == nullptr) { cabeza = actualPtr->siguiente; } else{ previoPtr->siguiente = actualPtr->siguiente; };
    cout << "Eliminando al implemento:  " << actualPtr->nombre << " (ID: " << actualPtr->idImplemento << ")." << endl;
    delete actualPtr;
}

void borrarListaImplementos(Implemento*& cabeza) {
    Implemento* actual = cabeza;
    while (actual != nullptr) {
        Implemento* temp = actual;
        actual = actual->siguiente;
        cout << "Borrando al implemento: " << temp->nombre << " (ID: " << temp->idImplemento << ")." << endl ;
        delete temp;
    }
    cout << "Lista de Implementos Vacia." << endl;
    cabeza = nullptr;
}

void agregarEstacion(Estacion*& cabeza) {
    Estacion *nuevoPtr = new Estacion();
    nuevoPtr->siguiente = nullptr;

    do {
        cout << "\nIntroduzca el ID de la Estacion: " << endl;
        nuevoPtr->idEstacion = -1;
        
        if (!(cin >> nuevoPtr->idEstacion)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. El ID debe ser un numero entero." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (buscarEstacionPorID(cabeza, nuevoPtr->idEstacion)) { 
                cout << "ERROR. El ID " << nuevoPtr->idEstacion << " ya existe. Introduzca uno diferente." << endl;
            } else if (nuevoPtr->idEstacion <= 0) {
                 cout << "ERROR. El ID debe ser un numero positivo." << endl;
            } else { cout << "El ID introducido es: " << nuevoPtr->idEstacion << endl; break; }
        }
    } while( true);

    do {
        cout << "Introduzca el NOMBRE de la Estacion: " << endl;
        getline(cin, nuevoPtr->nombre);
        if (validarCampoString(nuevoPtr->nombre, "NOMBRE")) {
            cout << "El NOMBRE introducido es: " << nuevoPtr->nombre << endl;
            break;
        }
    } while (true);

    do {
        cout << "Introduzca la cantidad de VALIJAS DE ORO (botin): " << endl;
        if (!(cin >> nuevoPtr->valijasOro)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout <<"ERROR. La cantidad debe ser un numero entero." << endl;
        } else if (nuevoPtr->valijasOro < 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. La cantidad de valijas no puede ser negativa." << endl;
        } else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Las VALIJAS DE ORO introducidas son: " << nuevoPtr->valijasOro << endl;
            break;
        }
    }while( true);

    nuevoPtr->siguiente = cabeza;
    cabeza = nuevoPtr;
};

void mostrarListaEstaciones(Estacion*& cabeza){
    Estacion* temp = cabeza;
    if (temp == nullptr){
        cout << "Lista de Estaciones Vacia." << endl;
    } else {
        cout << "\n//******LISTA DE ESTACIONES*******//" << endl;
        while (temp != nullptr ){
            cout << "ID: " << temp->idEstacion << "\nNOMBRE: " << temp->nombre << "\nVALIJAS DE ORO: " << temp->valijasOro << "\n//*************************************//\n" << endl;
            temp = temp->siguiente;
        };
        cout << "Lista mostrada completamente." << endl;
    }
}

int seleccionEstacion(Estacion*& cabeza) {
    Estacion* temp = cabeza;
    if (temp == nullptr){ cout << "No hay estaciones disponibles." << endl; return -1; }

    int id_contador = 1;
    int seleccion;

    cout << "\n//******SELECCION DE ESTACION*******//" << endl;
    while (temp != nullptr) {
        cout << id_contador << ". " << temp->nombre << " (ID: " << temp->idEstacion << ") - Botin: " << temp->valijasOro << "\n";
        temp = temp->siguiente;
        id_contador++;
    }

    do{
          cout << "Seleccione una estacion:\n";
          if (cin.fail() || !(cin >> seleccion)) {
             cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
             cout << "ERROR. Debe ingresar un numero valido.\n";
          } else if (seleccion < 1 || seleccion >= id_contador) {
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "ERROR. Seleccion fuera de rango.\n";
          } else {
              cin.ignore(numeric_limits<streamsize>::max(), '\n'); break;
            }
        } while (true);

      return seleccion;
}

void estacionModificar(Estacion*& cabeza, int seleccion) {

    if (seleccion == -1) return;
    Estacion* actualPtr = cabeza;
    int id_contador = 1;

    while (actualPtr != nullptr && id_contador < seleccion) { actualPtr = actualPtr->siguiente; id_contador++; }
    if (actualPtr == nullptr) { cout << "Error. No se pudo encontrar la estacion." << endl; return; }

    int campo;
    do {
        cout << "\n//******MODIFICAR ESTACION*******//" << endl;
        cout << "Estacion a modificar: " << actualPtr->nombre << " (ID: " << actualPtr->idEstacion << ")." << endl;
        cout <<"Campos a modificar: " << endl;
        cout << "1. ID de la Estacion" << endl;
        cout << "2. Nombre de la Estacion" << endl;
        cout << "3. Valijas de Oro (Botin) "<< endl;
        cout << "4. CANCELAR modificacion." << endl;
        cout << "Seleccione el campo (1-4): ";

        if (!(cin >> campo)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "ERROR. Debe ingresar un numero valido.\n"; continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); break;
    } while (true);

    string nuevo_valor_str;
    int nuevo_valor_int;

    switch (campo) {
        case 1: { // Modificar ID
            do {
                cout << "Indique el nuevo ID: " << endl;
                if (!(cin >> nuevo_valor_int)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout <<"ERROR. El ID debe ser un numero entero." << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (nuevo_valor_int <= 0) { cout << "ERROR. El ID debe ser un numero positivo." << endl; } 
                    else if (nuevo_valor_int != actualPtr->idEstacion && buscarEstacionPorID(cabeza, nuevo_valor_int)) {
                        cout << "ERROR. El ID " << nuevo_valor_int << " ya existe." << endl;
                    } else { actualPtr->idEstacion = nuevo_valor_int; cout << "ID modificado exitosamente." << endl; break; }
                }
            } while (true); break;
        }
        case 2: { // Modificar NOMBRE
            do {
                cout << "Indique el nuevo nombre: " << endl;
                getline(cin, nuevo_valor_str);
                if (validarCampoString(nuevo_valor_str, "NOMBRE")) {
                    actualPtr->nombre = nuevo_valor_str; cout << "NOMBRE modificado es: " << actualPtr->nombre << endl; break;
                }
            } while (true); break;
        }
        case 3: { // Modificar VALIJAS DE ORO
            do {
                cout << "Indique la nueva cantidad de valijas de oro: " << endl;
                if (!(cin >> nuevo_valor_int)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout <<"ERROR. Debe ser un numero entero." << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (nuevo_valor_int < 0) { cout << "ERROR. La cantidad no puede ser negativa." << endl; } 
                    else { actualPtr->valijasOro = nuevo_valor_int; cout << "VALIJAS DE ORO modificadas son: " << actualPtr->valijasOro << endl; break; }
                }
            } while (true); break;
        }
        case 4: cout << "Modificacion cancelada." << endl; break;
        default: cout << "Opción inválida." << endl; break;
    }
}

void estacionEliminar(Estacion*& cabeza, int seleccion) {
    if (seleccion == -1) return;
    Estacion* actualPtr = cabeza;
    Estacion* previoPtr = nullptr;
    int id_contador = 1;

    while (actualPtr != nullptr && id_contador < seleccion) { previoPtr = actualPtr; actualPtr = actualPtr->siguiente; id_contador++; }
    if (actualPtr == nullptr) { cout << "Error. No se pudo encontrar la estacion." << endl; return; }
    if (previoPtr == nullptr) { cabeza = actualPtr->siguiente; } else { previoPtr->siguiente = actualPtr->siguiente; }

    cout << "Eliminando a la estacion: " << actualPtr->nombre << " (ID: " << actualPtr->idEstacion << ")." << endl;
    delete actualPtr;
}

void borrarListaEstaciones(Estacion*& cabeza) {
    Estacion* actual = cabeza;

    while (actual != nullptr) {
        Estacion* temp = actual;
        actual = actual->siguiente;
        cout << "Borrando la estacion: " << temp->nombre << " (ID: " << temp->idEstacion << ")." << endl ;
        delete temp;
    }
    cout << "Lista de Estaciones Vacia." << endl;
    cabeza = nullptr;
}

void mostrarMenuPersonajes(Personaje*& lista) {
    int opcion = -1;
    do {
        cout << "\n//******GESTION DE PERSONAJES*******// " << endl;
        cout << "1. Crear personaje" << endl;
        cout << "2. Modificar personaje" << endl;
        cout << "3. Borrar personaje individual" << endl;
        cout << "4. Borrar todos los personajes" << endl;
        cout << "5. Mostrar lista de personajes" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        
        if (!(cin >> opcion)) {
            cout << "ERROR. Ingrese un numero." << endl;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: agregarPersonaje(lista); break;
            case 2:
                if (lista != nullptr) { personajeModificar(lista,seleccion(lista)); } else { cout << "No hay personajes para modificar." << endl; }
                break;
            case 3:
                if (lista != nullptr) { personajeEliminar(lista,seleccion(lista)); } else { cout << "No hay personajes para eliminar." << endl; }
                break;
            case 4: borrarLista(lista); break;
            case 5: mostrarLista(lista); break;
            case 6: cout << "Volviendo al Menu Principal." << endl; break;
            default: cout << "Opcion invalida." << endl; break;
        }
    } while (opcion != 6);
}

void mostrarMenuImplementos(Implemento*& lista) {
    int opcion = -1;
    do {
        cout << "\n//******GESTION DE IMPLEMENTOS*******// " << endl;
        cout << "1. Crear implemento" << endl;
        cout << "2. Modificar implemento" << endl;
        cout << "3. Borrar implemento individual" << endl;
        cout << "4. Borrar todos los implementos" << endl;
        cout << "5. Mostrar lista de implementos" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        
        if (!(cin >> opcion)) {
            cout << "ERROR. Ingrese un numero." << endl;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: agregarImplemento(lista); break;
            case 2:
                if (lista != nullptr) { implementarModificar(lista, seleccionImplemento(lista)); } else { cout << "No hay implementos para modificar." << endl; }
                break;
            case 3:
                if (lista != nullptr) { implementoEliminar(lista, seleccionImplemento(lista)); } else { cout << "No hay implementos para eliminar." << endl; }
                break;
            case 4: borrarListaImplementos(lista); break;
            case 5: mostrarListaImplementos(lista); break;
            case 6: cout << "Volviendo al Menu Principal." << endl; break;
            default: cout << "Opcion invalida." << endl; break;
        }
    } while (opcion != 6);
}

void mostrarMenuEstaciones(Estacion*& lista) {
    int opcion = -1;
    do {
        cout << "\n//******GESTION DE MAPA/ESTACIONES*******// " << endl;
        cout << "1. Crear nueva estacion" << endl;
        cout << "2. Modificar estacion" << endl;
        cout << "3. Borrar estacion individual" << endl;
        cout << "4. Borrar todas las estaciones" << endl;
        cout << "5. Mostrar lista de estaciones" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        
        if (!(cin >> opcion)) {
            cout << "ERROR. Ingrese un numero." << endl;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: agregarEstacion(lista); break;
            case 2:
                if (lista != nullptr) { estacionModificar(lista, seleccionEstacion(lista)); } else { cout << "No hay estaciones para modificar." << endl; }
                break;
            case 3:
                if (lista != nullptr) { estacionEliminar(lista, seleccionEstacion(lista)); } else { cout << "No hay estaciones para eliminar." << endl; }
                break;
            case 4: borrarListaEstaciones(lista); break;
            case 5: mostrarListaEstaciones(lista); break;
            case 6: cout << "Volviendo al Menu Principal." << endl; break;
            default: cout << "Opcion invalida." << endl; break;
        }
    } while (opcion != 6);
}


void mostrarMenu(Personaje*& listaPersonajes, Implemento*& listaImplementos, Estacion*& listaEstaciones){
    int campo = -1;
    do {

        cout << "\n//******MENU PRINCIPAL*******// " << endl;
        cout << "1. Gestion de Personajes" << endl;
        cout << "2. Gestion de Implementos/Accesorios" << endl;
        cout << "3. Gestion de Mapa/Estaciones" << endl;
        cout << "4. Salir "<< endl;

        if (!(cin >> campo)) {
            cout << "ERROR. Ingrese un numero." << endl;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

          switch (campo) {
            case 1: mostrarMenuPersonajes(listaPersonajes); break;
            case 2: mostrarMenuImplementos(listaImplementos); break;
            case 3: mostrarMenuEstaciones(listaEstaciones); break;
            case 4:
                  // Limpiar toda la memoria dinámica antes de salir
                  borrarLista(listaPersonajes);
                  borrarListaImplementos(listaImplementos);
                  borrarListaEstaciones(listaEstaciones);
                  cout << "Programa TERMINADO. Memoria liberada." << endl;
                  campo = 4;
                break;
            default: cout << "Opcion invalida." << endl; break;
            }
        } while( campo !=4 );
    }

int main() {
    // Inicializar los punteros cabeza de las tres listas
    Personaje* listaPersonajes = nullptr;
    Implemento* listaImplementos = nullptr;
    Estacion* listaEstaciones = nullptr; 

    // Iniciar el menú principal
    mostrarMenu(listaPersonajes, listaImplementos, listaEstaciones);

    return 0;
}
