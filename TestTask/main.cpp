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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Struct to store all date from file
struct StockData {
	int interval;
	int volume;
	double high;
	double low;

	StockData() { ; }

	StockData(int interval, int volume, double high, double low) {
		this->interval = interval;
		this->volume = volume;
		this->high = high;
		this->low = low;
	}
};

vector<pair<string, StockData>> getDataFromFile(string fileName);
unordered_map<string, int> getTotalPerStock(vector<pair<string, StockData>> stocks);
unordered_map<string, pair<double, double>> getHighLow(vector<pair<string, StockData>> stocks);
void print(vector<pair<string, StockData>> stocks);

int main() {
	vector<pair<string, StockData>> stocks = getDataFromFile("TEST.txt");
	print(stocks);

	return 0;
}

// Function to get data from file and put into vector of pairs, key - stock name, value - struct StockData
vector<pair<string, StockData>> getDataFromFile(string fileName) {
	ifstream inputFile(fileName);
	vector<pair<string, StockData>> stocks;

	string line;
	//Get string data from file 
	while (getline(inputFile, line)) {
		istringstream iss(line);
		string stock, intervalStr, volumeStr, highStr, lowStr;
		if (!(iss >> stock >> intervalStr >> volumeStr >> highStr >> lowStr)) {
			cerr << "Invalid input format: " << line << endl;
			continue;
		}

		int interval = stoi(intervalStr);
		int volume = stoi(volumeStr);
		double high = stod(highStr);
		double low = stod(lowStr);

		pair<string, StockData> stockInfo;
		stockInfo.first = stock;
		StockData stockData = StockData(interval, volume, high, low);
		stockInfo.second = stockData;

		stocks.push_back(stockInfo);
	}
	return stocks;
}

// Function to get total volume per Stock
unordered_map<string, int> getTotalPerStock(vector<pair<string, StockData>> stocks) {
	unordered_map<string, int> totalPerStock;

	for (int i = 0; i < stocks.size(); i++) {
		if (!totalPerStock.count(stocks[i].first)) {
			totalPerStock[stocks[i].first] = stocks[i].second.volume;
		}
		else {
			totalPerStock[stocks[i].first] += stocks[i].second.volume;
		}

	}
	return totalPerStock;
}

// Function to calculate maximum High and minimum Low for each Stock,
unordered_map<string, pair<double, double>> getHighLow(vector<pair<string, StockData>> stocks) {
	unordered_map<string, pair<double, double>> HLperStock;

	unordered_map<string, int> totalPerStock = getTotalPerStock(stocks);

	for (const auto& pair_ : totalPerStock) {
		double high = 0;
		double low = 0;
		for (int i = 0; i < stocks.size(); i++) {
			if (stocks[i].first == pair_.first) {
				if (stocks[i].second.high > high || !high) high = stocks[i].second.high;
				if (stocks[i].second.low < low || !low) low = stocks[i].second.low;
			}
		}
		pair<double, double> hl;
		hl.first = high;
		hl.second = low;
		high = 0;
		low = 0;
		HLperStock[pair_.first] = hl;
	}
	return HLperStock;
}

// Function to print answer in a given format
void print(vector<pair<string, StockData>> stocks) {
	unordered_map<string, int> totalPerStock = getTotalPerStock(stocks);

	for (int i = 0; i < stocks.size(); i++) {
		cout << stocks[i].first << "," << stocks[i].second.interval << "," << double(stocks[i].second.volume) / totalPerStock[stocks[i].first] * 100 << endl;
	}

	cout << "#" << std::endl;

	unordered_map<string, pair<double, double>> HLperStock = getHighLow(stocks);
	for (const auto& pair : HLperStock) {
		cout << pair.first << "," << pair.second.first << "," << pair.second.second << endl;
	}
}