#include "juegoPeg.h";

void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

istream & operator>> (istream & devolver, tFicha & ficha){
	int num;
	devolver >> num;
	switch (num){
	case 0: ficha = NULA; break;
	case 2: ficha = VACIA; break;
	case 10: ficha = FICHA; break;
	}
	return devolver;
}

tMovimientos convertir(int num){
	tMovimientos movimiento;
	switch (num){
	case 0: movimiento = ARRIBA; break;
	case 1: movimiento = DERECHA; break;
	case 2: movimiento = ABAJO; break;
	case 3: movimiento = IZQUIERDA; break;
	case 4: movimiento = ARIZ; break;
	case 5: movimiento = ARDE; break;
	case 6: movimiento = ABIZ; break;
	case 7: movimiento = ABDE; break;
	}
	return movimiento;
}

ostream & operator<< (ostream & devolver, tMovimientos movimiento){
	switch (movimiento){
	case ARRIBA: devolver << "Arriba\n"; break;
	case DERECHA:devolver << "Derecha\n"; break;
	case ABAJO: devolver << "Abajo\n"; break;
	case IZQUIERDA: devolver << "Izquierda\n"; break;
	case ARIZ: devolver << "Arriba e izquierda\n"; break;
	case ARDE: devolver << "Arriba y derecha\n"; break;
	case ABIZ: devolver << "Abajo e izquierda\n"; break;
	case ABDE: devolver << "Abajo y derecha\n"; break;
	}
	return devolver;
}

int fallo(){
	int numero;
	cin.sync(); cin.clear();
	cout << "Introduzca un valor valido:\n";
	cin >> numero;
	return numero;
}

bool cargar(tJuegoPeg & juego, const string & nombre, vector <tCasilla> & disponibles){
	ifstream cargar;
	cargar.open(nombre);
	bool cond = cargar.is_open();
	if (cond){
		cargar >> juego.filas >> juego.columnas;
		for (int filas = 0; filas < juego.filas; ++filas){
			for (int columnas = 0; columnas < juego.columnas; ++columnas){
				cargar >> juego.tablero[filas][columnas];
				if (juego.tablero[filas][columnas] == FICHA){
					disponibles.push_back({ filas, columnas });
				}
			}
		}
		cargar >> juego.meta.fila >> juego.meta.columna;
		cargar >> juego.bolas;
	}
	return cond;
}

void mostrar(const tJuegoPeg & juego){
	cout << setfill(' ') << setw(3);
	for (int i = 0; i < juego.columnas; ++i){
		cout << " " << (i + 1);
	}
	cout << "\n\n";
	for (int j = 0; j < juego.filas; ++j){
		cout << ' ' << j + 1;
		for (int t = 0; t < juego.columnas; ++t){
			cout << ' ';
			colorFondo(juego.tablero[j][t]);
			if (juego.meta.columna != t || juego.meta.fila != j) cout << ' ';
			else cout << char(254);
			colorFondo(BLANCO);
		}
		cout << "\n\n";
	}
}

void guardar(tJuegoPeg const & juego, const string & nombre){
	ofstream guardar;
	guardar.open(nombre);
	int fila = juego.filas, columna = juego.columnas;
	guardar << fila << ' ' << columna << '\n';
	for (int h = 0; h < fila; ++h){
		for (int j = 0; j < columna; ++j){
			guardar << juego.tablero[h][j] << ' ';
		}
		guardar << '\n';
	}
	guardar << juego.meta.fila << ' ' << juego.meta.columna << '\n' << juego.bolas;
	guardar.close();
}

bool rango(tCasilla cas, tCasilla & pos, tCasilla & sig,const tJuegoPeg & juego, int i){
	pos.fila = cas.fila + incF[i];
	pos.columna = cas.columna + incC[i];
	sig.fila = pos.fila + incF[i];
	sig.columna = pos.columna + incC[i];
	return sig.fila >= 0 && juego.filas > sig.fila && sig.columna >= 0 && sig.columna < juego.columnas;
}

bool comprobarvecinas(const tJuegoPeg & juego, tCasilla cas, tPosibles posibles){
	tCasilla pos, sig;
	bool cond = false;
	for (int i = 0; i < NUM_DIR; ++i){
		if (rango(cas, pos, sig, juego, i)){
			if (juego.tablero[pos.fila][pos.columna] == FICHA && juego.tablero[sig.fila][sig.columna] == VACIA){
				cond = true;
				posibles[i] = true;
			}
		}
	}
	return cond;
}

