#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int generarAleatorio(int mini, int maxi) {
    return mini + rand() % (maxi - mini + 1);
}

float generarDuracion() {
    return 0.01f + (float)rand() / RAND_MAX * 0.99f;
}

string generarFecha() {
    int dia = generarAleatorio(1, 28);
    int mes = generarAleatorio(1, 12);
    int ano = generarAleatorio(2020, 2026);
    string fecha = to_string(dia) + " de " + to_string(mes) + " del " + to_string(ano);
    return fecha;
}

struct Carta {
    int idCarta;
    string nombre;
    string rareza;
    string tipo;
    int costoElixir;
    int danoBase;
    int vidaBase;
    Carta* sig;

    Carta() : idCarta(0), costoElixir(0), danoBase(0), vidaBase(0), sig(nullptr) {}
};
Carta* listaCartas = nullptr;

void insercioninicio(int id, string nom, string rar, string tipo, int costE, int danB, int vidB) {

    Carta* aux = listaCartas;
    while (aux != nullptr) {
        if (aux->idCarta == id) {
            cout << "Error: este ID ya ha sido ingresado para la carta: " << aux->nombre << endl;
            return;
        }
        aux = aux->sig;
    };

    if (id < 0 || costE < 0 || danB < 0 || vidB < 0) {
        cout << "Error: los datos no pueden ser negativos :(" << endl;
        return;
    }

    if (nom == "" || rar == "" || tipo == "") {
        cout << "Error: los campos de texto no pueden estar vacios" << endl;
        return;
    }

    Carta* nueva = new Carta();
    nueva->idCarta     = id;
    nueva->nombre      = nom;
    nueva->rareza      = rar;
    nueva->tipo        = tipo;
    nueva->costoElixir = costE;
    nueva->danoBase    = danB;
    nueva->vidaBase    = vidB;
    nueva->sig         = listaCartas;
    listaCartas        = nueva;
    cout << "Carta guardada" << endl;
}

struct NodoCartaMazo {
    Carta* cartaReal;
    NodoCartaMazo* siguiente;

    NodoCartaMazo() : cartaReal(nullptr), siguiente(nullptr) {}

};

struct Mazo {
    int idMazo;
    string nombreMazo;
    string tipoMazo;
    int idJugador;
    NodoCartaMazo* listaCartasMazo = nullptr;
    Mazo* siguiente;
    Mazo() : idMazo(0), idJugador(0), listaCartasMazo(nullptr), siguiente(nullptr) {}
};
Mazo* listaGlobalMazos = nullptr;

int contarCartasEnMazo(Mazo* mazoActual) {
    int contador = 0;
    NodoCartaMazo* aux = mazoActual->listaCartasMazo;
    while (aux != nullptr) {
        contador++;
        aux = aux->siguiente;
    }
    return contador;
}

void insertarMazoFinal(int id, string nombre, string tipo, int idJ) {

    Mazo* check = listaGlobalMazos;
    while (check != nullptr) {
        if (check->idMazo == id) {
            cout << "ID repetido" << endl;
            return;
        }
        check = check->siguiente;
    }
    Mazo* nuevoMazo = new Mazo();
    nuevoMazo->idMazo          = id;
    nuevoMazo->nombreMazo      = nombre;
    nuevoMazo->tipoMazo        = tipo;
    nuevoMazo->idJugador       = idJ;
    nuevoMazo->listaCartasMazo = nullptr;
    nuevoMazo->siguiente       = nullptr;

    if (listaGlobalMazos == nullptr) {
        listaGlobalMazos = nuevoMazo;
    } else {

        Mazo* auxiliar = listaGlobalMazos;
        while (auxiliar->siguiente != nullptr) {
            auxiliar = auxiliar->siguiente;
        }
        auxiliar->siguiente = nuevoMazo;
    }
}

void agregarCartaASublista(Mazo* mazoDestino, int idCartaBuscar) {
    if (mazoDestino == nullptr) return;

    if (contarCartasEnMazo(mazoDestino) >= 8) {
        cout << "Mazo lleno" << endl;
        return;
    }

    NodoCartaMazo* auxBusqueda = mazoDestino->listaCartasMazo;
    while (auxBusqueda != nullptr) {
        if (auxBusqueda->cartaReal->idCarta == idCartaBuscar) {
            cout << "Carta repetida" << endl;
            return;
        }
        auxBusqueda = auxBusqueda->siguiente;
    }

    Carta* encontrada = nullptr;
    Carta* temp = listaCartas;
    while (temp != nullptr) {
        if (temp->idCarta == idCartaBuscar) {
            encontrada = temp;
            break;
        }
        temp = temp->sig;
    }
    if (encontrada == nullptr) {
        cout << "Carta no encontrada" << endl;
        return;
    }

    NodoCartaMazo* nuevoNodo = new NodoCartaMazo();
    nuevoNodo->cartaReal             = encontrada;
    nuevoNodo->siguiente             = mazoDestino->listaCartasMazo;
    mazoDestino->listaCartasMazo     = nuevoNodo;
}

int danoTotalMazo(Mazo* mazoActual) {
    int sumaDano = 0;
    NodoCartaMazo* aux = mazoActual->listaCartasMazo;
    while (aux != nullptr) {
        sumaDano += aux->cartaReal->danoBase;
        aux = aux->siguiente;
    }
    return sumaDano;
}

int vidaTotalMazo(Mazo* mazoActual) {
    int sumaVida = 0;
    NodoCartaMazo* aux = mazoActual->listaCartasMazo;
    while (aux != nullptr) {
        sumaVida += aux->cartaReal->vidaBase;
        aux = aux->siguiente;
    }
    return sumaVida;
}

Mazo* buscarMazoxJugador(int idJugador) {
    Mazo* temp = listaGlobalMazos;
    while (temp != nullptr) {
        if (temp->idJugador == idJugador) return temp;
        temp = temp->siguiente;
    }
    return nullptr;
}

Mazo* buscarMazoPorID(int idMazo) {
    Mazo* temp = listaGlobalMazos;
    while (temp != nullptr) {
        if (temp->idMazo == idMazo) return temp;
        temp = temp->siguiente;
    }
    return nullptr;
}

struct Arena {
    int IdArena;
    string nombreArena;
    int trofeosMin;
    int trofeosMax;
    Arena* sig;

    Arena() : IdArena(0), trofeosMin(0), trofeosMax(0), sig(nullptr) {}
};
Arena* listaArenas = nullptr;

void insertarArenaInicio(int id, string nom, int tMin, int tMax) {

    Arena* aux = listaArenas;
    while (aux) {
        if (aux->IdArena == id) { cout << "Arena repetida" << endl; return; }
        aux = aux->sig;
    }

    if (tMin < 0 || tMax < 0 || tMin >= tMax) {
        cout << "Rango de trofeos invalido (min debe ser menor que max y no negativos)." << endl;
        return;
    }
    Arena* nueva = new Arena();
    nueva->IdArena     = id;
    nueva->nombreArena = nom;
    nueva->trofeosMin  = tMin;
    nueva->trofeosMax  = tMax;
    nueva->sig         = listaArenas;
    listaArenas        = nueva;
    cout << "Arena creada!" << endl;
}

Arena* buscarArenaPorID(int id) {
    Arena* aux = listaArenas;
    while (aux != nullptr) {
        if (aux->IdArena == id) return aux;
        aux = aux->sig;
    }
    return nullptr;
}

void modificarArena() {
if (listaArenas == nullptr) { cout << "No hay arenas." << endl; return; }
   int id;
cout << "ID de la arena a modificar: "; cin >> id;
    Arena* aux = buscarArenaPorID(id);
    if (aux == nullptr) { cout << "Arena no encontrada." << endl; return; }
    cout << "1.Nombre  2.Trofeos Min  3.Trofeos Max\nOpcion: ";
    int op; cin >> op;
    if (op == 1) {
        cout << "Nuevo nombre: "; cin >> aux->nombreArena;
        cout << "Nombre actualizado." << endl;
} else if (op == 2) {
        int v; cout << "Nuevo min: "; cin >> v;
        if (v < 0 || v >= aux->trofeosMax) cout << "Valor invalido." << endl;
        else { aux->trofeosMin = v; cout << "Min actualizado." << endl; }
    } else if (op == 3) {
        int v; cout << "Nuevo max: "; cin >> v;
        if (v <= aux->trofeosMin) cout << "Valor invalido." << endl;
        else { aux->trofeosMax = v; cout << "Max actualizado." << endl; }
    } else {
        cout << "Opcion invalida." << endl;
    }
}

struct Jugadores;

struct Clanes {
    int idClan;
    string nombreClan;
    string region;
    int cantidadMiembros;
    int puntajeClan;
    Jugadores* subListaJugadores = nullptr;
    Clanes* sig;

    Clanes(int i, string n, string r, int c, int p) {
        idClan           = i;
        nombreClan       = n;
        region           = r;
        cantidadMiembros = c;
        puntajeClan      = p;
        subListaJugadores = nullptr;
        sig              = nullptr;
    }
};
Clanes* inicioClan = nullptr;

struct Jugadores {
    int idJugador;
    string nombreUsuario;
    int nivelRey;
    int trofeos;
    int idArena;
    int idClan;
    Jugadores* sig;
    Jugadores* ant;

    Jugadores(int j, string n, int r, int t, int a, int c) {
        idJugador    = j;
        nombreUsuario = n;
        nivelRey     = r;
        trofeos      = t;
        idArena      = a;
        idClan       = c;
        sig          = nullptr;
        ant          = nullptr;
    }
};
Jugadores* primerJugador = nullptr;

struct Batallas {
    int IDBatalla;
    int IDJugador1;
    int IDJugador2;
    int IDMazo1;
    int IDMazo2;
    int ganador;
    int coronasJ1;
    int coronasJ2;
    float duracion;
    int IDArena;
    string fecha;
    Batallas* sig;
    Batallas* ant;

