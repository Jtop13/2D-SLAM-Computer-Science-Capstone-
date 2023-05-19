#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Point {
    double x, y;
};

double euclideanDistance(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

vector<int> range(int n) {
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = i;
    }
    return result;
}

vector<int> getNeighbors(vector<Point>& dataset, int pIndex, double eps) {
    vector<int> neighbors;
    for (int i : range(dataset.size())) {
        if (i == pIndex) continue;
        if (euclideanDistance(dataset[i], dataset[pIndex]) <= eps) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

void expandCluster(vector<Point>& dataset, int pIndex, int clusterIndex, double eps, int minPts, vector<int>& clusterLabels) {
    vector<int> seeds = getNeighbors(dataset, pIndex, eps);
    if (seeds.size() < minPts) {
        clusterLabels[pIndex] = -1; // mark as noise
        return;
    }
    clusterLabels[pIndex] = clusterIndex;
    for (int i : seeds) {
        clusterLabels[i] = clusterIndex;
    }
    while (!seeds.empty()) {
        int currentPIndex = seeds.front();
        vector<int> currentPNeighbors = getNeighbors(dataset, currentPIndex, eps);
        if (currentPNeighbors.size() >= minPts) {
            for (int i : currentPNeighbors) {
                if (clusterLabels[i] == 0) {
                    seeds.push_back(i);
                    clusterLabels[i] = clusterIndex;
                }
            }
        }
        seeds.erase(seeds.begin());
    }
}

vector<int> runDBSCAN(vector<Point>& dataset, double eps, int minPts) {
    vector<int> clusterLabels(dataset.size(), 0); // 0 means unvisited
    int clusterIndex = 0;
    for (int i : range(dataset.size())) {
        if (clusterLabels[i] != 0) continue; // skip points that have been visited already
        vector<int> neighbors = getNeighbors(dataset, i, eps);
        if (neighbors.size() < minPts) {
            clusterLabels[i] = -1; // mark as noise
            continue;
        }
        clusterIndex++;
        expandCluster(dataset, i, clusterIndex, eps, minPts, clusterLabels);
    }
    return clusterLabels;
}

int main() {
    srand(time(NULL));
    vector<Point> dataset(100);
    for (Point& p : dataset) {
        p.x = rand() % 100;
        p.y = rand() % 100;
    }
    vector<int> clusterLabels = runDBSCAN(dataset, 10, 5);
    for (int label : clusterLabels) {
        cout << label << " ";
    }
    cout << endl;
    return 0;
}
