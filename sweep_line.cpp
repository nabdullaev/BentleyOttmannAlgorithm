#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <queue>

using namespace std;

// The structure of the point
struct Point {
    int x, y, label; // x and y coordinates of the point and the label represents the number of the segment that the point belongs to
    bool isLeft;
    Point(int x, int y, bool isLeft, int label) : x(x), y(y), isLeft(isLeft), label(label) {}
};

// The structure of the segment
struct Segment {
    int x1, y1, x2, y2;
    Segment(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

    // The method checks if the segment intersects with another segment
    bool intersects(Segment& other) {
        int d1 = direction(Segment::x1, Segment::y1, Segment::x2, Segment::y2, other.x1, other.y1);
        int d2 = direction(Segment::x1, Segment::y1, Segment::x2, Segment::y2, other.x2, other.y2);
        int d3 = direction(other.x1, other.y1, other.x2, other.y2, Segment::x1, Segment::y1);
        int d4 = direction(other.x1, other.y1, other.x2, other.y2, Segment::x2, Segment::y2);
        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
        if (d1 == 0 && onSegment(Segment::x1, Segment::y1, Segment::x2, Segment::y2, other.x1, other.y1)) return true;
        if (d2 == 0 && onSegment(Segment::x1, Segment::y1, Segment::x2, Segment::y2, other.x2, other.y2)) return true;
        if (d3 == 0 && onSegment(other.x1, other.y1, other.x2, other.y2, Segment::x1, Segment::y1)) return true;
        if (d4 == 0 && onSegment(other.x1, other.y1, other.x2, other.y2, Segment::x2, Segment::y2)) return true;
        return false;
    }

    // The method checks if the segment intersects with a point
    int direction(int x1, int y1, int x2, int y2, int x3, int y3) {
        int val = (y2 - y1) * (x3 - x2) - (y3 - y2) * (x2 - x1);
        if (val == 0) return 0;
        return (val > 0) ? 1 : -1;
    }

    // The method checks if the point is on the segment
    bool onSegment(int x1, int y1, int x2, int y2, int x3, int y3) {
        if (x3 <= max(x1, x2) && x3 >= min(x1, x2) && y3 <= max(y1, y2) && y3 >= min(y1, y2)) return true;
        return false;
    }
};

// The program uses quick sort to sort the points by x coordinate
// Quick sort is a divide and conquer algorithm that works by partitioning an array into two subarrays
// The time complexity of quick sort is O(n log n) in average case and O(n^2) in worst case
// Quick sort is a stable sorting algorithm
// The quick sort is in-space sorting algorithm
template <typename T>
void quickSort(vector<T>& points, int left, int right) {
    int i = left, j = right;
    T pivot = points[(left + right) / 2];
    while (i <= j) {
        while (points[i].x < pivot.x) i++;
        while (points[j].x > pivot.x) j--;
        if (i <= j) {
            swap(points[i], points[j]);
            i++;
            j--;
        }
    }
    if (left < j) quickSort(points, left, j);
    if (i < right) quickSort(points, i, right);
}



// For intermediate storage, a Red Black Tree is used
// The Red Black Tree is a self-balancing binary search tree
template <typename K, typename V>
class RedBlackTree {
public:
    bool RED = true;
    bool BLACK = false;
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Node* parent;
        bool color;
        Node(K key, V value, bool color, Node* parent) : key(key), value(value), color(color), left(nullptr), right(nullptr), parent(parent) {}
    };
    Node* root = nullptr;

    // The time complexity of the isRed method is O(1)
    bool isRed(Node* node) {
        if (node == nullptr) return false;
        return node->color == RED;
    }

    // The time complexity of the rotateLeft method is O(1)
    Node* rotateLeft(Node* node) {
        Node* x = node->right;
        node->right = x->left;
        if (x->left != nullptr) x->left->parent = node;
        x->parent = node->parent;
        if (node->parent == nullptr) root = x;
        else if (node == node->parent->left) node->parent->left = x;
        else node->parent->right = x;
        x->left = node;
        node->parent = x;
        return x;
    }

    // The time complexity of the rotateRight method is O(1)
    Node* rotateRight(Node* node) {
        Node* x = node->left;
        node->left = x->right;
        if (x->right != nullptr) x->right->parent = node;
        x->parent = node->parent;
        if (node->parent == nullptr) root = x;
        else if (node == node->parent->right) node->parent->right = x;
        else node->parent->left = x;
        x->right = node;
        node->parent = x;
        return x;
    }

