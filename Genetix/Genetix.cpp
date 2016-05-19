#include "Algorithm.h"
#include <fstream>

int main(int argc, char * argv[]){
	srand((unsigned)time(NULL));
	int argn = 1;
	/*
	for (int i = 0; i < argc; i++)
		cout << argv[i] << "\t";
		*/
	//ifstream problemFile(argv[argn], ios_base::in);
	//ifstream dataFile(argv[argn], ios_base::in);
	if(argc < 2) {
		cout << "#><GeNeTiX><#. Por Calele. Uso:" << endl;
		cout << "$ " << argv[0] << " [< datos.txt] [generaciones] [tamanoPoblacion] [cruces] [mutacion] [seleccion] salida.txt" << endl;
		cout << "NOTA: Los argumentos entre parentesis cuadrados son opcionales. Sin archivo de datos, se leera desde la entrada estandar. Los otros argumentos se generaran aleatoriamente." << endl;
		return 1;
	}
	ofstream outFile(argv[argc-1]);

	cout << "Leyendo problema:" << endl;
	string name;
	cout << "\tNombre del problema:\n\t\t";
	getline(cin,name);
	Problem* problem = makeProblem(name);
	if(problem == NULL){
		cout << "Codigo de problema desconocido. Terminando...\n";
		return 2;
	}

	problem->readProblem();
	cout << "--------------" << endl;
	cout << "Escribiendo problema a archivo '" << argv[argc-1] << "'..." << endl;
//Redirect stdout to file stream
	auto coutBackup = cout.rdbuf();
	cout.rdbuf(outFile.rdbuf());
//Write problem to file	
	problem->writeProblem();
//Reset stdout to normal
	cout.rdbuf(coutBackup);
	

	Algorithm algorithm;
	algorithm.problem = problem;
	int generations = getIntArg(argc,argv,argn,1+rand()%2048); //If generations not specified, use random up to 2048
	algorithm.popSize = getIntArg(argc,argv,argn,1+rand()%8192); //If popSize not specified, use random up to 8192
	algorithm.crosses = getIntArg(argc,argv,argn,1+rand()%4); //If crosses not specified, use random up to 4
	algorithm.mutation = getIntArg(argc,argv,argn,1+rand()%100); //If mutation not specified, use random up to 100
	algorithm.policy = (SelectPolicy) getIntArg(argc,argv,argn,rand()%3); //If policy not specified, select at random from all 3

	cout << "--------------" << endl;
	//Generate random population if population size greater than zero
	if(algorithm.popSize > 0){
		cout << "Generando genes..." << endl;
		algorithm.resetPopulation();
	}
	else {
		cout << "Leyendo genes:" << endl;
		algorithm.readPopulation();
	}

	cout << "--------------" << endl;
	cout << "Ejecutando algoritmo genetico para " << generations << " generaciones..." << endl;
	for(int i = 0; i < generations; i++) {
		cout << "RANKING DE POBLACION. Generaciones = " << i << endl;
		algorithm.generation();
	}

	cout << "--------------" << endl;
	cout << "El mejor gen es:" << endl;
	algorithm.writeBest();

	cout << "--------------" << endl;
	cout << "Escribiendo genes a archivo '" << argv[argc-1] << "'..." << endl;

//Redirect stdout to file stream
	cout.rdbuf(outFile.rdbuf());
//Write algorithm to file	
	algorithm.writePopulation();
//Reset stdout to normal
	cout.rdbuf(coutBackup);

	return 0;
}
