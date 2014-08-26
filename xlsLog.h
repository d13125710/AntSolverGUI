//************************************
// written by Gareth Hunter DIT Kevin Street artefact's for dissertation 
// MSc advanced software engineering Aug 2014
// The artefacts are available under the GNU license for download and modification
// any questions please email me at gar_hunter@yahoo.com
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//************************************
#include "StdAfx.h"
#pragma once

#include "Xlsx/Workbook.h"

#include "Utils.h"
#include "MakeString.h"

using namespace std;
using namespace SimpleXlsx;


class xlsLog
{
	CWorkbook book;

	CWorksheet *sheet;

	

public:
	xlsLog(void);
	~xlsLog(void);


	void create()
	{
		CWorksheet &sheet = book.AddSheet(_T("New sheet simple"));

		vector<CellDataDbl> data;   // (data:style_index)
		CellDataDbl def;
		def.style_id = 0;       // 0,1 - default styles
		for (int i = 0; i < 10; i++) 
		{
			def.value = (double)(rand() % 100) / 101.0;
			data.push_back(def);
		}

		for (int i = 0; i < 10; i++)
				sheet.AddRow(data);

		_tstring filename = "C:\\results\\exp1.xls";
		//bool Save(const _tstring& name);
		book.Save(filename);

	}
	void test2()
	{
		{   // Creating data sheet with styled cells
			CWorksheet &sheet = book.AddSheet(_T("New sheet with styled cells"));

			int colNum = 20;
			int rowNum =20;

			Style style;
			style.fill.patternType = PATTERN_NONE;
			style.font.size = 14;
			style.font.theme = true;
			style.font.attributes = FONT_BOLD;
			style.horizAlign = ALIGN_H_RIGHT;
			style.vertAlign = ALIGN_V_CENTER;
			int style_index_1 = book.m_styleList.Add(style);

			style.fill.patternType = PATTERN_NONE;
			style.font.size = 14;
			style.font.theme = true;
			style.font.attributes = FONT_ITALIC;
			style.horizAlign = ALIGN_H_LEFT;
			style.vertAlign = ALIGN_V_CENTER;
			size_t style_index_2 = book.m_styleList.Add(style);

			vector<CellDataFlt> data;
			CellDataFlt def;
			for (size_t i = 0; i < colNum; i++) {
				def.value = (double)(rand() % 100) / 101.0;
				if (i % 2 == 0) def.style_id = style_index_1;
				else        def.style_id = style_index_2;

				data.push_back(def);
			}

			for (int i = 0; i < rowNum; i++)
				sheet.AddRow(data, 5);
		}
		_tstring filename = "C:\\results\\exp1.xls";
		//bool Save(const _tstring& name);
		book.Save(filename);
	}


	void Add()
	{
		sheet = &book.AddSheet(_T("Data"));
	}
	void chartsaddTitle()
	{
		vector<CellDataStr> data;
		CellDataStr cellDbl;
		cellDbl.style_id = 0;
		cellDbl.value = "Alg";
		data.push_back(cellDbl);
		cellDbl.value="itt";
		cellDbl.style_id = 0;
		data.push_back(cellDbl);
		cellDbl.value ="distance";
		data.push_back(cellDbl);
		sheet->AddRow(data);
	}
	void experiment2Title(int itter)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = "Algorathm";
		def.style_id =0;
		sheet->AddCell(def);