    Batallas(int b, int j1, int j2, int m1, int m2, int g, int c1, int c2, float d, int a, string f) {
        IDBatalla  = b;
        IDJugador1 = j1;
        IDJugador2 = j2;
        IDMazo1    = m1;
        IDMazo2    = m2;
        ganador    = g;
        coronasJ1  = c1;
        coronasJ2  = c2;
        duracion   = d;
        IDArena    = a;
        fecha      = f;
        sig        = NULL;
        ant        = NULL;
    }
};
Batallas* primeraBatalla = nullptr;

Jugadores* buscarJugadorID(int IDJugador) {
    Jugadores* temp = primerJugador;
    while (temp) {
        if (temp->idJugador == IDJugador) return temp;
        temp = temp->sig;
    }
    return nullptr;
}

Jugadores* buscarJugadorPorNombre(string nombre) {
    Jugadores* temp = primerJugador;
    while (temp != nullptr) {
        if (temp->nombreUsuario == nombre) return temp;
        temp = temp->sig;
    }
    return nullptr;
}

void insertarJugadorOrdenadoPorNombre(int idJugador, string nombreUsuario, int nivelRey, int trofeos, int idArena, int idClan) {

    if (buscarJugadorID(idJugador) != nullptr) {
        cout << "Error: ya existe un jugador con el ID " << idJugador << "." << endl;
        return;
    }

    if (buscarJugadorPorNombre(nombreUsuario) != nullptr) {
        cout << "Error: ya existe un jugador con el nombre '" << nombreUsuario << "'." << endl;
        return;
    }

    Arena* auxA = listaArenas;
    bool arenaEncontrada = false;
    while (auxA != nullptr) {
        if (auxA->IdArena == idArena) {
            arenaEncontrada = true;
            if (trofeos < auxA->trofeosMin || trofeos > auxA->trofeosMax) {
                cout << "Error: los trofeos (" << trofeos << ") no corresponden al rango de la arena "
                     << auxA->nombreArena << " (" << auxA->trofeosMin << "-" << auxA->trofeosMax << ")." << endl;
                return;
            }
            break;
        }
auxA = auxA->sig;
    }
    if (!arenaEncontrada) {
        cout << "Error: no existe la arena con ID " << idArena << "." << endl;
        return;
    }

    if (nivelRey < 0 || trofeos < 0) {
        cout << "Error: nivel y trofeos no pueden ser negativos." << endl;
        return;
    }

    Jugadores* nuevo = new Jugadores(idJugador, nombreUsuario, nivelRey, trofeos, idArena, idClan);

    if (primerJugador == nullptr) {
        primerJugador = nuevo;
        cout << "Jugador '" << nombreUsuario << "' registrado." << endl;

        if (idClan != 0) {
            Clanes* clan = nullptr;
            if (inicioClan != nullptr) {
                Clanes* temp = inicioClan;
                do {
                    if (temp->idClan == idClan) { clan = temp; break; }
                    temp = temp->sig;
                } while (temp != inicioClan);
            }
            if (clan != nullptr) {

                Jugadores* copia = new Jugadores(idJugador, nombreUsuario, nivelRey, trofeos, idArena, idClan);
                if (clan->subListaJugadores == nullptr) {
                    clan->subListaJugadores = copia;
                } else {
                    Jugadores* t = clan->subListaJugadores;
                    while (t->sig != nullptr) t = t->sig;
                    t->sig = copia;
                    copia->ant = t;
                }
            }
        }
        return;
    }

    if (nombreUsuario < primerJugador->nombreUsuario) {
        nuevo->sig        = primerJugador;
        primerJugador->ant = nuevo;
        primerJugador     = nuevo;
        cout << "Jugador '" << nombreUsuario << "' registrado." << endl;

        if (idClan != 0) {
            Clanes* clan = nullptr;
            if (inicioClan != nullptr) {
                Clanes* temp = inicioClan;
                do {
                    if (temp->idClan == idClan) { clan = temp; break; }
                    temp = temp->sig;
                } while (temp != inicioClan);
            }
            if (clan != nullptr) {
                Jugadores* copia = new Jugadores(idJugador, nombreUsuario, nivelRey, trofeos, idArena, idClan);
                if (clan->subListaJugadores == nullptr) {
                    clan->subListaJugadores = copia;
                } else {
                    Jugadores* t = clan->subListaJugadores;
                    while (t->sig != nullptr) t = t->sig;
                    t->sig = copia;
                    copia->ant = t;
                }
            }
        }
        return;
    }

    Jugadores* temp = primerJugador;
    while (temp->sig != nullptr && temp->sig->nombreUsuario < nombreUsuario) {
        temp = temp->sig;
    }
    nuevo->sig = temp->sig;
    if (temp->sig != nullptr) temp->sig->ant = nuevo;
    nuevo->ant = temp;
    temp->sig  = nuevo;
    cout << "Jugador '" << nombreUsuario << "' registrado." << endl;

    if (idClan != 0) {
        Clanes* clan = nullptr;
        if (inicioClan != nullptr) {
            Clanes* tc = inicioClan;
            do {
                if (tc->idClan == idClan) { clan = tc; break; }
                tc = tc->sig;
            } while (tc != inicioClan);
        }
        if (clan != nullptr) {
            Jugadores* copia = new Jugadores(idJugador, nombreUsuario, nivelRey, trofeos, idArena, idClan);
            if (clan->subListaJugadores == nullptr) {
                clan->subListaJugadores = copia;
            } else {
                Jugadores* t = clan->subListaJugadores;
                while (t->sig != nullptr) t = t->sig;
                t->sig = copia;
                copia->ant = t;
            }
        }
    }
}

void modificarJugadorNombre(int IDJugador, string nombreAnterior, string nuevoNombre) {
    Jugadores* temp = buscarJugadorID(IDJugador);
    if (!temp) { cout << "Jugador no existente" << endl; return; }

    if (buscarJugadorPorNombre(nuevoNombre) != nullptr) {
        cout << "Error: el nombre '" << nuevoNombre << "' ya esta en uso." << endl;
        return;
    }
    if (temp->nombreUsuario == nombreAnterior) {
        temp->nombreUsuario = nuevoNombre;
        cout << "Nombre modificado" << endl;
    } else {
        cout << "El nombre anterior no coincide" << endl;
    }
}

void modificarJugadorNivelRey(int IDJugador, int nuevoNivel) {
    Jugadores* temp = buscarJugadorID(IDJugador);
    if (!temp) { cout << "Jugador no existente" << endl; return; }
    if (nuevoNivel < 0) { cout << "Nivel invalido." << endl; return; }
    temp->nivelRey = nuevoNivel;
    cout << "Nivel de rey actualizado" << endl;
}

void modificarJugadorTrofeos(int IDJugador, int nuevosTrofeos) {
    Jugadores* temp = buscarJugadorID(IDJugador);
    if (!temp) { cout << "Jugador no existente" << endl; return; }
    if (nuevosTrofeos < 0) { cout << "Trofeos invalidos." << endl; return; }
    temp->trofeos = nuevosTrofeos;
    cout << "Trofeos actualizados" << endl;
}

void modificarJugadorArena(int IDJugador, int nuevaArena) {
    Jugadores* temp = buscarJugadorID(IDJugador);
    if (!temp) { cout << "Jugador no existente" << endl; return; }
    temp->idArena = nuevaArena;
    cout << "Arena actualizada" << endl;
}

void modificarJugadorClan(int IDJugador, int nuevoClan) {
    Jugadores* temp = buscarJugadorID(IDJugador);
    if (!temp) { cout << "Jugador no existente" << endl; return; }
    temp->idClan = nuevoClan;
    cout << "Clan actualizado" << endl;
}

void eliminarJugador(int idJugador) {
    Jugadores* buscado = buscarJugadorID(idJugador);
    if (!buscado) { cout << "Jugador no encontrado" << endl; return; }

    if (buscado == primerJugador) {
        primerJugador = buscado->sig;
        if (primerJugador) primerJugador->ant = nullptr;
    } else {
        if (buscado->ant) buscado->ant->sig = buscado->sig;
        if (buscado->sig) buscado->sig->ant = buscado->ant;
    }

    if (buscado->idClan != 0 && inicioClan != nullptr) {
        Clanes* clan = nullptr;
        Clanes* tc = inicioClan;
        do {
            if (tc->idClan == buscado->idClan) { clan = tc; break; }
            tc = tc->sig;
        } while (tc != inicioClan);
        if (clan != nullptr && clan->subListaJugadores != nullptr) {
            Jugadores* sub = clan->subListaJugadores;
            Jugadores* antSub = nullptr;
            while (sub != nullptr) {
                if (sub->idJugador == idJugador) {
                    if (antSub == nullptr) clan->subListaJugadores = sub->sig;
                    else antSub->sig = sub->sig;
                    if (sub->sig != nullptr) sub->sig->ant = antSub;
                    delete sub;
                    break;
                }
                antSub = sub;
                sub = sub->sig;
            }
        }
    }
    delete buscado;
    cout << "Jugador eliminado" << endl;
}

void imprimirJugadores() {
    Jugadores* temp = primerJugador;
    if (!primerJugador) { cout << "No hay jugadores" << endl; return; }
    cout << "----- JUGADORES -----" << endl;

    while (temp->sig) temp = temp->sig;

    while (temp) {
        cout << "ID: " << temp->idJugador
             << ", Nombre: " << temp->nombreUsuario
             << ", Nivel: " << temp->nivelRey
             << ", Trofeos: " << temp->trofeos
             << ", Arena: " << temp->idArena
             << ", Clan: " << temp->idClan << endl;
        temp = temp->ant;
    }
}

void imprimirJugadores(Jugadores* lista) {
    if (!lista) { cout << "No hay jugadores en este clan" << endl; return; }
    Jugadores* temp = lista;
    while (temp != nullptr) {
        cout << "  ID: " << temp->idJugador
             << ", Nombre: " << temp->nombreUsuario
             << ", Trofeos: " << temp->trofeos << endl;
        temp = temp->sig;
    }
}

void insertarArena() {
    int id, min, max;
    string nom;
    cout << "ID: ";     cin >> id;
    cout << "Nombre: "; cin >> nom;
    cout << "Min: ";    cin >> min;
    cout << "Max: ";    cin >> max;
    insertarArenaInicio(id, nom, min, max);
}

