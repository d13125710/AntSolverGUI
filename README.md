AntSolverGUI
============

Version 1

AntSolver is implemented in C++ and the main algorithm classes have been implemented using ISO standards, therefore enabling these classes to be imported to any other C++ application on any operating system. 
Although Visual C++ 10 professional with service Pack 2 installed was used to compile this version. 

To compile download to desktop
and open AntSolver.vcxproj in visual C++ 10

1.	The main algorithm classes, the entire logic of all the ant algorithms, and genetic algorithm have been encapsulated into these classes. All are accessed via an interface and created via a factory;
2.	A Local search module with the methods of local search is discussed and can be applied to each solution of each algorithm (except the brute force algorithm); 
3.	The Problem module implements the Travelling Salesperson Problem to demonstrate the flexibility of these algorithms; 
4.	A GUI interface module was created to visualise the algorithms and a MS Windows 64bit and 32 bit GUI interface to the algorithms classes was built with MFC compiled using Visual C++ 10;

In compiling AntSolver the following code was used to provide access to MS Excel SimpleXlsxWriter 
A Thomas Stützle/Lin Kernighan local search was also added. 

1)The main algorithm classes 
Implemetation of the best worst Ant system
BestWorstAntSystem.cpp ,BestWorstAntSystem.h

Implemetation of the brute force system
BruteForceSystem.h

Implemetation of the best worst Ant system
CAntColonySystem.cpp , CAntColonySystem.h

Implemetation of the Ant system
CAntSystem.cpp,CAntSystem.h

Implemetation of the Elitist Ant system
CElitistAntSystem.cpp , CElitistAntSystem.h
Implemetation of the MinMax Ant system
CMinMaxAntSystem.cpp , CMinMaxAntSystem.h
Implemetation of the Random Ant system
RandomTest.h

Implemetation of the Mutated  Ant system
MutatedAnt1.cpp , MutatedAnt1.h

Implemetation of the pheromone matrix
PheroMatrix.h

Implemetation of Genetic Algorithm
CChromo.cpp the chromosome class 
CChromo.h
CGeneticSystem.cpp
CGeneticSystem.h

Interface Clasees
ProblemSolver.h
observer.h

Others for parsing strings
Log.h write a log file
MakeString.h string functions
TokenFinder.h string parsing
Utils.h  Tour and parameters classes
StopWatch.h for timing
xlsLog.cpp  Excel log file writer
xlsLog.h

Factory
TSPalgorithm.cpp
TSPalgorithm.h
TspsolverFactory.h


2) LocalSearch classes
CLocalSearch.h

3) TSP problem FileReader
CTSPLIBFileReader.cpp
CTSPLIBFileReader.h
DistanceHelper.h


4) MFC GUI
DExperiments.cpp
DExperiments.h
GA.cpp
GA.h
GADlg.cpp
GADlg.h
Resource.h


Automate experiments
Experiments.cpp
Experiments.h
XlsxHeaders.cpp

Others
SimpleXlsxWriter
Chartsheet.cpp
Chartsheet.h
SimpleXlsxDef.h
Workbook.cpp
Worksheet.cpp
zip.cpp
zip.h


