#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <stdio.h>

using namespace std;

class ObjParser {

public:
	vector<Point> v;
	vector<Vector> vn;
	vector<Triangle> triangles;


	void parse(string fileName) {

		cout << "parsing " << fileName << endl;

		int lineNum = 0;
		int count = 0;

		ifstream objFile(fileName);
		stringstream ss;
		string line;

		if (objFile.is_open()) {
			while (getline(objFile, line)) {
				
				lineNum++; count++;
				
				ss.clear();
				ss << line;


				string c;
				ss >> c;

				if (c != "v" && c != "vn" && c != "f") {
					ss.clear();
					continue;
				}

				if (count >= 500) {
					count = 0;
					cout << "line: " << lineNum << endl;
				}

				// -----------------------------------------
				if (c == "v") 
				{
					float coords[3];
					for (int i=0; i<3; i++) {
						ss >> coords[i];
					}
					v.push_back(Point(coords[0], coords[1], coords[2]));
				}

				// ----------------------------------------- 
				else if (c == "vn") 
				{
					float coords[3];
					for (int i=0; i<3; i++) {
						ss >> coords[i];
					}
					vn.push_back(Point(coords[0], coords[1], coords[2]));
				}

				// -----------------------------------------
				else if (c == "f") {
					vector<string> strs;

					string st;
					while (getline(ss, st, ' ')) {
						strs.push_back(st);
					}


					vector<string> nums(1);
					int pos = 0, charRead = 0;
					for (int i=0; i<strs.size(); i++) {
						for (auto it: strs[i]) {
							if (it == '/') {
								nums.push_back("");
								pos++;
								charRead = 0;
							}
							else if (it != ' ') {
								nums[pos] += it;
								charRead += 1;
							}
						}
						if (charRead > 0) {
							charRead = 0;
							pos++;
							nums.push_back("");
						}
					}

					vector<int> p;
					vector<int> n;

					for (int i=0; i<9; i++) {
						int a = atoi(nums[i].c_str());
						if ((i+1)%3 == 1) {
							// vertex
							p.push_back(a);
						}
						else if ((i+1)%3 == 0) {
							// normal
							n.push_back(a);
						}
					}


					// cout << "Triangle: " << endl;
					// for (int i=0; i<3; i++) {
					// 	cout << v[p[i]-1].x << "," << v[p[i]-1].y << "," << v[p[i]-1].z << endl;
					// }

					Triangle t({v[p[0]-1], v[p[1]-1], v[p[2]-1]});
					t.n0 = n[0]; t.n1 = n[1]; t.n2 = n[2];
					triangles.push_back(t);
				}

			}
			objFile.close();
		}


		cout << fileName << " parsed successfully. " << endl;
		cout << "vertices length: " << v.size() << endl;
		cout << "triangles size: " << triangles.size() << endl;
	}


	void simpleParse(string fileName) {
		ifstream objFile(fileName);
		stringstream ss;
		string line;

		if (objFile.is_open()) {
			while (getline(objFile, line)) {
				
				ss.clear();
				ss << line;

				char c;
				ss >> c;
				if (c == 'v') {
					float coords[3];
					for (int i=0; i<3; i++) {
						ss >> coords[i];
					}
					v.push_back(Point(coords[0], coords[1], coords[2]));
				}

				else {
					int a, b, c;
					ss >> a >> b >> c;
					a--; b--; c--; 
					triangles.push_back(Triangle(v[a], v[b], v[c]));
				}


			}
			objFile.close();
		}


		cout << fileName << " parsed successfully. " << endl;
		cout << "vertices length: " << v.size() << endl;
		cout << "triangles size: " << triangles.size() << endl;
	}

};
