#pragma once
#include "CAntSystem.h"


class CElitistAntSystem :
	public CAntSystem
{
	int mWeight; 
public:
	virtual void update();
	CElitistAntSystem(Parameters& Par , MatrixArrayTypeInt *distanceMatrix);
	virtual ~CElitistAntSystem(void);
};

