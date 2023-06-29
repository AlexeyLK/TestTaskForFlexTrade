#include <cstdio>
#include <cstring>
#include <map>
#include <iostream>
#include <string>

/**
VWAPer v0.1

// Read file in with the following format:
// [Stock],[Interval],[Volume Traded],[High],[Low]

// Calculate the total volume traded per Stock
// Calculate the total volume traded per Stock&Interval

// Write the total volume traded per Stock&Interval as a percentage of the total volume traded per Stock to stdout:
// [Stock],[Interval],[%Volume Traded]

// Write the delimiter '#' to stdout

// Write the maximum High and minimum Low for each Stock to stdout:
// [Stock],[Day High],[Day Low]

// example input:
VOD.L 1 100 184 183.7
BT.LN 1 300 449.4 448.2
VOD.L 2 25 184.1 182.4
BT.LN 2 900 449.8 449.5

// example output:
VOD.L,1,80
BT.LN,1,25
VOD.L,2,20
BT.LN,2,75
#
VOD.L,184.1,182.4
BT.LN,449.8,448.2

**/

#define DELIMITER "#"

using namespace std;

typedef basic_string<char> string;

class	CUpperLower
{
public:
	CUpperLower() : nCurLwr(0), nCurUpr(0) {};

	void	add(int nHigh, int nLow)
	{
		if (nHigh > nCurUpr)
			nCurUpr = nHigh;

		if (nLow < nCurLwr)
			nCurLwr = nLow;
	}

	int& getSum()
	{
		int sum = nCurLwr + nCurUpr;
		return sum;
	}

	int         nCurLwr;
	int         nCurUpr;
};

int main(int argc, char* argv[])
{
	if (!strcmp("version", argv[1]))
	{
		cerr << "VWAPer version 0.1" << endl;
		return 0;
	}

	FILE* file = fopen(argv[2], "r");

	cerr << "Reading file" << argv[2] << endl;

	char	line[256];
	char	Stocks[1000][10];
	int         Intervals[1000];
	int         Volumes[1000];
	float	Highs[1000];
	float	Lows[1000];

	int         i = 0;
	int         sum = 0;

	while (fgets(line, 256, file))
	{
		sscanf(line, "%s %d %d %f %f",
			Stocks[i], &Intervals[i],
			&Volumes[i], &Highs[i], &Lows[i++]);
	}

	cerr << "Calculating total volumes" << endl;

	map<std::string, int>		TotalVolumes;

	for (int s = 0; s <= i; ++s)
	{
		std::string	stockname = Stocks[s];

		for (int j = 0; j <= i; ++j)
		{
			if (!strcmp(Stocks[j], stockname.c_str()))
			{
				TotalVolumes[stockname] += Volumes[j];
			}
		}
	}

	cerr << "Calculating high lows" << endl;

	map<std::string, CUpperLower>	HighLows;

	for (int s = 0; s <= i; ++s)
	{
		HighLows[Stocks[s]].add(Highs[s], Lows[s]);
		cout << HighLows[Stocks[s]].getSum();
	}

	cerr << "Writing files" << endl;

	for (int s = 0; s <= i; ++s)
	{
		cout << Stocks[s] << "," << Intervals[s] << "," <<
			TotalVolumes[Stocks[s]] / Volumes[s] * 100 << endl;
	}

	cout << DELIMITER << endl;

	map<std::string, CUpperLower>::iterator itr = HighLows.begin();
	while (itr != HighLows.end())
	{
		cout << (*itr).first << "," << (*itr).second.nCurLwr << "," <<
			(*itr).second.nCurUpr << endl;

		++itr;
	}

	return 1;
}

/*
 - If use "using namespace std;", so no need to use std::, for ex line 102.
 - I think beter to use vector instead of Ñ style massive like Stocks, Intervals, Volumes, Highs, Lows.
 - Using std::ifstream instead of FILE*: Instead of using FILE* and fopen, you can use std::ifstream for file input operations. 
   It provides a more modern and safer way of reading files in C++.
 - Use std::fixed and std::setprecision for floating-point output: When printing floating-point values, 
   you can use std::fixed and std::setprecision from <iomanip> to control the precision of the output.
 - Improve efficiency by using unordered_map: Instead of using std::map for TotalVolumes and HighLows.
 - Handle division by zero
 - Properly handle end iterator in the loop: The loop while (itr != HighLows.end()) 
   can be replaced with a range-based for loop over HighLows to avoid explicitly handling the end iterator.
 - The code looks untidy due to bad indentation, it is better to stick to a single style.
 */