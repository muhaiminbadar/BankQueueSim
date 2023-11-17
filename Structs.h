#pragma once

int pc = 1;

struct Process {
    int pid, time, waitTime, startTime, endTime, execTime, curTicks;
    Process(int t) : time(t)  {
        pid = pc++;
        waitTime = 0;
        startTime = -1;
        execTime = 0;
        curTicks = 0;
    }
    void Tick(int t)
    {
        
        curTicks++;
        time --;
        execTime++;

    }
    void finish()
    {
        endTime = execTime + waitTime;
    }
};

struct Server {
    int sid;
    Process* activeProcess;
    Server(int s) : sid(s)
    {
        activeProcess = nullptr;
    }
};
