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
  int maxX = 0, maxY = 0;
  for (int i = 0; i < n; i++) {
    long long x1 = coordinates[i][0], y1 = coordinates[i][1];
    maxX = max(maxX, coordinates[i][1]);
    maxY = max(maxY, coordinates[i][0]);
    for (int j = i + 1; j < n; j++) {
      long long x2 = coordinates[j][0], y2 = coordinates[j][1];

      long long currArea = (abs(x2-x1) + 1) * (abs(y2 - y1) + 1);
      maxArea = max(currArea, maxArea);
    }
  }
  cout << maxX << " : " << maxY << endl;

  return maxArea;
}

struct P {
    long long x, y;
};

static inline long long cross(const P& a, const P& b, const P& c) {
    long long x1 = b.x - a.x;
    long long y1 = b.y - a.y;
    long long x2 = c.x - a.x;
    long long y2 = c.y - a.y;
    return x1 * y2 - x2 * y1;
}

static inline bool onSegment(const P& a, const P& b, const P& p) {
    if (cross(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

// Boundary-aware, vertex-safe ray casting.
// Returns true if inside OR on boundary.
bool insideOrOn(const vector<P>& poly, const P& p) {
    int n = (int)poly.size();

    // 1) Boundary check
    for (int i = 0; i < n; i++) {
        const P& a = poly[i];
        const P& b = poly[(i + 1) % n];
        if (onSegment(a, b, p)) return true;
    }

    // 2) Ray casting with half-open rule to avoid double counting vertices
    bool inside = false;
    for (int i = 0; i < n; i++) {
        const P& a = poly[i];
        const P& b = poly[(i + 1) % n];

        // We only consider edges that straddle p.y in a half-open manner:
        // (a.y > p.y) != (b.y > p.y)
        // This is the standard stable rule.
        if ((a.y > p.y) != (b.y > p.y)) {
            long double xInt =
                (long double)(b.x - a.x) * (p.y - a.y) / (long double)(b.y - a.y) + a.x;
            if (xInt > (long double)p.x) inside = !inside;
        }
    }
    return inside;
}

struct PolygonHelper {
    vector<P> poly;
    vector<long long> uniqX, uniqY;
    vector<long double> midX, midY;

    PolygonHelper(const vector<vector<int>>& points) {
        int n = points.size();
        poly.resize(n);
        for (int i = 0; i < n; i++) {
            poly[i] = { (long long)points[i][0], (long long)points[i][1] };
            uniqX.push_back(poly[i].x);
            uniqY.push_back(poly[i].y);
        }

        sort(uniqX.begin(), uniqX.end());
        uniqX.erase(unique(uniqX.begin(), uniqX.end()), uniqX.end());

        sort(uniqY.begin(), uniqY.end());
        uniqY.erase(unique(uniqY.begin(), uniqY.end()), uniqY.end());

        // Precompute midpoints between consecutive unique levels
        for (int i = 0; i + 1 < (int)uniqX.size(); i++) {
            midX.push_back(((long double)uniqX[i] + (long double)uniqX[i + 1]) * 0.5L);
        }
        for (int i = 0; i + 1 < (int)uniqY.size(); i++) {
            midY.push_back(((long double)uniqY[i] + (long double)uniqY[i + 1]) * 0.5L);
        }
    }

    // Check rectangle boundary using precomputed mid-level samples
    bool rectangleBoundaryInside(long long lx, long long rx, long long ly, long long ry) const {
        // 1) Corners must be inside or on boundary
        P A{lx, ly}, B{rx, ly}, C{rx, ry}, D{lx, ry};
        if (!insideOrOn(poly, A)) return false;
        if (!insideOrOn(poly, B)) return false;
        if (!insideOrOn(poly, C)) return false;
        if (!insideOrOn(poly, D)) return false;

        // 2) Check left & right vertical sides at Y midpoints
        // Only test midpoints that lie within (ly, ry)
        for (auto ymid : midY) {
            if ((long double)ly < ymid && ymid < (long double)ry) {
                P L{lx, (long long) llround(ymid)}; // temporary integer anchor
                P R{rx, (long long) llround(ymid)};

                // Use exact ymid by storing in point as long long is not possible,
                // so we instead test two nearby integer y samples safely:
                // y0 = floor(ymid), y1 = ceil(ymid)
                long long y0 = (long long) floor(ymid);
                long long y1 = (long long) ceil(ymid);

                if (!insideOrOn(poly, P{lx, y0}) && !insideOrOn(poly, P{lx, y1})) return false;
                if (!insideOrOn(poly, P{rx, y0}) && !insideOrOn(poly, P{rx, y1})) return false;
            }
        }

        // 3) Check bottom & top horizontal sides at X midpoints
        for (auto xmid : midX) {
            if ((long double)lx < xmid && xmid < (long double)rx) {
                long long x0 = (long long) floor(xmid);
                long long x1 = (long long) ceil(xmid);

                if (!insideOrOn(poly, P{x0, ly}) && !insideOrOn(poly, P{x1, ly})) return false;
                if (!insideOrOn(poly, P{x0, ry}) && !insideOrOn(poly, P{x1, ry})) return false;
            }
        }

        return true;
    }
};

long long maxRectangleArea(const vector<vector<int>>& points) {
    int R = points.size();
    PolygonHelper H(points);

    long long best = 0;

    for (int i = 0; i < R; i++) {
        for (int j = i + 1; j < R; j++) {
            long long x1 = points[i][0], y1 = points[i][1];
            long long x2 = points[j][0], y2 = points[j][1];

            if (x1 == x2 || y1 == y2) continue;

            long long lx = min(x1, x2), rx = max(x1, x2);
            long long ly = min(y1, y2), ry = max(y1, y2);

            if (!H.rectangleBoundaryInside(lx, rx, ly, ry)) continue;

            // AoC rectangle area in this puzzle is inclusive tile count
            long long area = (rx - lx + 1) * (ry - ly + 1);
            best = max(best, area);
        }
    }

    return best;
}



int main() {
  vector<vector<int>> input = readInput("Day 9 Input.txt");
  cout << input.size() << " " << input[0].size() << endl;
  cout << maxRectangleArea(input) << endl;
}