void imprimirArenas() {
    if (listaArenas == nullptr) { cout << "No hay arenas registradas." << endl; return; }
    cout << "----- ARENAS -----" << endl;
    Arena* aux = listaArenas;
    while (aux) {
        cout << "ID: " << aux->IdArena
             << ", Nombre: " << aux->nombreArena
             << ", Min trofeos: " << aux->trofeosMin
             << ", Max trofeos: " << aux->trofeosMax << endl;
        aux = aux->sig;
    }
}

void eliminarArena() {
    if (listaArenas == nullptr) { cout << "No hay arenas." << endl; return; }
    int id;
    cout << "ID arena a eliminar: "; cin >> id;
    Arena* aux = listaArenas;
    Arena* ant = nullptr;
    while (aux != nullptr) {
        if (aux->IdArena == id) {
            if (ant == nullptr) listaArenas = aux->sig;
            else ant->sig = aux->sig;
            delete aux;
            cout << "Arena eliminada." << endl;
            return;
        }
        ant = aux;
        aux = aux->sig;
    }
    cout << "No se encontro la arena con ID " << id << "." << endl;
}

bool listaVaciaClan() { return inicioClan == nullptr; }

Clanes* obtenerUltimoClan() {
    if (listaVaciaClan()) return nullptr;
    Clanes* temp = inicioClan;
    while (temp->sig != inicioClan) temp = temp->sig;
    return temp;
}

Clanes* buscarClanPorId(int idClan) {
    if (listaVaciaClan()) return nullptr;
    Clanes* temp = inicioClan;
    do {
        if (temp->idClan == idClan) return temp;
        temp = temp->sig;
    } while (temp != inicioClan);
    return nullptr;
}

bool existeClan(int idClan) { return buscarClanPorId(idClan) != nullptr; }

void imprimirClan(Clanes* c) {
    if (c == nullptr) { cout << "El clan no existe" << endl; return; }
    cout << "ID del clan: "          << c->idClan << endl;
    cout << "Nombre del clan: "      << c->nombreClan << endl;
    cout << "Region: "               << c->region << endl;
    cout << "Cantidad de miembros: " << c->cantidadMiembros << endl;
    cout << "Puntaje: "              << c->puntajeClan << endl;
}

bool insertarClanAlFinal(int idClan, string nombreClan, string region, int cantidadMiembros, int puntajeClan) {
    if (existeClan(idClan)) {
        cout << "Error: ya existe un clan con el ID " << idClan << "." << endl;
        return false;
    }

    if (cantidadMiembros < 0 || puntajeClan < 0) {
        cout << "Error: miembros y puntaje no pueden ser negativos." << endl;
        return false;
    }
    Clanes* nuevo = new Clanes(idClan, nombreClan, region, cantidadMiembros, puntajeClan);
    if (listaVaciaClan()) {
        inicioClan  = nuevo;
        nuevo->sig  = inicioClan;
        return true;
    }
    Clanes* temp = inicioClan;
    while (temp->sig != inicioClan) temp = temp->sig;
    temp->sig  = nuevo;
    nuevo->sig = inicioClan;
    return true;
}

void imprimirClanes() {
    if (listaVaciaClan()) { cout << "No hay clanes" << endl; return; }
    Clanes* temp = inicioClan;
    cout << "----- CLANES -----" << endl;
    do {
        imprimirClan(temp);
        cout << "-----------------------------" << endl;
        temp = temp->sig;
    } while (temp != inicioClan);
}

bool actualizarClanId(int idClan, string nuevoNombreClan, string nuevaRegion, int nuevaCantidadMiembros, int nuevoPuntajeClan) {
    Clanes* encontrado = buscarClanPorId(idClan);
    if (encontrado == nullptr) return false;
    encontrado->nombreClan       = nuevoNombreClan;
    encontrado->region           = nuevaRegion;
    encontrado->cantidadMiembros = nuevaCantidadMiembros;
    encontrado->puntajeClan      = nuevoPuntajeClan;
    return true;
}

bool eliminarClan(int idClan) {
    if (listaVaciaClan()) return false;

    if (inicioClan->idClan == idClan && inicioClan->sig == inicioClan) {
        delete inicioClan;
        inicioClan = nullptr;
        return true;
    }
    if (inicioClan->idClan == idClan) {
        Clanes* ultimo   = obtenerUltimoClan();
        Clanes* eliminar = inicioClan;
        inicioClan       = inicioClan->sig;
        ultimo->sig      = inicioClan;
        delete eliminar;
        return true;
    }

    Clanes* actual = inicioClan;
    while (actual->sig != inicioClan && actual->sig->idClan != idClan) {
        actual = actual->sig;
    }
    if (actual->sig == inicioClan) return false;
    Clanes* eliminar = actual->sig;
    actual->sig      = eliminar->sig;
    delete eliminar;
    return true;
}

void liberarListaClan() {
    if (listaVaciaClan()) return;
    Clanes* actual = inicioClan->sig;
    while (actual != inicioClan) {
        Clanes* temp = actual;
        actual = actual->sig;
        delete temp;
    }
    delete inicioClan;
    inicioClan = nullptr;
}

Jugadores* insertarJugadorLista(Jugadores* lista, int idJugador, string nombreUsuario, int nivelRey, int trofeos, int idArena, int idClan) {
    Jugadores* nuevo = new Jugadores(idJugador, nombreUsuario, nivelRey, trofeos, idArena, idClan);
    if (lista == nullptr) return nuevo;
    Jugadores* temp = lista;
    while (temp->sig != nullptr) temp = temp->sig;
    temp->sig  = nuevo;
    nuevo->ant = temp;
    return lista;
}

Jugadores* buscarJugadorLista(Jugadores* lista, int idJugador) {
    Jugadores* temp = lista;
    while (temp != nullptr) {
        if (temp->idJugador == idJugador) return temp;
        temp = temp->sig;
    }
    return nullptr;
}

Jugadores* eliminarJugadorLista(Jugadores* lista, int idJugador) {
    if (lista == nullptr) { cout << "Lista vacía" << endl; return nullptr; }
    Jugadores* eliminar = buscarJugadorLista(lista, idJugador);
    if (eliminar == nullptr) { cout << "Jugador no encontrado" << endl; return lista; }
    if (eliminar->ant == nullptr && eliminar->sig == nullptr) { delete eliminar; return nullptr; }
    if (eliminar->ant == nullptr) { lista = eliminar->sig; lista->ant = nullptr; delete eliminar; return lista; }
    if (eliminar->sig == nullptr) { eliminar->ant->sig = nullptr; delete eliminar; return lista; }
    eliminar->ant->sig = eliminar->sig;
    eliminar->sig->ant = eliminar->ant;
    delete eliminar;
    return lista;
}

Batallas* buscarBatallaPorID(int IDBatalla) {
    if (primeraBatalla == nullptr) return nullptr;
    Batallas* temp = primeraBatalla;
    do {
        if (temp->IDBatalla == IDBatalla) return temp;
        temp = temp->sig;
    } while (temp != primeraBatalla);
    return nullptr;
}

void insertarBatallaAlFinal(int IDBatalla, int IDJugador1, int IDJugador2, int IDMazo1, int IDMazo2, int ganador, int coronasJ1, int coronasJ2, float duracion, int IDArena, string fecha) {

    if (buscarBatallaPorID(IDBatalla) != nullptr) {
        cout << "Ya existe la batalla con ID " << IDBatalla << endl;
        return;
    }
    if (coronasJ1 < 0 || coronasJ1 > 3 || coronasJ2 < 0 || coronasJ2 > 3) {
        cout << "Error: las coronas deben estar entre 0 y 3." << endl;
        return;
    }
    if (duracion <= 0) {
        cout << "Error: la duracion debe ser positiva." << endl;
        return;
    }
    Batallas* nueva = new Batallas(IDBatalla, IDJugador1, IDJugador2, IDMazo1, IDMazo2, ganador, coronasJ1, coronasJ2, duracion, IDArena, fecha);

    if (primeraBatalla == nullptr) {
        primeraBatalla = nueva;
        nueva->sig     = nueva;
        nueva->ant     = nueva;
        return;
    }

    Batallas* ultimo = primeraBatalla->ant;
    nueva->sig           = primeraBatalla;
    nueva->ant           = ultimo;
    ultimo->sig          = nueva;
    primeraBatalla->ant  = nueva;
}

void modificarBatallaCompleta(int IDBatalla, int IDJugador1, int IDJugador2, int IDMazo1, int IDMazo2, int ganador, int coronasJ1, int coronasJ2, float duracion, int IDArena, string fecha) {
    Batallas* batalla = buscarBatallaPorID(IDBatalla);
    if (!batalla) { cout << "Batalla no encontrada" << endl; return; }
    batalla->IDJugador1 = IDJugador1;
    batalla->IDJugador2 = IDJugador2;
    batalla->IDMazo1    = IDMazo1;
    batalla->IDMazo2    = IDMazo2;
    batalla->ganador    = ganador;
    batalla->coronasJ1  = coronasJ1;
    batalla->coronasJ2  = coronasJ2;
    batalla->duracion   = duracion;
    batalla->IDArena    = IDArena;
    batalla->fecha      = fecha;
    cout << "Batalla actualizada correctamente" << endl;
}

bool eliminarBatalla(int IDBatalla) {
    if (primeraBatalla == nullptr) return false;
    Batallas* eliminar = buscarBatallaPorID(IDBatalla);
    if (!eliminar) return false;

    if (primeraBatalla->sig == primeraBatalla && eliminar == primeraBatalla) {
        delete primeraBatalla;
        primeraBatalla = nullptr;
        return true;
    }
    eliminar->ant->sig = eliminar->sig;
    eliminar->sig->ant = eliminar->ant;
    if (eliminar == primeraBatalla) primeraBatalla = eliminar->sig;
    delete eliminar;
    return true;
}

