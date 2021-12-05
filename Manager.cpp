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
        }
        else if(strcmp(pCmd, "PRINT") == 0){
            PrintError(Print());
        }
        else if(strcmp(pCmd, "BFS") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            PrintError(FindPathBfs(start, end));
        }
        else if(strcmp(pCmd, "DIJKKSTRA") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            PrintError(FindShortestPathDijkstraUsingSet(start, end));
        }
        else if(strcmp(pCmd, "BELLMANFORD") == 0){
            pCmd = strtok(NULL, " ");
            int start = pCmd == nullptr ? IN_FINITY : 0;
            int end = pCmd == nullptr ? IN_FINITY : m_graph.Size();
            PrintError(FindShortestPathBellmanFord(start, end));
        }
        else if(strcmp(pCmd, "FLOYD") == 0){
            PrintError(FindShortestPathFloyd());
        }
        else if(strcmp(pCmd, "RABINKARP") == 0){
            pCmd = strtok(NULL, "\n");
            PrintError(RabinKarpCompare(pCmd, comparedString.c_str()));
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
    // TODO: implement
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
    // TODO: implement
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
    // TODO: implement
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
    // TODO: implement
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
    // TODO: implement
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
    
}

Result Manager::RabinKarpCompare(const char* CompareString,const char* ComparedString)
{
    // TODO: implement
}
