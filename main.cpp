#include "juegoPeg.h";
using namespace std;

void convertirmayusculas(string & nombre){
	for (int i = 0; i < nombre.size(); ++i)
		nombre[i] = toupper(nombre[i]);
}

bool seleccionareleccion(){
	bool cond = false;
	string eleccion;
	cin >> eleccion;
	convertirmayusculas(eleccion);
	while (eleccion != "SI" && eleccion != "NO"){
		cout << "Por favor introduzca \"SI\" o \"NO\":";
		cin >> eleccion;
		convertirmayusculas(eleccion);
	}
	if (eleccion == "SI") cond= true;
	return cond;
}

bool posiblesestados(tJuegoPeg & juego, tCasilla & cas, tPosibles & posibles){
	bool cond = false;
	switch (posiblesmovimientos(juego, cas, posibles)){
	case FUERA: cout << "Valor fuera de rango, por favor, elija fila de 1 a "
		<< juego.filas << " y de columna de 1 a " << juego.columnas << '\n'; break;
	case NULO: cout << "No se ha podido encontrar una ficha disponible en la casilla seleccionada\n"; break;
	case INMOVILIZADA: cout << "La casilla seleccionada no tiene ningun movimiento disponible\n"; break;
	case CORRECTO: cond = true; cambiaraseleccionada(cas.fila, cas.columna, juego); break;
	}
	return cond;
}


bool comprobardimension(tCasilla dimension, int numbolas){
	if (dimension.fila > 2 && dimension.fila <= DIM && dimension.columna > 2 && dimension.fila <= DIM){
		if (numbolas < dimension.fila * dimension.columna) return true;
		else cout << "Introduzca un numero de bolas de 2 a " << dimension.fila * dimension.columna - 1 << '\n';
	}
	else cout << "Dimension incorrecta. Por favor, introduzca un numero de filas y columnas entre 3 y " << DIM << '\n';
	return false;
}

bool leerMovimiento(tJuegoPeg & juego, tMovimiento & mov, vector<tCasilla> & disponibles){
	tCasilla cas;
	bool cond = false;
	int opc = 0;
	while (!cond){
		tPosibles posibles;
		for (int i = 0; i < NUM_DIR; ++i){
			posibles[i] = false;
		}
		if (juego.movimiento.cont == 0) cout << "Seleccione ficha (fila y columna), o 0 para salir:";
		else cout << "Seleccione ficha (fila y columna), -1 para retroceder o 0 para salir(quedan " << juego.movimiento.cont << " retrocesos):";
		cin >> cas.fila;
		if (cas.fila == -1 && juego.movimiento.cont != 0){
			marchatras(juego, disponibles);
			system("cls");
			mostrar(juego);
		}
		else if (cas.fila != 0){
			cin >> cas.columna;
			--cas.fila, --cas.columna;
			cond = posiblesestados(juego, cas, posibles);
			if (cond){
				cambiaraposible(posibles, juego, cas);
				for (int h = 0; h < NUM_DIR; ++h){
					if (posibles[h]){
						tMovimientos movimiento = convertir(h);
						cout << h + 1 << "-. " << movimiento;
					}
				}
				cout << "0-. Salir\n";

				cout << "Introduzca una posicion valida: ";
				cin >> opc;
				while (!cinfallo(opc, posibles)){
					cin >> opc;
				}
				if (opc == 0){
					cond = false;
					volveraestado(juego, cas.fila, cas.columna, posibles);
				}
				else {
					mover(mov, cas, juego, posibles, opc);
					return true;
				}
			}
		}
		else cin.sync(), cin.clear(), cond = true;
	}
	return false;
}

bool partida(tJuegoPeg & juego, vector <tCasilla> & disponibles){
	tMovimiento mov;
	juego.estado = JUGANDO;
	juego.movimiento.cont = 0;
	bool cond = true;
	while (juego.estado == JUGANDO && cond){
		mostrar(juego);
		if (leerMovimiento(juego, mov, disponibles)){
			if (!ejecutarmovimiento(juego, mov, disponibles)){
				if (juego.tablero[juego.meta.fila][juego.meta.columna] == FICHA && juego.bolas == 1) juego.estado = GANADOR;
				else juego.estado = BLOQUEO;
				}
			system("cls");
			}
		else cond = false;
}
	return cond;
}

void pedirtablero(tJuegoPeg & juego){
	int numbolas;
	tCasilla dimension;
	string opcion;
	do{
		cout << "Introduzca el tamaño del juego(filas y columnas):";
		cin >> dimension.fila >> dimension.columna;
		cout << "Introduzca el número de bolas:";
		cin >> numbolas;
	} while (!comprobardimension(dimension, numbolas));
	generarTablero(juego,dimension,numbolas);
	cout << "Se ha generado un tablero de " << juego.bolas << " bolas\n";
	mostrar(juego);
	cout << "¿Desea guardar el tablero actual?: ";
	if (seleccionareleccion()){
		cout << "\nPor favor, seleccione el nombre del archivo donde desea guardarlo: ";
		cin >> opcion;
		guardar(juego, opcion);
	}
}

int menu(){
	int nummenu;
	cout << setw(10) << setfill('-') << '\n';
	cout << "1.- Jugar partida\n";
	cout << "2.- Generar tablero\n";
	cout << "0.- Salir\n";
	cout << setw(10) << setfill('-') << '\n';
	cin >> nummenu;
	while (cin.fail() || nummenu < 0 || nummenu >3){
		nummenu=fallo();
	}
	return nummenu;
}

void opcion1(tJuegoPeg & juego){
	string nombre;
	vector <tCasilla> disponibles;
	bool deseojugar = true;
	while (deseojugar) {
		cout << "Introduza el nombre del tablero: ";
		cin >> nombre;
		system("cls");
		if (cargar(juego, nombre, disponibles)){
			if (!partida(juego, disponibles)){
				cout << "Usted salio de la partida. ¿Quiere guardar el fichero?";
				if (seleccionareleccion()){
					cout << "Introduzca el nombre del fichero: ";
					cin >> nombre;
					guardar(juego, nombre);
				}
			}
			else{
				if (juego.estado == GANADOR) cout << "¡Felicidades! Usted ha ganado\n";
				else cout << "No existen movimientos posibles\n";
			}
			cout << "¿Desea jugar otra partida?";
			if (!seleccionareleccion()) deseojugar = false;
		}
		else cout << "No se pudo abrir el archivo\n";
	}
}

int main(){
	srand(time(NULL));
	tJuegoPeg juego;
	int nummenu;
	colorFondo(BLANCO);
	nummenu = menu();
	system("cls");
	while (nummenu != 0){
		switch (nummenu){
		case 1:opcion1(juego); break;
		case 2:pedirtablero(juego); break;
		}
		nummenu = menu();
	}
	return 0;
}