void imprimirBatallas() {
    if (primeraBatalla == nullptr) { cout << "No hay batallas registradas" << endl; return; }
    Batallas* temp = primeraBatalla;
    cout << "----- BATALLAS -----" << endl;
    do {
        cout << "ID Batalla: "  << temp->IDBatalla
             << ", Jugador1: "  << temp->IDJugador1
             << ", Jugador2: "  << temp->IDJugador2
             << ", Mazo1: "     << temp->IDMazo1
             << ", Mazo2: "     << temp->IDMazo2
             << ", Ganador: "   << temp->ganador
             << ", Coronas J1: "<< temp->coronasJ1
             << ", Coronas J2: "<< temp->coronasJ2
             << ", Duracion: "  << temp->duracion
             << ", Arena: "     << temp->IDArena
             << ", Fecha: "     << temp->fecha << endl;
        cout << "-----------------------------" << endl;
        temp = temp->sig;
    } while (temp != primeraBatalla);
}

void liberarListaBatallas() {
    if (primeraBatalla == nullptr) return;
    Batallas* actual    = primeraBatalla;
    Batallas* siguiente = nullptr;
    do {
        siguiente = actual->sig;
        delete actual;
        actual = siguiente;
    } while (actual != primeraBatalla);
    primeraBatalla = nullptr;
}

int generarIdNoRepetidoBatalla() {
    int id;
    do { id = generarAleatorio(1000, 9999); } while (buscarBatallaPorID(id) != nullptr);
    return id;
}

bool existeJugador(int jugador) { return buscarJugadorID(jugador) != nullptr; }

int generarArenaAleatorio(int jugador1, int jugador2) {
    int arenaN = generarAleatorio(1, 2);
    Jugadores* jug = buscarJugadorID(arenaN == 1 ? jugador1 : jugador2);
    return jug->idArena;
}

void registrarBatalla(int jugador1, int jugador2) {
    Mazo* maz1 = buscarMazoxJugador(jugador1);
    Mazo* maz2 = buscarMazoxJugador(jugador2);

    if (maz1 != nullptr && maz1->idJugador != jugador1) {
        cout << "Error: el mazo no pertenece al jugador 1." << endl;
        return;
    }
    if (maz2 != nullptr && maz2->idJugador != jugador2) {
        cout << "Error: el mazo no pertenece al jugador 2." << endl;
        return;
    }

    if (existeJugador(jugador1) && existeJugador(jugador2) && jugador1 != jugador2 && maz1 != nullptr && maz2 != nullptr) {
        float duracion = generarDuracion();
        string fecha   = generarFecha();
        int idArena    = generarArenaAleatorio(jugador1, jugador2);

        int coronas1 = generarAleatorio(0, 3);
        int coronas2 = generarAleatorio(0, 2);
        int coronasJ1 = 0, coronasJ2 = 0;
        int ganador;
        int vida1 = vidaTotalMazo(maz1);
        int vida2 = vidaTotalMazo(maz2);
        int dano1 = danoTotalMazo(maz1);
        int dano2 = danoTotalMazo(maz2);

        int estadistica1 = vida1 - dano2;
        int estadistica2 = vida2 - dano1;

        if (estadistica1 < estadistica2) {
            ganador   = jugador2;
            coronasJ2 = coronas1;
            coronasJ1 = coronas2;
        } else if (estadistica2 < estadistica1) {
            ganador   = jugador1;
            coronasJ1 = coronas1;
            coronasJ2 = coronas2;
        } else {

            int maybe = generarAleatorio(1, 2);
            if (maybe == 1) {
                ganador = jugador1; coronasJ1 = coronas1; coronasJ2 = coronas2;
            } else {
                ganador = jugador2; coronasJ2 = coronas1; coronasJ1 = coronas2;
            }
        }

        Jugadores* jugadorGanador = buscarJugadorID(ganador);
        jugadorGanador->nivelRey++;

        int idBatalla = generarIdNoRepetidoBatalla();
        insertarBatallaAlFinal(idBatalla, jugador1, jugador2, maz1->idMazo, maz2->idMazo, ganador, coronasJ1, coronasJ2, duracion, idArena, fecha);
        cout << "Batalla registrada! Ganador: jugador " << ganador << endl;
    } else {
        cout << "Datos no validos para la batalla." << endl;
    }
}

void insertarCarta() {
    int id, costE, danB, vidB;
    string nom, rar, tipo;
    cout << "ID carta: ";                                      cin >> id;
    cout << "Nombre: ";                                        cin >> nom;
    cout << "Rareza (Comun/Rara/Epica/Legendaria): ";          cin >> rar;
    cout << "Tipo (Tropa/Hechizo/Edificio): ";                 cin >> tipo;
    cout << "Costo elixir: ";                                  cin >> costE;
    cout << "Dano base: ";                                     cin >> danB;
    cout << "Vida base: ";                                     cin >> vidB;
    insercioninicio(id, nom, rar, tipo, costE, danB, vidB);
}

void imprimirCartas() {
    if (listaCartas == nullptr) { cout << "No hay cartas registradas." << endl; return; }
    cout << "----- CARTAS -----" << endl;
    Carta* aux = listaCartas;
    while (aux != nullptr) {
        cout << "ID: " << aux->idCarta
             << ", Nombre: " << aux->nombre
             << ", Rareza: " << aux->rareza
             << ", Tipo: " << aux->tipo
             << ", Elixir: " << aux->costoElixir
             << ", Dano: " << aux->danoBase
             << ", Vida: " << aux->vidaBase << endl;
        aux = aux->sig;
    }
}

void eliminarCarta() {
    if (listaCartas == nullptr) { cout << "No hay cartas." << endl; return; }
    int id;
    cout << "ID carta a eliminar: "; cin >> id;

    Carta* aux = listaCartas;
    Carta* ant = nullptr;
    while (aux != nullptr) {
        if (aux->idCarta == id) {
            if (ant == nullptr) listaCartas = aux->sig;
            else ant->sig = aux->sig;
            delete aux;
            cout << "Carta eliminada." << endl;
            return;
        }
        ant = aux;
        aux = aux->sig;
    }
    cout << "No se encontro..." << endl;
}

void modificarCartaDatos() {
    if (listaCartas == nullptr) { cout << "No hay cartas..." << endl; return; }
    int id;
    cout << "ID de la carta a modificar: "; cin >> id;
    Carta* aux = listaCartas;
    while (aux != nullptr) {
        if (aux->idCarta == id) {
            cout << "1.Nombre  2.Rareza  3.Tipo  4.Elixir  5.Dano  6.Vida\nOpcion: ";
            int op; cin >> op;
            if (op == 1) {
                cout << "Nuevo nombre: "; cin >> aux->nombre;
                cout << "Nombre actualizado" << endl;
            }
            else if (op == 2) {
                cout << "Nueva rareza: "; cin >> aux->rareza;
                cout << "Rareza actualizada" << endl;
            }
            else if (op == 3) {
                cout << "Nuevo tipo: "; cin >> aux->tipo;
                cout << "Tipo actualizado." << endl;
            }
            else if (op == 4) {
                int v; cout << "Nuevo elixir: "; cin >> v;
                if (v < 0) cout << "Valor invalido." << endl;
                else { aux->costoElixir = v; cout << "Elixir actualizado." << endl; }
            }
            else if (op == 5) {
                int v; cout << "Nuevo dano: "; cin >> v;
                if (v < 0) cout << "Valor invalido." << endl;
                else { aux->danoBase = v; cout << "Dano actualizado." << endl; }
            }
            else if (op == 6) {
                int v; cout << "Nueva vida: "; cin >> v;
                if (v < 0) cout << "Valor invalido." << endl;
                else { aux->vidaBase = v; cout << "Vida actualizada." << endl; }
            }
            else cout << "Opcion invalida." << endl;
            return;
        }
        aux = aux->sig;
    }
    cout << "No se encontro la carta con ID " << id << "." << endl;
}

void modificarDatos() {
    cout << "Modificar:\n1.Nombre jugador\n2.Nivel Rey\n3.Trofeos\n4.Arena\n5.Clan\nOpcion: ";
    int op; cin >> op;
    int id;
    cout << "ID jugador: "; cin >> id;
    if (op == 1) {
        string ant, nuevo;
        cout << "Nombre actual: "; cin >> ant;
        cout << "Nuevo nombre: ";  cin >> nuevo;
        modificarJugadorNombre(id, ant, nuevo);
    } else if (op == 2) {
        int nivel; cout << "Nuevo nivel: "; cin >> nivel;
        modificarJugadorNivelRey(id, nivel);
    } else if (op == 3) {
        int trof; cout << "Nuevos trofeos: "; cin >> trof;
        modificarJugadorTrofeos(id, trof);
    } else if (op == 4) {
        int arena; cout << "Nueva arena ID: "; cin >> arena;
        modificarJugadorArena(id, arena);
    } else if (op == 5) {
        int clan; cout << "Nuevo clan ID: "; cin >> clan;
        modificarJugadorClan(id, clan);
    }
}

void insertarJugador() {
    int id, nivelRey, trofeos, idArena, idClan;
    string nombre;
    cout << "ID jugador: ";             cin >> id;
    cout << "Nombre usuario: ";         cin >> nombre;
    cout << "Nivel Rey: ";              cin >> nivelRey;
    cout << "Trofeos: ";                cin >> trofeos;
    cout << "ID Arena: ";               cin >> idArena;
    cout << "ID Clan (0=sin clan): ";   cin >> idClan;
    insertarJugadorOrdenadoPorNombre(id, nombre, nivelRey, trofeos, idArena, idClan);
}

void eliminarJugadorMenu() {
    int id;
    cout << "ID jugador a eliminar: "; cin >> id;
    eliminarJugador(id);
}

