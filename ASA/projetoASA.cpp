#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <iterator>
#include <algorithm>
using namespace std;

class StudentNetwork {
	int num_students, visited;
	int *d, *low, *grades;
	list<int> *graph;
public:
	StudentNetwork(int num) {
		d = new int[num];
		low = new int[num];
		grades = new int[num];

		graph = new list<int>[num];
		num_students = num;
	}

    int getGrade(int id){
        return this->grades[id];
    }

	void assignGrade(int id, int grade){
		grades[id] = grade;
	}

	void assignRelation(int src, int dest) {
		graph[src-1].push_back(dest-1);
	}
	
	void tarjanVisit(stack<int> *L, int u, bool *inStack) {
		int v;
		d[u] = low[u] = visited++;
		L->push(u);
		inStack[u] = true;

		list<int> :: iterator it;
		
		for (it = graph[u].begin(); it != graph[u].end(); ++it) {
			v = *it;
			if (d[v] == -1 || inStack[v] == true) {
				if (d[v] == -1){
					tarjanVisit(L, v, inStack);
				}
				low[u] = min(low[u], low[v]);
			}
		}

		if (d[u] == low[u]){
			list<int> SCC;
			int max_grade = 0;
			do {
				v = L->top();							
				L->pop();
				inStack[v] = false; 					
				
				for (it = graph[v].begin(); it != graph[v].end(); ++it){	//check for connection between SCCs
					if (low[*it] != low[v])									//if yes, obtain the SCC with the highest grade
						max_grade = max(max_grade, grades[*it]);
				}
				max_grade = max(max_grade, grades[v]);						//obtain the highest grade in the SCC
				SCC.push_back(v);						
			}
			while (u != v);

			for (it = SCC.begin(); it != SCC.end(); ++it) {	//change the grades of the SCC to the highest value
				grades[*it] = max_grade;
			}
		}
	}
	
	void tarjanAlgorythm() {
		visited = 0;
		stack<int> *L = new stack<int>;
		bool *inStack = new bool[num_students];

		for (int i = 0; i < num_students; i++){
			d[i] = -1;
			low[i] = -1;
		}
		
		for (int u = 0; u < num_students; u++){
			if (d[u] == -1)
				tarjanVisit(L, u, inStack);
		}
	}
};
	
int main(){
	int num_students, num_friendships, i, grade;

	//gather graph info
	cin >> num_students;
	cin.ignore();
	cin >> num_friendships;
	StudentNetwork network = StudentNetwork(num_students);
	
	//gather grades for each students 
	for (i = 0; i < num_students; i++){
		cin >> grade;
		network.assignGrade(i, grade);
	}

	//gather relationships and form the graph
	for (i = 0; i < num_friendships; i++){
		int src, dest;
		cin >> src;
		cin.ignore();
		cin >> dest;
		network.assignRelation(src, dest);
	}
	
	//change grades accordingly
	network.tarjanAlgorythm();

	//display grades
	for (i = 0; i < num_students; i++){
		cout << network.getGrade(i)<< endl;
	}

	return 0;
}
