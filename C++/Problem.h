#include <iostream>
#include <algorithm>
#include <sstream>
#include <set>
#include <vector>
#include <valarray>
#include <string>
#include <stdlib.h>

using namespace std;
/*
void removeCharsFromString( string &str, char* charsToRemove ) {
	for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
		str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
	}
}

int factorial(int n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
*/
typedef vector<bool> Gene;

class Problem {
public:
	virtual void readProblem() =0;
	virtual void writeProblem() {cout << name << endl;}
	virtual int geneSize() =0;
	virtual double fitness(Gene x) =0;
	//Either reads problem data from arguments, or generates them if not enough. Begin with argument number argn
	virtual int generateRandomProblem(int argc = 0, char* argv[] = NULL, int argn=1, int maxN = 2048) =0;
	virtual void printSolution(Gene x) =0;
	string name;
};

int getIntArg(int argc, char* argv[], int& argn, int defValue=0){
	//If enough arguments and numerical argument, return desired argument as int
	if(argc > argn && isdigit(argv[argn][0])) 
		return strtol(argv[argn++], NULL, 10);
	//Else return the default value
	else
		return defValue;
}

class GT1 : public Problem {
/* GT1: Vertex Cover 
Instance: Graph G = (V, E), positive integer K ≤ |V|.
Question: Is there a vertex cover of size K or less for G, i.e. a subset V'⊆ V with |V'|≤ K such that for each edge [u,v]∈ E at least one of u and v belongs to V'?
Variables: 
	N = |V|.
	M = |E|.
	K = desired cover size.
	edges = E
V' will be encoded in gene. Must minimize |V'|.
Gene representation: There are N = |V| bits in each gene. Given a position p(i), with 0 ≤ i < N:
	p = 1 if V[i]∈ V',
	else p = 0.
*/
public:
	int N, M, K;
	set <pair<int, int> > edges;
	void readProblem(){
		cout << "\tCantidad de vertices:\n\t\t";
		cin >> N;
		cout << "\tCantidad de aristas:\n\t\t";
		cin >> M;
		cout << "\tRecubrimiento:\n\t\t";
		cin >> K;
		cout << "\tE = {\n";
		int Vi, Vf;
		for (int i = 0; i<M; i++){
			cout << "\t\tE" << i << " = ";
			cin >> Vi;
			cin >> Vf;
			edges.insert(make_pair(Vi, Vf));
		}
		cout << "\t}\n";
	}

	void writeProblem() {
		Problem::writeProblem();
		cout << N << endl;
		cout << M << endl;
		cout << K << endl;
		//Print out in order, without duplicates
		for (auto const &it : edges) {
			cout << it.first << " " << it.second << endl;
		}
	}

	int geneSize(){
		return N;
	}

	double fitness(Gene x) {
		double result = 0;
		for(const auto &edge : edges){
			//If either vertex of edge in gene, increase. Else decrease
			if(x[edge.first] || x[edge.second])
				result++;
			else
				result--;
		}

		return result/K;
	}

	GT1() { name = "GT1"; }

	int generateRandomProblem(int argc, char* argv[], int argn, int maxN ) {
		N = getIntArg(argc, argv, argn, rand()%maxN);
		M = getIntArg(argc, argv, argn, rand()%(1+N*maxN));
		K = getIntArg(argc, argv, argn, rand()%N);
		for (int i = 0; i<M; i++){
			edges.insert(make_pair(rand()%N, rand()%N));
		}
		return argn;
	}

	void printSolution(Gene x) {
		cout << "V' = {";
		for (int i = 0; i<N; i++){
			if(x[i])
				cout << i << ", ";
		}
		cout << "}" <<endl;
	}
};

