/*

Created by Sujal on 22/04/19.
Copyright © 2019 Sujal. All rights reserved.

CSC-505: Homework 5

Members:
Sujal Sujal (ssujal)
Swastik Mittal (smittal6)

*/
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

int V=0, E=0, K=0;
bool flag=false;
vector< pair<int, pair<int, int> > > myvec;
vector< vector <int> > table;

struct Edge
{
    int s,d,wt;
};
class Graph
{
public:
    int vertices;
    int edges;
    vector< pair<int, int> > *adj;
    vector< Edge* > edge;
    Graph(int x, int y)
    {
        this->vertices = x;
        this->edges = y;
        adj = new vector< pair<int, int> > [x];
    }
    void add_edge(int u, int v, int wt)
    {
        adj[u].push_back(make_pair(v, wt));
    }
    void make_adjlist()
    {
        for(int i=0; i<myvec.size(); i++)
            this->add_edge(myvec[i].first, myvec[i].second.first, myvec[i].second.second);
    }
    void make_edgelist()
    {
        for(int i=0; i<myvec.size(); i++)
        {
            struct Edge* myedge = (struct Edge*)malloc(sizeof(struct Edge));   

            myedge->s = myvec[i].first;
            myedge->d = myvec[i].second.first;
            myedge->wt = myvec[i].second.second;

            edge.push_back(myedge);
        }
    }
    void print_Graph()
    {
        // for(int u=0 ;u<this->vertices; u++)
        // {   
        //     for(auto i=adj[u].begin(); i!= adj[u].end(); i++)
        //     {
        //         cout<<i->first<<":"<<i->second<<"---->";
        //     }
        //     cout<<"NULL"<<endl;
        // }
        for(int i=0; i<edge.size(); i++)
        {
            cout<<edge[i]->s<<"->"<<edge[i]->d<<":"<<edge[i]->wt<<endl;
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

void swapNodes(struct HeapNode** a, struct HeapNode** b)
{
    struct HeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct Heap* minHeap, int index)
{   
    int temp = index;
    int l = 2*index +1;
    int r = 2*index +2;

    if( (l<minHeap->c_size) && (minHeap->heap_array[l]->key < minHeap->heap_array[temp]->key))
        temp=l;
    if( (r<minHeap->c_size) && (minHeap->heap_array[r]->key < minHeap->heap_array[temp]->key))
        temp=r;
    
    if(temp!=index)
    {
        //swap positions
        minHeap->position[minHeap->heap_array[temp]->v] = index;
        minHeap->position[minHeap->heap_array[index]->v] = temp;

        //swap nodes
        swapNodes(&minHeap->heap_array[temp], &minHeap->heap_array[index]);
        minHeapify(minHeap, temp);
    }
}

//extract minimum node (root)
struct HeapNode* extractMin(struct Heap* minHeap)
{
    if(minHeap->c_size==0)
        return NULL;
    
    //swap with the last node in heap
    struct HeapNode* root = minHeap->heap_array[0];
    struct HeapNode* last = minHeap->heap_array[minHeap->c_size - 1];
    
    //swap root and last node
    minHeap->heap_array[0] = last;
    minHeap->heap_array[minHeap->c_size-1] = root;

    //updating the positions 
    minHeap->position[root->v] = minHeap->c_size-1;
    minHeap->position[last->v]=0;

    minHeap->c_size--;
    minHeapify(minHeap, 0);
    
    return root;
};

void decreaseKey(int v, int key, struct Heap* minHeap)
{
    int index = minHeap->position[v];
    if(index>=minHeap->c_size)
    {
        //swap positions
        minHeap->position[v] = minHeap->c_size;
        minHeap->position[minHeap->heap_array[minHeap->c_size]->v] = index;

        //swap nodes
        swapNodes(&minHeap->heap_array[index], &minHeap->heap_array[minHeap->c_size]);
        index = minHeap->c_size;

        minHeap->c_size++;
    }
   
    minHeap->heap_array[index]->key = key;
    
    while(index && (minHeap->heap_array[index]->key < minHeap->heap_array[((index-1)/2)]->key))
    {
        //swap position of nodes
        minHeap->position[minHeap->heap_array[index]->v] = ((index-1)/2);
        minHeap->position[minHeap->heap_array[((index-1)/2)]->v] = index;

        //swap nodes
        swapNodes(&minHeap->heap_array[index], &minHeap->heap_array[((index-1)/2)]);

        index=((index-1)/2);
    }
}

vector<int> Dijkstra(Graph g, int s)
{
    vector<int> dist2;
    int p_v= g.vertices;
    
    //distance array
    int dist[p_v];

    int result[p_v];

    //initialize heap
    struct Heap* minHeap=createHeap(p_v);
    
    //initialize number of nodes in heap
    minHeap->c_size= p_v;

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

    while(minHeap->c_size>0)
    {
        struct HeapNode* min= extractMin(minHeap);
        int u = min->v;
        
        vector< pair<int, int> >::iterator i;

        for(i=g.adj[u].begin(); i!=g.adj[u].end(); i++)
        {
            int v = i->first;
            //check if 'v' is in minHeap and the key value is greater than the edge weight
            if((i->second + dist[u] < dist[v]))
            {
                dist[v] = dist[u] + i->second;
                result[v] = u;
                decreaseKey(v, dist[v], minHeap);
            }
        }
    }
    for(int i=0; i<p_v; i++)
    {
        dist2.push_back(dist[i]);
    }
    return dist2;
}

vector<int> BellmanFord(Graph g, int s)
{
    vector<int> dist1;

    int l_v = g.vertices;
    int l_e = g.edges;
    
    int dist[l_v+1];

    //intialize source distance
    dist[s]=0;

    //intialize dist for all vertices
    for(int i=0; i<l_v; i++)
        dist[i] = INT_MAX;

    //add edges from new vertex to all vertices
    for(int i=0; i<l_v; i++)
    {
        struct Edge* myedge = (struct Edge*)malloc(sizeof(struct Edge));   
        
        myedge->s = s;
        myedge->d = i;
        myedge->wt = 0;

        g.edge.push_back(myedge);
    }
    //update the no. of edges
    l_e+=s;

    //Relax the edges
    for(int i=0; i<s; i++)
    {
        for(int j=0; j<l_e; j++)
        {
            // check adding condition dist[g->edge[j].s] != INT_MAX
            if((dist[g.edge[j]->s] != INT_MAX) && (dist[g.edge[j]->s] + g.edge[j]->wt < dist[g.edge[j]->d]))
                dist[g.edge[j]->d] = dist[g.edge[j]->s] + g.edge[j]->wt;
        }
    }

    //check for negative edge cycle
    for(int i=0; i<l_e; i++)
    {
        if(dist[g.edge[i]->s] + g.edge[i]->wt < dist[g.edge[i]->d])
            flag=true;
    }
    for(int i=0; i<l_v; i++)
    {
        dist1.push_back(dist[i]);
    }
    return dist1;
}

void Johnson(Graph g)
{
    vector<int> modifiedwt;

    modifiedwt = BellmanFord(g, g.vertices);

    for(int i=0; i<g.edges; i++)
        g.edge[i]->wt+= modifiedwt[g.edge[i]->s] - modifiedwt[g.edge[i]->d];
    
    if(!flag)
    {
        for(int i=0; i<g.vertices; i++)
            table.push_back(Dijkstra(g,i));
    }
    else
        cout<<"Negative edge weight cycle"<<endl;
}

void results(int K, vector< pair <int,int> > query)
{
    for(int i=0; i<K; i++)
    {
        if(table[query[i].first][query[i].second] != INT_MAX)
            cout<<query[i].first<<" -> "<<query[i].second<<" = "<<table[query[i].first][query[i].second]<<endl;
        else
            cout<<query[i].first<<" -> "<<query[i].second<<" = x"<<endl;
    }
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

    Graph g(V,E);

    g.make_adjlist();
    g.make_edgelist();
    Johnson(g);
    if(!flag)
        results(K, query);
    return 0;
}
