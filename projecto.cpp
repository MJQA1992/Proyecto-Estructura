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
        if (!isspace(static_cast<unsigned char>(c))) //Cambio adrede de tipo ya que si es un char con signo puede causar problemas
            return false;
    return true;
}

//Funcion para revisar si es constituida de numeros
bool validacionNumeros(const string& str) {
    if (str.empty()) return false;
    for (char c : str) //Recorre caracter a caracter en la cadena para verificar si esta constituido de numeros
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    return true;
}

// Función para buscar un personaje por ID y verificar si ya existe
bool buscarPersonajePorID(Personaje* cabeza, int id) {
    Personaje* temp = cabeza;
    while (temp != nullptr) {
        if (temp->identificador == id) {
            return true; // ID encontrado, no es único
        }
        temp = temp->siguiente;
    }
    return false; // ID no encontrado, es único
}

//Agregar personajes por parametro direccion del tope de la lista dada
void agregarPersonaje (Personaje*& cabeza ) {

    Personaje *nuevoPtr = new Personaje();
    nuevoPtr->siguiente = nullptr;

    // Validación ID 
    do {
        cout << "Introduzca el ID del personaje: " << endl;
        // Reiniciar el ID a un valor no válido para el control de errores inicial
        nuevoPtr->identificador = -1; 
        
        if (!(cin >> nuevoPtr->identificador)) { // 1. Validar que sea INT
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "ERROR. Entrada de dato no valida (debe ser un numero entero)." << endl;
        } 
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            // 2. Validar Unicidad del ID
            if (buscarPersonajePorID(cabeza, nuevoPtr->identificador)) {
                cout << "ERROR. El ID " << nuevoPtr->identificador << " ya existe. Introduzca uno diferente." << endl;
            } 
            else if (nuevoPtr->identificador <= 0) {
                 cout << "ERROR. El ID debe ser un numero positivo." << endl;
            }
            else {
                // Validación Exitosa
                cout << "El ID introducido es: " << nuevoPtr->identificador << endl;
                break;
            }
        }
    } while( true);

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
        if (!(cin >> nuevoPtr->vida)) {

            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
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

    // Enlace con la lista
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
        return -1; // Retorna un valor no válido si la lista está vacía
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
        else if (seleccion < 1 || seleccion >= id) { // Nota: se usa >= id porque id es la cuenta total + 1

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

    if (seleccion == -1) return; // Evita error si la lista estaba vacía

    Personaje* actualPtr = cabeza;
    Personaje* previoPtr = nullptr;
    Personaje* tempPtr = nullptr ;
    int id = 1;

    // Se busca el nodo
    while (actualPtr != nullptr && id < seleccion) {

        previoPtr = actualPtr;
        actualPtr = actualPtr->siguiente;
        id++;

    }

    if (actualPtr == nullptr) {
        cout << "Error. No se pudo encontrar el personaje." << endl;
        return;
    }

    cout << "El personaje a eliminar es: " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

    if (previoPtr == nullptr) { // Es el primer nodo

        cabeza = actualPtr->siguiente;
        }
    else{ // No es el primer nodo

        previoPtr->siguiente = actualPtr->siguiente;
        };

    cout << "Eliminando al personaje:  " << actualPtr->nombre << " (ID: " << actualPtr->identificador << ")." << endl;

      delete actualPtr; // Eliminación correcta
    }

    //FUNCION PARA MODIFICAR PERSONAJE EN SUS DISTINTOS CAMPOS.
void personajeModificar(Personaje*& cabeza, int seleccion) {

        if (seleccion == -1) return; // Evita error si la lista estaba vacía

        Personaje* actualPtr = cabeza;
        int id = 1;
        int campo;

    // Buscar el nodo: se detiene justo en el nodo a seleccionar
    while (actualPtr != nullptr && id < seleccion) {
        actualPtr = actualPtr->siguiente;
        id++;
    }
    
    if (actualPtr == nullptr) {
        cout << "Error. No se pudo encontrar el personaje para modificar." << endl;
        return;
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
          // NOTA: Para cumplir el requerimiento del proyecto, esta sección
          // debe ser modificada para incluir la validación de unicidad de ID 
          // y la validación de tipo de dato (similar a como se hizo en agregarPersonaje).
          cout << "Indique el nuevo ID: " << endl;
          cin >> actualPtr->identificador;
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          break;
      case 2:
          cout << "Indique la nueva clase: " << endl;
          getline(cin, actualPtr->clase);
          // NOTA: Se deben agregar las validaciones de vacio/espacios/numeros
          break;
      case 3:
          cout << "Indique la nueva faccion: " << endl;
          getline(cin, actualPtr->faccion);
           // NOTA: Se deben agregar las validaciones de vacio/espacios/numeros
          break;
      case 4:
          cout << "Indique el nuevo nombre: " << endl;
          getline(cin, actualPtr->nombre);
           // NOTA: Se deben agregar las validaciones de vacio/espacios/numeros
          break;
      case 5:
          cout << "Indique el nuevo estado: " << endl;
          getline(cin, actualPtr->estado);
           // NOTA: Se deben agregar las validaciones de vacio/espacios/numeros
          break;
      case 6:
          cout << "Indique los nuevos puntos de vida:  " << endl;
          cin >> actualPtr->vida;
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
           // NOTA: Se deben agregar las validaciones de tipo de dato y no negativos
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
        if (!(cin >> campo)) {
            cout << "ERROR. Ingrese un numero." << endl;
            cin.clear(); // limpiando bufer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

          switch (campo) {
            case 1:
                agregarPersonaje(lista);
                break;
            case 2:
                // Se debe verificar si la lista no está vacía antes de llamar a seleccion
                if (lista != nullptr) {
                    personajeModificar(lista,seleccion(lista));
                } else {
                    cout << "No hay personajes para modificar." << endl;
                }
                break;
            case 3:
                 if (lista != nullptr) {
                    personajeEliminar(lista,seleccion(lista));
                } else {
                    cout << "No hay personajes para eliminar." << endl;
                }
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

        return 0;
        }