class SP5 : public Problem {
public:
/* SP5: Minimum Cover
Instance: Collection C of subsets of a finite set S, positive integer K ≤ |C|.	 
Question: Does C contain a cover for S of size K or less, i.e., a subset C'⊆ C with |C'| ≤ K such that every element of S belongs to at least one member of C'?
Variables: 
	C = the collection 
	N = |S|, assuming S = { i : 1 ≤ i ≤ N }. Thus N fully determines S.
	M = |C|. 
	K is a reference point for the minimization. 
The cover C' will be encoded in the gene.
Gene representation: There are M = |C| bits in each gene. Given a position p(i), with 0 ≤ i < M:
	p = 1 if C[i]∈ C',
	else p = 0.
*/
	vector<set <int>> C;
	int N, M, K;

	void readProblem(){
		cout << "\tTamaño de S:\n\t\t";
		cin >> N;
		cout << "\tCantidad de subconjuntos de S que se encuentran en C:\n\t\t";
		cin >> M;
		cout << "\tRecubrimiento:\n\t\t";
		cin >> K;
		cout << "\tC = {\n";

		set<int> auxSet;
		string s;
		getline(cin,s); //Kill newline
		for(int i = 0; i < M; i++) {
			cout << "\t\tC" << i << " = ";
			addSetToC(s, auxSet);
		}
		cout << "\t}\n";
	}

	void writeProblem() {
		Problem::writeProblem();
		cout << N << endl;
		cout << M << endl;
		cout << K << endl;
		//Print out in order, without duplicates
		for (auto const &subset : C) {
			for (auto const &it : subset) {
				cout << it << " ";
			}
			cout << endl;
		}
	}

	int geneSize(){
		return M;
	}

	SP5() { name = "SP5"; }

	int generateRandomProblem(int argc, char* argv[], int argn, int maxN){
		N = getIntArg(argc, argv, argn, rand()%maxN);

		int maxM = (1 << N); //Maximum is 2^N
		M = getIntArg(argc, argv, argn, rand()%maxM);
		M %= (1 << 20); //Make M less than 2^20 to make computation feasible

		K = getIntArg(argc, argv, argn, rand()%M);

		set<int> auxSet;
		int jMax;
		for (int i = 0; i<M; i++){
			//We need to randomize the randomness because computation is too fast and produces repeated random numbers
			jMax = abs(rand())%N + 1;
			for(int j = 0; j < jMax; j++)
				auxSet.insert(rand()%N + 1);
			C.push_back(auxSet);
			auxSet.clear();
		}
		return argn;
	}

	double fitness(Gene x) {
		double result = 0;
		for(int i = 0; i < N; i++){ //Iterate over S
			if(intInGene(i, x))
				result++;
			else
				result--;
		}
		return result/K;
	}

	void printSolution(Gene x) {
		cout << "C' = {";
		for (int i = 0; i<M; i++){
			if(x[i]){
				printSet(C[i]);
				cout << ",\n";
			}
		}
		cout << "}" <<endl;
	}

private:
	void printSet(set<int> &set){
		cout << "{";
		for(int element : set)
			cout << element << ", ";
		cout << "}";
	}

	bool intInGene(int i, Gene &x) {
		for(int j = 0; j < M; j++){ //Iterate over gene
			//If gene contains jth set in collection
			//and jth set contains current int, true
			if(x[j] && intInSet(i, C[j])) 
				return true;
		}
		return false;
	}

	bool intInSet(int i, set<int> &set){
		for(int element : set){
			if(i == element)
				return true;
		}
		return false;
	}

	void addSetToC(string &s, set<int> &auxSet) {
		getline(cin,s);
		if(s == "") //Ignore empty lines
			return;

		putToSet(s, auxSet); 
		while (s.find(" ") != string::npos){
			s = s.substr(s.find(" "));
			putToSet(s, auxSet);
			s.erase(s.begin());
		}
		C.push_back(auxSet);
		auxSet.clear(); //Clear for next call to addSetToC
	}

	void putToSet(string &s, set<int> &auxSet) {
		stringstream ss;
		int E; //Element of subset to be added
		ss << s;
		ss >> E;
		auxSet.insert(E);
//		cout << s << "\n\t\t" << E << endl;
	}
};

Problem* makeProblem(string name) {
	if(name == "GT1")
		return new GT1();
	else if (name == "SP5")
		return new SP5();
	return NULL;
}
