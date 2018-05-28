#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>

#include "scheduling.h"

using namespace std;

pqueue_arrival read_workload(string filename)
{
    pqueue_arrival workload;
    //YOUR CODE HERE
    string buf;
    char curr; ifstream file (filename); 
    if(file.is_open()){
        string line;
        Process p;
        while(getline(file, line)){
            buf = "";
            for(int i = 0; i < line.length(); i++){
                curr = line.at(i);
                if(curr == ' '){
                    p.arrival = stoi(buf, nullptr, 10);
                    buf = "";
                }else{
                    buf += curr;
                }
            }
            p.duration = stoi(buf, nullptr, 10);
            p.first_run = -1;
            p.completion = -1;
            workload.push(p);
        }
        file.close();
    }
    return workload;
}

void show_workload(pqueue_arrival workload)
{
    pqueue_arrival xs = workload;
    cout << "Workload:" << endl;
    while (!xs.empty())
    {
        Process p = xs.top();
        cout << '\t' << p.arrival << ' ' << p.duration << endl;
        xs.pop();
    }
}

void show_processes(list<Process> processes)
{
    list<Process> xs = processes;
    cout << "Processes:" << endl;
    while (!xs.empty())
    {
        Process p = xs.front();
        cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
            << ", first_run=" << p.first_run << ", completion=" << p.completion << endl;
        xs.pop_front();
    }
}

list<Process> fifo(pqueue_arrival workload)
{
    list<Process> complete;
    // YOUR CODE HERE
    if(workload.empty()){return complete;}
    pqueue_arrival arrived;
    int numProcesses = workload.size();
    int time = 0;
    Process p;
    while(!workload.empty()){
        p = workload.top();
        if(time < p.arrival){
            time = p.arrival;
        }
        p.first_run = time;
        time = p.first_run + p.duration;
        p.completion = time;
        p.duration = 0;
        complete.push_back(p);
        if(workload.size() > 1){
            workload.pop();
        }else{
            break;
        }
    }

    return complete;
}

list<Process> sjf(pqueue_arrival workload)
{
    list<Process> complete;
    // YOUR CODE HERE
    if(workload.empty()){return complete;}
    pqueue_duration durations;
    int numProcesses = workload.size();
    Process p;
    bool done = false;;
    int time = 0;
    while(complete.size() < numProcesses){
        p = workload.top();
        while(p.arrival <= time && !done){
            cout << p.arrival << '\n';
            durations.push(p);
            if(workload.size() <= 1){
                done = true;
                break;
            }else{
                workload.pop();
                p = workload.top();
            }
        }
        p = durations.top();
        durations.pop();
        p.first_run = time;
        time += p.duration;
        p.completion = time;
        p.duration = 0;
        complete.push_back(p);
    }

    return complete;
}

list<Process> stcf(pqueue_arrival workload)
{
    list<Process> complete;
    // YOUR CODE HERE 
    if(workload.empty()){return complete;}
    pqueue_duration durations;
    int numProcesses = workload.size();
    Process p;
    int time = 0;
    while(complete.size() < numProcesses){
        p = workload.top();
        while(p.arrival == time){
            durations.push(p);
            if(workload.size() <= 1){
                break;
            }else{
                workload.pop();
                p = workload.top();
            }
        }
        if(!durations.empty()){
            p = durations.top();
            durations.pop();
            if(p.first_run == -1){p.first_run = time;}
            time++;
            p.duration--;
            if(p.duration <= 0){
                p.completion = time;
                complete.push_back(p);
            }else{
                durations.push(p);
            }
        }else{
            time++;
        }
    }
    return complete;
}

list<Process> rr(pqueue_arrival workload)
{
    list<Process> complete;
    // YOUR CODE HERE
    if(workload.empty()){return complete;}
    queue<Process> arrived;
    int numProcesses = workload.size();
    Process p;
    int time = 0;
    while(complete.size() < numProcesses){
        p = workload.top();
        while(p.arrival == time){
            arrived.push(p);
            if(workload.size() <= 1){
                break;
            }else{
                workload.pop();
                p = workload.top();
            }
        }
        if(!arrived.empty()){
            p = arrived.front();
            arrived.pop();
            if(p.first_run == -1){p.first_run = time;}
            time++;
            p.duration--;
            if(p.duration <= 0){
                p.completion = time;
                complete.push_back(p);
            }else{
                arrived.push(p);
            }
        }else{
            time++;
        }
    }
    return complete;
}

float avg_turnaround(list<Process> processes)
{
    if(processes.empty()){return 0;}
    float sumTurnaround = 0;
    for(auto p : processes){
        sumTurnaround += (float)(p.completion-p.arrival);
    }
    return (sumTurnaround/processes.size());
}

float avg_response(list<Process> processes)
{
    if(processes.empty()){return 0;}
    float sumResponse = 0;
    for(auto p : processes){
        sumResponse += (float)(p.first_run-p.arrival);
    }
    return (sumResponse/processes.size());
}

void show_metrics(list<Process> processes)
{
    float avg_t = avg_turnaround(processes);
    float avg_r = avg_response(processes);
    show_processes(processes);
    cout << '\n';
    cout << "Average Turnaround Time: " << avg_turnaround(processes) << endl;
    cout << "Average Response Time:   " << avg_response(processes) << endl;
}
