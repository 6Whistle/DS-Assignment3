#include "Manager.h"
#include <cstring>

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();

    if (ferr.is_open())
        ferr.close();
}

void Manager::Run(const char* filepath)
{
    fout.open(RESULT_LOG_PATH);
    ferr.open(ERROR_LOG_PATH, ios::app);

    ifstream fin;
    fin.open(filepath);

    if(!fin.is_open()){
        ferr << "====== SYSTEM ======" << endl <<
        "CommandFileNotExist" << endl <<
        "====================" << endl;
        return;
    }

    char cmd[50];
    char* pCmd;
    string comparedString;

    while(!fin.eof()){
        fin.getline(cmd, 50);
        pCmd = strtok(cmd, " ");

        if(pCmd == nullptr){
            continue;
        }
        else if(pCmd[0] == '/' && pCmd[1] == '/'){
            continue;
        }
        else if(strcmp(pCmd, "LOAD") == 0){
            pCmd = strtok(NULL, " ");
            PrintError(Load(pCmd));
        }
        else if(strcmp(pCmd, "LOADREPORT") == 0){
            fout << "====== LOADREPORT ======" << endl;
            pCmd = strtok(NULL, " ");
            ifstream fcompare;
            fcompare.open(pCmd);
            int state = 0;

            if(!fcompare.is_open()){
                fout << "FaildtoUpdatePath" << endl;
                fout << "====================" << endl;
                PrintError(FaildtoUpdatePath);
            }
            else{
                comparedString.clear();
                char ch;
                while(!fcompare.eof()){
                    fcompare.get(ch);
                    comparedString.push_back(ch);
                }
                fout << "Success" << endl;
                fout << "====================" << endl;
                PrintError(Success);
            } 
            fcompare.close();
        }
        else if(strcmp(pCmd, "PRINT") == 0){
            Result r = Print();
            if(r != Success)  PrintError(r);
        }
        else if(strcmp(pCmd, "BFS") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            Result r = FindPathBfs(start, end);
            if(r != Success)  PrintError(r);
        }
        else if(strcmp(pCmd, "DIJKKSTRA") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            Result r = FindShortestPathDijkstraUsingSet(start, end);
            if(r != Success)    PrintError(r);
        }
        else if(strcmp(pCmd, "BELLMANFORD") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            Result r = FindShortestPathBellmanFord(start, end);
            if(r != Success)    PrintError(r);
        }
        else if(strcmp(pCmd, "FLOYD") == 0){        
        Result r = FindShortestPathFloyd();
            if(r != Success)    PrintError(r);
        }
        else if(strcmp(pCmd, "RABINKARP") == 0){
            pCmd = strtok(NULL, "\n");
            Result r = RabinKarpCompare(pCmd, comparedString.c_str());
            if(r != Success)    PrintError(r);
        }
        else{
            PrintError(NonDefinedCommand);
        }
    }
}
void Manager::PrintError(Result result)
{
    ferr << endl << "===================" << endl;
    ferr << "Error code: " << result << std::endl;
    ferr << "===================" << endl << endl;
}

