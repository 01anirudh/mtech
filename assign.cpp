#include <bits/stdc++.h>
using namespace std;

struct Assignment {
    int id, input1, input2, outcome;
    string food_name;
};

unordered_map<string, int> food_cost;  
vector<int> books;                     
vector<int> final_outcome;             
vector<Assignment> assignments;        
int group_size;                        


void parseInput(const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    string line;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '%') continue;  
        stringstream ss(line);
        if (line[0] == 'C') {
            char c;
            string food;
            int value;
            ss >> c >> food >> value;
            food_cost[food] = value;
        } else if (line[0] == 'G') {
            char c;
            ss >> c >> group_size;
        } else if (line[0] == 'I') {
            char c;
            int book;
            ss >> c;
            while (ss >> book && book != -1) {
                books.push_back(book);
            }
        } else if (line[0] == 'O') {
            char c;
            int outcome;
            ss >> c;
            while (ss >> outcome && outcome != -1) {
                final_outcome.push_back(outcome);
            }
        } else if (line[0] == 'A') {
            char c;
            int id, input1, input2, outcome;
            string food_name;
            ss >> c >> id >> input1 >> input2 >> outcome >> food_name;
            assignments.push_back({id, input1, input2, outcome, food_name});
        }
    }

    f.close();
}


vector<vector<int>> scheduleAssignments() {
    unordered_map<int, vector<int>> adj;        
    unordered_map<int, int> indegree;           
    unordered_map<int, Assignment> id_map;      


    for (const auto& a : assignments) {
        if (a.input1 > 0) {
            adj[a.input1].push_back(a.id);
            indegree[a.id]++;
        }
        if (a.input2 > 0) {
            adj[a.input2].push_back(a.id);
            indegree[a.id]++;
        }
        if (indegree.find(a.id) == indegree.end()) {
            indegree[a.id] = 0;  
        }
        id_map[a.id] = a;
    }

    
    queue<int> q;
    for (const auto& pair : indegree) {
        if (pair.second == 0) {  
            q.push(pair.first);
        }
    }

    vector<vector<int>> schedule;
    vector<int> day_assignments;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        day_assignments.push_back(node);
        if (day_assignments.size() == group_size) {
            schedule.push_back(day_assignments);
            day_assignments.clear();
        }


        for (int neighbor : adj[node]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if (!day_assignments.empty()) schedule.push_back(day_assignments);  
    return schedule;
}
void displaySchedule(const vector<vector<int>>& schedule) {
    if (schedule.empty()) {
        cout << "No schedule available!" << endl;
        return;
    }

    cout << "\nSchedule:\n";
    for (size_t i = 0; i < schedule.size(); ++i) {
        cout << "Day " << i + 1 << ": ";
        unordered_map<string, int> daily_menu;

        
        for (int id : schedule[i]) {
            cout << "A" << id << " ";
            string food = assignments[id - 1].food_name;  
            daily_menu[food]++;
        }
        cout << "\nMenu: ";
        for (const auto& pair : daily_menu) {
            string food = pair.first;
            int count = pair.second;
            cout << count << "-" << food << " ";
        }
        cout << endl;
    }
}

int main() {
    
    parseInput("input.txt");


    cout << "Assignments:\n";
    for (const auto& a : assignments) {
        cout << "A" << a.id << ": " << a.food_name << " (" << a.input1 << ", " << a.input2 << ")\n";
    }

    vector<vector<int>> schedule = scheduleAssignments();

    cout << "\nGenerated Schedule:\n";
    for (size_t i = 0; i < schedule.size(); ++i) {
        cout << "Day " << i + 1 << ": ";
        for (int id : schedule[i]) {
            cout << "A" << id << " ";
        }
        cout << endl;
    }

    displaySchedule(schedule);

    return 0;
}