		def.value ="FileName";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i<itter;i++)
		{
			//std dev
			//branching factor
			//distance
			def.value = "std dev";
			def.style_id =0;
			sheet->AddCell(def);
			def.value = "bf";
			def.style_id =0;
			sheet->AddCell(def);
			def.value = "best distance";
			def.style_id =0;
			sheet->AddCell(def);
			def.value = "worst distance";
			def.style_id =0;
			sheet->AddCell(def);
			def.value = "Time";
			def.style_id =0;
			sheet->AddCell(def);

		}
		
		
		
		

		sheet->EndRow();
	}


	void ShowPar(Parameters &p)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = "Parameters";
		def.style_id =0;
		sheet->AddCell(def);


			std::string s = make_string()  << "rho =" << p.rho << ",iterations =" << p.Epochs<< ",lambda =" << p.lambda 
			<< ",local search LK" << p.LocalSearchGreed << ",local search Opt2 =" << p.LocalSearchOpt2  << ",local search Opt3 =" 
			<< p.LocalSearchOpt3 << ",stagnations =" << p.maxStagnationIterations << ",ants/population =" << p.noAnts 
			<< ",Elite Weight =" << p.EliteWeight << ",mutation =" << p.mutationRatio 
			<< ",q0 =" << p.q0 <<  ",xi =" << p.xi << ",r0 =" << p.r0 ;
			
		//	CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);
	
		sheet->EndRow();

	}

	void chartsaddvalue(std::string Alg, int itter , int distance)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = Alg.c_str();
		def.style_id =0;
		sheet->AddCell(def);
		CellDataDbl cellDbl;
	   	cellDbl.value =itter;
		cellDbl.style_id = 0;
		sheet->AddCell(cellDbl);
		cellDbl.value =distance;
		sheet->AddCell(cellDbl);
		sheet->EndRow();
    }

	void AddResults(int distance)
	{
		CellDataDbl cellDbl;
		cellDbl.value =distance;
		cellDbl.style_id = 0;
		sheet->AddCell(cellDbl);
	}
	void AddResults(double distance)
	{
		CellDataDbl cellDbl;
		cellDbl.value =distance;
		cellDbl.style_id = 0;
		sheet->AddCell(cellDbl);
	}
	void AddResults(std::string& distance)
	{
		CellDataStr  def; 
		def.value = distance.c_str();
		def.style_id = 0;
		sheet->AddCell(def);
	}
	void AddResults(char *distance)
	{
		CellDataStr  def; 
		def.value = distance;
		def.style_id = 0;
		sheet->AddCell(def);
	}
	void EndRow()
	{
		sheet->EndRow();
	}
	void startRow(Parameters &p)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		std::string s = make_string() << p.getAlgType() << " , Problem = " << p.fileName;
		def.value = s.c_str();
		def.style_id =0;
		sheet->AddCell(def);
	}

	void startRow2(Parameters &p)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		std::string s = make_string() << p.getAlgType() ;
		def.value = s.c_str();
		def.style_id =0;
		sheet->AddCell(def);

		// Problem = " << p.fileName;
		def.value = p.fileName.c_str();
		def.style_id =0;
		sheet->AddCell(def);

