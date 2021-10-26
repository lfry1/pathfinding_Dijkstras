/* File Name: generate_map.cpp
 * Authors: Mikaela Atkinson and Lousie Fry
 * Date: 24 October 2021
 * Description: This program generates an NxN  map of random
 * 	tiles given N from input.
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]){
	
	stringstream ss;
	ofstream dataFile;
	string fileName, fn, filePath;
	map<int, char> tiles { {1, 'g'}, {2, 'G'}, {3, 'f'}, {4, 'h'}, {5, 'r'}, {7, 'm'} };
	map<int, char>::iterator it;
//	int n[] = {10, 20, 50, 100, 200, 500, 1000};
	int n, i, j, value, fileNumber = 3;
	char letter;
//	enum Tiles { g=1, G, f, h, r, m = 7 };

	while( cin >> n ){
//	for( int k; k < 7; ++k){
		vector<vector<int>> myMap(n, vector<int> (n));
		srand(time(0));
		for( i = 0; i < n; ++i ){
			for( j = 0; j < n; ++j ){
				// generate random value
				//myMap[i][j] = (Tiles).Random.Range(1, System.Enum.GetValues(typeof(Tiles)).Length);
				value = ( rand() %7 + 1 );
				if( value == 6 ){
					while( value == 6 ){
						value = ( rand() %7 + 1 );
					}
				}
			myMap[i][j] = value;
			}
		}
		// Print map to data directory
		// use switch function to print letters instead of numbers
		//https://stackoverflow.com/questions/3168306/print-text-instead-of-value-from-c-enum
		fileNumber++;
		ss.str("");
		ss << fileNumber;
		fn = ss.str();
		fileName = "input" + fn + ".txt"; 
		filePath = fileName;
		dataFile.open(filePath);
		dataFile << tiles.size() << endl;
		for( it = tiles.begin(); it != tiles.end(); ++it ){
			dataFile << it->second << " " << it->first << endl;
		}
		dataFile << n << " " << n << endl;
		for( i = 0; i < n; ++i ){
			for( j = 0; j < n; ++j ){
				value = myMap[i][j];
//				letter = m[value];
				it = tiles.find(value);
//				letter = it->second;
				if( (j == n-1 ) ){
					dataFile << it->second << endl;
				}
				else{
					dataFile << it->second << " ";
				}
			}
		}
		dataFile << "0 0" << endl;
		dataFile << n-1 << " " << n-1 << endl;
		dataFile.close();
	}
return 0;
}
