#include "Algorithm.h"
#include <fstream>

int main(int argc, char* argv[]){
	srand((unsigned)time(NULL));

	string name;
	if(argc >= 2)
		name = argv[1];
	else {
		cout << "Generador de problemas. Por Calele. Uso:\n $ " << argv[0] << " tipoProblema [argProblema1] .. [argProblemaN] [salida.txt]\n";
		cout << "NOTA: Los argumentos entre parentesis cuadrados son opcionales. Sin archivo de salida, se escribira a la salida estandar. Los otros argumentos se generaran aleatoriamente." << endl;
		return 1;
	}

	Problem* problem = makeProblem(name);
	if(problem == NULL){
		cout << "Codigo de problema desconocido. Terminando...\n";
		return -1;
	}
	int argn = problem->generateRandomProblem(argc, argv, 2);

	Algorithm algorithm;
	algorithm.problem = problem;
	algorithm.popSize = 1+rand()%8192;
	algorithm.resetPopulation();

	if(argc >= 3) {
		cout << "Generando problema de tipo " << name << " a " << argv[argn] << "...\n";
		ofstream outFile(argv[argn]);
	//Redirect stdout to file stream
		auto coutBackup = cout.rdbuf();
		cout.rdbuf(outFile.rdbuf());
	//Write problem and population to file	
		problem->writeProblem();
		algorithm.writePopulation();
	//Reset stdout to normal
		cout.rdbuf(coutBackup);
	}
	else {
		problem->writeProblem();
		algorithm.writePopulation();
	}

	return 0;
}