void insertarMazo() {
    int id, idJ;
    string nombre, tipo;
    cout << "ID mazo: ";    cin >> id;
    cout << "Nombre: ";     cin >> nombre;
    cout << "Tipo: ";       cin >> tipo;
    cout << "ID Jugador: "; cin >> idJ;

    if (buscarJugadorID(idJ) == nullptr) {
        cout << "Error: no existe el jugador con ID " << idJ << "." << endl;
        return;
    }
    insertarMazoFinal(id, nombre, tipo, idJ);

    Mazo* nuevoMazo = listaGlobalMazos;
    while (nuevoMazo != nullptr && nuevoMazo->idMazo != id) nuevoMazo = nuevoMazo->siguiente;
    if (nuevoMazo == nullptr) return;

    cout << "Agrega 8 cartas al mazo:" << endl;
    int agregadas = 0;
    while (agregadas < 8) {
        int idCarta;
        cout << "Carta " << (agregadas + 1) << " de 8 (ID): "; cin >> idCarta;
        int antes = contarCartasEnMazo(nuevoMazo);
        agregarCartaASublista(nuevoMazo, idCarta);
        if (contarCartasEnMazo(nuevoMazo) > antes) agregadas++;
    }
    cout << "Mazo completado con 8 cartas :)" << endl;
}

void imprimirMazos() {
    if (listaGlobalMazos == nullptr) { cout << "No hay mazos registrados." << endl; return; }
    cout << " ___________MAZOS___________ " << endl;
    Mazo* aux = listaGlobalMazos;
    while (aux != nullptr) {
        cout << "ID: " << aux->idMazo
             << ", Nombre: " << aux->nombreMazo
             << ", Tipo: " << aux->tipoMazo
             << ", Jugador: " << aux->idJugador
             << ", Cartas: " << contarCartasEnMazo(aux) << endl;

        NodoCartaMazo* c = aux->listaCartasMazo;
        while (c != nullptr) {
            cout << "   -> " << c->cartaReal->nombre
                 << " (Elixir: " << c->cartaReal->costoElixir << ")" << endl;
            c = c->siguiente;
        }
        aux = aux->siguiente;
    }
}

void eliminarMazo() {
    if (listaGlobalMazos == nullptr) { cout << "No hay mazos." << endl; return; }
    int id;
    cout << "ID mazo a eliminar: "; cin >> id;
    Mazo* aux = listaGlobalMazos;
    Mazo* ant = nullptr;
    while (aux != nullptr) {
        if (aux->idMazo == id) {
            if (ant == nullptr) listaGlobalMazos = aux->siguiente;
            else ant->siguiente = aux->siguiente;

            NodoCartaMazo* c = aux->listaCartasMazo;
            while (c != nullptr) {
                NodoCartaMazo* temp = c;
                c = c->siguiente;
                delete temp;
            }
            delete aux;
            cout << "Mazo eliminado." << endl;
            return;
        }
        ant = aux;
        aux = aux->siguiente;
    }
    cout << "No se encontro el mazo con ID " << id << "." << endl;
}

void modificarMazo() {
    if (listaGlobalMazos == nullptr) { cout << "No hay mazos." << endl; return; }
    int id;
    cout << "ID del mazo a modificar: "; cin >> id;
    Mazo* m = buscarMazoPorID(id);
    if (m == nullptr) { cout << "Mazo no encontrado." << endl; return; }
    cout << "1.Nombre  2.Tipo\nOpcion: ";
    int op; cin >> op;
    if (op == 1) {
        cout << "Nuevo nombre: "; cin >> m->nombreMazo;
        cout << "Nombre del mazo actualizado." << endl;
    } else if (op == 2) {
        cout << "Nuevo tipo: "; cin >> m->tipoMazo;
        cout << "Tipo del mazo actualizado." << endl;
    } else {
        cout << "Opcion invalida." << endl;
    }
}

void insertarClan() {
    int id, cant, pts;
    string nom, reg;
    cout << "ID: ";       cin >> id;
    cout << "Nombre: ";   cin >> nom;
    cout << "Region: ";   cin >> reg;
    cout << "Miembros: "; cin >> cant;
    cout << "Puntaje: ";  cin >> pts;
    insertarClanAlFinal(id, nom, reg, cant, pts);
}

void eliminarClanMenu() {
    int id;
    cout << "ID clan a eliminar: "; cin >> id;
    if (eliminarClan(id)) cout << "Clan eliminado." << endl;
    else cout << "Clan no encontrado." << endl;
}

void modificarClan() {
    if (listaVaciaClan()) { cout << "No hay clanes." << endl; return; }
    int id;
    cout << "ID del clan a modificar: "; cin >> id;
    Clanes* c = buscarClanPorId(id);
    if (c == nullptr) { cout << "Clan no encontrado." << endl; return; }
    cout << "1.Nombre  2.Region  3.Miembros  4.Puntaje\nOpcion: ";
    int op; cin >> op;
    if (op == 1) {
        cout << "Nuevo nombre: "; cin >> c->nombreClan;
        cout << "Nombre del clan actualizado." << endl;
    } else if (op == 2) {
        cout << "Nueva region: "; cin >> c->region;
        cout << "Region actualizada." << endl;
    } else if (op == 3) {
        int v; cout << "Nuevos miembros: "; cin >> v;
        if (v < 0) cout << "Valor invalido." << endl;
        else { c->cantidadMiembros = v; cout << "Miembros actualizados." << endl; }
    } else if (op == 4) {
        int v; cout << "Nuevo puntaje: "; cin >> v;
        if (v < 0) cout << "Valor invalido." << endl;
        else { c->puntajeClan = v; cout << "Puntaje actualizado." << endl; }
    } else {
        cout << "Opcion invalida." << endl;
    }
}

void insertarBatalla() {
    int j1, j2;
    cout << "ID Jugador 1: "; cin >> j1;
    cout << "ID Jugador 2: "; cin >> j2;
    registrarBatalla(j1, j2);
}

void eliminarBatallaMenu() {
    int id;
    cout << "ID batalla a eliminar: "; cin >> id;
    if (eliminarBatalla(id)) cout << "Batalla eliminada." << endl;
    else cout << "Batalla no encontrada." << endl;
}

void modificarBatallaMenu() {
    if (primeraBatalla == nullptr) { cout << "No hay batallas." << endl; return; }
    int id;
    cout << "ID de la batalla a modificar: "; cin >> id;
    Batallas* b = buscarBatallaPorID(id);
    if (b == nullptr) { cout << "Batalla no encontrada." << endl; return; }
    cout << "1.Ganador  2.Coronas J1  3.Coronas J2  4.Duracion  5.Arena  6.Fecha\nOpcion: ";
    int op; cin >> op;
    if (op == 1) {
        int g; cout << "Nuevo ganador (ID jugador): "; cin >> g;
        if (!existeJugador(g)) cout << "Jugador no existe." << endl;
        else { b->ganador = g; cout << "Ganador actualizado." << endl; }
    } else if (op == 2) {
        int v; cout << "Nuevas coronas J1 (0-3): "; cin >> v;
        if (v < 0 || v > 3) cout << "Valor invalido." << endl;
        else { b->coronasJ1 = v; cout << "Coronas J1 actualizadas." << endl; }
    } else if (op == 3) {
        int v; cout << "Nuevas coronas J2 (0-3): "; cin >> v;
        if (v < 0 || v > 3) cout << "Valor invalido." << endl;
        else { b->coronasJ2 = v; cout << "Coronas J2 actualizadas." << endl; }
    } else if (op == 4) {
        float d; cout << "Nueva duracion: "; cin >> d;
        if (d <= 0) cout << "Duracion invalida." << endl;
        else { b->duracion = d; cout << "Duracion actualizada." << endl; }
    } else if (op == 5) {
        int a; cout << "Nuevo ID arena: "; cin >> a;
        b->IDArena = a; cout << "Arena actualizada." << endl;
    } else if (op == 6) {
        string f; cout << "Nueva fecha: "; cin >> f;
        b->fecha = f; cout << "Fecha actualizada." << endl;
    } else {
        cout << "Opcion invalida." << endl;
    }
}

void consultaCartaMasUsada() {
    if (listaCartas == nullptr) { cout << "No hay cartas." << endl; return; }
    Carta* cartaMax  = nullptr;
    int maxCount = 0;
    Carta* auxC = listaCartas;
    while (auxC != nullptr) {
        int count = 0;
        Mazo* auxM = listaGlobalMazos;
        while (auxM != nullptr) {
            NodoCartaMazo* n = auxM->listaCartasMazo;
            while (n != nullptr) {
                if (n->cartaReal->idCarta == auxC->idCarta) count++;
                n = n->siguiente;
            }
            auxM = auxM->siguiente;
        }
        if (count > maxCount) { maxCount = count; cartaMax = auxC; }
        auxC = auxC->sig;
    }
    if (cartaMax != nullptr)
        cout << "Carta mas usada: " << cartaMax->nombre << " (en " << maxCount << " mazos)" << endl;
    else
        cout << "No hay datos suficientes." << endl;
}

void consultaMazoCostoMenor() {
    if (listaGlobalMazos == nullptr) { cout << "No hay mazos." << endl; return; }
    Mazo* menorMazo   = nullptr;
    float menorProm   = 9999;
    Mazo* aux = listaGlobalMazos;
    while (aux != nullptr) {
        int total = 0, cant = 0;
        NodoCartaMazo* c = aux->listaCartasMazo;
        while (c != nullptr) { total += c->cartaReal->costoElixir; cant++; c = c->siguiente; }
        if (cant > 0) {
            float prom = (float)total / cant;
            if (prom < menorProm) { menorProm = prom; menorMazo = aux; }
        }
        aux = aux->siguiente;
    }
    if (menorMazo != nullptr)
        cout << "Mazo con menor costo promedio: " << menorMazo->nombreMazo
             << " (promedio elixir: " << menorProm << ")" << endl;
}

void consultaArenaMasJugadores() {
    if (listaArenas == nullptr) { cout << "No hay arenas." << endl; return; }
    Arena* maxArena = nullptr;
    int maxCount = 0;
    Arena* auxA = listaArenas;
    while (auxA != nullptr) {
        int count = 0;
        Jugadores* auxJ = primerJugador;
        while (auxJ != nullptr) {
            if (auxJ->idArena == auxA->IdArena) count++;
            auxJ = auxJ->sig;
        }
        if (count > maxCount) { maxCount = count; maxArena = auxA; }
        auxA = auxA->sig;
    }
    if (maxArena != nullptr)
        cout << "Arena con mas jugadores: " << maxArena->nombreArena
             << " (" << maxCount << " jugadores)" << endl;
    else
        cout << "No hay jugadores asignados a ninguna arena." << endl;
}

