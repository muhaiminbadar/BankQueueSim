#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "ArgumentManager.h"
#include "Structs.h"
#include "Queue.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Invalid parameters. Please specifiy I/O files" << std::endl;
        return 0;
    }


    // Parse arguments into a mapable format.
    ArgumentManager am(argc, argv);

    fstream ifs(am.get("input"), fstream::in);
    fstream cmdfs(am.get("command"), fstream::in);
    fstream ofs(am.get("output"), fstream::out);


    /** Input Parsing **/
    int maxTime, numProcess, numServers;
    Queue <Process*> processes;
    
    ifs >> maxTime;
    ifs >> numProcess;
    ifs >> numServers;

    int buffer;
    while(!ifs.eof() && processes.getContainer()->size() < numProcess)
    {
        ifs >> buffer;
        processes.push(new Process(buffer));
    }
    cout << "\n\nProcesses::" << endl;
    for(auto i : *processes.getContainer())
    {
        cout << i->pid << endl;
    }
    cout << "End of List\n\n" << endl;
    vector<Server*> servers;
    for(int s = 0; s < numServers; s++)
    {
        servers.push_back(new Server(s+1));
    }

    // Simulation.
    cout << "\n\n\n";
    Queue <Process*> completedProcesses;
    Queue <Process*> pushingQueue;

    while(!processes.empty() && processes.front()->time == 0)
    {
        cout << "processes.front()->startTime: " << processes.front()->startTime << endl;
        if(processes.front()->startTime == -1)
            processes.front()->startTime = 0;

        processes.front()->endTime = 0;
        completedProcesses.push(processes.front());
        cout << "(eT= 0) Process #" << processes.front()->pid << " has been completed " << endl;
        processes.pop();
    }

    for(int k = 0; completedProcesses.getContainer()->size() < numProcess; k++)
    {

        //cout << " TIME: " << k << endl;
        for(int s = 0; s < numServers; s++)
        {
            if(servers[s]->activeProcess != nullptr)
            {
                servers[s]->activeProcess->Tick(k);
                if(servers[s]->activeProcess->time == 0)
                {
                    // Delete
                    servers[s]->activeProcess->finish();
                    completedProcesses.push(servers[s]->activeProcess);
                    cout << "Finished process #" << servers[s]->activeProcess->pid << " from server #" << servers[s]->sid << endl;
                    servers[s]->activeProcess = nullptr;
                }
                else if(servers[s]->activeProcess->curTicks == maxTime)
                {
                    // add to queue to push back to main queue
                    pushingQueue.push(servers[s]->activeProcess);
                    cout << "Moved process #" << servers[s]->activeProcess->pid << " from server #" << servers[s]->sid << " back to queue, exceeded maxTime" << endl;
                    servers[s]->activeProcess = nullptr;
                }
            }
        }
        while(!pushingQueue.empty())
        {
            processes.push(pushingQueue.front());
            pushingQueue.pop();
        }
        for(int s = 0; s < numServers; s++)
        {
            if(!processes.empty() && servers[s]->activeProcess == nullptr)
            {
                if(processes.front()->startTime == -1)
                    processes.front()->startTime = k;

                servers[s]->activeProcess = processes.front();
                servers[s]->activeProcess->curTicks = 0;
                processes.pop();
                cout << "Added process #" << servers[s]->activeProcess->pid << " to server #" << servers[s]->sid << endl;
            }
        }
        for(auto i : *processes.getContainer())
        {
            i->waitTime++;
        }
        while(!processes.empty() && processes.front()->time == 0)
        {
            cout << "processes.front()->startTime: " << processes.front()->startTime << endl;
            if(processes.front()->startTime == -1)
                processes.front()->startTime = k+2;
            processes.front()->waitTime++;
            processes.front()->endTime = k+2;
            completedProcesses.push(processes.front());
            cout << "(eT= 0) Process #" << processes.front()->pid << " has been completed " << endl;
            processes.pop();
        }
    }
    ofs << "Max time: " << maxTime << endl;
    ofs << "Number of processes: " << numProcess << endl;
    ofs << "Number of servers: " << numServers << endl;

    completedProcesses.getContainer()->sort([](Process* one, Process* two) {
        return (one->pid) < (two->pid);
    });

    while(!completedProcesses.empty())
    {
        ofs << "\nProcess: " << completedProcesses.front()->pid << endl;
        ofs << "Execution time: " << completedProcesses.front()->execTime << endl;
        ofs << "Wait Time: " << completedProcesses.front()->waitTime << endl;
        ofs << "Start Time: " << completedProcesses.front()->startTime << endl;
        ofs << "End Time: " << completedProcesses.front()->endTime << endl;
        completedProcesses.pop();
    }
    return 0;
}


/*
        cout << "Start execution of " << k << " --------" << endl;
        for(int s = 0; s < numServers; s++)
        {
            cout << "TIME: " << time << endl;
            // Print all server status
            for(int r = 0; r < numServers; r++)
            {
                cout << "Server #" << servers[r]->sid << " is " << (servers[r]->activeProcess == nullptr ? "free" : "busy") << "." << endl;
            }   
            // Pop off all current zero execution timed
            while(!processes.empty() && processes.front()->time == 0)
            {
                if(processes.front()->startTime == -1)
                    processes.front()->startTime = time;

                processes.front()->endTime = time;
                completedProcesses.push(processes.front());
                cout << "(eT= 0) Process #" << processes.front()->pid << " has been completed in server #" << servers[s]->sid << endl;
                processes.pop();
            }
            // If there is something in queue, then put it in this server.
            if(!processes.empty() && servers[s]->activeProcess == nullptr)
            {
                servers[s]->activeProcess = processes.front();
                cout << "Put Process #" << servers[s]->activeProcess->pid << " into Server#" << servers[s]->sid << endl;
                processes.pop();
                cout << "Server #" << servers[s]->sid << " is no longer available." << endl;
            }

            if(servers[s]->activeProcess != nullptr)
            {
                servers[s]->activeProcess->Tick(time);
            }
            else
            {
                cout << "Server #" << servers[s]->sid << " is still available." << endl;
            }
        }  
        for(auto i : *processes.getContainer())
        {
            i->waitTime++;
        }
        time++;
        for(int r = 0; r < numServers; r++)
        {
            if(servers[r]->activeProcess != nullptr && servers[r]->activeProcess->time <= 0)
            {
                cout << "Process #" << servers[r]->activeProcess->pid << " has been completed in server #" << servers[r]->sid << endl;
                servers[r]->activeProcess->endTime = time;
                completedProcesses.push(servers[r]->activeProcess);
                servers[r]->activeProcess = nullptr;
            }
        } 
        // Check if any processes are in the server that haven't completed execution.
        for(int s = 0; s < numServers; s++)
        {
            if(servers[s]->activeProcess != nullptr)
            {
                cout << "Ejected process #" << servers[s]->activeProcess->pid << ", not finished in server #" << servers[s]->sid << endl;
                processes.push(servers[s]->activeProcess);
                servers[s]->activeProcess = nullptr;
            }
        }
        cout << "-------- End execution of " << k << "\n" << endl;

        if(processes.empty())
            break;
*/