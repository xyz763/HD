#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

struct Node {
    int data;
    vector<Node*> children;
};
void parallel_BFS(Node* root, int target) {
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    bool found = false;
    cout << "search flow bfs: ";
    while (!q.empty() && !found) {
        int n = q.size();
        vector<Node*> nextLevel;
        #pragma omp parallel shared(found)
        {
            vector<Node*> localNext;
            vector<int> localVisited;
            #pragma omp for
            for (int i = 0; i < n; i++) {
                Node* curr;
                #pragma omp critical
                {
                    curr = q.front();
                    q.pop();
                }
                if (curr && !found) {
                    localVisited.push_back(curr->data);
                    if (curr->data == target) {
                        #pragma omp critical
                        {
                            if (!found) {
                                found = true;
                            }
                        }
                    }
                    for (auto child : curr->children)
                        localNext.push_back(child);
                }
            }
            #pragma omp critical
            {
                for (auto x : localVisited)
                    cout << x;
                nextLevel.insert(nextLevel.end(), localNext.begin(), localNext.end());
            }
        }
        for (auto node : nextLevel) q.push(node);
    }
    cout << endl;
    if (found)
        cout << "Found node with data: " << target << endl;
    else
        cout << "Error: Node with data " << target << " not found." << endl;
}
int main() {
    Node* root = new Node{8, {
        new Node{2, {new Node{5}, new Node{6}}},
        new Node{3, {new Node{4}}}
    }};
    int target;
    cout << "Enter the node value to search: ";
    cin >> target;
    parallel_BFS(root, target);
    return 0;
}



//g++ bfs.cpp -o bfs   (create .exe file)
// .\bfs