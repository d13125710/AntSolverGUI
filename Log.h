#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils.h"


namespace antlogger {

class CLog
{
	std::ofstream m_inFileStream; 
	int m_bestTourToDateLenght;
public:
	CLog( const char *filename , Parameters &p)
	{
		init(filename , p);
	}
	CLog(){	
	}
	void init(const char *filename , Parameters &p)
	{
		m_inFileStream.open(filename);	
		if (m_inFileStream.is_open()) {
			m_inFileStream << "Ants System " << p.lambda << std::endl;
			m_inFileStream << "Log file created" << std::endl << std::endl;
			m_inFileStream.flush();
		}
	
		m_bestTourToDateLenght = (std::numeric_limits<int>::max)();
	}
	~CLog(void){
		if (m_inFileStream.is_open()) 
		{
			m_inFileStream.flush();
			m_inFileStream.close();	
		}
	}
	friend CLog &operator << (CLog &logger, const char *text) {

		logger.m_inFileStream << text << std::endl;
		return logger;

	}
	//clog << std::string("test data\n");
	friend CLog &operator << (CLog &logger, std::string *text) {

		logger.m_inFileStream << *text << std::endl;
		return logger;

	}
	friend CLog &operator << (CLog &logger, std::vector<size_t> &data) {

		for(size_t i=0; i< data.size();i++)
			logger.m_inFileStream << data[i] << ",";
		logger.m_inFileStream <<std::endl;
			logger.m_inFileStream.flush();
		return logger;
	}
	// Parameters &p
	friend CLog &operator << (CLog &logger, Parameters &p) {

	//	for(size_t i=0; i< data.size();i++)
	//		logger.m_inFileStream << data[i] << ",";
		logger.m_inFileStream << " === Parameters for all including defaults ===" << std::endl  << std::endl;
		logger.m_inFileStream << "Alpha =" << p.Alpha << std::endl;
		logger.m_inFileStream << "Beta =" << p.Beta << std::endl;
		logger.m_inFileStream << "rho =" << p.rho << std::endl;

		logger.m_inFileStream << "iterations =" << p.Epochs << std::endl;
		logger.m_inFileStream << "lambda =" << p.lambda << std::endl;
		logger.m_inFileStream << "local search LK" << p.LocalSearchGreed << std::endl;
		logger.m_inFileStream << "local search Opt2 =" << p.LocalSearchOpt2 << std::endl;
		logger.m_inFileStream << "local search Opt3 =" << p.LocalSearchOpt3 << std::endl;
		logger.m_inFileStream << "stagnations =" << p.maxStagnationIterations << std::endl;


		logger.m_inFileStream << "ants/population =" << p.noAnts << std::endl;

		logger.m_inFileStream <<  std::endl << " === Problem Details ===" << std::endl;
		logger.m_inFileStream << "opt solution =" << p.problemLength << std::endl;
		logger.m_inFileStream << "tsp problem =" << p.problemName << std::endl;

		logger.m_inFileStream <<  std::endl <<" === elitist Ant System ===" << std::endl;
		logger.m_inFileStream << "Elite Weight =" << p.EliteWeight << std::endl;

		logger.m_inFileStream <<  std::endl << " === Genetic system ===" << std::endl;
		logger.m_inFileStream << "mutation =" << p.mutationRatio << std::endl;
	
		logger.m_inFileStream << std::endl << " === Ant colony System ==="<< std::endl; 
		logger.m_inFileStream << "q0 =" << p.q0 << std::endl;
		logger.m_inFileStream << "xi =" << p.xi << std::endl;
		logger.m_inFileStream << std::endl <<  " === Ant colony System and mutated ant system ===" << std::endl;;
		logger.m_inFileStream << "r0 =" << p.r0 << std::endl;
		logger.m_inFileStream << " ================================================="<< std::endl;;
		logger.m_inFileStream <<std::endl;


		logger.m_inFileStream << std::endl << " Running " << p.getAlgType() <<std::endl <<std::endl;
		logger.m_inFileStream.flush();
		return logger;
	}
	friend CLog &operator << (CLog &logger, Tour &p) {

		logger.m_inFileStream << "Irreation =" << p.currentIrreation;
		logger.m_inFileStream << " , Best Tour Length =" << p.bestTourToDateLenght << ", Avg branching factor =" << p.branching_factor << 
			", Distance between Ants =" << p.Distance_betweenAnts << ", pop_stddev =" << p.pop_stddev  << ", mean =" << p.mean << 
			", Worse Tour To Date Length =" << p.worseTourToDateLenght << ", extra info =" << p.extrainfo <<
			std::endl;
		logger.m_inFileStream.flush();
		return logger;
	}
	template<typename T>
	void logMatrix(std::vector<std::vector<T> > *matrix)
	{
		m_inFileStream << ("Matrix:\n");
		for (size_t i = 0 ; i < matrix->size() ; i++) {
			m_inFileStream << ("From :") << i << "\t" ;
			for ( size_t j = 0 ; j < matrix->size() - 1 ; j++ ) {
				 m_inFileStream <<  (*matrix)[i][j] << ",";
			}
			m_inFileStream << (*matrix)[i][ matrix->size() - 1] << std::endl;

		}
		m_inFileStream <<  std::endl; 

	}
	void close()
	{
		m_inFileStream.flush();
		m_inFileStream.close();
	}


};
}
