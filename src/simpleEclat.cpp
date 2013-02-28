#include<stdio.h>
#include <iostream>
#include <fstream>
#include<map>
#include<string>
#include<vector>
#include<set>


class Pattern{
	int nTransactions;

	public:
	std::map<std::string, std::set<int> > invList;
	Pattern(){nTransactions=0;}
	Pattern(std::string fileName);

	void readTransaction(std::vector<std::string> &);
	int getPatternFrequency(std::string);
	int getPatternFrequency(std::string, std::string);
	double getPatternRelFrequency(std::string item);
	double getPatternRelFrequency(std::string item1, std::string item2);

};

Pattern::Pattern(std::string fileName){
	std::ifstream f;
	int sTrans;
	std::string item;
	f.open(fileName.c_str());
	nTransactions = 0;

	while(f.good()){
		// Optimizable
		std::vector<std::string> trans;
		f >> sTrans;
		for (int i = 0; i < sTrans; i++){
			f >> item;
			trans.push_back(item);
		}
		readTransaction(trans);
	}
}

void Pattern :: readTransaction(std::vector<std::string> &trans){
	for(std::vector<std::string>::iterator item = trans.begin(); item != trans.end(); ++item)
		invList[*item].insert(nTransactions);
	if(trans.size())
		nTransactions++;
}

int Pattern :: getPatternFrequency(std::string item){
	return invList[item].size();
}

int Pattern :: getPatternFrequency(std::string item1, std::string item2){
	std::set<int>::iterator p1 = invList[item1].begin();
	std::set<int>::iterator p2 = invList[item2].begin();

	int interSize = 0;
	while(p1 != invList[item1].end() && p2 != invList[item2].end()){
		if (*p1 == *p2){
			interSize++;
			++p1;
			++p2;
		}else{
			if (*p1 > *p2) ++p2;
			else ++p1;
		}
	}

	return interSize;
}


double Pattern :: getPatternRelFrequency(std::string item){
	return ((double)getPatternFrequency(item)/(double)nTransactions);
}
double Pattern :: getPatternRelFrequency(std::string item1, std::string item2){
	return ((double)getPatternFrequency(item1, item2)/(double)nTransactions);
}

