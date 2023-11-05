/* 

Jinn Ha (PSID: 1954549)
COSC 3360 - Operating Systems
Section 10716
Assignment 1

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

//SET UP GLOBAL VARIABLES  

        struct event
        {
            int processNumber;
            string resourceReq;
            int startEndTime;
            int duration;
            int lockNumber;
        };

        struct process
        {
            int processNumber;
            string status;
        };

        int nUsedcores;
        int nFreeCores;
        vector <event> eventList; 
        vector <event> runningList;
        queue <event> coreReadyQueue;
        queue <event> ssdReadyQueue;
        vector <event> lockReadyQueue;
        vector < queue<event>> processList; 
        vector <process> processTable;
        int globalTime =0;
        int currProcessNumber;
        int processCount =0;
        
        vector <int> locks; //0 IS UNUSED, 1 IS IN USED
        int ssd = 0; //0 IS UNUSED, 1 IS IN USED
        vector <event> waitingQueue;


//FUNCTIONS

bool checkIfEmpty(vector <queue<event>> temp)
{
    int count=0;
    bool p = false;
    for (int i=0; i<temp.size();i++)
    {
        if(temp.at(i).empty())
        {   
            count++;
        }
    }
    if (count == temp.size())
    {
        p = true;
    }
    if(p == true && eventList.empty() == true)
    {
        return true;
    }

    
}

void printAll()
{
    cout << endl << "PROCESS LIST: "<<endl;
        
        
        for (int i = 0; i < processList.size(); i++)
        {
            queue <event> p = processList.at(i);
            while (p.size() != 0)
            {
                cout << p.front().processNumber << " "<<p.front().resourceReq<<" "<<p.front().duration<<endl;
                p.pop();
            }

        }

        cout << endl << "EVENT LIST: "<<endl;
        
        for (int i = 0; i < eventList.size(); i++)
        {
            if (eventList.at(i).resourceReq == "START")
            {
                cout << eventList.at(i).processNumber<< " "<<eventList.at(i).resourceReq<<" "<< eventList.at(i).startEndTime<<endl;
            }
            else
            {
                cout << eventList.at(i).processNumber<< " "<<eventList.at(i).resourceReq<<" "<< eventList.at(i).startEndTime<< " "<<eventList.at(i).duration<<endl;
            }
        } 

        cout << endl << "CPU READY QUEUE:"<<endl;
        queue <event> qe = coreReadyQueue;
        for (int i=0; i<qe.size();i++)
        {
            cout << qe.front().processNumber <<endl;
            qe.pop();
        }
            
}

void clearQueue(queue <event> &q) 
 {
    while (!q.empty())
    {
        q.pop();
    }
}


void swap(event* x, event* y)
{
    event temp = *x;
    *x = *y;
    *y = temp;
}

void sortMin(vector <event> &list)
{
    for ( int i = 0; i < list.size()-1; i++)
    {
        for (int j = 0; j < list.size() - i -1; j++)
        {
            if (list.at(j).startEndTime > list.at(j + 1).startEndTime)
            {
                swap(&list.at(j), &list.at(j + 1));
            }
        }
    }
}

void callStart(event curr)
{
    globalTime = curr.startEndTime;
    processTable.at(curr.processNumber).status = "READY";

    cout << "Process " << curr.processNumber<<" starts at time "<< globalTime<<" ms"<<endl;
    cout << "Current number of busy cores "<<nUsedcores<<endl;
    cout << "Process Table:"<<endl;
    for (int i=0; i<processTable.size();i++)
    {
        struct process x = processTable.at(i);
        if (x.status != "NOTSTARTED")
        {
            cout << "Process " << x.processNumber << " is "<<x.status<<endl;
        }
    }
    cout << "Ready queue is empty."<<endl;
}

void callEnd(event curr)
{
    processTable.at(curr.processNumber).status = "TERMINATED";

    cout << "Process " << curr.processNumber<<" terminates at time "<< globalTime<<" ms"<<endl;
    cout << "Current number of busy cores "<<nUsedcores<<endl;
    cout << "Process Table:"<<endl;
    for (int i=0; i<processTable.size();i++)
    {
        struct process x = processTable.at(i);
        if (x.status != "NOTSTARTED")
        {
            cout << "Process " << x.processNumber << " is "<<x.status<<endl;
        }
    }
    cout << "Ready queue is empty."<<endl;
}

void callCPU (event curr)
{
    globalTime = curr.startEndTime;
}

        
int main()
    {
         
        //GET FILE DATA
        cout << "Please enter file name. "<<endl;
        string filename;
        cin >> filename;
        ifstream input(filename);

        //PARSE DATA INTO VECTOR, MAIN VECTOR HAS PROCESS VECTORS WHICH HAS INSTRUCTIONS INSIDE
        vector <vector<pair<string, int>>> inputdata; 
        string line = "";
        string get = "";
        pair<string, int> temp;
        vector <pair<string, int>> section; 
        queue <event> currProc;
        while (getline(input, line)) //PUT DATA INTO INPUTDATA VECTOR
        {
            struct event xTemp;
            get = line.substr(0,line.find(' '));

            if (get == "NCORES")
            {   
                string temp = line.substr(line.find(" "));
                temp = temp.substr(temp.find_first_not_of(" "));
                nFreeCores = stoi(temp);
            }
            else if (get == "START")
            {
                string temp = line.substr(line.find(" "));
                temp = temp.substr(temp.find_first_not_of(" "));

                processCount++;
                currProcessNumber = processCount-1;
                cout << "-- Scheduling arrival of process "<<currProcessNumber<<" at time "<<temp<< " - -"<<endl;
                
                xTemp.processNumber = currProcessNumber;
                xTemp.resourceReq= get;
                xTemp.startEndTime = stoi(temp);
                eventList.push_back(xTemp);

                struct process t;
                t.processNumber = currProcessNumber;
                t.status = "NOTSTARTED";
                processTable.push_back(t);
            }
            else if (get.substr(0,3) == "END")
            {
                struct event xTemp;
                xTemp.processNumber = currProcessNumber;
                xTemp.resourceReq= "END";
                currProc.push(xTemp);
                processList.push_back(currProc);
                clearQueue(currProc);
            }
            else
            {
                string temp = line.substr(line.find(" "));
                temp = temp.substr(temp.find_first_not_of(" "));

                struct event xTemp;
                xTemp.processNumber = currProcessNumber;
                xTemp.resourceReq= get;
                xTemp.startEndTime = 0;
                if (get == "LOCK" || get == "UNLOCK")
                {
                    xTemp.lockNumber = stoi(temp);
                }
                else
                {
                    xTemp.duration = stoi(temp);
                }
                currProc.push(xTemp);
            }
        }

        for (int i=0; i<65;i++)
        {
            locks.push_back(0);
        }

        //PROCESS INSTRUCTIONS

        cout << "starting loop:"<<endl;
        bool started = false;
        while (!checkIfEmpty(processList))
        {            
            sortMin(eventList);
            event curr = eventList.at(0);

            //printAll();

            if (curr.resourceReq == "START")
            {
                callStart(curr);
                eventList.erase(eventList.begin());

                struct event g = processList.at(curr.processNumber).front();
                g.startEndTime = 0;
                processList.at(curr.processNumber).pop();
                eventList.push_back(g);
            }
            if (curr.resourceReq == "END")
            {
                callEnd(curr);
                eventList.erase(eventList.begin());
            }
            if (curr.resourceReq == "CPU")
            {
                if(curr.startEndTime!=0) //END CORE USAGE
                {
                    globalTime = curr.startEndTime;
                    cout << "-- Process "<<curr.processNumber<< " releases a core at time " << globalTime<< " ms."<<endl;
                    eventList.erase(eventList.begin());
                    eventList.push_back(processList.at(curr.processNumber).front());
                    processList.at(curr.processNumber).pop();
                    nFreeCores++;
                    nUsedcores--;

                    //GIVE CORE TO CORE READY QUEUE
                    if(coreReadyQueue.size() != 0)
                    {
                        coreReadyQueue.front().startEndTime = globalTime + coreReadyQueue.front().duration;
                        eventList.push_back(coreReadyQueue.front());
                        coreReadyQueue.pop();
                        cout <<"-- Give core to process "<<eventList.at(eventList.size()-1).processNumber << " from the ready queue."<<endl;
                        cout << "-- Ready queue now contains "<<coreReadyQueue.size()<<" process(es) waiting for a core."<<endl;
                        cout << "-- Process "<<eventList.at(eventList.size()-1).processNumber<< " will release a core at time "<< eventList.at(eventList.size()-1).startEndTime <<" ms."<<endl;
                        nFreeCores--;
                        nUsedcores++;
                    }
                }
                else
                {
                    cout << "-- Process "<<curr.processNumber<<" requests a core at time "<<globalTime<<" ms for "<<curr.duration<<" ms."<<endl;
                    if (nFreeCores>0) //START CPU PROCESSING
                    {
                        cout << "-- Process "<<curr.processNumber<<" gets a core at time "<<globalTime<<" ms."<<endl;
                        curr.startEndTime = globalTime+curr.duration;
                        eventList.at(0).startEndTime = globalTime+curr.duration;
                        cout << "-- Process "<<curr.processNumber<<" will release a core at time "<<curr.startEndTime<<" ms."<<endl;
                        runningList.push_back(curr);
                        processTable.at(curr.processNumber).status = "RUNNING";
                        nFreeCores--;
                        nUsedcores++;
                    }
                    else
                    {
                        processTable.at(curr.processNumber).status = "BLOCKED";
                        cout << "-- Process "<< curr.processNumber<< " must wait for a core."<<endl;
                        coreReadyQueue.push(curr);
                        eventList.erase(eventList.begin());
                        cout << "-- Ready queue now contains "<<coreReadyQueue.size()<<" process(es) waiting for a core."<<endl;
                    }
                }
            }
            if (curr.resourceReq == "SSD")
            {
                if(curr.startEndTime!=0) //END SSD PROCESSING
                {
                    globalTime = curr.startEndTime;
                    cout << "-- Process "<<curr.processNumber<< " releases the SSD at time " << globalTime<< " ms."<<endl;
                    eventList.erase(eventList.begin());
                    eventList.push_back(processList.at(curr.processNumber).front());
                    processList.at(curr.processNumber).pop();
                    ssd = 0;

                    //GIVE SSD TO SSD READY QUEUE
                    if(ssdReadyQueue.size() != 0)
                    {
                        ssdReadyQueue.front().startEndTime = globalTime + ssdReadyQueue.front().duration;
                        eventList.push_back(ssdReadyQueue.front());
                        ssdReadyQueue.pop();
                        ssd = 1;
                        cout <<"-- Give SSD to process "<<eventList.at(eventList.size()-1).processNumber << " from the ssd queue."<<endl;
                        cout << "-- SSD queue now contains "<<ssdReadyQueue.size()<<" process(es) waiting for a core."<<endl;
                        cout << "-- Process "<<eventList.at(eventList.size()-1).processNumber<< " will release the SSD at time "<< eventList.at(eventList.size()-1).startEndTime<<" ms."<<endl;
                    }
                }
                else
                {
                    cout << "-- Process "<< curr.processNumber<<" requests SSD access at time "<< globalTime<<" ms for "<<curr.duration<<" ms."<<endl;
                    if (ssd == 0) //START CPU PROCESSING
                        {
                            cout << "-- Process "<<curr.processNumber<<" is granted access to the SSD at time "<<globalTime<<" ms."<<endl;
                            curr.startEndTime = globalTime+curr.duration;
                            eventList.at(0).startEndTime = globalTime+curr.duration;
                            cout << "-- Process "<<curr.processNumber<<" will release the SSD at time "<<curr.startEndTime<<" ms."<<endl;
                            runningList.push_back(curr);
                            processTable.at(curr.processNumber).status = "RUNNING";
                            ssd = 1;
                        }
                        else
                        {
                            processTable.at(curr.processNumber).status = "BLOCKED";
                            cout << "-- Process "<< curr.processNumber<<" must wait for the SSD."<<endl;
                            ssdReadyQueue.push(curr);
                            eventList.erase(eventList.begin());
                            cout << "-- SSD queue now contains "<<ssdReadyQueue.size()<<" process(es) waiting for the SSD."<<endl;
                        }
                }
            }
            if (curr.resourceReq == "LOCK")
            {
                cout << "-- Process "<< curr.processNumber<<" requests lock "<<curr.lockNumber<<" at time "<< globalTime<<" ms. "<<endl;
                if (locks.at(curr.lockNumber) == 0)
                {
                    cout << "-- Process " << curr.processNumber << " gets lock "<<curr.lockNumber<<" at time "<<globalTime<<" ms."<<endl;
                    locks.at(curr.lockNumber) = 1;
                    eventList.erase(eventList.begin());
                    processTable.at(curr.processNumber).status = "RUNNING";

                    struct event g = processList.at(curr.processNumber).front();
                    g.startEndTime = 0;
                    processList.at(curr.processNumber).pop();
                    eventList.push_back(g);
                }
                else
                {
                    processTable.at(curr.processNumber).status = "BLOCKED";
                    cout << "-- Process "<< curr.processNumber<<" must wait for lock "<< curr.lockNumber<<endl;
                    lockReadyQueue.push_back(curr);
                    eventList.erase(eventList.begin());
                    cout << "-- Lock queue now contains "<<lockReadyQueue.size()<<" process(es) waiting for a lock."<<endl;
                }
            }
            if (curr.resourceReq == "UNLOCK")
            {
                cout << "-- Process "<<curr.processNumber<< " releases lock " << curr.lockNumber<< " at time "<<globalTime<< " ms."<<endl;
                locks.at(curr.lockNumber) = 0;

                eventList.erase(eventList.begin());
                eventList.push_back(processList.at(curr.processNumber).front());
                processList.at(curr.processNumber).pop();

                for (int i=0; i<lockReadyQueue.size();i++)
                {
                    if (lockReadyQueue.at(i).lockNumber == curr.lockNumber)
                    {
                        cout << "Process " << curr.processNumber << " gets lock "<<curr.lockNumber<<" at time "<<globalTime<<" ms."<<endl;
                        locks.at(curr.lockNumber) = 1;
                        processTable.at(curr.processNumber).status = "RUNNING";
                    }
                }
            }
            if (curr.resourceReq == "OUTPUT")
            {
                cout << "-- Process "<<curr.processNumber<<" starts I/O request at time "<<globalTime<<" for "<<curr.duration<<" ms. "<<endl;
                globalTime = globalTime + curr.duration;
                cout << "-- Process "<<curr.processNumber<<" complete its I/O step at time "<<globalTime<<" ms. "<<endl;

                eventList.erase(eventList.begin());
                eventList.push_back(processList.at(curr.processNumber).front());
                processList.at(curr.processNumber).pop();
            }

            
        }
    }