/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)
{
    fout << "====== LOAD ======" << endl;
    ifstream fdata;
    fdata.open(filepath);

    if(!fdata.is_open()){
        fout << "LoadFileNotExist" << endl;
        fout << "====================" << endl;
        return LoadFileNotExist;
    }

    char data[50];
    char *pData;
    int i = 0;

    while(!fdata.eof()){
        fdata.getline(data, 50);
        pData = strtok(data, " / ");
        name[i] = pData;
        
        m_graph.AddVertex(i);
        
        pData = strtok(NULL, " ");
        int j = 0;
        while(pData){
            int numData = atoi(pData);
            if(numData != 0)    m_graph.AddEdge(i, j, numData);
            pData = strtok(NULL, " ");
            j++;
        }
        i++;
    }

    fout << "Success" << endl;
    fout << "====================" << endl;
    fdata.close();
    return Success;
}
/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print()
{
    fout << "====== PRINT ======" << endl;

    if(m_graph.Size() == 0){
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl;
        return GraphNotExist;
    }

    m_graph.Print(fout);
    fout << "====================" << endl;
    return Success;
}
/// <summary>
/// find the path from startVertexKey to endVertexKey with DFS 
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    fout << "====== BFS ======" << endl;
    
    if(m_graph.Size() == 0){
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindPathBfs(startVertexKey, endVertexKey);
    
    if(printPath.size() == 0){
        fout << "VertexKeyNotExist" << endl;
        fout << "====================" << endl;
        return VertexKeyNotExist;
    }

    fout << "shortist path : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << printPath[i] << " ";
    }

    fout << endl << "path length : ";
    int length = 0;
    for(int i = 0; i < printPath.size() - 1; i++){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i+1]){
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    fout << " course : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl;

    if(m_graph.IsNegativeEdge()){
        return InvalidAlgorithm;;
    }
    return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    fout << "====== DIJKSTRA ======" << endl;
    
    if(m_graph.Size() == 0){
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey);
    
    if(printPath.size() == 0){
        fout << "VertexKeyNotExist" << endl;
        fout << "====================" << endl;
        return VertexKeyNotExist;
    }

    fout << "shortist path : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << printPath[i] << " ";
    }

    fout << endl << "path length : ";
    int length = 0;
    for(int i = 0; i < printPath.size() - 1; i++){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i+1]){
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    fout << " course : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl;

    if(m_graph.IsNegativeEdge()){
        return InvalidAlgorithm;;
    }
    return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    fout << "====== BELLMANFORD ======" << endl;
    
    if(m_graph.Size() == 0){
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);

    if(printPath.size() == 1 && printPath[0] == -1){
        fout << "NegativeCycleDetected" << endl;
        fout << "====================" << endl;
        return NegativeCycleDetected;
    }

    fout << "shortist path : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << printPath[i] << " ";
    }

    fout << endl << "path length : ";
    int length = 0;
    for(int i = 0; i < printPath.size() - 1; i++){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i+1]){
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    fout << " course : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl;

    return Success;
}

/// <summary>
/// find all of the shortest path with FLOYD
/// </summary>
///
/// <returns>
/// Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathFloyd()
{
    fout << "====== FLOYD ======" << endl;
    
    if(m_graph.Size() == 0){
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl;
        return GraphNotExist;
    }

    vector<vector<int> > printPath = m_graph.FindShortestPathFloyd();

    if(printPath[0][0] == -IN_FINITY){
        fout << "NegativeCycleDetected" << endl;
        fout << "====================" << endl;
        return NegativeCycleDetected;
    }

    for(int i = 0; i < printPath.size(); i++){
        for(int j = 0; j < printPath[i].size(); j++){
            fout << printPath[i][j] << " ";
        }
        fout << endl;
    }
    fout << endl << "====================" << endl;

    return Success;
}

Result Manager::RabinKarpCompare(const char* CompareString, const char* ComparedString)
{
    fout << "====== RABINKARP ======" << endl;
    int compareSum = 0;

    int i = 0;
    for(;CompareString[i] != '\0'; i++){
        compareSum *= 2;
        compareSum += (int)(CompareString[i]);
    }

    if(compareSum < 0){
        fout << "InvalidOptionNumber" << endl;
        fout << "====================" << endl;
        return InvalidOptionNumber;
    }

    int sum = 0;
    int mul = 1;
    for(int k = 0; k < i-1; k++){
        mul *= 2;    
    }

    for(int j = 0; ComparedString[j] != '\0'; j++){
        sum *= 2;
        sum += (int)(ComparedString[j]);

        if(j < i)   continue;
        sum -= (mul * ComparedString[j - i]);
        
        if(sum == compareSum){
            bool state = true;
            for(int temp = 0; temp < i; temp++){
                if(CompareString[temp] != ComparedString[j - (i - 1) + temp]){
                    state = false;
                    break;
                }
            }
            if(state == true){
                fout << "DUPLICATE TITLE EXISTS" << endl;
                for(int temp = 0; temp < j; temp++){
                    fout << ComparedString[temp];
                }
                fout << endl << "====================" << endl;
                return Success;
            }
        }
    }

    fout << "NO DUPLICATE TITLE EXISTS" << endl;
    fout << "====================" << endl;
    return Success;
}