//		def.value ="found times";
//		def.style_id =0;
//		sheet->AddCell(def);


	}


	void insertBlankRow()	{
		sheet->BeginRow();
		sheet->EndRow();
	}
	void insertAvgBestHeader(std::vector<int> &alg){
		Parameters p;
		sheet->BeginRow();
		AddResults("Problem");
		for (int i=0;i<alg.size();i++)
		{
			p.ALG = alg[i];
			CellDataStr cellDbl;
			cellDbl.style_id = 0;
			cellDbl.value =p.getAlgType();
			sheet->AddCell(cellDbl);
			AddResults("Mean");
			AddResults("itter found");
			AddResults("Time taken");
			//AddResults("std");

		}
		sheet->EndRow();
	}


	void insertParametersHeader(Parameters &p){
		std::map<std::string, std::string> pmap=p.SplitIntoMap();
		std::map<std::string, std::string> ::iterator it;
		sheet->BeginRow();
		for (it = pmap.begin(); it != pmap.end(); ++it)
		{
			CellDataStr cellDbl;
			cellDbl.style_id = 0;
			cellDbl.value = it->first;
			sheet->AddCell(cellDbl);
		
		}
		sheet->EndRow();
		sheet->BeginRow();
		for (it = pmap.begin(); it != pmap.end(); ++it)
		{
			CellDataStr cellDbl;
			cellDbl.style_id = 0;
			cellDbl.value = it->second;
			sheet->AddCell(cellDbl);
		}
		sheet->EndRow();
	}
	void insertComment(char *pTitle , double info)
	{
		this->insertBlankRow();
		this->insertBlankRow();
		this->sheet->BeginRow();
		this->AddResults(pTitle);
		this->AddResults(info);
		sheet->EndRow();
	}
	void insertTitleLine(std::string &p){
		sheet->BeginRow();
		CellDataStr cellDbl;
		cellDbl.style_id = 0;
		cellDbl.value = p;
		sheet->AddCell(cellDbl);
		sheet->EndRow();
	}
	void insertParameters(Parameters &p){
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = "Parameters";
		def.style_id =0;
		sheet->AddCell(def);


		std::string s = make_string()  << "rho =" << p.rho << ",iterations =" << p.Epochs<< ",lambda =" << p.lambda 
			<< ",local search LK" << p.LocalSearchGreed << ",local search Opt2 =" << p.LocalSearchOpt2  << ",local search Opt3 =" 
			<< p.LocalSearchOpt3 << ",stagnations =" << p.maxStagnationIterations << ",ants/population =" << p.noAnts 
			<< ",Elite Weight =" << p.EliteWeight << ",mutation =" << p.mutationRatio 
			<< ",q0 =" << p.q0 <<  ",xi =" << p.xi << ",r0 =" << p.r0 ;

		//	CellDataStr  def; 
		def.value = s.c_str();
		def.style_id =0;
		sheet->AddCell(def);

		sheet->EndRow();


		sheet->EndRow();
	}

	void startRow(string &p , string &s2)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = p.c_str();
		def.style_id =0;
		sheet->AddCell(def);
		def.value = s2.c_str();
		def.style_id =0;
		sheet->AddCell(def);
	}
	void startRow(string &p)
	{
		sheet->BeginRow();
		CellDataStr  def; 
		def.value = p.c_str();
		def.style_id =0;
		sheet->AddCell(def);
	}

	void AddParameters(std::vector<Parameters> &par_array)
	{
		sheet->BeginRow();

		AddResults("");
		AddResults("");

		CellDataStr  def; 
		def.value = "[Alpha,Beta]";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i< par_array.size();i++)
		{
			int Alpha = par_array[i].Alpha;
			int Beta = par_array[i].Beta;
			std::string s = make_string() << "[ " << Alpha << "," << Beta <<"]";
			CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);
		}
			sheet->EndRow();
	}

	void AddEvapParameters(std::vector<Parameters> &par_array)
	{
		sheet->BeginRow();

		AddResults("");
		CellDataStr  def; 
		def.value = "Q";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i< par_array.size();i++)
		{
			double rho = par_array[i].rho;
		
			std::string s = make_string() <<rho ;
			CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);

			def.value = "found";
			def.style_id =0;
			sheet->AddCell(def);

		}
		sheet->EndRow();
	}

	void AddACOParameters(std::vector<Parameters> &par_array)
	{
		sheet->BeginRow();

		CellDataStr  def; 
		def.value = " ";
		def.style_id =0;
		sheet->AddCell(def);

		//CellDataStr  def; 
		def.value = "[xi,q0]";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i< par_array.size();i++)
		{
			double Alpha = par_array[i].xi;
			double Beta = par_array[i].q0;
			std::string s = make_string() << "[ " << Alpha << "," << Beta <<"]";
			CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);
		}
		sheet->EndRow();
	}

	void AddEliteParameters(std::vector<Parameters> &par_array)
	{
		sheet->BeginRow();

		CellDataStr  def; 
		def.value = " ";
		def.style_id =0;
		sheet->AddCell(def);

		//CellDataStr  def; 
		def.value = "elite values";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i< par_array.size();i++)
		{
			double EliteWeight = par_array[i].EliteWeight;
			std::string s = make_string() << EliteWeight;
			CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);
		}
		sheet->EndRow();
	}

	void AddNoOfAntsParameters(std::vector<Parameters> &par_array)
	{
		sheet->BeginRow();

		CellDataStr  def; 
		def.value = " ";
		def.style_id =0;
		sheet->AddCell(def);

		//CellDataStr  def; 
		def.value = "Ant values";
		def.style_id =0;
		sheet->AddCell(def);

		for(int i=0;i< par_array.size();i++)
		{
			double noAnts = par_array[i].noAnts;
			std::string s ;
			if(noAnts ==0 )
				s= make_string() << "n";
			else
				s= make_string() << noAnts;
			CellDataStr  def; 
			def.value = s.c_str();
			def.style_id =0;
			sheet->AddCell(def);

			def.value = "found";
			def.style_id =0;
			sheet->AddCell(def);
		}
		sheet->EndRow();
	}


	void chartsaddRow(int itter , int distance)
	{
		
	}

 /*
    // create series object, that contains most chart settings
    CChartsheet::Series ser;

    // adding chart to the workbook the reference to a newly created object is returned
    CChartsheet &line_chart = book.AddChart(_T("Line Chart"), CHART_LINEAR);

    // leave category sequence (X axis) not specified (optional) - MS Excel will generate the default sequence automatically
    ser.catSheet =  NULL;

    // specify range for values` sequence (Y axis)
    ser.valAxisFrom = CellCoord(0,0);
    ser.valAxisTo = CellCoord(0, 10);
    ser.valSheet =  &sheet; // don`t forget to set the pointer to the data sheet

    ser.title = _T("Line series test");
    ser.isSmoothed = true;  // determines whether series will be a smoothed or straight-lined curve
    ser.isMarked = false;   // if true add diamond marks in each node of the sequence set

    // add series into the chart (you can add as many series as you wish into the same chart)
    line_chart.AddSeries(ser);

    // adding chart to the workbook the reference to a newly created object is returned
    CChartsheet &bar_chart = book.AddChart(_T("Bar Chart"), CHART_BAR);

    // leave category sequence (X axis) not specified (optional) - MS Excel will generate the default sequence automatically
    ser.catSheet =  NULL;

    // specify range for values` sequence (Y axis)
    ser.valAxisFrom = CellCoord(0,0);
    ser.valAxisTo = CellCoord(0, 10);
    ser.valSheet =  &sheet; // don`t forget to set the pointer to the data sheet
    ser.title = _T("Bar series test");

    // optionally it is possible to set some additional parameters for bar chart
    bar_chart.SetBarDirection(CChartsheet::BAR_DIR_HORIZONTAL);
    bar_chart.SetBarGrouping(CChartsheet::BAR_GROUP_CLUSTERED);

    bar_chart.SetTableDataState(CChartsheet::TBL_DATA);

    // add series into the chart (you can add as many series as you wish into the same chart)
    bar_chart.AddSeries(ser);

    // adding chart to the workbook the reference to a newly created object is returned
    CChartsheet &scatter_chart = book.AddChart(_T("Scatter Chart"), CHART_SCATTER);
    // for scatter charts it is obligatory to specify both category (X axis) and values (Y axis) sequences
    ser.catAxisFrom = CellCoord(2,1);
    ser.catAxisTo = CellCoord(2, 11);
    ser.catSheet =  &sheet;

    ser.valAxisFrom = CellCoord(0,0);
    ser.valAxisTo = CellCoord(0, 10);
    ser.valSheet =  &sheet;

    // optional parameters
    ser.title = _T("Scatter series test");
    ser.isSmoothed = true;  // determines whether series will be a smoothed or straight-lined curve
    ser.isMarked = true;    // if true add diamond marks in each node of the sequence set
    scatter_chart.SetScatterStyle(CChartsheet::SCATTER_FILL);

    // add series into the chart (you can add as many series as you wish into the same chart)
    scatter_chart.AddSeries(ser);

    // optional parameters to set
    scatter_chart.SetLegendPos(CChartsheet::POS_RIGHT);
    scatter_chart.SetXAxisGrid(CChartsheet::GRID_MAJOR_N_MINOR);
    scatter_chart.SetYAxisGrid(CChartsheet::GRID_MAJOR_N_MINOR);
    //scatter_chart.SetTableDataState(CChartsheet::TBL_DATA); - it is not possible to add table into scatter-contained chart
    // due to specificy of the data
    scatter_chart.SetDiagrammName(_T("Scatter curves` chart"));

	*/
    // at the end save created workbook wherever you need
	void save(std::string filename="MyBook.xlsx")
	{
		bool bRes = book.Save(_T(filename.c_str()));
	  //  if (bRes) cout << "Book saved successfully";
	  //  else cout << "Error at book saving. Operation failed";

	}


	
	void chartPlots()
	{
		CWorksheet &sheet = book.AddSheet(_T("Data1"));

		vector<CellDataDbl> data;
		CellDataDbl cellDbl;
		cellDbl.style_id = 0;

	}


};

