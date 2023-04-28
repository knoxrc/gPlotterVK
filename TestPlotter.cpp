//#pragma once
#include "gPlotter.h"
#include <windows.h>
#include "BerkeleyLabSensorSpecific.h"

using namespace std;

//varnum reference: 0-Temp (C), 1-Humidity (%), 2-Light (LUX), 3-Volt (V)

void getLineColor(int nodeID, double& red, double& blue, double& green)
{
	//switch (nodeID)
	//{
	//case 1:

	//}
	if ((nodeID % 6) == 0)
	{














		red = 1;
		blue = 0;
		green = 0;
	}
	if ((nodeID % 6) == 1)
	{
		red = 0;
		blue = 1;
		green = 0;
	}
	if ((nodeID % 6) == 2)
	{
		red = 0;
		blue = 0;
		green = 1;
	}
	if ((nodeID % 6) == 3)
	{
		red = 1;
		blue = 0;
		green = 1;
	}
	if ((nodeID % 6) == 4)
	{
		red = 1;
		blue = 1;
		green = 0;
	}
	if ((nodeID % 6) == 5)
	{
		red = 0;
		blue = 1;
		green = 1;
	}
}
void testInputFromFile(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network)
{
	ifstream ifs;
	string basedir = "C:\\Users\\knoxr\\Desktop\\Dis V1\\";

	string filename = "data_sampled_6_Nodes.txt";

	string fullFilename = basedir;
	fullFilename += filename;

	ifs.open(fullFilename);
	if (ifs.is_open())
	{
		// skip first line if any (not needed here)
		if (test_network.loadNetworkDataFromFile(basedir,filename))
		{
			cout << endl << "Data Loaded. Summary:" << endl;
			test_network.printNetworkInfoToScreen();
		}
		else
			cout << "Loading from file failed" << endl;
	}
	ifs.close();

	cout << endl << "Done." << endl;
}
void testPlotting(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network)
{
	gPlotter<GridLabelsHour> BKplot;
	unsigned char max_nodeID, month, day, hour, min;
	unsigned short int year;
	unsigned int max_pos;
	BKLabSensorDataEntry testEntry;
	float val, sec;
	float sixty = 60.0;
	float twentyfour = 24.0;
	double sum_time, red, blue, green;
	const int nodeNo = 7;
	string valName, unit;
	int emptyNodes = 0;
	const int varnum = 2; //Data to be plotted

	unsigned short int sampleYear_start = 2004;
	unsigned char sampleMonth_start = 2;
	unsigned char sampleDay_start = 28;

	unsigned short int sampleYear_end = 2004;
	unsigned char sampleMonth_end = 3;
	unsigned char sampleDay_end = 2;
	unsigned char sampleHour_start = 0;
	unsigned char sampleHour_end = 0;


	max_nodeID = test_network.getNetworkSize()-1;

	test_network.getDataFromNetwork(0, 0, testEntry, false);

	testEntry.reading.getName(varnum, valName);
	testEntry.reading.getUnit(varnum, unit);

	string titleS = valName + " vs Time";
	string yLabelS = valName + " (" + unit + ")";
	

	wstring wideTitle = wstring(titleS.begin(), titleS.end());
	wstring wide_yLabel = wstring(yLabelS.begin(), yLabelS.end());

	bool success;

	BKplot.SetTitle(titleS);
	BKplot.SetXLabel("Time (H)");
	BKplot.SetYLabel(yLabelS);

	vector<double> y_val[nodeNo];
	vector<double> t_val[nodeNo]; //Dyn Alloc



	for (unsigned char in_nodeID = 0; in_nodeID <= 5; in_nodeID++)
	{
		if (test_network.returnDataListSize(in_nodeID) > 0)
		{
			cout << "NODE: " << (int)in_nodeID << " In set. Empty Nodes:" << emptyNodes << endl;
			for (unsigned int in_pos = 0; in_pos < test_network.returnDataListSize(in_nodeID); in_pos++)
			{
				test_network.getDataFromNetwork(in_nodeID, in_pos, testEntry, false);
				testEntry.dateNtime.date.getYear(year);
				testEntry.dateNtime.date.getMonth(month);
				testEntry.dateNtime.date.getDay(day);
				//testEntry.dateNtime.time.getHour(hour);

				if (year == sampleYear_start && month == sampleMonth_start && day >= sampleDay_start && day <= sampleDay_end)
				{
					testEntry.reading.getVal(varnum, val);
					testEntry.dateNtime.time.getHour(hour);
					testEntry.dateNtime.time.getMinute(min);
					sum_time = min / sixty;
					testEntry.dateNtime.time.getSecond(sec);
					sec = sec / sixty / sixty;
					day = day * twentyfour;
					sum_time = day + hour + sum_time + sec;
					y_val[in_nodeID - emptyNodes].push_back(val);
					t_val[in_nodeID - emptyNodes].push_back(sum_time);
				}
				else if ((year > sampleYear_end) || ((year == sampleYear_end) && (month > sampleMonth_end)) || ((year == sampleYear_end) && (month == sampleMonth_end) && (day > sampleDay_end)))
				{
					cout << "DAY: " << (int)day << ", NODE: " << (int)in_nodeID << endl;

					BKplot.AddData(in_nodeID - emptyNodes, &t_val[in_nodeID - emptyNodes], &y_val[in_nodeID - emptyNodes], "Node: " + to_string((int)in_nodeID));
					cout << "Adding Series." << endl;
					break;
					
				}
			}
		}
		if (test_network.returnDataListSize(in_nodeID) < 1)
		{
			emptyNodes++; 
			cout << "NODE: " << (int)in_nodeID << " Out of set. Empty Nodes:" << emptyNodes << endl;
		}
	}

	
	success = BKplot.DrawScatterPlotFromSettings();

	//Legend
	BKplot.DrawLegend(true);

	string legend;
	wstring wLegend;

	
	BKplot.PrintPlot(titleS);
	
	string PNGName = titleS + ".png";
	string filePath = "C:\\Users\\knoxr\\Desktop\\Dis V1\\";
	string filePathPNG = filePath + PNGName;

	wstring widefilePath = wstring(filePath.begin(), filePath.end());
	wstring wideFilePathPNG = wstring(filePathPNG.begin(), filePathPNG.end());
	ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);




	double corr12 = BKplot.computeCorrelation(0, 1);
	cout << "Correlation between nodes 1 and 2: " << corr12 << endl;
	double corr149 = BKplot.computeCorrelation(0, 2);
	cout << "Correlation between nodes 1 and 49: " << corr149 << endl;
	double corr249 = BKplot.computeCorrelation(2, 1);
	cout << "Correlation between nodes 2 and 49: " << corr249 << endl;
	double cov12 = BKplot.computeCovariance(0, 1);
	cout << "Covariance between nodes 1 and 2: " << cov12 << endl;
	double cov149 = BKplot.computeCovariance(1, 2);
	cout << "Covariance between nodes 1 and 49: " << cov149 << endl;
	double cov249 = BKplot.computeCovariance(2, 1);
	cout << "Covariance between nodes 2 and 49: " << cov249 << endl;
}




void Ktestplotting()
{
	// loads data from txt file:
	BKLabSensorNetwork test_network;
	testInputFromFile(test_network);

	//converts test_network to a BasicSensorNetwork
	BasicSensorNetwork basic_network;
	crossCopyNetworkSourceToDest<BKLabSensorNetwork, BasicSensorNetwork, BKLabSensorDataEntry, BasicSensorDataEntry>
		(test_network, basic_network);

	
	gPlotter<GridLabelsDay> BasicPlot;
	int starts[4] = { 2004, 2, 28, 0 };
	int ends[4] = { 2004, 3, 2, 0 };
	unsigned char nodestoplot[3] = { 1, 2, 50 };
	BasicPlot.PlotNetwork(basic_network, 2, 2, starts, ends, nodestoplot, 3);

	

}


void testInputOutputFromFile()
{
	CS_SensorNetworkData<BKLabSensorDataEntry> test_network;
	testInputFromFile(test_network);
	testPlotting(test_network);
}

int main()
{
	//testInputOutputFromFile();
	Ktestplotting();

	
}