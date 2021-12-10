#include "Manager.h"
#include <cstring>

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();

}

void Manager::Run(const char* filepath)
{
    fout.open(RESULT_LOG_PATH);     //print output and error

    ifstream fin;                   //read command
    fin.open(filepath);

    if(!fin.is_open()){             //if filepath is not exist, print error code
        ferr << "====== SYSTEM ======" << endl <<
        "CommandFileNotExist" << endl <<
        "====================" << endl << endl;
        return;
    }

    char cmd[50];               //store commandline
    char* pCmd;
    string comparedString;      //store LOADREPORT's string

    while(!fin.eof()){          //while read all of data
        fin.getline(cmd, 50);   //read commandline
        pCmd = strtok(cmd, " ");

        if(pCmd == NULL){       //if no command or "//" exists, continue  
            continue;
        }
        else if(pCmd[0] == '/' && pCmd[1] == '/'){
            continue;
        }
        else if(strcmp(pCmd, "LOAD") == 0){     //if command is LOAD
            pCmd = strtok(NULL, " ");
            PrintError(Load(pCmd));         //Load execute and print result
        }
        else if(strcmp(pCmd, "LOADREPORT") == 0){       //if command is LOADREPORT
            fout << "====== LOADREPORT ======" << endl;
            pCmd = strtok(NULL, " ");
            ifstream fcompare;              //open file
            fcompare.open(pCmd);
            int state = 0;

            if(!fcompare.is_open()){        //if file doesn't exist, print error
                fout << "FaildtoUpdatePath" << endl;
                fout << "====================" << endl << endl;
                PrintError(FaildtoUpdatePath);
            }
            else{
                comparedString.clear();     //string clear
                char ch;
                while(!fcompare.eof()){     //string store all of data in file
                    fcompare.get(ch);
                    comparedString.push_back(ch);
                }
                fout << "Success" << endl;
                fout << "====================" << endl << endl;
                PrintError(Success);        //print Success code
            } 
            fcompare.close();
        }
        else if(strcmp(pCmd, "PRINT") == 0){        //if command is PRINT
            Result r = Print();         //Print execute
            if(r != Success)  PrintError(r);        //print error
        }
        else if(strcmp(pCmd, "BFS") == 0){          //if command is BFS
            pCmd = strtok(NULL, " ");
            int start = pCmd != NULL ? IN_FINITY : 0;
            int end = pCmd != NULL ? IN_FINITY : m_graph.Size();    //Check more command exists
            Result r = FindPathBfs(start, end);     //execute BFS
            if(r != Success)  PrintError(r);        //print error
        }
        else if(strcmp(pCmd, "DIJKSTRA") == 0){     //if command is DIJKSRA
            pCmd = strtok(NULL, " ");
            int start = pCmd != NULL ? IN_FINITY : 0;
            int end = pCmd != NULL ? IN_FINITY : m_graph.Size();    //Check more command exists
            Result r = FindShortestPathDijkstraUsingSet(start, end);    //Execute Dijkstra
            if(r != Success)    PrintError(r);      //print error
        }
        else if(strcmp(pCmd, "BELLMANFORD") == 0){      //if command is BELLMANFORD
            pCmd = strtok(NULL, " ");
            int start = pCmd != NULL ? IN_FINITY : 0;
            int end = pCmd != NULL ? IN_FINITY : m_graph.Size();        //Check more command exists
            Result r = FindShortestPathBellmanFord(start, end);         //Execute Bellman-Ford
            if(r != Success)    PrintError(r);      //print error
        }
        else if(strcmp(pCmd, "FLOYD") == 0){        //if command is FLOYD
        Result r = FindShortestPathFloyd();         //Execute Floyd
            if(r != Success)    PrintError(r);      //print error
        }
        else if(strcmp(pCmd, "RABINKARP") == 0){        //if command is RABINKARP
            pCmd = strtok(NULL, "\n");      
            Result r = RabinKarpCompare(pCmd, comparedString.c_str());      //Execute Rabin-Karp
            if(r != Success)    PrintError(r);      //print error
        }
        else{
            PrintError(NonDefinedCommand);      //if command isn't exist, print error
        }
    }
}

