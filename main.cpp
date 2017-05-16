#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#define ROWS 10
#define COLUMNS 3

using namespace std;

int numPackages = 20;
int clockBefore = 0;
int clockAux = 0;

double matTec [ROWS][COLUMNS] = {
	{(double)(0+5)/2, 0.01, 0.35},
	{(double)(5+10)/2, 0.36, 0.54},
	{(double)(10+15)/2, 0.55, 0.73},
	{(double)(15+20)/2, 0.74, 0.86},
	{(double)(20+25)/2, 0.87, 0.89},
	{(double)(25+30)/2, 0.90, 0.96},
	{(double)(30+35)/2, 0.97, 0.97},
	{(double)(35+40)/2, 0.98, 0.99},
	{(double)(40+45)/2, 1.00, 1.00},
	{(double)(45+50)/2, 0.00, 0.00}
};

double matTs [ROWS][COLUMNS] = {
	{(double)(0+5)/2, 0.01, 0.06},
	{(double)(5+10)/2, 0.07, 0.11},
	{(double)(10+15)/2, 0.12, 0.34},
	{(double)(15+20)/2, 0.35, 0.54},
	{(double)(20+25)/2, 0.55, 0.75},
	{(double)(25+30)/2, 0.76, 0.87},
	{(double)(30+35)/2, 0.88, 0.96},
	{(double)(35+40)/2, 0.97, 0.97},
	{(double)(40+45)/2, 0.98, 0.99},
	{(double)(45+100)/2, 1.00, 1.00}
};

//Fun��o para gerar n�mero aleat�rio entre 0 e 1
double zeroToOne(){
    return (double)rand()/(double)RAND_MAX;
}

double roundNumber(double x){
    return ceilf(x * 100) / 100;
}

//Coluna 2 e 4 -> m�todo de monte carlo usando as matrizes
double generateColumn(double m[ROWS][COLUMNS]){
    double urounded = 0;
    double u = zeroToOne();
    u = zeroToOne();
	bool v = true;
	double result = 0;
	urounded = roundNumber(u);
	cout << "Gerou o numero aleatorio: " << urounded << endl;

	while (v == true){
		for (int row=0; row<10; row++){
			if (urounded >= m[row][1] && urounded <= m[row][2]){
				result = m[row][0];
				v = false;
			}
		}
	}
	return result;
}

//Fun��o para transformar o valor em string
string to_string(double value) {
    stringstream sstr;
    string straux;
    sstr << value;
    straux = sstr.str();

    return straux;
}

//Fun��o para substituir os '.' em ',' (para melhor visualiza��o na tabela)
string replaceChar(string str, char ch1, char ch2) {
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == ch1)
            str[i] = ch2;
    }

  return str;
}

int main(){
    srand((unsigned)time(0));
    double tec, tecClock, serviceTime, initialTime = 0, packageTimeQueue, packageTimeSystem, freeTime, clockFinal;

    ofstream file;
    file.open("resultado.csv");
    file << "N� Pacote;Tempo desde a �ltima chegada do pacote anterior;Tempo de chegada no rel�gio;Tempo de servi�o;Tempo de in�cio do roteamento;Tempo do pacote na fila do roteador;Tempo final do roteamento no rel�gio;Tempo do pacote no roteador;Tempo livre do servidor;\n";

    for (int countPackage = 1; countPackage <= numPackages; countPackage++){
        tec = generateColumn(matTec);        //Coluna 2 -> Tempo desde a �ltima chegada
        tecClock = clockBefore + tec;         //Coluna 3 -> Tempo de chegada no rel�gio
        serviceTime = generateColumn(matTs);               //Coluna 4 -> Tempo de servi�o

        //Coluna 5 -> Tempo de in�cio do servi�o no rel�gio
        if (tecClock > clockAux)
            initialTime = tecClock;
        else
            initialTime = clockAux;

        packageTimeQueue = initialTime - tecClock;   //Coluna 6 -> Tempo do pacote na fila
        clockFinal = initialTime + serviceTime;              //Coluna 7 -> Tempo final do servi�o no rel�gio
        packageTimeSystem = clockFinal - tecClock;   //Coluna 8 -> Tempo do pacote no sistema
        freeTime = initialTime - clockAux;        //Coluna 9 -> Tempo livre do operador

        cout << countPackage << " " << tec << " " << tecClock << " "
            << serviceTime << " " << initialTime << " " <<  packageTimeQueue << " "
            << clockFinal << " " << packageTimeSystem << " " << freeTime << endl;
        file << countPackage << ";" << replaceChar(to_string(tec), '.', ',') << ";" << replaceChar(to_string(tecClock), '.', ',')  << ";"
             << replaceChar(to_string(serviceTime), '.', ',')  << ";" << replaceChar(to_string(initialTime), '.', ',')  << ";" << replaceChar(to_string(packageTimeQueue), '.', ',')  << ";"
             << replaceChar(to_string(clockFinal), '.', ',')  << ";" << replaceChar(to_string(packageTimeSystem), '.', ',')  << ";" << replaceChar(to_string(freeTime), '.', ',')  << ";\n";

        clockBefore = tecClock;
        clockAux = clockFinal;
    }
    file.close();
    return 0;
}
