#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <stack>
using namespace std;
enum tFicha{ NULA, VACIA = 2, FICHA = 10, SELECCIONADA=14,POSIBLE=12};
enum tEstado { BLOQUEO, GANADOR, JUGANDO };
enum  tFallo{ FUERA, NULO, INMOVILIZADA, CORRECTO };
enum tMovimientos { ARRIBA, DERECHA, ABAJO, IZQUIERDA, ARIZ, ARDE, ABIZ, ABDE };
const int DIM = 10, NUM_DIR = 8, MAX_REWIND = 3, BLANCO = 0;
typedef tFicha tMatriz[DIM][DIM];
typedef bool tPosibles[NUM_DIR];

const int incF[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
const int incC[] = { 0, 1, 0, -1, -1, 1, -1, 1 };

struct tCasilla{
	int fila;
	int columna;
};
struct tMovimiento{
	int fila;
	int columna;
	tMovimientos direccion;
};


using tBolas = vector<tCasilla>;

struct tPila{
	stack <tMovimiento> pila;
	int cont;
};


struct tJuegoPeg{
	int filas;
	int columnas;
	tMatriz tablero;
	tCasilla meta;
	int bolas;
	tEstado estado;
	tPila movimiento;
};




tMovimientos convertir(int num);
ostream & operator<< (ostream & devolver, tMovimientos movimiento);
int fallo();
bool cargar(tJuegoPeg & juego, const string & nombre, vector <tCasilla> & disponibles);
void guardar(tJuegoPeg const & juego, const string & nombre);
void mostrar(const tJuegoPeg & juego);
tFallo posiblesmovimientos(tJuegoPeg const & juego, tCasilla cas, tPosibles posibles);
bool ejecutarmovimiento(tJuegoPeg & juego, const tMovimiento & mov, vector <tCasilla> & disponibles);
bool cinfallo(int opc, tPosibles &posibles);
void cambiaraseleccionada(int fila, int columna, tJuegoPeg & juego);
void cambiaraposible(int auxfila, int auxcolumna, tJuegoPeg & juego, int h);
void reiniciarposibles(tJuegoPeg & juego, int fila, int columna, tPosibles & posibles);
void volveraestado(tJuegoPeg & juego, int auxfila, int auxcolumna, tPosibles & posibles);
void generarTablero(tJuegoPeg & tablero, tCasilla dimension, int numbolas);
void marchatras(tJuegoPeg & juego, vector <tCasilla> & disponibles);
void modificarstack(tJuegoPeg & juego, const tMovimiento & mov);
bool posiblesestados(tJuegoPeg & juego, tCasilla & cas, tPosibles & posibles);
void cambiaraposible(tPosibles & posibles, tJuegoPeg & juego, tCasilla & cas);
void mover(tMovimiento & mov, tCasilla & cas, tJuegoPeg & juego, tPosibles & posibles, int opc);
void colorFondo(int color);