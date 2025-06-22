#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent a 2D point with additional information (e.g., name)
struct Point {
    int x, y;
    string name;

    Point(int x, int y, string name) : x(x), y(y), name(name) {}
};

// Node structure for the range tree
struct Node {
    Point point;
    Node *left, *right;
    vector<Point> ySorted; // Sorted by y-coordinate

    Node(Point point) : point(point), left(nullptr), right(nullptr) {}
};

// Helper function to sort points by x or y
bool compareX(const Point& a, const Point& b) { return a.x < b.x; }
bool compareY(const Point& a, const Point& b) { return a.y < b.y; }

// Function to build the Range Tree
Node* buildRangeTree(vector<Point>& points, int start, int end) {
    if (start > end) return nullptr;

    // Points sorted by x-coordinate
    sort(points.begin() + start, points.begin() + end + 1, compareX);
    int mid = (start + end) / 2;

    Node* root = new Node(points[mid]);
    root->ySorted.assign(points.begin() + start, points.begin() + end + 1);
    sort(root->ySorted.begin(), root->ySorted.end(), compareY);

    root->left = buildRangeTree(points, start, mid - 1);
    root->right = buildRangeTree(points, mid + 1, end);

    return root;
}

// Query function to find points within a given range
void rangeQuery(Node* root, int x1, int x2, int y1, int y2, vector<Point>& result) {
    if (!root) return;

    // Current point check
    if (root->point.x >= x1 && root->point.x <= x2 &&
        root->point.y >= y1 && root->point.y <= y2) {
        result.push_back(root->point);
    }

    // Check left subtree if it may contain points in range
    if (root->left && x1 <= root->point.x) {
        rangeQuery(root->left, x1, x2, y1, y2, result);
    }

    // Check right subtree if it may contain points in range
    if (root->right && x2 >= root->point.x) {
        rangeQuery(root->right, x1, x2, y1, y2, result);
    }
}

// Function to print query results
void printResults(const vector<Point>& result) {
    for (const auto& point : result) {
        cout << "(" << point.x << ", " << point.y << ", " << point.name << ")\n";
    }
}

int main() {
    // List of points with x, y coordinates and names
    vector<Point> points = {
        {3, 6, "A"}, {17, 15, "B"}, {13, 15, "C"},
        {6, 12, "D"}, {9, 1, "E"}, {2, 7, "F"}
    };

    // Build the Range Tree
    Node* root = buildRangeTree(points, 0, points.size() - 1);

    // Define query range
    int x1 = 3, x2 = 15, y1 = 5, y2 = 15;

    // Perform range query
    vector<Point> result;
    rangeQuery(root, x1, x2, y1, y2, result);

    // Display results
    cout << "Points in range (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << "):\n";
    printResults(result);

    return 0;
}
