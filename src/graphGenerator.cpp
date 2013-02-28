/*
* Name: graphGenerator.cpp
* 
* Author: Alexandre Davis (agdavis@dcc.ufmg.br)
* Description: 
*   - Defines Opinion Agreement Graph (OAG) according Calais et. al 2011
*      
*/
#include<vector>
#include<map>
#include"simpleEclat.cpp"

int cmp(double x, double y, const double eps=1e-9){
	if (x + eps < y) return -1;
		if (x - eps > y) return +1;
			return 0;
}

double lift(Pattern &patterns, std::string A, std::string B){
	if (patterns.getPatternRelFrequency(A) == 0 || patterns.getPatternRelFrequency(B) == 0) return -1;
	return (patterns.getPatternRelFrequency(A,B)/(patterns.getPatternRelFrequency(A) * patterns.getPatternRelFrequency(B)));
}

class OAG {
	std::map <std::string, int> uid;
	std::map <std::string, std::vector < std::string > > active;
	std::map <std::string, std::vector < std::string > > passive;
	Pattern activePatterns;
	Pattern passivePatterns;
	std::vector < std::pair< std::pair < std::string, std::string >, double > > edges; 
	
	public:
	int nUsers;
	OAG(){nUsers=0;}
	OAG(std::string fileInteractions);
	void readInteraction(std::string A, std::string B);
	double weightEdge(std::string A, std::string B);
	void generateGraph();
	void computatePatternFrequency();
	void printGraph();
};

OAG::OAG(std::string fileInteractions){
	std::ifstream f;
	nUsers = 0;
	f.open(fileInteractions.c_str());
	std::string A, B;

	while(f.good()){
		f >> A >> B;
		readInteraction(A, B);
	}
}

void OAG::readInteraction(std::string A, std::string B){
	active[A].push_back(B);
	passive[B].push_back(A);

	if(uid.find(A) == uid.end()){
		uid[A] = nUsers;
		nUsers++;
	}
	if(uid.find(B) == uid.end()){
		uid[B] = nUsers;
		nUsers++;
	}
}

double OAG::weightEdge(std::string A, std::string B){
	double activeScore = lift(activePatterns, A, B);
	double passiveScore = lift(passivePatterns, A, B);
	if (activeScore != -1 || passiveScore != -1){
		return std::max(activeScore, passiveScore);
	}else{
		return 0.0;
	}
}

void OAG::computatePatternFrequency(){
	std::map <std::string, std::vector < std::string > >::iterator it;
	for (it = active.begin(); it != active.end(); ++it){
		activePatterns.readTransaction(it->second);
	}

	for (it = passive.begin(); it != passive.end(); ++it){
		passivePatterns.readTransaction(it->second);
	}
}


void OAG::generateGraph(){
	std::map < std::string, int>::iterator i;		
	std::map < std::string, int>::iterator j;
	double w;

	for(i = uid.begin(); i != uid.end(); ++i){
		for(j = i; j != uid.end(); ++j){
			if (i != j){
				w = weightEdge(i->first, j->first);
				if (cmp(w, 0.0) != 0){
					edges.push_back(make_pair(make_pair(i->first, j->first), w));
				}
			}
		}
	}
}

void OAG::printGraph(){
	std::vector < std::pair< std::pair < std::string, std::string >, double > > :: iterator it;

	std::cout << nUsers << " " << edges.size() << std::endl;
	for (it = edges.begin(); it != edges.end(); ++it){
		std::cout << it->first.first << " " << it->first.second << " " << it->second << std::endl;
	}
}
	
int main(){
	OAG graph("ent.txt");
	graph.computatePatternFrequency();
	graph.generateGraph();
	graph.printGraph();
	return 0;
}
