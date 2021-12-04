#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>
#include <queue>
#include <vector>

#define BFS_FIRST_PATH

Graph::Graph()
{
    m_pVHead = nullptr;
    m_vSize = 0;
}
Graph::~Graph()
{
    Clear();
}

    // add vertex with vertexNum at the end of the linked list for the vertics
void Graph::AddVertex(int vertexKey){
    Vertex* newVertex = new Vertex(vertexKey);

    if(m_pVHead == nullptr){
        m_pVHead = newVertex;
        m_vSize++;
        return;
    }

    Vertex* temp = m_pVHead;
    while(temp->GetNext()){
        temp = temp->GetNext();
    }
    temp->SetNext(newVertex);
    m_pVHead = newVertex;
    return;
}

    // add edge from the vertex which the number is startVertexKey to the vertex which the number is endVertexKey
void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight){
    if(m_pVHead == nullptr){
        return;
    }

    Vertex* temp = m_pVHead;
    while(temp)
    {
        if(temp->GetKey() == startVertexKey){
            temp->AddEdge(endVertexKey, weight);
            return;
        }
        temp = temp->GetNext();
    }
    return;
}

    // get the vertex which the key is vertexNum
Vertex* Graph::FindVertex(int key){
    Vertex* temp = m_pVHead;
    while(temp){
        if(temp->GetKey() == key){
            return temp;
        }
        temp = temp->GetNext();
    }
    return nullptr;
}

    // get the number of the vertics
int Graph::Size() const{
    return m_vSize;
}
    // memory free for the vertics
void Graph::Clear(){
    Vertex* temp = m_pVHead;
    while(m_pVHead){
        m_pVHead = m_pVHead->GetNext();
        temp->Clear();
        delete temp;
        temp = m_pVHead;
    }
    return;
}

    // print out the graph as matrix form
void Graph::Print(std::ofstream& fout){
    Vertex* tempV = m_pVHead;
    while(tempV){
        Edge* tempE = tempV->GetHeadOfEdge();
        while(tempE){
            fout << tempE->GetWeight() << " ";
        }
        fout << endl;
    }
    return;
}

    // check whether the graph has negative edge or not.
bool Graph::IsNegativeEdge(){
    Vertex* tempV = m_pVHead;
    while(tempV){
        Edge* tempE = tempV->GetHeadOfEdge();
        while(tempE){
            if(tempE->GetWeight() < 0){
                return true;
            }
        }
    }
    return false;
}

    // find the path from startVertexKey to endVertexKey with BFS
std::vector<int> Graph::FindPathBfs(int startVertexKey, int endVertexKey){
    queue<int> q;
    int* visit = new int[m_vSize];
    q.push(startVertexKey);
    visit[startVertexKey] = true;

    while(!q.empty()){
        int x = q.front();
        q.pop();
        for(int i = 0; i < m_vSize; i++){
        }

    }
    
}


    // find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey){

}

    // find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey){

}
    
std::vector<vector<int>> Graph::FindShortestPathFloyd(){

}