    // The time complexity of the flipColors method is O(1)
    void flipColors(Node* node) {
        if (node == nullptr) return;
        node->color = !node->color;
        if (node->left != nullptr) node->left->color = !node->left->color;
        if (node->right != nullptr) node->right->color = !node->right->color;
    }

    // The time complexity of the put method is O(log n)
    void put(K key, V value) {
        root = put(root, key, value, nullptr);
        root->color = BLACK;
    }

    Node* put(Node* node, K key, V value, Node* parent) {
        if (node == nullptr) return new Node(key, value, RED, parent);
        if (key < node->key) node->left = put(node->left, key, value, node);
        else if (key > node->key) node->right = put(node->right, key, value, node);
        else node->value = value;
        if (isRed(node->right) && !isRed(node->left)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left)) node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);
        return node;
    }

    // The time complexity of the get method is O(log n)
    V get(K key) {
        Node* node = root;
        while (node != nullptr) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return node->value;
        }
        return V();
    }

    // The time complexity of the find method is O(log n)
    Node* find(K key) {
        Node* node = root;
        while (node != nullptr) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return node;
        }
        return nullptr;
    }

    // The time complexity of the remove method is O(log n)
    void remove(K key) {
        if (!contains(key)) return;
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = remove(root, key);
        if (root != nullptr) root->color = BLACK;
    }

    Node* remove(Node* node, K key) {
        if (key < node->key) {
            if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);
            node->left = remove(node->left, key);
        } else {
            if (isRed(node->left)) node = rotateRight(node);
            if (key == node->key && node->right == nullptr) return nullptr;
            if (!isRed(node->right) && !isRed(node->right->left)) node = moveRedRight(node);
            if (key == node->key) {
                Node* x = min(node->right);
                node->key = x->key;
                node->value = x->value;
                node->right = deleteMin(node->right);
            } else node->right = remove(node->right, key);
        }
        return balance(node);
    }


    // The time complexity of the moveRedLeft method is O(1)
    Node* moveRedLeft(Node* node) {
        flipColors(node);
        if (node->right != nullptr && isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
            flipColors(node);
        }
        return node;
    }

    // The time complexity of the moveRedRight method is O(1)
    Node* moveRedRight(Node* node) {
        flipColors(node);
        if (node->left != nullptr && isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        return node;
    }

    // The time complexity of the balance method is O(1)
    Node* balance(Node* node) {
        if (isRed(node->right)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left)) node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);
        return node;
    }

    // The time complexity of the deleteMin method is O(log n)
    void deleteMin() {
        if (!containsMin()) return;
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = deleteMin(root);
        if (root != nullptr) root->color = BLACK;
    }

    Node* deleteMin(Node* node) {
        if (node->left == nullptr) return nullptr;
        if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);
        node->left = deleteMin(node->left);
        return balance(node);
    }

    // The time complexity of the isEmpty method is O(1)
    bool isEmpty() {
        return root == nullptr;
    }

    // The time complexity of the contains method is O(log n)
    bool contains(K key) {
        return get(key) != V();
    }

    // The time complexity of the containsMin method is O(1)
    bool containsMin() {
        return !isEmpty();
    }

    // The time complexity of the min method is O(log n)
    Node* min(Node* node) {
        if (node->left == nullptr) return node;
        return min(node->left);
    }

    // The time complexity of the max method is O(log n)
    Node* max(Node* node) {
        if (node->right == nullptr) return node;
        return max(node->right);
    }

    // The time complexity of the predecessor method is O(log n)
    Node* predecessor(Node* node) {
        if (node->left != nullptr) return max(node->left);
        Node* x = node->parent;
        while (x != nullptr && node == x->left) {
            node = x;
            x = x->parent;
        }
        return x;
    }

    // The time complexity of the successor method is O(log n)
    Node* successor(Node* node) {
        if (node->right != nullptr) return min(node->right);
        Node* x = node->parent;
        while (x != nullptr && node == x->right) {
            node = x;
            x = x->parent;
        }
        return x;
    }

    K successor(K key) {
        Node* node = find(key);
        if (node == nullptr) return -1;
        Node* x = successor(node);
        if (x == nullptr) return -1;
        return x->key;
    }

    K predecessor(K key) {
        Node* node = find(key);
        if (node == nullptr) return -1;
        Node* x = predecessor(node);
        if (x == nullptr) return -1;
        return x->key;
    }

    // The time complexity of the deleteKey method is O(log n)
    void deleteKey(K key) {
        if (!contains(key)) return;
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = deleteKey(root, key);
        if (root != nullptr) root->color = BLACK;
    }

    Node* deleteKey(Node* node, K key) {
        if (key < node->key) {
            if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);
            node->left = deleteKey(node->left, key);
        } else {
            if (isRed(node->left)) node = rotateRight(node);
            if (key == node->key && node->right == nullptr) return nullptr;
            if (!isRed(node->right) && !isRed(node->right->left)) node = moveRedRight(node);
            if (key == node->key) {
                Node* x = min(node->right);
                node->key = x->key;
                node->value = x->value;
                node->right = deleteMin(node->right);
            } else node->right = deleteKey(node->right, key);
        }
        return balance(node);
    }

    // The time complexity of the deleteRoot method is O(log n)
    void deleteRoot() {
        if (root == nullptr) return;
        if (root->left == nullptr && root->right == nullptr) {
            root = nullptr;
            return;
        }
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = deleteRoot(root);
        if (root != nullptr) root->color = BLACK;
    }

    Node* deleteRoot(Node* node) {
        if (node->left == nullptr) return nullptr;
        if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);
        node->left = deleteRoot(node->left);
        return balance(node);
    }

    // The time complexity of the print method is O(n)
    void print() {
        print(root);
    }

    void print(Node* node) {
        if (node == nullptr) return;
        print(node->left);
        cout << node->key << " " << node->value << " " << node->color << endl;
        print(node->right);
    }
};