void consultaBatallasPorArena() {
    if (primeraBatalla == nullptr) { cout << "No hay batallas." << endl; return; }
    imprimirArenas();
    int idA;
    cout << "Ingrese ID de arena: "; cin >> idA;
    cout << "Batallas en arena " << idA << ":" << endl;
    bool hay = false;
    Batallas* aux = primeraBatalla;
    do {
        if (aux->IDArena == idA) {
            cout << "  Batalla " << aux->IDBatalla
                 << " | J1: " << aux->IDJugador1
                 << " vs J2: " << aux->IDJugador2
                 << " | Ganador: " << aux->ganador
                 << " | Fecha: " << aux->fecha << endl;
            hay = true;
        }
        aux = aux->sig;
    } while (aux != primeraBatalla);
    if (!hay) cout << "No hay batallas en esa arena." << endl;
}

void consultaCartasLegendarias() {
    cout << "Cartas Legendarias:" << endl;
    bool hay = false;
    Carta* aux = listaCartas;
    while (aux != nullptr) {
        if (aux->rareza == "Legendaria") {
            cout << "  ID: " << aux->idCarta << ", Nombre: " << aux->nombre << endl;
            hay = true;
        }
        aux = aux->sig;
    }
    if (!hay) cout << "No hay cartas legendarias." << endl;
}

void consultaMazosPorJugador() {
    imprimirJugadores();
    int idJ;
    cout << "Ingrese ID de jugador: "; cin >> idJ;
    cout << "Mazos del jugador " << idJ << ":" << endl;
    bool hay = false;
    Mazo* aux = listaGlobalMazos;
    while (aux != nullptr) {
        if (aux->idJugador == idJ) {
            cout << "  ID: " << aux->idMazo << ", Nombre: " << aux->nombreMazo
                 << ", Tipo: " << aux->tipoMazo << endl;
            hay = true;
        }
        aux = aux->siguiente;
    }
    if (!hay) cout << "No hay mazos para ese jugador." << endl;
}

Jugadores* jugadoresDelClan(int idClan) {
    if (listaVaciaClan()) { cout << "No hay clanes" << endl; return nullptr; }
    Clanes* temp = inicioClan;
    do {
        if (temp->idClan == idClan) return temp->subListaJugadores;
        temp = temp->sig;
    } while (temp != inicioClan);
    cout << "Clan no encontrado" << endl;
    return nullptr;
}

Jugadores* jugadorMasTrofeos() {
    if (!primerJugador) { cout << "No hay jugadores" << endl; return nullptr; }
    Jugadores* temp = primerJugador;
    Jugadores* maxi = primerJugador;
    while (temp != nullptr) {
        if (temp->trofeos > maxi->trofeos) maxi = temp;
        temp = temp->sig;
    }
    return maxi;
}

Clanes* clanMasMiembros() {
    if (listaVaciaClan()) { cout << "No hay clanes" << endl; return nullptr; }
    Clanes* temp = inicioClan;
    Clanes* maxi = inicioClan;
    do {
        if (temp->cantidadMiembros > maxi->cantidadMiembros) maxi = temp;
        temp = temp->sig;
    } while (temp != inicioClan);
    return maxi;
}

Jugadores* jugadorMasBatallas() {
    if (!primerJugador) { cout << "No hay jugadores" << endl; return nullptr; }
    Jugadores* temp = primerJugador;
    Jugadores* maxi = primerJugador;
    while (temp != nullptr) {
        if (temp->nivelRey > maxi->nivelRey) maxi = temp;
        temp = temp->sig;
    }
    return maxi;
}

int menuConsultas() {
    cout << "--------------Consultas--------------" << endl;
    cout << "        Elija una opcion        " << endl;
    cout << "1.  Cual es la carta mas utilizada en todos los mazos?" << endl;
    cout << "2.  Que jugador tiene mas trofeos registrados?" << endl;
    cout << "3.  Que clan tiene mas miembros?" << endl;
    cout << "4.  Cual es el mazo con menor costo promedio de elixir?" << endl;
    cout << "5.  Que arena contiene mas jugadores?" << endl;
    cout << "6.  Que jugador ha ganado mas batallas?" << endl;
    cout << "7.  Cuales batallas se realizaron en una arena determinada?" << endl;
    cout << "8.  Cuales jugadores pertenecen a un clan especifico?" << endl;
    cout << "9.  Que cartas de rareza legendaria existen en el sistema?" << endl;
    cout << "10. Que mazos pertenecen a un jugador determinado?" << endl;
    cout << "0.  Salir" << endl;
    int opcion;
    cin >> opcion;
    return opcion;
}

void consultas() {
    int opcion;
    do {
        opcion = menuConsultas();

        if (opcion == 1) {
            cout << "Consulta 1: carta mas utilizada en todos los mazos" << endl;
            consultaCartaMasUsada();
        }
        else if (opcion == 2) {
            cout << "Consulta 2: jugador con mas trofeos" << endl;
            Jugadores* jugMax = jugadorMasTrofeos();
            if (jugMax) cout << "Nombre: " << jugMax->nombreUsuario << " /ID: " << jugMax->idJugador << endl;
        }
        else if (opcion == 3) {
            cout << "Consulta 3: clan con mas miembros" << endl;
            Clanes* clanMax = clanMasMiembros();
            if (clanMax) cout << "Nombre del clan: " << clanMax->nombreClan << " /ID: " << clanMax->idClan << endl;
        }
        else if (opcion == 4) {
            cout << "Consulta 4: mazo con menor costo promedio de elixir" << endl;
            consultaMazoCostoMenor();
        }
        else if (opcion == 5) {
            cout << "Consulta 5: arena con mas jugadores" << endl;
            consultaArenaMasJugadores();
        }
        else if (opcion == 6) {
            cout << "Consulta 6: jugador con mas batallas ganadas" << endl;
            Jugadores* jugMaxB = jugadorMasBatallas();
            if (jugMaxB) cout << "Nombre: " << jugMaxB->nombreUsuario << " /ID: " << jugMaxB->idJugador << endl;
        }
        else if (opcion == 7) {
            cout << "Consulta 7: batallas realizadas en una arena determinada" << endl;
            consultaBatallasPorArena();
        }
        else if (opcion == 8) {
            cout << "Consulta 8: jugadores que pertenecen a un clan especifico" << endl;
            imprimirClanes();
            int idClanParaJugadores;
            cout << "Ingrese el ID del clan: "; cin >> idClanParaJugadores;
            Jugadores* listaJ = jugadoresDelClan(idClanParaJugadores);
            imprimirJugadores(listaJ);
        }
        else if (opcion == 9) {
            cout << "Consulta 9: cartas de rareza legendaria existentes" << endl;
            consultaCartasLegendarias();
        }
        else if (opcion == 10) {
            cout << "Consulta 10: mazos que pertenecen a un jugador determinado" << endl;
            consultaMazosPorJugador();
        }
        else if (opcion == 0) {
            cout << "FIN DE CONSULTAS" << endl;
        }
        else {
            cout << "Opcion no valida" << endl;
            break;
        }

    } while (opcion != 0);
}

void reporte1TodasListas() {
    imprimirCartas();
    imprimirJugadores();
    imprimirMazos();
    imprimirClanes();
    imprimirArenas();
    imprimirBatallas();
}

void reporte2DetalladoJugadores() {
    if (!primerJugador) { cout << "No hay jugadores." << endl; return; }
    cout << "\n===== REPORTE DETALLADO DE JUGADORES =====" << endl;
    Jugadores* auxJ = primerJugador;
    while (auxJ != nullptr) {
        cout << "\n[" << auxJ->idJugador << "] " << auxJ->nombreUsuario
             << " | Nivel: " << auxJ->nivelRey << " | Trofeos: " << auxJ->trofeos << endl;

        Arena* auxA = listaArenas;
        while (auxA != nullptr) {
            if (auxA->IdArena == auxJ->idArena) { cout << "  Arena: " << auxA->nombreArena << endl; break; }
            auxA = auxA->sig;
        }

        if (!listaVaciaClan()) {
            Clanes* auxC = inicioClan;
            do {
                if (auxC->idClan == auxJ->idClan) { cout << "  Clan: " << auxC->nombreClan << endl; break; }
                auxC = auxC->sig;
            } while (auxC != inicioClan);
        }

        cout << "  Mazos:" << endl;
        Mazo* auxM = listaGlobalMazos;
        bool hayMazo = false;
        while (auxM != nullptr) {
            if (auxM->idJugador == auxJ->idJugador) {
                cout << "    -> " << auxM->nombreMazo << " (" << contarCartasEnMazo(auxM) << " cartas)" << endl;
                hayMazo = true;
            }
            auxM = auxM->siguiente;
        }
        if (!hayMazo) cout << "    (sin mazos)" << endl;
        auxJ = auxJ->sig;
    }
}

void reporte3MazosConCartas() {
    if (listaGlobalMazos == nullptr) { cout << "No hay mazos." << endl; return; }
    cout << "\n===== MAZOS CON SUS CARTAS =====" << endl;
    Mazo* aux = listaGlobalMazos;
    while (aux != nullptr) {
        cout << "\nMazo [" << aux->idMazo << "] " << aux->nombreMazo
             << " | Jugador ID: " << aux->idJugador << endl;
        NodoCartaMazo* c = aux->listaCartasMazo;
        int i = 1;
        while (c != nullptr) {
            cout << "  " << i++ << ". " << c->cartaReal->nombre
                 << " | Elixir: " << c->cartaReal->costoElixir
                 << " | Tipo: " << c->cartaReal->tipo << endl;
            c = c->siguiente;
        }
        aux = aux->siguiente;
    }
}

