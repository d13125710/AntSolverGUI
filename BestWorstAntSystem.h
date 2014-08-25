#pragma once
#include "cantsystem.h"
class CBestWorstAntSystem :
	public CAntSystem
{

	double  trail_0;
	int restart_iteration;
	DWORD restart_time;
	DWORD starttime;

	void BWUpdate(const CAnt &bestAnt, const CAnt &worstAnt);
	void Mutation();

public:

	CBestWorstAntSystem(Parameters &Par, MatrixArrayTypeInt *distanceMatrix);
	virtual ~CBestWorstAntSystem(void);
	virtual void updatePheromes();
	void initPheromoneTrails(double initialValue) const;
	virtual void initPheromones();

	virtual void initSystem(){
		initPheromones();
		calculateHeuristicMatrix();
	}
	virtual void update(){
		updatePheromes();
	}

};

