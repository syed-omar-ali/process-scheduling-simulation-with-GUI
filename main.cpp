#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <graphics.h>
#include <conio.h>
#include <sstream>
using namespace std;
struct process{
	string name;
	int burst_time,arrival_time,waiting_time,turnAround_time;
};
class comparator1{
public:
	bool operator()(process p1,process p2){
		if(p1.arrival_time>p2.arrival_time)
			return true;
		return false;
	}
};
class comparator2{
public:
	bool operator()(process p1,process p2){
		if(p1.burst_time>p2.burst_time)
			return true;
		return false;
	}
};
bool cmp(process p1,process p2){
	if(p1.name<p2.name)
		return true;
	return false;
}
void paintBlock(int xcor,int ycor,int CPUtime,string name);
void paintDetails(vector<process> v);
int main(){
	cout << "Enter number of processes: ";
	int n;
	cin >> n;
	vector<process> v;
	string name;
	while(n--){
		process p;
		cout << "Enter process name: ";
		cin >> p.name;
		cout << "Enter arrival time: ";
		cin >> p.arrival_time;
		cout << "Enter burst time: ";
		cin >> p.burst_time;
		v.push_back(p);
	}
	int choice;
	cout << "Select type of scheduling: \n1.FCFS\n2.SJF\n3.Round-Robin\n";
	cout << "Enter Choice: ";
	cin >> choice;
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(27,40,"Gantt Chart:");
    int xcor = 70,ycor = 170,side =60;
	if(choice==1){
		priority_queue<process,vector<process>, comparator1> q;
		for(int i=0;i<v.size();i++)
			q.push(v[i]);
		int CPUtime=0;
		v.clear();
		while(!q.empty()){
			process p = q.top();
			q.pop();
			if(CPUtime<p.arrival_time)
				CPUtime=p.arrival_time;
			p.waiting_time = (CPUtime-p.arrival_time);
			paintBlock(xcor,ycor,CPUtime,p.name);
			xcor+=side;
			p.turnAround_time = p.waiting_time+p.burst_time;
			CPUtime += p.burst_time;
			v.push_back(p);
		}
		cout << endl;
        sort(v.begin(),v.end(),cmp);
        paintDetails(v);
        getch();
        closegraph();
	}else if(choice==2){
		int type;
		cout << "1.Preemptive\n2.Non-preemptive\nEnter: ";
		cin >> type;

		if(type==2){
			priority_queue<process,vector<process>, comparator2> q;
			for(int i=0;i<v.size();i++)
				q.push(v[i]);
			int CPUtime=0;
			v.clear();
			while(!q.empty()){
				stack<process> s;
				int minPossibleArrival = 99999;
				while(!q.empty() && q.top().arrival_time>CPUtime){
					minPossibleArrival = min(minPossibleArrival,q.top().arrival_time);
					s.push(q.top());
					q.pop();
				}
				if(q.empty()){
					CPUtime = minPossibleArrival;
				}else{
					process p = q.top();
					q.pop();
					p.waiting_time = CPUtime - p.arrival_time;
                    paintBlock(xcor,ycor,CPUtime,p.name);
                    xcor+=side;
					CPUtime += p.burst_time;
					p.turnAround_time = p.waiting_time+p.burst_time;
					v.push_back(p);
				}
				while(!s.empty()){
					q.push(s.top());
					s.pop();
				}
			}
			cout << endl;
            sort(v.begin(),v.end(),cmp);
            paintDetails(v);
            getch();
		}else if(type==1){
			map<string,int> intialBurst;
			priority_queue<process,vector<process>, comparator1> arrival_q;
			priority_queue<process,vector<process>, comparator2> burst_q;
			for(int i=0;i<v.size();i++){
				v[i].waiting_time=0;
				arrival_q.push(v[i]);
				intialBurst[v[i].name] = v[i].burst_time;
			}
			v.clear();
			int CPUtime=0;
			while(!arrival_q.empty() || !burst_q.empty()){
				while(!arrival_q.empty() && arrival_q.top().arrival_time <= CPUtime){
					burst_q.push(arrival_q.top());
					arrival_q.pop();
				}
				if(!burst_q.empty()){
					process p = burst_q.top();
					burst_q.pop();
					p.waiting_time += (CPUtime - p.arrival_time);
					p.burst_time--;
                    paintBlock(xcor,ycor,CPUtime,p.name);
                    xcor+=side;
					if(p.burst_time!=0){
						p.arrival_time = CPUtime+1;
						burst_q.push(p);
					}else
						v.push_back(p);
				}
				cout << "	";
				CPUtime++;
			}
			cout << endl;
			for(int i=0;i<v.size();i++){
				v[i].turnAround_time = v[i].waiting_time + intialBurst[v[i].name];
			}
			sort(v.begin(),v.end(),cmp);
			paintDetails(v);
			getch();
		}
	}else if(choice==3){
		map<string,int> intialBurst;
		priority_queue<process,vector<process>, comparator1> arrival_q;
		queue<process> q;
		for (int i = 0; i < v.size(); ++i){
			v[i].waiting_time=0;
			arrival_q.push(v[i]);
			intialBurst[v[i].name] = v[i].burst_time;
		}
		int CPUtime =0;
		int TQ;
		cout << "Enter Time Quantum: ";
		cin >> TQ;
		v.clear();
		while(!q.empty() || !arrival_q.empty()){
			while(!arrival_q.empty() && arrival_q.top().arrival_time<=CPUtime){
				q.push(arrival_q.top());
				arrival_q.pop();
			}
			if(!q.empty()){
				if(q.front().burst_time!=0)
					q.push(q.front());
				else{
					v.push_back(q.front());
				}
				q.pop();
			}else{
				CPUtime++;
			}
			if(!q.empty()){
				process p = q.front();
                paintBlock(xcor,ycor,CPUtime,p.name);
                xcor+=side;
				q.front().waiting_time += (CPUtime - q.front().arrival_time);
				if(p.burst_time<TQ){
					CPUtime += p.burst_time;
					q.front().burst_time = 0;
				}else{
					q.front().burst_time -= TQ;
					q.front().arrival_time = CPUtime+TQ;
					CPUtime += TQ;
				}
			}
		}
		cout << endl;
		for(int i=0;i<v.size();i++){
			v[i].turnAround_time = v[i].waiting_time + intialBurst[v[i].name];
		}
		sort(v.begin(),v.end(),cmp);
		paintDetails(v);
		getch();
	}
	return 0;
}
void paintBlock(int xcor,int ycor,int CPUtime,string name){
    std::ostringstream ss;
    ss << CPUtime;
    int side = 60;
    line(xcor-side/2,ycor-side/2,xcor+side/2,ycor-side/2);
    line(xcor-side/2,ycor-side/2,xcor-side/2,ycor+side/2);
    line(xcor+side/2,ycor+side/2,xcor-side/2,ycor+side/2);
    line(xcor+side/2,ycor+side/2,xcor+side/2,ycor-side/2);
    outtextxy(xcor-10,ycor-10,(char *)name.c_str());
    outtextxy(xcor-side/2-5,ycor+side/2+10,(char *)(ss.str()).c_str());
}
void paintDetails(vector<process> v){
    outtextxy(27,300,"Process Name");
    outtextxy(250,300,"Waiting Time");
    outtextxy(473,300,"Turnaround Time");
    int ycor = 350;
    for(int i=0;i<v.size();i++){
        outtextxy(100-10,ycor,(char *)v[i].name.c_str());
        std::ostringstream ss;
        ss << v[i].waiting_time;
        outtextxy(323-20,ycor,(char *)(ss.str()).c_str());
        std::ostringstream ss1;
        ss1 << v[i].turnAround_time;
        outtextxy(546-30,ycor,(char *)(ss1.str()).c_str());
        ycor += 35;
    }
}
