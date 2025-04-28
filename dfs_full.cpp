#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;
struct Node {
    int data;
    vector<Node*> children;
};
void parallel_DFS(Node* root, int target, bool& found) {
    if (!root || found) return;
    #pragma omp critical
    {
        if (!found) {
            cout << root->data << " ";
            if (root->data == target) {
                found = true;
                cout << "\nFound node with data: " << target << endl;
            }
        }
    }
    // Create tasks for children
    #pragma omp parallel if(!found)
    {
        #pragma omp single nowait
        {
            for (auto child : root->children) {
                if (!found) {
                    #pragma omp task firstprivate(child)
                    {
                        parallel_DFS(child, target, found);
                    }}}}}}
int main() {
    Node* root = new Node{8, {
        new Node{2, {new Node{5}, new Node{6}}},
        new Node{3, {new Node{4}}}
    }};

    int target;
    cout << "Enter the node value to search: ";
    cin >> target;

    bool found = false;

    cout << "search flow dfs: ";
    #pragma omp parallel
    {
        #pragma omp single
        parallel_DFS(root, target, found);
    }

    if (!found) {
        cout << "\nError: Node with data " << target << " not found." << endl;
    }

    return 0;
}

//g++ dfs.cpp -o dfs
//   .\dfs