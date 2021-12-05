#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>
#include <queue>
#include <vector>
#include <algorithm>

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
    if(weight == 0){
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
    queue<pair<int, int> > q;           //Queue for BFS(current Key, before Key)
    stack<pair<int, int> > s;           //Stack for finding path(current Key, before Key)
    vector<int> path;                   //path vector

    int* visit = new int[m_vSize];      //Check visit
    q.push(make_pair(startVertexKey, startVertexKey));      //Start point
    visit[startVertexKey] = true;

    //BFS
    while(!q.empty()){
        Vertex* tempV = FindVertex(q.front().first);        //Current Vertex
        s.push(q.front());                                  //Save used queue
        q.pop();                                            //pop queue

        Edge* tempE = tempV->GetHeadOfEdge();               //Current Vertex's Edge pointer
        while(tempE){
            int y = tempE->GetKey();                        //Edge
            int weight = tempE->GetWeight();                //Weight
            tempE = tempE->GetNext();

            if(weight < 0)   continue;                      //if NegWeight, continue
            if(!visit[y]){                                  //if Unvisited Key
                if(y == endVertexKey){                      //if Unvisited Key is endVertexKey
                    delete[] visit;
                    path.push_back(endVertexKey);           //Save End Key
                    int curKey = tempV->GetKey();           //Save Current Key
                    path.push_back(curKey);

                    while(!s.empty()){                      //check all Edge that used
                        if(s.top().first == curKey){        //if Edge is used in path
                            if(s.top().first == s.top().second) return path;    //if CurKey is StartVertexKey, return path

                            curKey = s.top().second;        //if CurKey is not StartVertexkey, add path
                            path.push_back(curKey);
                        }
                        s.pop();                            //pop stack
                    }
                }

                q.push(make_pair(y, tempV->GetKey()));      //Visit Vertex
                visit[y] = true;
            }
        }
    }

    delete[] visit;                                         //Don't have path
    return path;
}

    // find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey){
    int* distance = new int[m_vSize];       //distance
    for(int i = 0; i < m_vSize; i++){
        distance[i] = IN_FINITY;
    }
    distance[startVertexKey] = 0;

    vector<int> path[m_vSize];              //Shortist path
    

    set<pair<int, int> > pq;
    pq.insert(make_pair(startVertexKey, 0));    //Start Vertex

    //Dijkstra
    while(!pq.empty()){
        int curKey = pq.begin()->first;         //Current Key
        int curDis = -pq.begin()->second;       //Current Distance
        pq.erase(pq.begin());

        if(distance[curKey] < curDis)   continue;   //if Current distance is bigger than distance

        Vertex* tempV = FindVertex(curKey);         //Current Vertex
        Edge* tempE = tempV->GetHeadOfEdge();       //Current Vertex's Edge pointer
        while(tempE){
            int nextKey = tempE->GetKey();          //next Key
            int nextWeight = tempE->GetWeight();    //next Weight
            int nextDis = curDis + nextWeight;      //next Distance
            tempE = tempE->GetNext();
            if(nextWeight < 0)  continue;           //if Weight is Neg, Continue

            if(nextDis < distance[nextKey]){        //if nextDistance is Short
                distance[nextKey] = nextDis;        //Distance update
                
                path[nextKey].clear();              //Path update
                for(int i = 0; i < path[curKey].size(); i++){
                    path[nextKey].push_back(path[curKey][i]);
                }
                path[nextKey].push_back(nextKey);
                pq.insert(make_pair(nextKey, -nextDis));    //push at set
            }

        }
    }
    delete[] distance;
    return path[endVertexKey];          //Print Path(start -> end)
}

    // find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey){
    int* distance = new int[m_vSize];
    for(int i = 0; i < m_vSize; i++){
        distance[i] = IN_FINITY;
    }
    distance[startVertexKey] = 0;
    vector<int> path[m_vSize];

    Vertex* tempV = m_pVHead;
    while(tempV){
        int from = tempV->GetKey();
        Edge* tempE = tempV->GetHeadOfEdge();
        while(tempE){
            int to = tempE->GetKey();
            int dis = tempE->GetWeight();
            
            tempE = tempE->GetNext();

            if(distance[from] == IN_FINITY) continue;
            if(distance[to] > distance[from] + dis){
                distance[to] = distance[from] + dis;
                path[to].clear();
                for(int i = 0; i < path[from].size(); i++){
                    path[to].push_back(path[from][i]);
                }
                path[to].push_back(to);
            }
        }
        tempV = tempV->GetNext();
    }

    while(tempV){
        int from = tempV->GetKey();
        Edge* tempE = tempV->GetHeadOfEdge();
        while(tempE){
            int to = tempE->GetKey();
            int dis = tempE->GetWeight();
            
            tempE = tempE->GetNext();

            if(distance[from] == IN_FINITY) continue;
            if(distance[to] > distance[from] + dis){
                path[endVertexKey].clear();
                path[endVertexKey].push_back(-1);
                delete[] distance;
                return path[endVertexKey];
            }
        }
        tempV = tempV->GetNext();
    }
    delete[] distance;
    return path[endVertexKey];
}
    
    
std::vector<vector<int> > Graph::FindShortestPathFloyd(){
    vector<vector<int> > path;
    for(int i = 0; i < m_vSize; i++){
        for(int j = 0; j < m_vSize; j++){
            path[i][j] = 0;
        }
    }

    Vertex* tempV = m_pVHead;
    while(tempV){
        Edge* tempE = tempV->GetHeadOfEdge();
        while(tempE){
            path[tempV->GetKey()][tempE->GetKey()] = tempE->GetWeight();
            tempE = tempE->GetNext();
        }
        tempV = tempV->GetNext();
    }

    for(int k = 0; k < m_vSize; k++){
        for(int i = 0; i < m_vSize; i++){
            for(int j = 0; j < m_vSize; j++){
                int temp = path[i][k] + path[k][j];
                if(temp < path[i][j]){
                    path[i][j] = temp;    
                }
            }
        }
    }
    return path;
}