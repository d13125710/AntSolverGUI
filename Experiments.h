#pragma once
#include "tspalgorithm.h"
#include "utils.h"

#include <string>
#include <vector>
#include <map>


class Experiments :
	public TSPalgorithm
{
	
	std::map<std::string,int> m_problemFiles;
	//std::vector<std::string> m_problemFiles;
	std::vector<Parameters> m_parameters;

public:
	virtual void Run(Parameters &par ,  std::vector<std::vector<int> > &matrix, int experiment);	
	Experiments(void);
	~Experiments(void);

	void PopulateInstances();
	void constructParameterExp1();

	std::map<std::string,int> GetInstancesArray(){
		return m_problemFiles;
	}
	const std::vector<Parameters> &GetParametersArray(){
		return m_parameters;
	}


};

