#include "Problem.h"
#include <map>

enum SelectPolicy { RANDOM, FORTUNE, TOURNAMENT };

class Algorithm {
public:
	SelectPolicy policy;
	int crosses;
	double mutation;
	int popSize;
	Problem* problem;

	void generation(){
		calculateRanking();
		int i = 0;
		for (auto gene : fitnessRanking){
			cout << i++ << " : " << gene.first << " : ";
			writeGene(population[gene.second]);
		}
		vector<Gene> newPopulation;
		for(int i = 0; i < popSize/2; i++) {
			pair<Gene, Gene> parents = selectParents();
			pair<Gene, Gene> children = makeChildren(parents);
			newPopulation.push_back(children.first);
			newPopulation.push_back(children.second);
		}
		population = newPopulation;
	}

	~Algorithm() {
		delete problem;
	}

	void resetPopulation(){
		for(int i = 0; i < popSize; i++)
			addRandomGene();

	}

	void readPopulation(){
		popSize = 0;
		while(getGene() && ++popSize);
	}

	void writePopulation(){
		for(int i = 0; i < popSize; i++)
			writeGene(population[i]);
	}

	Gene getBest(int order = 1){ //By default get first best.
		auto rankIterator = fitnessRanking.end();
		while(order--)
			--rankIterator;
		int bestIndex = rankIterator->second;
		return population[bestIndex];
	}
	
	void writeBest() {
		Gene best = getBest();
		writeGene(best);
		problem->printSolution(best);
	}

private:
	vector<Gene> population;
	multimap<double, int> fitnessRanking;

	pair<Gene, Gene> selectParents(){
		switch (policy) {
			case RANDOM:
				return selectRandom();
			case FORTUNE:
				return selectFortune();
			case TOURNAMENT:
				return selectTournament();
		}
	}

	pair<Gene, Gene> selectRandom() {
		Gene parent1 = population[rand()%popSize];
		Gene parent2 = population[rand()%popSize];
		return make_pair(parent1, parent2);
	}

	pair<Gene, Gene> selectFortune() {
		//Stores the ratio between fitness and fitnessSum, for each gene.
		vector<double> fitnessProportion;
		double fitnessSum = 0;
		for(auto f : fitnessRanking) {
			fitnessProportion[f.second] = f.first; //Load fitnesses
			fitnessSum += f.first;
		}

		vector<int> geneIndexes;
		int i = 0;
		int scale = 100;
		for(double &ratio : fitnessProportion){
			ratio /= fitnessSum;//Ratio is fitness/fitnessSum
			for(int j = 0; j < ratio*scale; j++)
				geneIndexes.push_back(i);
			i++;
		}


		Gene parent1 = population[geneIndexes[rand()%scale]];
		Gene parent2 = population[geneIndexes[rand()%scale]];
		return make_pair(parent1, parent2);
	}

	pair<Gene, Gene> selectTournament() {
		Gene parent1 = getBest();
		Gene parent2 = getBest(2);
		return make_pair(parent1, parent2);
	}

	pair<Gene, Gene> makeChildren(pair<Gene, Gene> parents){
		int geneSize = problem->geneSize();
		set<int> points;
		for(int i = 0; i < crosses; i++)
			points.insert(rand()%geneSize);

		Gene child1, child2;

		auto par1 = parents.first.begin();
		auto par2 = parents.second.begin();

		for(int i = 0; i < geneSize; i++) {
			if(points.find(i) != points.end()){
//				cout << "Swapping parents..." << i << endl;
				auto temp = par1;
				par1 = par2;
				par2 = temp;
			}
			child1.push_back(par1[i]);
			child2.push_back(par2[i]);
		}
		cout << "Parents : ";
		writeGene(parents.first);
		writeGene(parents.second);

		cout << "Children : ";
		writeGene(child1);
		writeGene(child2);

		
		return make_pair(child1, child2);
	}

	void addRandomGene() {
		int geneSize = problem->geneSize();
		Gene newGene(geneSize);
		for(int i = 0; i < geneSize; i++)
			newGene[i] = rand()%2;

		population.push_back(newGene);
	}

	bool getGene() {
		cout << "\tGen: ";
		int geneSize = problem->geneSize();
		Gene newGene(geneSize);
		string line;
		getline(cin,line); 
		if(line == "")
			return false;
		line.resize(geneSize, '0'); //Resize string to geneSize. Fill missing with 0's
		for(int i = 0; i < geneSize; i++)
			newGene[i] = line[i] - '0'; //Convert char to number

		population.push_back(newGene);
		return true;
	}

	void writeGene(Gene gene) {
		int geneSize = problem->geneSize();
		for(int i = 0; i < geneSize; i++)
			cout << gene[i];
		cout << endl;
	}

	void calculateRanking(){
		fitnessRanking.clear();
		for(int i = 1; i < popSize; i++){
			Gene current = population[i];
			double currentFitness = problem->fitness(current);
			fitnessRanking.emplace(currentFitness, i);
		}
	}
};