//print error code
void Manager::PrintError(Result result)
{
    fout << endl << "===================" << endl;
    fout << "Error code: " << result << std::endl;
    fout << "===================" << endl << endl;
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
    fdata.open(filepath);       //map data file open

    if(!fdata.is_open()){       //if map data doesn't exists, return error
        fout << "LoadFileNotExist" << endl;
        fout << "====================" << endl << endl;
        return LoadFileNotExist;
    }

    char data[100];
    char *pData;
    int i = 0;

    while(!fdata.eof()){        //while all of data is read
        fdata.getline(data, 100);       //read line of data
        pData = strtok(data, "/");
        name[i] = pData;        //Save name
        
        m_graph.AddVertex(i);       //Vertex add
        
        pData = strtok(NULL, " ");
        int j = 0;
        while(pData){           //Edge add
            int numData = atoi(pData);
            if(numData != 0)    m_graph.AddEdge(i, j, numData);
            pData = strtok(NULL, " ");
            j++;
        }
        i++;
    }

    fout << "Success" << endl;
    fout << "====================" << endl << endl;
    fdata.close();
    return Success;     //print success
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

    if(m_graph.Size() == 0){            //if data isn't exists
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl << endl;
        return GraphNotExist;
    }

    m_graph.Print(fout);            //print data
    fout << "====================" << endl << endl;
    return Success;                 //print success code
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
    if(m_graph.Size() == 0){        //if graph doesn't exist
        fout << "====== BFS ======" << endl;
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){        //if input data is wrong
        fout << "====== BFS ======" << endl;
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindPathBfs(startVertexKey, endVertexKey);      //Execute BFS
    
    if(printPath.size() == 0){      //if Shortist path isn't exist
        fout << "====== BFS ======" << endl;
        fout << "VertexKeyNotExist" << endl;
        fout << "====================" << endl << endl;
        return VertexKeyNotExist;
    }

    bool r = m_graph.IsNegativeEdge();      //check negative edge
    if(r == true){
    fout << "====== InvalidAlgorithm ======" << endl;
    }
    else{
    fout << "====== BFS ======" << endl;
    }


    //print path
    fout << "shortist path : ";
    for(int i = printPath.size() - 1; i >= 0; i--){ 
        fout << printPath[i] << " ";
    }

    //calculate path's disance and print
    fout << endl << "path length : ";
    int length = 0;
    for(int i = printPath.size() - 1; i >= 1; i--){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i-1]){      //i -> i-1 path's weight
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    //print Vertex's name
    fout << " course : ";
    for(int i = printPath.size() - 1; i >= 0; i--){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl << endl;

    return (r == true) ? InvalidAlgorithm : Success;        //return error or success
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
    if(m_graph.Size() == 0){        //if graph doesn't exist, print error
        fout << "====== DIJKSTRA ======" << endl;
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){        //if input data is wrong, print error
        fout << "====== DIJKSTRA ======" << endl;
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey); //Execute Dijkstra
    
    if(printPath[0] != 0){          //if Shortist path doesn't exist
        fout << "====== DIJKSTRA ======" << endl;
        fout << "VertexKeyNotExist" << endl;
        fout << "====================" << endl << endl;
        return VertexKeyNotExist;
    }

    bool r = m_graph.IsNegativeEdge();      //check negative edge
    if(r == true){
    fout << "====== InvalidAlgorithm ======" << endl;
    }
    else{
    fout << "====== DIJKSTRA ======" << endl;
    }

    //print path
    fout << "shortist path : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << printPath[i] << " ";
    }

    //Calculate path's disance and print
    fout << endl << "path length : ";
    int length = 0;
    for(int i = 0; i < printPath.size() - 1; i++){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i+1]){  //i -> i+1's length
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    //print name
    fout << " course : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl << endl;

    return (r == true) ? InvalidAlgorithm : Success;        //return error or success
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
    
    if(m_graph.Size() == 0){                //if graph doesn't exist
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl << endl;
        return GraphNotExist;
    }

    if(startVertexKey == IN_FINITY){        //if input data is wrong
        fout << "InvalidVertexKey" << endl;
        fout << "====================" << endl << endl;
        return InvalidVertexKey;
    }

    vector<int> printPath = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);      //Execute Bellman-Ford

    if(printPath[0] != 0){        //if negative cycle is found
        fout << "NegativeCycleDetected" << endl;
        fout << "====================" << endl << endl;
        return NegativeCycleDetected;
    }

    //print path
    fout << "shortist path : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << printPath[i] << " ";
    }

    //Calculate path's disance and print it
    fout << endl << "path length : ";
    int length = 0;
    for(int i = 0; i < printPath.size() - 1; i++){
        Edge* tempE = m_graph.FindVertex(printPath[i])->GetHeadOfEdge();

        while(tempE){
            if(tempE->GetKey() == printPath[i+1]){      //i -> i+1's distance
                length += tempE->GetWeight();
                break;
            }
            tempE = tempE->GetNext();
        }
    } 
    fout << length << " " << endl;
    
    //print name
    fout << " course : ";
    for(int i = 0; i < printPath.size(); i++){
        fout << name[printPath[i]] << " ";
    }
    fout << endl << "====================" << endl << endl;

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
    
    if(m_graph.Size() == 0){            //if graph doesn't exist, return error
        fout << "GraphNotExist" << endl;
        fout << "====================" << endl << endl;
        return GraphNotExist;
    }

    vector<vector<int> > printPath = m_graph.FindShortestPathFloyd();       //Execute Floyd

    if(printPath[0][0] == -IN_FINITY){      //if negative cycle is found, print error
        fout << "NegativeCycleDetected" << endl;
        fout << "====================" << endl << endl;
        return NegativeCycleDetected;
    }

    //print path matrix
    for(int i = 0; i < printPath.size(); i++){
        for(int j = 0; j < printPath[i].size(); j++){
            fout << printPath[i][j] << " ";
        }
        fout << endl;
    }
    fout << endl << "====================" << endl << endl;

    return Success;     //return success
}

