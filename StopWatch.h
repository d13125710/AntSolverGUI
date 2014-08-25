#pragma once

#include <windows.h>


//taken from http://cplus.about.com/od/howtodothingsi2/a/timing.htm



class CStopWatch
{
	typedef struct {
		LARGE_INTEGER start;
		LARGE_INTEGER stop;
	} stopWatch;

	stopWatch timer;
	stopWatch maintimer;
	LARGE_INTEGER frequency;

public:


	double CStopWatch::LIToSecs( LARGE_INTEGER & L) {
		return ((double)L.QuadPart /(double)frequency.QuadPart) ;
	}

	CStopWatch::CStopWatch(){
		timer.start.QuadPart= maintimer.start.QuadPart = 0;
		timer.stop.QuadPart=maintimer.stop.QuadPart = 0; 
		QueryPerformanceFrequency( &frequency ) ;
	}

	void CStopWatch::startTimer( ) {
		timer.start.QuadPart= maintimer.start.QuadPart = 0;
		timer.stop.QuadPart=maintimer.stop.QuadPart = 0;
		QueryPerformanceCounter(&maintimer.start) ;
		QueryPerformanceCounter(&timer.start) ;
		
	}
	void CStopWatch::stopTimer( ) {
		QueryPerformanceCounter(&timer.stop) ;
		QueryPerformanceCounter(&maintimer.stop) ;
	}
	void CStopWatch::stopiTimer( ) {
		QueryPerformanceCounter(&timer.stop) ;
	}
	void CStopWatch::startiTimer( ) {
		timer.start.QuadPart= 0;
		timer.stop.QuadPart=0; 
		QueryPerformanceCounter(&timer.start) ;
	}
	double getRunningTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		LARGE_INTEGER time;
		time.QuadPart = li.QuadPart-timer.start.QuadPart;
		return LIToSecs( time) ;
	}
	


	double CStopWatch::getElapsedTime() {
		stopiTimer();
		LARGE_INTEGER time;
		time.QuadPart = timer.stop.QuadPart -  maintimer.start.QuadPart;
		startiTimer();
		return LIToSecs( time) ;
	}

};