int main () {
    // The code snippet below is used to speed up the input and output operations
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;
    vector<Point> points;
    vector<Segment> segments;

    // Notice that we use both segments and points to solve the problem
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        segments.emplace_back(Segment(x1, y1, x2, y2));
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        points.emplace_back(Point(x1, y1, 1, i));
        points.emplace_back(Point(x2, y2, 0, i));
    }

    quickSort(points, 0, points.size() - 1);
    // After the sorting, the points are sorted by their x-coordinate

    RedBlackTree<int, int> tree;

    for (int i = 0; i < points.size(); i++) {
        if (points[i].isLeft) { // If the point is a left endpoint, we insert it into the tree, and check if there is an intersection with the segment to the right and the segment to the left
            tree.put(points[i].label, points[i].x);
            int successor = tree.successor(points[i].label);
            if (successor != -1 && segments[points[i].label].intersects(segments[successor])) {
                cout << "INTERSECTION" << endl;
                cout << segments[points[i].label].x1 << " " << segments[points[i].label].y1 << " " << segments[points[i].label].x2 << " " << segments[points[i].label].y2 << endl;
                cout << segments[successor].x1 << " " << segments[successor].y1 << " " << segments[successor].x2 << " " << segments[successor].y2 << endl;
                return 0;
            }

            int predecessor = tree.predecessor(points[i].label);
            if (predecessor != -1 && segments[points[i].label].intersects(segments[predecessor])) {
                cout << "INTERSECTION" << endl;
                cout << segments[points[i].label].x1 << " " << segments[points[i].label].y1 << " " << segments[points[i].label].x2 << " " << segments[points[i].label].y2 << endl;
                cout << segments[predecessor].x1 << " " << segments[predecessor].y1 << " " << segments[predecessor].x2 << " " << segments[predecessor].y2 << endl;
                return 0;
            }
        } else {
            // If the point is a right endpoint, we delete it from the tree, and check if there is an intersection with the segment to the right and the segment to the left
            int successor = tree.successor(points[i].label);
            int predecessor = tree.predecessor(points[i].label);
            if (successor != -1 && predecessor != -1 && segments[successor].intersects(segments[predecessor])) {
                cout << "INTERSECTION" << endl;
                cout << segments[successor].x1 << " " << segments[successor].y1 << " " << segments[successor].x2 << " " << segments[successor].y2 << endl;
                cout << segments[predecessor].x1 << " " << segments[predecessor].y1 << " " << segments[predecessor].x2 << " " << segments[predecessor].y2 << endl;
                return 0;
            }
            tree.deleteKey(points[i].label);
        }
    }
    // If no segment intersections are found, we print NO INTERSECTIONS
    cout << "NO INTERSECTIONS" << endl;
    return 0;
}
