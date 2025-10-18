#include <iostream>
#include <string>
#include <limits>

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

//Estructura LISTA
struct Lista {

    Personaje *inicialPtr;

    };

//Agregar personajes parametro direccion del tope de la lista dada
void agregarPersonaje (Personaje*& cabeza ) {

            Personaje *nuevoPtr = new Personaje();

            cout << "Introduzca el ID del personaje: " << endl;
            cin >> nuevoPtr->identificador;
            cin.clear(); // Limpieza buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Prevenir errores de input
            cout << "El ID introducido es: " << nuevoPtr->identificador << endl;

            cout << "Introduzca la CLASE del personaje: " << endl;
            getline(cin, nuevoPtr->clase); // Agarra toda la linea en vez de una sola cadena de caracteres
            cin.clear();
            cout << "La CLASE introducida es: " <<nuevoPtr->clase << endl;

            cout << "Introduzca la FACCION del personaje: " << endl;
            getline(cin, nuevoPtr->faccion);
            cin.clear();;
            cout << "El  FACCION introducida es: " << nuevoPtr->faccion << endl;

            cout << "Introduzca el NOMBRE del personaje: " << endl;
            getline(cin, nuevoPtr->nombre);
            cin.clear();
            cout << "El NOMBRE introducido es: " << nuevoPtr->nombre << endl;

            cout << "Introduzca el ESTADO del personaje: " << endl;
            getline(cin, nuevoPtr->estado);
            cin.clear();
            cout << "El ESTADO introducido es: " <<nuevoPtr->estado << endl;

            cout << "Introduzca los PUNTOS DE VIDA del personaje: " << endl;
            cin >> nuevoPtr->vida;
            cin.clear();
            cout << "Los PUNTOS DE VIDA  introducidos es: " << nuevoPtr->vida << endl;

            nuevoPtr->siguiente = cabeza; // Asignando nulo al final de la lista
            cabeza = nuevoPtr; // Asignando la nueva cabeza como el nuevo Personaje
    };

//FUNCION PARA MOSTRAR LISTA
void mostrarLista(Personaje* cabeza){

        if (cabeza == nullptr){

            cout << "Lista Vacia." << endl;
        }
        else {
            while (cabeza != nullptr ){

            cout << "ID: " << cabeza->identificador << " CLASE: " << cabeza->clase <<  " FACCION: "<<  cabeza->faccion << " NOMBRE: " << cabeza->nombre << " ESTADO: " << cabeza->estado << " PUNTOS DE VIDA: " << cabeza->vida << endl;
            cabeza = cabeza->siguiente;
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
    cabeza = nullptr;
}

//FUNCION GENERICA DE SELECCION DE PERSONAJES
int seleccion(Personaje* cabeza) {

    int id = 1;
    int seleccion;
    while (cabeza != nullptr) {
        cout << id << ". " << cabeza->nombre << " (ID: " << cabeza->identificador << ")\n";
        cabeza = cabeza->siguiente;
        id++;
    }
     cout << "Seleccione un personaje :\n";
     cin >> seleccion;

     return seleccion;
}

//FUNCION PARA BORRAR PERSONAJE POR FUNCION SELECCION
void personajeEliminar(Personaje* cabeza, int seleccion) {

    Personaje* actualPtr = cabeza;
    Personaje* previoPtr;
    Personaje* tempPtr ;
    int id = 1;

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
void personajeModificar(Personaje* cabeza, int seleccion) {

        Personaje* actualPtr = cabeza;
        int id = 1;
        int campo;

    while (actualPtr != nullptr && id < seleccion) {

        actualPtr = actualPtr->siguiente;
        id++;

        }
        // MENU DE MODIFICACIONES.
         cout << "El personaje a modificar es: " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

         cout <<"Campos a modificar: " << endl;
         cout << "1. ID del personaje" << endl;
        cout << "2. Clase del personaje" << endl;
        cout << "3. Bando del personaje " << endl;
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
            cout << "Indique el nuevo bando: " << endl;
            getline(cin, actualPtr->faccion);
            cin.clear();
            break;
        case 4:
            cout << "Indique el nuevo nombre: " << endl;
            getline(cin, actualPtr->nombre);
            cin.clear();
            break;
        case 5:
            cout << "Indique el nuevo estado: " << endl;
            getline(cin, actualPtr->estado);
            cin.clear();
            break;
        case 6:
            cout << "Indique los nuevos puntos de vida:  " << endl;
            cin >> actualPtr->vida;
            cin.clear();
            break;
        default:
            cout << "Opci¨®n inv¨¢lida." << endl;
            break;
        }
    }

void mostrarMenu(Personaje* lista){
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
                 cout << "Programa TERMINADO." << endl;
                 campo = 6;
                break;
            default:
                cout << "Opci¨®n inv¨¢lida." << endl;
                break;
            }
        } while( campo !=6 );
    }

int main() {

        Personaje* lista = nullptr;

        mostrarMenu(lista);

        }


