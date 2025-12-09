#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> readInput(const string& filename) {
    vector<vector<int>> coordinates;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        vector<int> coord;
        stringstream ss(line);
        string num;
        
        while (getline(ss, num, ',')) {
            coord.push_back(stoi(num));
        }
        coordinates.push_back(coord);
    }
    
    file.close();
    return coordinates;
}

long long solve(vector<vector<int>> &coordinates) {
  int n = coordinates.size();
  long long maxArea = 0;
  for (int i = 0; i < n; i++) {
    long long x1 = coordinates[i][0], y1 = coordinates[i][1];
    for (int j = i + 1; j < n; j++) {
      long long x2 = coordinates[j][0], y2 = coordinates[j][1];

      long long currArea = (abs(x2-x1) + 1) * (abs(y2 - y1) + 1);
      maxArea = max(currArea, maxArea);
    }
  }

  return maxArea;
}

int main() {
  vector<vector<int>> input = readInput("Day 9 Input.txt");
  cout << input.size() << " " << input[0].size() << endl;
  cout << solve(input) << endl;
}