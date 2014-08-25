#pragma once

#include "Utils.h"
#include "StopWatch.h"



class iProblemSolver
{
	//disallow usuall stuff
protected:
	Tour m_tourStats;
	unsigned long m_iterations;
	unsigned long m_noOfToursContructed;
	int m_populationSize;
	size_t m_noAnts;

	CStopWatch m_startTime;
	CStopWatch m_irrTime;
	CStopWatch m_tourTime;

	double fRand(double a, double b)
	{
		long _rnd = rand();
		if (_rnd == 0)
			_rnd++;
		return b * (double)_rnd / RAND_MAX;	
	}
	int getRandomNumber(int low, int high, int except){
		//Generate random value between low and high (inclusive), except
		int temp;
		bool done = false;
		while(!done){
			temp = (rand() % ((high + 1) - low)) + low;
			if(temp != except){
				done = true;
			}
		}
		return temp;
	}

public:
	virtual Tour getStats()=0;
	virtual void constructSolutions()=0;
	virtual void localSearch()=0;
	virtual void update()=0;
	virtual void initSystem()=0;
	const virtual std::vector<size_t> &getBestSoFarPath()=0;
	virtual double getBestPathLengthSofar()=0;
	
	virtual void incrementItteration() {
		m_iterations++;
		m_noOfToursContructed = m_noOfToursContructed + m_noAnts;
		m_tourStats.currentIrreation = m_iterations;
		m_tourStats.noOfToursContructed =  m_noOfToursContructed;
		m_tourStats.timeRunning = m_startTime.getRunningTime();
	}
	long int getTourNo() {
		return m_noOfToursContructed;
	}

	unsigned long getItteration() const{
		return m_iterations;
	}
	iProblemSolver(): m_iterations(1) , m_noOfToursContructed(0) {
		srand((unsigned int)time(NULL));
	}
	virtual ~iProblemSolver(void){
	}

};






