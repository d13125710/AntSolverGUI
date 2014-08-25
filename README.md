AntSolverGUI
============

Version 1

AntSolver is implemented in C++ and the main algorithm classes have been implemented using ISO standards, therefore enabling these classes to be imported to any other C++ application on any operating system. 
1.	The main algorithm classes, the entire logic of all the ant algorithms, and genetic algorithm have been encapsulated into these classes. All are accessed via an interface and created via a factory;
2.	A Local search module with the methods of local search is discussed and can be applied to each solution of each algorithm (except the brute force algorithm); 
3.	The Problem module implements the Travelling Salesperson Problem to demonstrate the flexibility of these algorithms; 
4.	A GUI interface module was created to visualise the algorithms and a MS Windows 64bit and 32 bit GUI interface to the algorithms classes was built with MFC compiled using Visual C++ 10;


Ant Algorithms Classes 
Implemetation of the best worst Ant system
BestWorstAntSystem.cpp 
BestWorstAntSystem.h
Implemetation of the brute force system
BruteForceSystem.h
Implemetation of the best worst Ant system
CAntColonySystem.cpp
CAntColonySystem.h
Implemetation of the best worst Ant system
CAntSystem.cpp
CAntSystem.h
Implemetation of the best worst Ant system
CElitistAntSystem.cpp
CElitistAntSystem.h
Implemetation of the best worst Ant system
CMinMaxAntSystem.cpp
CMinMaxAntSystem.h
Implemetation of the best worst Ant system

Implemetation of the best worst Ant system
RandomTest.h
Implemetation of the best worst Ant system
MutatedAnt1.cpp
MutatedAnt1.h

PheroMatrix.h

ProblemSolver.h
observer.h

Genetic Algorithm
CChromo.cpp
CChromo.h
CGeneticSystem.cpp
CGeneticSystem.h

LocalSearch
CLocalSearch.h

TSP FileReader
CTSPLIBFileReader.cpp
CTSPLIBFileReader.h
DistanceHelper.h


GUI
DExperiments.cpp
DExperiments.h
GA.cpp
GA.h
GADlg.cpp
GADlg.h
Resource.h


Experiments.cpp
Experiments.h
xlsLog.cpp
xlsLog.h
XlsxHeaders.cpp

Timing
StopWatch.h

Others for Experiments
Log.h
MakeString.h
TokenFinder.h
Utils.h

Factory
TSPalgorithm.cpp
TSPalgorithm.h
TspsolverFactory.h



Chartsheet.cpp
Chartsheet.h
SimpleXlsxDef.h
Workbook.cpp
Worksheet.cpp
zip.cpp
zip.h


