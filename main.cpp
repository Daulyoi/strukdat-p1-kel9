#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <fstream>
#define cls system("cls");

using namespace std;

struct Task {
    string name;
    string assignee;
    string deadline;
    string status;
};

// Comparator untuk sorting berdasarkan deadline
bool compareByDeadline(const Task &a, const Task &b) {
    return a.deadline < b.deadline;
}

bool checkTaskToRemove(Task task, Task ref){
    return task.name == ref.name && task.assignee == ref.assignee && task.deadline == ref.deadline && task.status == ref.status;
}

class TaskManager {
    private:
        queue<Task> taskQueue;
        stack<Task> undoStack;
        vector<Task> taskList;
    
    public:
        void addTask(const string &name, const string &assignee, const string &deadline, const string &status = "Belum Selesai") {
            Task newTask = {name, assignee, deadline, status};
            taskQueue.push(newTask);
            taskList.push_back(newTask);
            undoStack.push(newTask);
            cout << "Task ditambahkan: " << name << " ditugaskan ke " << assignee << " (Deadline: " << deadline << ", Status: " << status << ")\n";
        }
    
        void undoLastTask() {
            if (!undoStack.empty()) {
                Task lastTask = undoStack.top();
                undoStack.pop();
                taskList.erase(remove_if(taskList.begin(), taskList.end(), [&](const Task task){return checkTaskToRemove(task, lastTask);}), taskList.end());
                cout << "Task terbaru telah di-undo: " << lastTask.name << "\n";
            } else {
                cout << "Tidak ada task untuk di-undo.\n";
            }
        }
    
        void sortTasks() {
            sort(taskList.begin(), taskList.end(), compareByDeadline);
            cout << "Tasks telah disortir berdasarkan deadline.\n";
        }
    
        void showTasks() {
            cout << "\nTask List:\n";
            for (const auto &task : taskList) {
                cout << "- " << task.name << " (" << task.assignee << ", Deadline: " << task.deadline << ", Status: " << task.status << ")\n";
            }
        }
    
        void completeTask(const string &taskName) {
            for (auto &task : taskList) {
                if (task.name == taskName) {
                    task.status = "Selesai";
                    cout << "Task telah selesai: " << task.name << "\n";
                    return;
                }
            }
            cout << "Task tidak ditemukan: " << taskName << "\n";
        }
    };

void printBanner() {
    cout << "========================================================================\n";
    cout << R"(
     _____         _      __  __                                   
    |_   _|_ _ ___| | __ |  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
      | |/ _` / __| |/ / | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
      | | (_| \__ \   <  | |  | | (_| | | | | (_| | (_| |  __/ |   
      |_|\__,_|___/_|\_\ |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                   |___/           
)";
    cout << "========================================================================\n";
}

void welcome(){
    printBanner();
    cout << "[1] Tambah Task\n";
    cout << "[2] Undo Task Terakhir\n";
    cout << "[3] Sort Tasks berdasarkan Deadline\n";
    cout << "[4] Tampilkan Tasks\n";
    cout << "[5] Selesaikan Task\n";
    cout << "[6] Import Tasks dari File\n";
    cout << "[7] Export Tasks ke File\n";
    cout << "[0] Exit\n";
    cout << "Tolong pilih sebuah opsi: ";
}

int main() {
    TaskManager manager;
    welcome();

    int option;

    while(true){
        cin >> option;
        if(option == 0){
            cls;
            cout << "Keluar Task Manager!\n";
            break;
        }else if(option == 1){
            cls;
            cout << "TAMBAH TASK\n";
            string name, assignee, deadline;
            cout << "Masukkan nama tugas: ";
            cin.ignore(); 
            getline(cin, name);
            cout << "Masukkan penerima tugas: ";
            getline(cin, assignee);
            cout << "Masukkan Deadline task menggunakan format (yyyy-mm-dd): ";
            getline(cin, deadline);
            manager.addTask(name, assignee, deadline);
            cout << "\n";
            welcome();
        }else if(option == 2){
            cls;
            cout << "UNDO TASK TERAKHIR\n";
            manager.undoLastTask();
            cout << "\n";
            welcome();
        }else if(option == 3){
            cls;
            cout << "SORT TASKS BERDASARKAN DEADLINE\n";
            manager.sortTasks();
            cout << "\n";
            welcome();
        }else if(option == 4){
            cls;
            cout << "TAMPILKAN TASKS\n";
            manager.showTasks();
            cout << "\n";
            welcome();
        }else if(option == 5){
            cls;
            cout << "SELESAIKAN TASK\n\n";
            string taskName;
            cout << "Masukkan nama task yang telah diselesaikan: ";
            cin.ignore();
            getline(cin, taskName);
            manager.completeTask(taskName);
            cout << "\n";
            welcome();
        } else if(option == 6){
            cls;
            cout << "IMPORT TASKS DARI FILE\n";
            cout << "\n";
            welcome();
        } else if(option == 7){
            cls;
            cout << "EXPORT TASKS KE FILE\n";
            cout << "\n";
            welcome();
        }
    }
    
    return 0;
}
