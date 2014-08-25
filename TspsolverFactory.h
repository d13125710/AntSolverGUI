#pragma once

#include "BestWorstAntSystem.h"
#include "CAntColonySystem.h"
#include "CAntSystem.h"
#include "CElitistAntSystem.h"
#include "RandomTest.h"
#include "CMinMaxAntSystem.h"
#include "CGeneticSystem.h"

#include "BruteForceSystem.h"
#include "MutatedAnt1.h"

class TSPSolverFactory
{
public:
	static iProblemSolver *create(Parameters &par , MatrixArrayTypeInt *matrix_)
	{
		iProblemSolver *pSolver =0;
		switch (par.ALG)
		{
				case (MMAS) :
					pSolver = new CMinMaxAntSystem(par , matrix_);
					break;
				case (ACS) :
					pSolver = new CAntColonySystem(par , matrix_);
					break;
				case (AS) :
					pSolver= new CAntSystem(par , matrix_);
					break;
				case (GS) :
					pSolver = new CGeneticSystem(par , matrix_);
					break;
				case (BWAS) :
					pSolver= new CBestWorstAntSystem(par , matrix_);
					break;
				case (MA) :
					pSolver = new CMutatedAnt(par , matrix_);
					break;
				case (EAS) :
					pSolver = new CElitistAntSystem(par , matrix_);
					break;
				case (BF) :
					pSolver = new BruteForceSystem(matrix_);
					break;
				case (RA) :
					pSolver = new CRandomAnt(par , matrix_);
					break;
				default:
					break;
		}
		return pSolver;
	}

};


