//（沒有 auto）
#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <fstream>
using namespace std; 

class Edge { 
public:
    int from, to, weight;
    Edge(){};
    Edge(int u, int v, int w):from(u), to(v), weight(w){};
};

class MSTree{
private:
    int num_vertex;
    vector<vector<int> > AdjMatrix;

public:
    MSTree():num_vertex(0){};
    MSTree(int n):num_vertex(n){
        AdjMatrix.resize(num_vertex);
        for (int i = 0; i < num_vertex; i++) {
            AdjMatrix[i].resize(num_vertex);
        }
        for (int i = 0; i < num_vertex; i++) {
            for (int j = i+1; j < num_vertex; j++) {
                AdjMatrix[i][j] = 1000;
            }
        }
    }
    void EdgeAdding(int from, int to, int weight);
    void KruskalMSTree();
    void GetSortedEdge(vector<Edge> &vec);
    friend int FindSetCollapsing(int *subset, int i);
    friend void UnionSet(int *subset, int x, int y);
};
int FindSetCollapsing(int *subset, int i) {

    int root;  // root
    for (root = i; subset[root] >= 0; root = subset[root]);

    while (i != root) {
        int parent = subset[i];
        subset[i] = root;
        i = parent;
    }

    return root;
}
void UnionSet(int *subset, int x, int y){

    int xroot = FindSetCollapsing(subset, x),
        yroot = FindSetCollapsing(subset, y);

    if (subset[xroot] <= subset[yroot]) {
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else {
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}
bool WeightComp(Edge e1, Edge e2) {
    return (e1.weight < e2.weight);
}
bool FromComp(Edge e1, Edge e2) {
    if(e1.weight == e2.weight){
        if(e1.from == e2.from)
            return (e1.to < e2.to);
        return (e1.from < e2.from);
    }
    else
        return (e1.weight < e2.weight);
}
void MSTree::GetSortedEdge(vector<Edge> &edgearray) {

    for (int i = 0; i < num_vertex-1; i++) {
        for (int j = i+1; j < num_vertex; j++) {
            if (AdjMatrix[i][j] >= 0) {
                edgearray.push_back(Edge(i,j,AdjMatrix[i][j]));
            }
        }
    }

    sort(edgearray.begin(), edgearray.end(), WeightComp);
}
void MSTree::KruskalMSTree() {
    vector<Edge> edgesetMST;

    int edgesetcount = 0;
    int minimumcost = 0;
    int subset[num_vertex];

    for (int i = 0; i < num_vertex; i++) {
        subset[i] = -1;
    }

    vector<Edge> increaseWeight;

    GetSortedEdge(increaseWeight);

    for (int i = 0; i < increaseWeight.size(); i++) {
        if (FindSetCollapsing(subset, increaseWeight[i].from) != FindSetCollapsing(subset, increaseWeight[i].to)) {
            edgesetMST.push_back(increaseWeight[i]);
            edgesetcount++;
            UnionSet(subset, increaseWeight[i].from, increaseWeight[i].to);
        }
    }
    
    sort(edgesetMST.begin(), edgesetMST.end(), FromComp);

    cout << "\nminimum spanning tree:" << endl;
    for (int i = 0; i < edgesetcount; i++) {
        cout << i+1 << ": <" << edgesetMST[i].from << "," << edgesetMST[i].to << ">" << endl;
        minimumcost += edgesetMST[i].weight;
    }
    cout << "\nThe cost of minimum spanning tree: " << minimumcost << endl;
}

void MSTree::EdgeAdding(int from, int to, int weight){
    AdjMatrix[from][to] = weight;
}

void AdjacencyListAdding(vector<int> adj[], int from, int to) {
    adj[from].push_back(to);
    adj[to].push_back(from);
}

int AdjacencyList(vector<int> adj[], int lines) {
    cout << "Adjacency list:" << endl;
    int node;
    for (int d = 0; d < lines; d++) {
        node = d;
        if(adj[d].empty()) break;
        cout << d << ": ";
        for(int x = 0; x < adj[d].size(); x++) cout << adj[d][x] << " -> ";
        cout << "end" << endl;
    }
    return node;
}


int main() {
    int n = 50000;
    int lines = 0;
    int node;
    Edge arr[n];

    //############# read by cin, begin
    // for(int j=0; j<=n; j++) {
    //     cin >> arr[j].from >> arr[j].to >> arr[j].weight;
    //     lines = j;
    //     if(cin.eof()) break;
    // }
    //############# read by cin, end

    //############# read from file, begin
    string filename("data99.txt");
    int number;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }
                
    int i = 0;
    while (input_file >> number) {
        lines = i/3;
        int ii = i%3;
        if (ii==0) arr[lines].from = number;
        else if(ii==1) arr[lines].to = number;
        else if(ii==2) arr[lines].weight = number;
        i++;
    }
    input_file.close();
    //############# read from file, end

    vector<int> adj[lines];
    for(int l=0; l<lines; l++) AdjacencyListAdding(adj, arr[l].from, arr[l].to);
    node = AdjacencyList(adj, lines);

    MSTree tree(node);

    for(int i=0; i<lines; i++) tree.EdgeAdding(arr[i].from, arr[i].to, arr[i].weight);

    tree.KruskalMSTree();

    return 0;
}