tFallo posiblesmovimientos(tJuegoPeg const & juego, tCasilla cas, tPosibles posibles){
	tFallo fallo = CORRECTO;
	if (cas.fila< 0 || cas.fila > juego.filas - 1 || cas.columna < 0 || cas.columna >juego.columnas - 1) fallo = FUERA;
	else if (juego.tablero[cas.fila][cas.columna] == NULA || juego.tablero[cas.fila][cas.columna] == VACIA) fallo = NULO;
	else if (!comprobarvecinas(juego, cas, posibles)) fallo = INMOVILIZADA;
	return fallo;
}

bool comprobarbloqueo(vector <tCasilla> & disponibles, tJuegoPeg & juego){
	int bolas = juego.bolas;
	tPosibles posibles;
	for (int i = 0; i < bolas; ++i){
		if (comprobarvecinas(juego, disponibles[i], posibles)) return true;
	}
	return false;
}

void modificartCasilla(vector <tCasilla> & disponibles, const tMovimiento & mov){
	bool encontrado = false, modificado = false; unsigned int i = 0;
	while (!encontrado || !modificado && i<disponibles.size()){
		if (disponibles[i].fila == mov.fila + incF[mov.direccion] && disponibles[i].columna == mov.columna + incC[mov.direccion]){
			encontrado = true;
			for (unsigned int j = i; j < disponibles.size() - 1; ++j){
				disponibles[j].fila = disponibles[j + 1].fila;
				disponibles[j].columna = disponibles[j + 1].columna;
			}
			disponibles.pop_back();
			--i;
		}
		else if (disponibles[i].fila == mov.fila && disponibles[i].columna == mov.columna){
			modificado = true;
			disponibles[i].fila += 2 * incF[mov.direccion];
			disponibles[i].columna += 2 * incC[mov.direccion];
		}
		++i;
	}
}

bool ejecutarmovimiento(tJuegoPeg & juego, const tMovimiento & mov, vector<tCasilla> &disponibles){
	bool cond = false;
	juego.tablero[mov.fila][mov.columna] = VACIA;
	juego.tablero[mov.fila + incF[mov.direccion]][mov.columna + incC[mov.direccion]] = VACIA;
	juego.tablero[mov.fila + 2 * incF[mov.direccion]][mov.columna + 2 * incC[mov.direccion]] = FICHA;
	--juego.bolas;
	modificartCasilla(disponibles, mov);
	if (comprobarbloqueo(disponibles, juego)) {
		cond = true;
	}
	return cond;
}

bool cinfallo(int opc, tPosibles &posibles){
	bool cond = false;
	if (opc > NUM_DIR || opc < 0) cout << "Valor fuera de rango. Introduzca valor correcto:\n";
	else if (opc == 0)cond = true;
	else if (opc != 0 && posibles[opc - 1])	cond = true;
	else if (!posibles[opc - 1]) cout << "Elija un valor correcto:";
	return cond;
}

void cambiaraseleccionada(int fila, int columna, tJuegoPeg & juego){
	system("cls");
	juego.tablero[fila][columna] = SELECCIONADA;
	mostrar(juego);
}

void cambiaraposible(int fila,int columna, tJuegoPeg & juego, int h){
	int auxfila = 0, auxcolumna = 0;
	auxfila = fila + 2 * incF[h];
	auxcolumna = columna + 2 * incC[h];
	juego.tablero[auxfila][auxcolumna] = POSIBLE;
}

void reiniciarposibles(tJuegoPeg & juego, int fila, int columna, tPosibles & posibles){
	int auxfila = 0, auxcolumna = 0;
	for (int h = 0; h < NUM_DIR; ++h){
		if (posibles[h]){
			tMovimientos movimiento = convertir(h);
			auxfila = fila + 2 * incF[h];
			auxcolumna = columna + 2 * incC[h];
			juego.tablero[auxfila][auxcolumna] = VACIA;
		}
	}
}

void volveraestado(tJuegoPeg & juego, int fila, int columna, tPosibles & posibles){
	system("cls");
	juego.tablero[fila][columna] = FICHA;
	reiniciarposibles(juego, fila, columna, posibles);
	mostrar(juego);
}

