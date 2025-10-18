#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

//Estructura PERSONAJES

struct Personaje {

        int identificador ;
        string clase;
        string faccion;
        string nombre;
        string estado;
        int vida;
        Personaje *siguiente;
    };

//Funcion para revisar si es constituida por espacios en blancos
bool validacionEspacios(const string& str) {

    if (str.empty()) return false;

    for (char c : str) //Recorre caracter a caracter en la cadena para verifiicar si constituido de espacios

        if (!isspace(static_cast<unsigned char>(c)))

            return false;
    return true;
}

//Funcion para revisar si es constituida de numeros
bool validacionNumeros(const string& str) {

    if (str.empty()) return false;

    for (char c : str) //REcorre caracter a caracter en la cadena para verificar si esta constituido de numeros

        if (!isdigit(static_cast<unsigned char>(c)))

            return false;
    return true;
}

//Agregar personajes por parametro direccion del tope de la lista dada
void agregarPersonaje (Personaje*& cabeza ) {

            Personaje *nuevoPtr = new Personaje();
            nuevoPtr->siguiente = nullptr;

            // Validacion ID
            do {

                cout << "Introduzca el ID del personaje: " << endl;
                cin >> nuevoPtr->identificador;

                if (cin.fail()) { //cin.fail verifica si es del tipo int o  no

                    cin.clear(); // Limpieza buffer
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Prevenir errores de input
                    cout <<"ERROR. Entrada de dato no valida." << endl;
                }
                else{

                     cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Prevenir errores de input
                     cout << "El ID introducido es: " << nuevoPtr->identificador << endl;
                     break;
                }
            }while( true);

            // Validacion CLASE
            do {

                cout << "Introduzca la CLASE del personaje: " << endl;
                getline(cin, nuevoPtr->clase);

                if (nuevoPtr->clase.empty()) {

                    cout << "ERROR: La CLASE no puede estar vacia.\n";
                }
                else if (validacionEspacios(nuevoPtr->clase)) {

                    cout << "ERROR. La CLASE no puede contener solo espacios.\n";
                }
                else if (validacionNumeros(nuevoPtr->clase)) {

                    cout << "ERROR. La CLASE no puede ser solo de numeros.\n";
                }
                else {

                    cout << "La CLASE introducida es: " << nuevoPtr->clase << endl;
                    break;
                }
            } while (true);

        // Validacion FACCION
            do {

            cout << "Introduzca la FACCION del personaje: " << endl;
            getline(cin, nuevoPtr->faccion);

            if (nuevoPtr->faccion.empty()) {

                cout << "ERROR. La FACCION no puede estar vacia.\n";
            }
            else if (validacionEspacios(nuevoPtr->faccion)) {

                cout << "ERROR. La FACCION no puede contener solo espacios.\n";
            }
            else if (validacionNumeros(nuevoPtr->faccion)) {

                cout << "ERROR. La FACCION no puede ser solo de numeros.\n";
            }
            else {

                cout << "La FACCION introducida es: " << nuevoPtr->faccion << endl;
                break;
            }
        } while (true);

        // Validacion NOMBRE
        do {

            cout << "Introduzca el NOMBRE del personaje: " << endl;
            getline(cin, nuevoPtr->nombre);

            if (nuevoPtr->nombre.empty()) {

                cout << "ERROR. El NOMBRE no puede estar vacio.\n";
            }
            else if (validacionEspacios(nuevoPtr->nombre)) {

                cout << "ERROR. El NOMBRE no puede contener solo espacios.\n";
            }
            else if (validacionNumeros(nuevoPtr->nombre)) {

                cout << "ERROR. El NOMBRE no puede ser solo de numeros.\n";
            }
            else {

                cout << "El NOMBRE introducido es: " << nuevoPtr->nombre << endl;
                break;
            }
        } while (true);

        // VALIDACIÓN ESTADO
        do {

            cout << "Introduzca el ESTADO del personaje: " << endl;
            getline(cin, nuevoPtr->estado);

            if (nuevoPtr->estado.empty()) {

                cout << "ERROR. El ESTADO no puede estar vacio.\n";
            }
            else if (validacionEspacios(nuevoPtr->estado)) {

                cout << "ERROR. El ESTADO no puede contener solo espacios.\n";
            }
            else if (validacionNumeros(nuevoPtr->estado)) {

                cout << "ERROR. El ESTADO no puede ser solo de numeros.\n";
            }
            else {

                cout << "El ESTADO introducido es: " << nuevoPtr->estado << endl;
                break;
            }
        } while (true);

             //Valdiacion VIDA
             do {

                cout << "Introduzca los PUNTOS DE VIDA del personaje: " << endl;
                cin >> nuevoPtr->vida;

                if (cin.fail()) { //cin.fail verifica si es del tipo int o  no

                    cin.clear(); // Limpieza buffer
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Prevenir errores de input
                    cout <<"ERROR. Entrada de PUNTOS DE VIDA no valido." << endl;
                }
            else{

                     cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Prevenir errores de input
                     cout << "Los PUNTOS DE VIDA  introducidos son: " << nuevoPtr->vida << endl;
                     break;
                }
            }while( true);

            //Enlace con la lista
            nuevoPtr->siguiente = cabeza; // Asignando nulo al final de la lista
            cabeza = nuevoPtr; // Asignando la nueva cabeza como el nuevo Personaje
    };