void reporte4ClanesMiembros() {
    if (listaVaciaClan()) { cout << "No hay clanes." << endl; return; }
    cout << "\n===== CLANES Y SUS MIEMBROS =====" << endl;
    Clanes* auxC = inicioClan;
    do {
        cout << "\nClan [" << auxC->idClan << "] " << auxC->nombreClan
             << " | Region: " << auxC->region << endl;

        Jugadores* auxJ = primerJugador;
        bool hay = false;
        while (auxJ != nullptr) {
            if (auxJ->idClan == auxC->idClan) {
                cout << "  -> " << auxJ->nombreUsuario << " (ID: " << auxJ->idJugador << ")" << endl;
                hay = true;
            }
            auxJ = auxJ->sig;
        }
        if (!hay) cout << "  (sin jugadores)" << endl;
        auxC = auxC->sig;
    } while (auxC != inicioClan);
}

void reporte5ArenaJugadores() {
    if (listaArenas == nullptr) { cout << "No hay arenas." << endl; return; }
    cout << "\n===== ARENAS Y SUS JUGADORES =====" << endl;
    Arena* auxA = listaArenas;
    while (auxA != nullptr) {
        cout << "\nArena [" << auxA->IdArena << "] " << auxA->nombreArena
             << " (" << auxA->trofeosMin << "-" << auxA->trofeosMax << " trofeos)" << endl;
        Jugadores* auxJ = primerJugador;
        bool hay = false;
        while (auxJ != nullptr) {
            if (auxJ->idArena == auxA->IdArena) {
                cout << "  -> " << auxJ->nombreUsuario << " (" << auxJ->trofeos << " trofeos)" << endl;
                hay = true;
            }
            auxJ = auxJ->sig;
        }
        if (!hay) cout << "  (sin jugadores)" << endl;
        auxA = auxA->sig;
    }
}

void reporte6Batallas() {
    imprimirBatallas();
}

void reporte7CartasPorElixir() {
    if (listaCartas == nullptr) { cout << "No hay cartas." << endl; return; }
    int n = 0;
    Carta* aux = listaCartas;
    while (aux != nullptr) { n++; aux = aux->sig; }

    for (int i = 0; i < n - 1; i++) {
        aux = listaCartas;
        for (int j = 0; j < n - i - 1; j++) {
            if (aux->costoElixir > aux->sig->costoElixir) {
                swap(aux->idCarta,     aux->sig->idCarta);
            swap(aux->nombre,      aux->sig->nombre);
                swap(aux->rareza,      aux->sig->rareza);
                swap(aux->tipo,        aux->sig->tipo);
                swap(aux->costoElixir, aux->sig->costoElixir);
                swap(aux->danoBase,    aux->sig->danoBase);
                swap(aux->vidaBase,    aux->sig->vidaBase);
            }
            aux = aux->sig;
        }
    }
    cout << "\n===== CARTAS ORDENADAS POR ELIXIR =====" << endl;
    aux = listaCartas;
    while (aux != nullptr) {
        cout << "[Elixir " << aux->costoElixir << "] "
             << aux->nombre << " | " << aux->rareza << " | " << aux->tipo << endl;
        aux = aux->sig;
    }
}

void reporte8JugadoresAlfabetico() {
    cout << "\n===== JUGADORES EN ORDEN ALFABETICO =====" << endl;
    if (!primerJugador) { cout << "No hay jugadores." << endl; return; }
    Jugadores* temp = primerJugador;
    while (temp != nullptr) {
        cout << temp->nombreUsuario << " (ID: " << temp->idJugador
             << ", Trofeos: " << temp->trofeos << ")" << endl;
        temp = temp->sig;
    }
}

void reporte9BatallasPorJugador() {
    if (!primerJugador) { cout << "No hay jugadores." << endl; return; }
    if (primeraBatalla == nullptr) { cout << "No hay batallas." << endl; return; }
    cout << "\n===== BATALLAS GANADAS POR JUGADOR =====" << endl;
    Jugadores* auxJ = primerJugador;
    while (auxJ != nullptr) {
        int ganadas = 0;
        Batallas* auxB = primeraBatalla;
        do {
            if ((auxB->ganador == auxJ->idJugador)) ganadas++;
            auxB = auxB->sig;
        } while (auxB != primeraBatalla);
        cout << auxJ->nombreUsuario << ": " << ganadas << " victorias" << endl;
        auxJ = auxJ->sig;
    }
}

void menuReportes() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n----- REPORTES -----" << endl;
        cout << "1. Todas las listas principales" << endl;
        cout << "2. Detalle de jugadores (arena, clan, mazos)" << endl;
        cout << "3. Mazos con sus 8 cartas" << endl;
        cout << "4. Clanes con sus miembros" << endl;
        cout << "5. Arenas con sus jugadores" << endl;
        cout << "6. Batallas (jugadores, mazos, coronas, ganador)" << endl;
        cout << "7. Cartas ordenadas por costo de elixir" << endl;
        cout << "8. Jugadores en orden alfabetico" << endl;
        cout << "9. Batallas ganadas por cada jugador" << endl;
        cout << "0. Salida" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        if      (opcion == 1) reporte1TodasListas();
        else if (opcion == 2) reporte2DetalladoJugadores();
        else if (opcion == 3) reporte3MazosConCartas();
        else if (opcion == 4) reporte4ClanesMiembros();
        else if (opcion == 5) reporte5ArenaJugadores();
        else if (opcion == 6) reporte6Batallas();
        else if (opcion == 7) reporte7CartasPorElixir();
        else if (opcion == 8) reporte8JugadoresAlfabetico();
        else if (opcion == 9) reporte9BatallasPorJugador();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            break;
        }
    }
}

void cargaAutomatica() {

    insertarArenaInicio(1,  "Campamento",          0,    299);
    insertarArenaInicio(2,  "Isla Goblin",          300,  599);
    insertarArenaInicio(3,  "Valle Boneyard",       600,  999);
    insertarArenaInicio(4,  "PEKKAs Playhouse",     1000, 1299);
    insertarArenaInicio(5,  "Capilla de la Bruja",  1300, 1599);
    insertarArenaInicio(6,  "Arena Real",           1600, 1999);
    insertarArenaInicio(7,  "Pico Helado",          2000, 2299);
    insertarArenaInicio(8,  "Jungla",               2300, 2599);
    insertarArenaInicio(9,  "Montana del Jabali",   2600, 2999);
    insertarArenaInicio(10, "Valle Electrico",      3000, 9999);

    insercioninicio(1,  "Pekka",          "Epica",      "Tropa",    7, 600, 3000);
    insercioninicio(2,  "Bola de fuego",  "Rara",       "Hechizo",  4, 350, 0);
    insercioninicio(3,  "Torre Inferno",  "Rara",       "Edificio", 5, 180, 1500);
    insercioninicio(4,  "Minipekka",      "Rara",       "Tropa",    4, 300, 900);
    insercioninicio(5,  "Dragon Inferno", "Epica",      "Tropa",    4, 100, 1000);
    insercioninicio(6,  "Esqueletos",     "Comun",      "Tropa",    1, 67,  81);
    insercioninicio(7,  "Caballero",      "Comun",      "Tropa",    3, 115, 1000);
    insercioninicio(8,  "Megacaballero",  "Legendaria", "Tropa",    7, 370, 3300);
    insercioninicio(9,  "Curandera",      "Legendaria", "Tropa",    3, 0,   600);
    insercioninicio(10, "Rayo",           "Comun",      "Hechizo",  3, 200, 0);
    insercioninicio(11, "Barbaro",        "Comun",      "Tropa",    5, 150, 1200);
    insercioninicio(12, "Arqueras",       "Comun",      "Tropa",    3, 119, 252);

    insertarClanAlFinal(1,  "LosPro",       "CR", 15, 5000);
    insertarClanAlFinal(2,  "EliteForce",   "MX", 20, 8000);
    insertarClanAlFinal(3,  "DragonTeam",   "ES", 10, 3000);
    insertarClanAlFinal(4,  "VikingsCR",    "US", 25, 9500);
    insertarClanAlFinal(5,  "ShadowClan",   "AR", 8,  2000);
    insertarClanAlFinal(6,  "PhoenixRise",  "CO", 18, 6500);
    insertarClanAlFinal(7,  "IronFist",     "PE", 12, 4000);
    insertarClanAlFinal(8,  "StormBreakers","BR", 22, 7500);
    insertarClanAlFinal(9,  "GoldenKings",  "CL", 30, 12000);
    insertarClanAlFinal(10, "NightWolves",  "VE", 5,  1500);

    insertarJugadorOrdenadoPorNombre(1,  "Alex",     10, 250,  1, 1);
    insertarJugadorOrdenadoPorNombre(2,  "Bruno",    8,  550,  2, 2);
    insertarJugadorOrdenadoPorNombre(3,  "Carlos",   11, 750,  3, 3);
    insertarJugadorOrdenadoPorNombre(4,  "Diana",    9,  1100, 4, 4);
    insertarJugadorOrdenadoPorNombre(5,  "Elena",    12, 1400, 5, 5);
    insertarJugadorOrdenadoPorNombre(6,  "Fernando", 7,  1700, 6, 6);
    insertarJugadorOrdenadoPorNombre(7,  "Gabriela", 13, 2100, 7, 7);
    insertarJugadorOrdenadoPorNombre(8,  "Hugo",     10, 2400, 8, 8);
    insertarJugadorOrdenadoPorNombre(9,  "Isabella", 11, 2700, 9, 9);
    insertarJugadorOrdenadoPorNombre(10, "Jorge",    14, 3100, 10, 10);

insertarMazoFinal(1, "MazoAlpha",   "Ataque",  1);
    Mazo* m = listaGlobalMazos;
    agregarCartaASublista(m,1); agregarCartaASublista(m,2); agregarCartaASublista(m,3); agregarCartaASublista(m,4);
    agregarCartaASublista(m,5); agregarCartaASublista(m,6); agregarCartaASublista(m,7); agregarCartaASublista(m,8);

    insertarMazoFinal(2, "MazoBeta",    "Defensa", 2);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,1); agregarCartaASublista(m,9);  agregarCartaASublista(m,3); agregarCartaASublista(m,10);
    agregarCartaASublista(m,5); agregarCartaASublista(m,11); agregarCartaASublista(m,7); agregarCartaASublista(m,12);

    insertarMazoFinal(3, "MazoGamma",   "Ciclo",   3);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,6); agregarCartaASublista(m,7);  agregarCartaASublista(m,10); agregarCartaASublista(m,12);
    agregarCartaASublista(m,4); agregarCartaASublista(m,2);  agregarCartaASublista(m,11); agregarCartaASublista(m,9);

    insertarMazoFinal(4, "MazoDelta",   "Ataque",  4);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,8); agregarCartaASublista(m,1); agregarCartaASublista(m,2); agregarCartaASublista(m,3);
    agregarCartaASublista(m,4); agregarCartaASublista(m,5); agregarCartaASublista(m,6); agregarCartaASublista(m,7);

    insertarMazoFinal(5, "MazoEpsilon", "Defensa", 5);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,9);  agregarCartaASublista(m,10); agregarCartaASublista(m,11); agregarCartaASublista(m,12);
    agregarCartaASublista(m,1);  agregarCartaASublista(m,2);  agregarCartaASublista(m,3);  agregarCartaASublista(m,4);

    insertarMazoFinal(6, "MazoZeta",    "Ciclo",   6);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,5);  agregarCartaASublista(m,6);  agregarCartaASublista(m,7);  agregarCartaASublista(m,8);
    agregarCartaASublista(m,9);  agregarCartaASublista(m,10); agregarCartaASublista(m,11); agregarCartaASublista(m,12);

    insertarMazoFinal(7, "MazoEta",     "Ataque",  7);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,8);  agregarCartaASublista(m,2); agregarCartaASublista(m,4); agregarCartaASublista(m,6);
    agregarCartaASublista(m,9);  agregarCartaASublista(m,11);agregarCartaASublista(m,1); agregarCartaASublista(m,3);

    insertarMazoFinal(8, "MazoTheta",   "Defensa", 8);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,12); agregarCartaASublista(m,10); agregarCartaASublista(m,8); agregarCartaASublista(m,6);
    agregarCartaASublista(m,4);  agregarCartaASublista(m,2);  agregarCartaASublista(m,9); agregarCartaASublista(m,7);

    insertarMazoFinal(9, "MazoIota",    "Ciclo",   9);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,1); agregarCartaASublista(m,3); agregarCartaASublista(m,5); agregarCartaASublista(m,7);
    agregarCartaASublista(m,9); agregarCartaASublista(m,11);agregarCartaASublista(m,2); agregarCartaASublista(m,4);

    insertarMazoFinal(10,"MazoKappa",   "Ataque",  10);
    m = listaGlobalMazos; while(m->siguiente != nullptr) m = m->siguiente;
    agregarCartaASublista(m,6);  agregarCartaASublista(m,8);  agregarCartaASublista(m,10); agregarCartaASublista(m,12);
    agregarCartaASublista(m,1);  agregarCartaASublista(m,3);  agregarCartaASublista(m,5);  agregarCartaASublista(m,7);

    srand(time(nullptr));
    registrarBatalla(1, 2);
    registrarBatalla(3, 4);
    registrarBatalla(5, 6);
    registrarBatalla(7, 8);
    registrarBatalla(9, 10);
    registrarBatalla(1, 3);
    registrarBatalla(2, 4);
    registrarBatalla(5, 7);
    registrarBatalla(6, 8);
    registrarBatalla(9, 1);

    cout << "\nCarga automatica completada :)" << endl;
}

