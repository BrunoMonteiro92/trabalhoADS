/***
UECE - Universidade Estadual do Ceará
Curso de Ciência da Computação
6ª Lista - Avaliação de Desempenho de Sistemas

Aluno: Bruno Marques Monteiro
Matrícula: 1157934
***/

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

double matTecA [3][3] = {
    {10, 0.01, 0.35},
    {12, 0.36, 0.75},
    {14, 0.76, 1.00}
};

double matTsA [3][3] = {
    {9, 0.01, 0.30},
    {10, 0.31, 0.80},
    {11, 0.81, 1.0}
};

double matTecB [ROWS][COLUMNS] = {
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

double matTsB [ROWS][COLUMNS] = {
	{(double)roundNumber((9.00+9.55)/2), 0.01, 0.06},
	{(double)roundNumber((9.55+10.10)/2), 0.07, 0.11},
	{(double)roundNumber((10.10+10.65)/2), 0.12, 0.34},
	{(double)roundNumber((10.65+11.20)/2), 0.35, 0.54},
	{(double)roundNumber((11.20+11.75)/2), 0.55, 0.75},
	{(double)roundNumber((11.75+12.30)/2), 0.76, 0.87},
	{(double)roundNumber((12.30+12.85)/2), 0.88, 0.96},
	{(double)roundNumber((12.85+13.40)/2), 0.97, 0.97},
	{(double)roundNumber((13.40+13.95)/2), 0.98, 0.99},
	{(double)roundNumber((13.95+14.50)/2), 1.00, 1.00}
};

//Função para gerar número aleatório entre 0 e 1
double zeroToOne(){
    return (double)rand()/(double)RAND_MAX;
}

//Função para arredondar o número para 2 casas decimais
double roundNumber(double x){
	return ceilf(x * 100) / 100;
}


//Coluna 2 e 4 -> (Letra a)
double generateColumnSM(double m[3][3]){
    double urounded = 0;
    double u = zeroToOne();
    u = zeroToOne();
	bool v = true;
	double result = 0;
	urounded = roundNumber(u);
	cout << "Gerou o numero aleatorio: " << urounded << endl;

	while (v == true){
		for (int row=0; row<3; row++){
			if (urounded >= m[row][1] && urounded <= m[row][2]){
				result = m[row][0];
				v = false;
			}
		}
	}
	return result;
}

//Coluna 2 e 4 -> método de monte carlo usando as matrizes (Letra B)
double generateColumnMC(double m[ROWS][COLUMNS]){
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

//Função para transformar o valor em string
string to_string(double value) {
    stringstream sstr;
    string straux;
    sstr << value;
    straux = sstr.str();

    return straux;
}

//Função para substituir os '.' em ',' (para melhor visualização na tabela)
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
    int opcao;
    ofstream file;

    cout << "Digite a opcao desejada: " << endl;
    cout << "1 - Simulacao de acordo com a Letra A" << endl;
    cout << "2 - Simulacao de acordo com a Letra B (Metodo de Monte Carlo)" << endl;
    cin >> opcao;

    switch(opcao){
        case 1:
            file.open("letraA.csv");
            file << "Nº Pacote;Tempo desde a última chegada do pacote anterior;Tempo de chegada no relógio;Tempo de serviço;Tempo de início do roteamento;Tempo do pacote na fila do roteador;Tempo final do roteamento no relógio;Tempo do pacote no roteador;Tempo livre do servidor;\n";

            for (int countPackage = 1; countPackage <= numPackages; countPackage++){
                tec = generateColumnSM(matTecA);        //Coluna 2 -> Tempo desde a última chegada
                tecClock = clockBefore + tec;         //Coluna 3 -> Tempo de chegada no relógio
                serviceTime = generateColumnMC(matTsA);               //Coluna 4 -> Tempo de serviço

                //Coluna 5 -> Tempo de início do serviço no relógio
                if (tecClock > clockAux)
                    initialTime = tecClock;
                else
                    initialTime = clockAux;

                packageTimeQueue = initialTime - tecClock;   //Coluna 6 -> Tempo do pacote na fila
                clockFinal = initialTime + serviceTime;              //Coluna 7 -> Tempo final do serviço no relógio
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
            break;
        case 2:
            file.open("letraB.csv");
            file << "Nº Pacote;Tempo desde a última chegada do pacote anterior;Tempo de chegada no relógio;Tempo de serviço;Tempo de início do roteamento;Tempo do pacote na fila do roteador;Tempo final do roteamento no relógio;Tempo do pacote no roteador;Tempo livre do servidor;\n";

            for (int countPackage = 1; countPackage <= numPackages; countPackage++){
                tec = generateColumnMC(matTecB);        //Coluna 2 -> Tempo desde a última chegada
                tecClock = clockBefore + tec;         //Coluna 3 -> Tempo de chegada no relógio
                serviceTime = generateColumnMC(matTsB);               //Coluna 4 -> Tempo de serviço

                //Coluna 5 -> Tempo de início do serviço no relógio
                if (tecClock > clockAux)
                    initialTime = tecClock;
                else
                    initialTime = clockAux;

                packageTimeQueue = initialTime - tecClock;   //Coluna 6 -> Tempo do pacote na fila
                clockFinal = initialTime + serviceTime;              //Coluna 7 -> Tempo final do serviço no relógio
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
            break;

    }
}