//FUNCION PARA MOSTRAR LISTA
void mostrarLista(Personaje*& cabeza){

        Personaje* temp = cabeza;
        if (temp == nullptr){

            cout << "Lista Vacia." << endl;
        }
        else {
            while (temp != nullptr ){

            cout << "ID: " << temp->identificador << "\nCLASE: " << temp->clase <<  "\nFACCION: "<<  temp->faccion << "\nNOMBRE: " << temp->nombre << "\nESTADO: " << temp->estado << "\nPUNTOS DE VIDA: " << temp->vida << "\n//*************************************//\n" << endl;
            temp = temp->siguiente;
        };
            cout << "Lista mostrada completamente." << endl;
        }
        };

// FUNCION PARA BORRAR TODA LA LISTA.
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

//FUNCION GENERICA DE SELECCION DE PERSONAJES
int seleccion(Personaje*& cabeza) {

    Personaje* temp = cabeza;
    if (temp == nullptr){
        cout << "No hay personajes disponibles." << endl;
    }

    int id = 1;
    int seleccion;

    while (temp != nullptr) {

        cout << id << ". " << temp->nombre << " (ID: " << temp->identificador << ")\n";
        temp = temp->siguiente;
        id++;
    }

    //Validacion Seleccion
    do{

         cout << "Seleccione un personaje :\n";
         cin >> seleccion;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ERROR. Debe ingresar un numero valido.\n";
        }
        else if (seleccion < 1 || seleccion > id) {

            cout << "ERROR. Seleccion fuera de rango.\n";
        }

        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
            }
        } while (true);

     return seleccion;
}

//FUNCION PARA BORRAR PERSONAJE POR FUNCION SELECCION
void personajeEliminar(Personaje*& cabeza, int seleccion) {

    Personaje* actualPtr = cabeza;
    Personaje* previoPtr = nullptr;
    Personaje* tempPtr = nullptr ;
    int id = 1;

    if (actualPtr == nullptr) {
        cout << "La lista está vacía, no hay personajes para eliminar." << endl;
        return;
    }
    while (actualPtr != nullptr && id < seleccion) {

        previoPtr = actualPtr;
        actualPtr = actualPtr->siguiente;
        id++;

     }
    cout << "El personaje a eliminar es: " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

    if (previoPtr == nullptr) {

        cabeza = cabeza->siguiente;
        }
    else{

        previoPtr->siguiente = actualPtr->siguiente;
        };

    cout << "Eliminando al personaje:  " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

     tempPtr = actualPtr;
    delete tempPtr;

    }

    //FUNCION PARA MODIFICAR PERSONAJE EN SUS DISTINTOS CAMPOS.
void personajeModificar(Personaje*& cabeza, int seleccion) {

        Personaje* actualPtr = cabeza;
        int id = 1;
        int campo;

    while (actualPtr != nullptr && id <=  seleccion) {

        actualPtr = actualPtr->siguiente;
        id++;

        }
        // MENU DE MODIFICACIONES.
         cout << "El personaje a modificar es: " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

         cout <<"Campos a modificar: " << endl;
         cout << "1. ID del personaje" << endl;
        cout << "2. Clase del personaje" << endl;
        cout << "3. Faccion del personaje " << endl;
        cout << "4. Nombre del personaje" << endl;
        cout << "5. Estado del personaje "<< endl;
        cout << "6. Puntos de vida del personaje "<< endl;
        cin >> campo;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //CASE PARA MODIFICACIONES DE DISTINTOS CAMPOS
        switch (campo) {
        case 1:
            cout << "Indique el nuevo ID: " << endl;
            cin >> actualPtr->identificador;
            cin.clear();
            break;
        case 2:
            cout << "Indique la nueva clase: " << endl;
            getline(cin, actualPtr->clase);
            cin.clear();
            break;
        case 3:
            cout << "Indique la nueva faccion: " << endl;
            getline(cin, actualPtr->faccion);
            break;
        case 4:
            cout << "Indique el nuevo nombre: " << endl;
            getline(cin, actualPtr->nombre);
            break;
        case 5:
            cout << "Indique el nuevo estado: " << endl;
            getline(cin, actualPtr->estado);
            break;
        case 6:
            cout << "Indique los nuevos puntos de vida:  " << endl;
            cin >> actualPtr->vida;
            cin.clear();
            break;
        default:
            cout << "Opción inválida." << endl;
            break;
        }
    }

void mostrarMenu(Personaje*& lista){
    int campo = -1;
    do {

        cout <<"//******MENU*******// " << endl;
        cout << "1. Crear personaje" << endl;
        cout << "2. Modificar personaje" << endl;
        cout << "3. Borrar personaje " << endl;
        cout << "4. Borrar todos los personajes" << endl;
        cout << "5. Mostrar lista" << endl;
        cout << "6. Salir "<< endl;
        cin >> campo;
        cin.clear(); // limpiando bufer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

         switch (campo) {
            case 1:
               agregarPersonaje(lista);
                break;
            case 2:
               personajeModificar(lista,seleccion(lista));
                break;
            case 3:
                personajeEliminar(lista,seleccion(lista));
                break;
            case 4:
               borrarLista(lista);
                break;
             case 5:
                 mostrarLista(lista);
                break;
             case 6:
                  borrarLista(lista);
                 cout << "Programa TERMINADO." << endl;
                 campo = 6;
                break;
            default:
                cout << "Opcion invalida." << endl;
                break;
            }
        } while( campo !=6 );
    }

int main() {

        Personaje* lista = nullptr;

        mostrarMenu(lista);

        }