void menuCartas() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n1.Crear\n2.Leer\n3.Actualizar\n4.Eliminar\n0.Salida\nOpcion: ";
        cin >> opcion;
        if      (opcion == 1) insertarCarta();
        else if (opcion == 2) imprimirCartas();
        else if (opcion == 3) modificarCartaDatos();
        else if (opcion == 4) eliminarCarta();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}

void menuModificacionesJugador() {
    cout<<"Elija una opcion:"<<endl;
    cout<<"1.nombreUsuario"<<endl;
    cout<<"2.nivelRey"<<endl;
    cout<<"3.trofeos"<<endl;
    cout<<"4.IDArena"<<endl;
    cout<<"5.IDClan"<<endl;
    cout<<"0.Salida"<<endl;

    int opcion;
    cin>>opcion;
    cout<<"Ingrese el id de jugador a modificar"<<endl;
    int id;
    cin>>id;
    while (opcion) {
        if (opcion == 1) {
            string nombreant, nombre;
            cout<<"Ingrese el nombre anterior y nuevo nombre"<<endl;
            cin>>nombreant>>nombre;
            modificarJugadorNombre(id, nombreant, nombre);
            return menuModificacionesJugador();
        }
        else if (opcion == 2) {
            int nivel;
            cout<<"Ingrese el nuevo nivel"<<endl;
            cin>>nivel;
            modificarJugadorNivelRey(id, nivel);
            return menuModificacionesJugador();
        }
        else if (opcion == 3) {
            int trofeo;
            cout<<"Ingrese el nuevo trofeo"<<endl;
            cin>>trofeo;
            modificarJugadorTrofeos(id,trofeo);
            return menuModificacionesJugador();
        }
        else if (opcion == 4) {
            int arena;
            cout<<"Ingrese la nueva arena"<<endl;
            cin>>arena;
            modificarJugadorArena(id,arena);
            return menuModificacionesJugador();
        }

        else if (opcion == 5) {
            int clan;
            cout<<"Ingrese el nuevo clan"<<endl;
            cin>>clan;
            modificarJugadorClan(id,clan);
            return menuModificacionesJugador();
        }
        else{
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}
void menuJugadores() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\nElija una opcion:" << endl;
        cout << "1.Crear" << endl;
        cout << "2.Leer" << endl;
        cout << "3.Actualizar" << endl;
        cout << "4.Eliminar" << endl;
        cout << "0.Salida" << endl;
        cin >> opcion;
        if      (opcion == 1) insertarJugador();
        else if (opcion == 2) imprimirJugadores();
        else if (opcion == 3) menuModificacionesJugador();
        else if (opcion == 4) eliminarJugadorMenu();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}

void menuMazos() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\nElija una opcion:" << endl;
        cout << "1.Crear" << endl;
        cout << "2.Leer" << endl;
        cout << "3.Actualizar" << endl;
        cout << "4.Eliminar" << endl;
        cout << "0.Salida" << endl;
        cin >> opcion;
        if      (opcion == 1) insertarMazo();
        else if (opcion == 2) imprimirMazos();
        else if (opcion == 3) modificarMazo();
        else if (opcion == 4) eliminarMazo();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}

void menuBatallas() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\nElija una opcion:" << endl;
        cout << "1.Crear" << endl;
        cout << "2.Leer" << endl;
        cout << "3.Actualizar" << endl;
        cout << "4.Eliminar" << endl;
        cout << "0.Salida" << endl;
        cin >> opcion;
        if      (opcion == 1) insertarBatalla();
        else if (opcion == 2) imprimirBatallas();
        else if (opcion == 3) modificarBatallaMenu();
        else if (opcion == 4) eliminarBatallaMenu();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}

void menuClanes() {
        int opcion = -1;
        while (opcion != 0) {
            cout << "\nElija una opcion:" << endl;
            cout << "1.Crear" << endl;
            cout << "2.Leer" << endl;
            cout << "3.Actualizar" << endl;
            cout << "4.Eliminar" << endl;
            cout << "0.Salida" << endl;
            cin >> opcion;
            if      (opcion == 1) insertarClan();
            else if (opcion == 2) imprimirClanes();
            else if (opcion == 3) modificarClan();
            else if (opcion == 4) eliminarClanMenu();
            else if (opcion != 0){
                cout << "Opcion invalida." << endl;
                exit(0);
            }
        }
    }


void menuArenas() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\nElija una opcion:" << endl;
        cout << "1.Crear" << endl;
        cout << "2.Leer" << endl;
        cout << "3.Actualizar" << endl;
        cout << "4.Eliminar" << endl;
        cout << "0.Salida" << endl;
        cin >> opcion;
        if      (opcion == 1) insertarArena();
        else if (opcion == 2) imprimirArenas();
        else if (opcion == 3) modificarArena();
        else if (opcion == 4) eliminarArena();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            exit(0);
        }
    }
}

void mantenimiento() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\nElija una opcion:" << endl;
        cout << "1.Cartas" << endl;
        cout << "2.Jugadores" << endl;
        cout << "3.Mazos" << endl;
        cout << "4.Clanes" << endl;
        cout << "5.Arenas" << endl;
        cout << "6.Batallas" << endl;
        cout << "0.Salida" << endl;
        cin >> opcion;
        if      (opcion == 1) menuCartas();
        else if (opcion == 2) menuJugadores();
        else if (opcion == 3) menuMazos();
        else if (opcion == 4) menuClanes();
        else if (opcion == 5) menuArenas();
        else if (opcion == 6) menuBatallas();
        else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
            break;
        }
    }
}

int printmenu() {
    cout << "--------------MENU--------------" << endl;
    cout << "        Elija una opcion        " << endl;
    cout << "1.Mantenimiento" << endl;
    cout << "2.Consultas" << endl;
    cout << "3.Reportes" << endl;
    cout << "4.Simulacion de batallas" << endl;
    cout << "5.Salida" << endl;
    int opcion;
    cin >> opcion;
    return opcion;
}

int main() {
    srand(time(nullptr));
    cargaAutomatica();

    int opcion = -1;
    while (opcion != 5) {
        opcion = int(printmenu());
        if      (opcion == 1) mantenimiento();
        else if (opcion == 2) consultas();
        else if (opcion == 3) menuReportes();
        else if (opcion == 4) {
            int j1, j2;
            cout << "ID Jugador 1: "; cin >> j1;
            cout << "ID Jugador 2: "; cin >> j2;
            registrarBatalla(j1, j2);
        }
        else if (opcion == 5) cout << "FIN DEL PROGRAMA" << endl;
        else {
            cout << "Opcion invalida." << endl;
            break;
        }
    }
    return 0;
}