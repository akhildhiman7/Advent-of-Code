#include<bits/stdc++.h>
using namespace std;
using pdpii = pair<double, pair<int, int>>;
using pipii = pair<int, pair<int, int>>;

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

pipii getPair(vector<int> &coord) {
  return {coord[0], {coord[1], coord[2]}};
}

double getDist(int a, int b, int c, int p, int q, int r) {
  long long dx = p - a;
  long long dy = q - b;
  long long dz = r - c;
  return dx*dx + dy*dy + dz*dz;
}

vector<int> parents;
vector<int> ranks;

void initUnionFind(int n=1e3) {
  parents.resize(n);
  ranks.resize(n, 0);

  for (int i = 0; i < n; i++) {
    parents[i] = i;
  }
}

int findParent(int node) {
  if (node == parents[node]) return node;
  return parents[node] = findParent(parents[node]);
}

bool unionNodes(int n1, int n2) {
  int p1 = findParent(n1);
  int p2 = findParent(n2);

  if (p1 == p2) return false;

  if (ranks[p1] < ranks[p2]) swap(p1, p2);
  else if (ranks[p1] == ranks[p2]) ranks[p1]++;
  parents[p2] = p1;
  return true;
}

int playground(vector<vector<int>> &coordinates, int k) {
  vector<pair<double, pair<int, int>>> all;

  for (int i = 0; i < coordinates.size(); i++) {
    for (int j = i + 1; j < coordinates.size(); j++) {

      int a = coordinates[i][0], b = coordinates[i][1], c = coordinates[i][2];
      int p = coordinates[j][0], q = coordinates[j][1], r = coordinates[j][2];
      all.push_back({getDist(a, b, c, p, q, r), {i, j}});
    }
  }

  sort(all.begin(), all.end(), [](const pdpii &a, const pdpii &b) {
    return a.first < b.first;
  });

  initUnionFind(coordinates.size());

  long long x1, x2;

  for (int i = 0; i < all.size(); i++) {
    if (unionNodes(all[i].second.first, all[i].second.second)) {
      x1 = coordinates[all[i].second.first][0];
      x2 = coordinates[all[i].second.second][0];
    }
  }

  cout << x1 << "*" << x2 << " = " << x1*x2 << endl;

  unordered_map<int, int> clusters;
  for (int i = 0; i < coordinates.size(); i++) {
    clusters[findParent(i)]++;
  }

  vector<int> clusterSizes;
  for (auto &[clusterHead, clusterSize]: clusters) {
    clusterSizes.push_back(clusterSize);
  }

  sort(clusterSizes.begin(), clusterSizes.end(), greater<int>());

  int x = min(3, (int)clusterSizes.size());

  int ans = 1;

  for (int i = 0; i < x; i++) {
    cout << clusterSizes[i] << " " ;
    ans *= clusterSizes[i];
  }
  cout << endl;
  return ans;
}

int main() {
    vector<vector<int>> coordinates = readInput("Day 8 Input.txt");

    cout << playground(coordinates, 1e3) << endl;
    
    return 0;
}