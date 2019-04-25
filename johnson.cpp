#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

int V=0, E=0, K=0;
int forest=1;
unsigned int total_sum = 0;
int c=0, bf=2;
vector< pair<int, pair<int, int> > > myvec;

class Graph
{
public:
    int vertices;
    vector< pair<int, int> > *adj;
    Graph(int x)
    {
        this->vertices=x;
        adj = new vector< pair<int, int> > [x];
    }
    void add_edge(int u, int v, int wt)
    {
        adj[u].push_back(make_pair(v, wt));
        // adj[v].push_back(make_pair(u, wt));
    }
    void make_adjlist()
    {
        for(int i=0; i<myvec.size(); i++)
            this->add_edge(myvec[i].first, myvec[i].second.first, myvec[i].second.second);
    }
    void print_Graph()
    {
        for(int u=0 ;u<vertices; u++)
        {   
            for(auto i=adj[u].begin(); i!= adj[u].end(); i++)
            {
                cout<<i->first<<":"<<i->second<<"---->";
            }
            cout<<"NULL"<<endl;
        }
    }
};

//Min-Heap

//single node
struct HeapNode
{
    int v;
    int key;
};

//complete heap
struct Heap
{
    int c_size; //current size
    int t_size; //total size
    struct HeapNode** heap_array;
    int* position;
};

//create a node in heap
struct HeapNode* createHeapNode(int v, int key)
{
    struct HeapNode* node=  (struct HeapNode*)malloc(sizeof(struct HeapNode));
    node->v= v;
    node->key= key;
    return node;
};

//create a heap with heapnodes
struct Heap* createHeap(int total_size)
{
    struct Heap* minHeap = (struct Heap*)malloc(sizeof(struct Heap));
    minHeap->c_size= 0;
    minHeap->t_size= total_size;
    minHeap->heap_array = (struct HeapNode**)malloc(total_size*sizeof(struct HeapNode*));
    minHeap->position= (int *)malloc(total_size*sizeof(int));
    return minHeap;
};

void minHeapify(struct Heap* minHeap, int index)
{   
    int mfact = index << c, temp = index;
    for(unsigned int i = 1 ; i <= bf; i++)
	{   
        if((mfact+i) < minHeap->c_size)
			if(minHeap->heap_array[temp]->key > minHeap->heap_array[mfact+i]->key)
				temp = mfact+i;
    }            
    if(temp!=index)
    {
        //swap positions
        minHeap->position[minHeap->heap_array[temp]->v] = index;
        minHeap->position[minHeap->heap_array[index]->v] = temp;

        struct HeapNode* temp1 = minHeap->heap_array[index];
        minHeap->heap_array[index] = minHeap->heap_array[temp];
        minHeap->heap_array[temp] = temp1;

        minHeapify(minHeap, temp);
    }
}

//extract minimum node (root)
struct HeapNode* extractMin(struct Heap* minHeap)
{
    if(minHeap->c_size==0)
        return NULL;
    
    struct HeapNode* root = minHeap->heap_array[0];

    minHeap->heap_array[0] = minHeap->heap_array[minHeap->c_size - 1];

    //updating the positions 
    minHeap->position[root->v] = minHeap->c_size-1;
    minHeap->position[minHeap->heap_array[minHeap->c_size-1]->v]=0;

    minHeap->c_size--;
    minHeapify(minHeap, 0);
    return root;
};

void decreaseKey(int v, int key, struct Heap* minHeap)
{
    int index = minHeap->position[v];
    minHeap->heap_array[index]->key = key;
    int parent = 0;
    
    if(index!=0)
        parent = (index-1) >> c;

    while(index && (minHeap->heap_array[index]->key < minHeap->heap_array[parent]->key))
    {
        minHeap->position[minHeap->heap_array[index]->v] = parent;
        minHeap->position[minHeap->heap_array[parent]->v] = index;

        struct HeapNode* temp = minHeap->heap_array[index];
        minHeap->heap_array[index] = minHeap->heap_array[parent];
        minHeap->heap_array[parent] = temp;

        index=parent;
        parent = (index-1) >> c;
    }
}

void dijkstra(Graph g, int s)
{
    int p_v= g.vertices;
    
    //distance array
    int dist[p_v];

    int result[p_v];
    // int key[p_v];

    //initialize heap
    struct Heap* minHeap=createHeap(p_v);

    //intialize distance values for each vertex
    for(int i=0; i<p_v; i++)
    {
        result[i]=-1;
        dist[i]=INT_MAX;
        minHeap->heap_array[i] = createHeapNode(i, dist[i]);
        minHeap->position[i] = i;
    }

    //intialize source vertex
    dist[s]=0;
    minHeap->heap_array[s]= createHeapNode(s, dist[s]);
    minHeap->position[s] = s;
    decreaseKey(s, dist[s], minHeap);

    //initialize number of nodes in heap
    minHeap->c_size= p_v;

    //initialize key values
    // key[0]=0;
    // minHeap->heap_array[0] = createHeapNode(0, key[0]);
    // minHeap->position[0] = 0;

    // for(int i=1; i<p_v; i++)
    // {
    //     result[i]=-1;
    //     key[i]=INT_MAX;
    //     minHeap->heap_array[i] = createHeapNode(i, key[i]);
    //     minHeap->position[i] = i;
    // }
    // cout<<minHeap->c_size;
    while(minHeap->c_size>0)
    {
        // if(minHeap->heap_array[0]->key==INT_MAX)
        //     forest++;
        // else
        //     total_sum+=minHeap->heap_array[0]->key ;
        struct HeapNode* min= extractMin(minHeap);
        int u = min->v;

        for(auto i=g.adj[u].begin(); i!=g.adj[u].end(); i++)
        {
            int v = i->first;

            //check if 'v' is in minHeap and the key value is greater than the edge weight
            if((i->second + dist[u] < dist[v]) && (minHeap->position[v] < minHeap->c_size))
            {
                dist[v] = dist[u] + i->second;
                // key[v] = i->second;
                result[v] = u;
                decreaseKey(v, dist[v], minHeap);
            }
        }
    }
    for(int i=0; i<p_v; i++)
    {
        // cout<<i<<":"<<dist[i]<<endl;
        cout<<i<<":"<<result[i]<<endl;
    }
    // cout<<bf<<" "<<forest<<" "<<total_sum<<endl;
}

int main(int argc, char* argv[])
{
    vector<string> str_input;
    vector< pair <int,int> > query;
	string str;

	while(getline(cin,str))
		str_input.push_back(str);

    for(int i=0; i<str_input.size();i++)
    {
        stringstream stream(str_input[i]);

        if(i==0)
        {            
            vector<int> var;
            for(int j=0; stream>>j;)
                var.push_back(j);
            V=var[0]; E=var[1];
        }
        else if(i==E+1)
        {
            for(int j=0; stream>>j;)
                K=j;
        }
        else if(i>E+1)
        {
            //Store queries
            vector<int> var1;
            for(int i=0; stream>>i;)
                var1.push_back(i);
            query.push_back(make_pair(var1[0], var1[1]));
        }
        else
        {
            vector<int> var2;
            for(int i=0; stream>>i;)
                var2.push_back(i);
            myvec.push_back(make_pair(var2[0], make_pair(var2[1],var2[2])));
        }
    }

    Graph g(V);

    g.make_adjlist();
    // Prim(g);
    dijkstra(g,0);
    // g.print_Graph();
    return 0;
}