void iniciarTablero(tJuegoPeg & juego, tCasilla dimension, int numbolas){
	juego.filas = dimension.fila;
	juego.columnas = dimension.columna;
	juego.bolas = numbolas;
	for (int filas = 0; filas < dimension.fila; ++filas){
		for (int columnas = 0; columnas < dimension.columna; ++columnas){
			juego.tablero[filas][columnas] = NULA;
		}
	}
}

void fijarMeta(tJuegoPeg & juego){
	juego.meta.fila = rand() % juego.filas;
	juego.meta.columna = rand() % juego.columnas;
	juego.tablero[juego.meta.fila][juego.meta.columna] = FICHA;
}

bool movimientoInverso(tJuegoPeg & juego, tBolas & bolas, int ficha){
	tCasilla pos, sig, act;
	bool cond = false;
	int aleatorio,inicial;
	inicial = rand() % NUM_DIR;
	aleatorio = inicial;
	act = bolas[ficha];
	do{
		if (rango(act, pos, sig, juego, aleatorio)){
			if (juego.tablero[pos.fila][pos.columna] != FICHA && juego.tablero[sig.fila][sig.columna] != FICHA){
				cond = true;
				bolas[ficha] = pos;
				bolas.push_back(sig);
				juego.tablero[pos.fila][pos.columna] = FICHA;
				juego.tablero[sig.fila][sig.columna] = FICHA;
				juego.tablero[act.fila][act.columna] = VACIA;
			}
		}
		aleatorio = (aleatorio + 1) % NUM_DIR;
	} while (!cond && aleatorio != inicial);
	return cond;
}

bool sustituir(tJuegoPeg & juego, tBolas & bolas){
	int inicial = rand() % bolas.size();
	int ficha = inicial; bool sustituida = false;
	do{
		sustituida = movimientoInverso(juego, bolas, ficha);
		ficha = (ficha + 1) % bolas.size();
	} while(!sustituida && ficha != inicial);
	return sustituida;
}

void generarTablero(tJuegoPeg & juego,tCasilla dimension, int numbolas){
	tBolas bolas;
	iniciarTablero(juego,dimension,numbolas);
	fijarMeta(juego);
	bolas.push_back(juego.meta);
	int cont = 1;
	while (cont < numbolas && sustituir(juego, bolas)){
		++cont;
	}
	juego.bolas = cont;
}

void modificarAtras(vector<tCasilla> & disponibles, const tMovimiento & mov){
	bool modificado = false;
	int i = 0;
	int fil = mov.fila + 2 * incF[mov.direccion], col = mov.columna + 2 * incC[mov.direccion];
	disponibles.push_back({mov.fila + incF[mov.direccion], mov.columna + incC[mov.direccion]});
	while (!modificado){
		if (fil == disponibles[i].fila && col == disponibles[i].columna){
			disponibles[i].fila = mov.fila;
			disponibles[i].columna = mov.columna;
			modificado = true;
		}
		++i;
	}
}

void marchatras(tJuegoPeg & juego,vector <tCasilla> & disponibles){
	tMovimiento movimiento;
	movimiento=juego.movimiento.pila.top();
	juego.tablero[movimiento.fila][movimiento.columna] = FICHA;
	juego.tablero[movimiento.fila + incF[movimiento.direccion]][movimiento.columna + incC[movimiento.direccion]] = FICHA;
	juego.tablero[movimiento.fila + 2*incF[movimiento.direccion]][movimiento.columna + 2*incC[movimiento.direccion]] = VACIA;
	juego.movimiento.pila.pop();
	--juego.movimiento.cont;
	++juego.bolas;
	modificarAtras(disponibles, movimiento);
}

void modificarstack(tJuegoPeg & juego, const tMovimiento & mov){
	if (juego.movimiento.cont != MAX_REWIND){
		++juego.movimiento.cont;
	}
	juego.movimiento.pila.push(mov);
}


void cambiaraposible(tPosibles & posibles, tJuegoPeg & juego, tCasilla & cas){
	for (int h = 0; h < NUM_DIR; ++h){
		if (posibles[h]){
			cambiaraposible(cas.fila, cas.columna, juego, h);
		}
	}
	system("cls");
	mostrar(juego);
}

void mover(tMovimiento & mov, tCasilla & cas, tJuegoPeg & juego, tPosibles & posibles,int opc){
	mov.fila = cas.fila;
	mov.columna = cas.columna;
	mov.direccion = convertir(opc - 1);
	posibles[opc - 1] = false;
	modificarstack(juego, mov);
	reiniciarposibles(juego, cas.fila, cas.columna, posibles);
}