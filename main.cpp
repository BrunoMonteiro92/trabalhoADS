/***
UECE - Universidade Estadual do Ceará
Curso de Ciência da Computação
7ª Lista - Avaliação de Desempenho de Sistemas

Aluno: Bruno Marques Monteiro
Matrícula: 1157934
***/

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

int numPackages = 20;
int clockBefore = 0;
int clockAux = 0;

//Função para gerar número aleatório entre 0 e 1
double zeroToOne(){
	return (double)rand()/(double)RAND_MAX;
}

//Coluna 2 -> método da transformação inversa
double inverseTransformation(int lambda){
	double u = zeroToOne();
	u = zeroToOne();
	return (-lambda) * log(u);
}

//Coluna 4 -> método da rejeição utilizando distribuição de poisson
double rejectionMethod(int lambda){
	double u = 0;
	double p = 1;
	double n = 0;
	double x = 0;
	double f = exp(-lambda);
	
	u = zeroToOne();
	p = p * u;
	
	bool condition = true;
	while (condition){
		if (p < f){
			x = n;
			condition = false;
		}else{
			u = zeroToOne();
			p = p * u;
			n++;
		}
	}
	
	return (60/x);
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

	ofstream file;
	file.open("resultado.csv");
	file << "Nº Pacote;Tempo desde a última chegada do pacote anterior;Tempo de chegada no relógio;Tempo de serviço;Tempo de início do roteamento;Tempo do pacote na fila do roteador;Tempo final do roteamento no relógio;Tempo do pacote no roteador;Tempo livre do servidor;\n";

	for (int countPackage = 1; countPackage <= numPackages; countPackage++){
		tec = inverseTransformation(12);        //Coluna 2 -> Tempo desde a última chegada
		tecClock = clockBefore + tec;         //Coluna 3 -> Tempo de chegada no relógio
		serviceTime = rejectionMethod(6);               //Coluna 4 -> Tempo de serviço

		//Coluna 5 -> Tempo de início do serviço no relógio
		if (tecClock > clockAux)
			initialTime = tecClock;
		else
			initialTime = clockAux;

		packageTimeQueue = initialTime - tecClock;   //Coluna 6 -> Tempo do pacote na fila
		clockFinal = initialTime + serviceTime;              //Coluna 7 -> Tempo final do serviço no relógio
		packageTimeSystem = clockFinal - tecClock;   //Coluna 8 -> Tempo do pacote no sistema
		freeTime = initialTime - clockAux;        //Coluna 9 -> Tempo livre do operador

		cout << countPackage << " " << tec << " " << tecClock << " " << serviceTime << " " << initialTime << " " <<  packageTimeQueue << " " << clockFinal << " " << packageTimeSystem << " " << freeTime << endl;
		file << countPackage << ";" << replaceChar(to_string(tec), '.', ',') << ";" << replaceChar(to_string(tecClock), '.', ',')  << ";"
			 << replaceChar(to_string(serviceTime), '.', ',')  << ";" << replaceChar(to_string(initialTime), '.', ',')  << ";" << replaceChar(to_string(packageTimeQueue), '.', ',')  << ";"
			 << replaceChar(to_string(clockFinal), '.', ',')  << ";" << replaceChar(to_string(packageTimeSystem), '.', ',')  << ";" << replaceChar(to_string(freeTime), '.', ',')  << ";\n";

		clockBefore = tecClock;
		clockAux = clockFinal;
	}
	file.close();
	return 0;
}