//Upper char -> lower char, and return int form
int MakeLowerChar(char ch) {
    int a;
    if (isalpha(ch)) {      //if ch is alphabet
        if (isupper(ch)) {
            a = (int)tolower(ch);       //if ch is upper case, make lower case
        }
        else {
            a = (int)ch;
        }
    }
    else {
        a = (int)ch;
    }

    return a;       //return ch as int form
}

//Compare two string with Rabin-Karp algorithm
Result Manager::RabinKarpCompare(const char* CompareString, const char* ComparedString)
{
    fout << "====== RABINKARP ======" << endl;

    int compareSum = 0;     //CompareString's Hash
    int i = 0;      //CompareString's length

    //Calculate CompareString's Hash
    for(;CompareString[i] != '\0'; i++){
        compareSum *= 2;
        compareSum += MakeLowerChar(CompareString[i]);
        if (compareSum < 0) {       //if hash overflow, return error
            fout << "InvalidOptionNumber" << endl;
            fout << "====================" << endl << endl;
            return InvalidOptionNumber;
        }
    }


    int sum = 0;        //ComparedString's Hash
    int mul = 1;        //2^mul
    for(int k = 0; k < i; k++){     //calculate mul
        mul *= 2;    
    }

    for(int j = 0; ComparedString[j] != '\0'; j++){
        //Calculate ComparedString's Hash
        sum *= 2;
        sum += MakeLowerChar(ComparedString[j]);
        if (j >= i) {       //Remove last Char of ComparedString's Hash
            sum -= (mul * MakeLowerChar(ComparedString[j - i]));
        }

        //Compare Hash
        if(sum == compareSum){
            bool state = true;
            for(int temp = 0; temp < i; temp++){        //Check all of char
                if(MakeLowerChar(CompareString[temp]) != MakeLowerChar(ComparedString[j - (i - 1) + temp])){
                    state = false;
                    break;
                }
            }
            if(state == true){      //if all of char is same, print string and return success
                fout << "DUPLICATE TITLE EXISTS" << endl;
                for(int temp = 0; temp <= j; temp++){
                    fout << ComparedString[temp];
                }
                fout << endl << "====================" << endl << endl;
                return Success;
            }
        }
    }

    //if same string doesn't exist, print and return success
    fout << "NO DUPLICATE TITLE EXISTS" << endl;
    fout << "====================" << endl << endl;
    return Success;
}
