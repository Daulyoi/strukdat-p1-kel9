#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <fstream>
#define cls system("cls");

using namespace std;

#define RESET "\033[0m"
#define BLACK "\033[0;1m"
#define DARK_RED "\033[0;31m"
#define RED "\033[31;1m"
#define DARK_GREEN "\033[0;32m"
#define GREEN "\033[32;1m"
#define DARK_YELLOW "\033[0;33m"
#define YELLOW "\033[33;1m"
#define BLUE "\033[34;1m"
#define MAGENTA "\033[35;1m"
#define CYAN "\033[36;1m"
#define WHITE "\033[37;1m"

// Task 
struct Task {
    int id;
    string name;
    string deadline;
    string status;

    // Task Constructor
    Task(int _id, const string &_name, const string &_deadline, const string &_status) : id(_id), name(_name), deadline(_deadline), status(_status) {}
};


// Macam macam Operasi : Add, Delete, Complete, Import, Export
enum class Operation {
    ADD,
    DELETE,
    COMPLETE,
    IMPORT,
    EXPORT
};

// Struct action untuk menyimpan operasi yang dilakukan
struct Action {
    Operation type;
    Task task;
    
    Action(Operation _type, const Task& _task) : type(_type), task(_task) {}
};

// Helper Functions
// Returns the deadline of a < b as a boolean
bool compareByDeadline(const Task &a, const Task &b) {
    return a.deadline < b.deadline;
}

// Check if task is the same as reference task
bool checkTaskToRemove(Task task, Task ref){
    return task.name == ref.name && task.deadline == ref.deadline && task.status == ref.status;
}

// Check if deadline is valid
// Format: yyyy-mm-dd
bool checkDeadline(string dl){
    if(dl.length() != 10){
        return false;
    }
    string year = dl.substr(0, 4);
    string month = dl.substr(5, 2);
    string day = dl.substr(8, 2);
    if (year.length() != 4 || month.length() != 2 || day.length() != 2 || stoi(month) > 12 || stoi(day) > 31) {
        return false;
    }return true;
}

// Linked List for sorting tasks
class ListNode {
    public:
    Task task;
    ListNode* next;
    ListNode(const Task& t) : task(t), next(nullptr){};
};

// Insertion sort for sorting tasks in the Linked List
void insertToSortedList(ListNode*& head, Task newTask){
    ListNode* newNode = new ListNode(newTask);

    if (!head || compareByDeadline(newTask, head->task)) {
        newNode->next = head;
        head = newNode;
        return;
    }

    ListNode* current = head;
    while (current->next && !compareByDeadline(newTask, current->next->task)) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}


// Stack
class TaskStack {
    private:
        struct Node {
            Action ac;
            Node* next;
            Node(Action action, Node* n = nullptr) : ac(action), next(n) {}
        };
        Node* topNode;
    
    public:
        TaskStack() : topNode(nullptr) {}
    
        void push(Action action) {
            topNode = new Node(action, topNode);
        }
    
        bool empty() {
            return topNode == nullptr;
        }
    
        void pop() {
            if (topNode) {
                Node* temp = topNode;
                topNode = topNode->next;
                delete temp;
            }
        }
    
        Action& top() {
            if (!topNode) throw runtime_error("Stack is empty");
            return topNode->ac;
        }
    
        ~TaskStack() {
            while (!empty()) pop();
        }
    };
    
// Queue
class TaskQueue {
    private:
        struct Node {
            Task data;
            Node* next;
            Node(Task t) : data(t), next(nullptr) {}
        };
        Node *front, *rear;

        // Helper to deep copy from another TaskQueue
        void copyFrom(const TaskQueue& other) {
            Node* current = other.front;
            while (current) {
                push(current->data);
                current = current->next;
            }
        }

        // Helper to delete all nodes
        void clear() {
            while (!empty()) {
                pop();
            }
        }

    public:
        TaskQueue() : front(nullptr), rear(nullptr) {}

        // Deep Copy Constructor
        TaskQueue(const TaskQueue& other) : front(nullptr), rear(nullptr) {
            copyFrom(other);
        }

        // Assignment Operator
        TaskQueue& operator=(const TaskQueue& other) {
            if (this != &other) {
                clear(); // Clear existing data
                copyFrom(other); // Copy data from the other queue
            }
            return *this;
        }

        void push(const Task &task) {
            Node* newNode = new Node(task);
            if (!rear) {
                front = rear = newNode;
            } else {
                rear->next = newNode;
                rear = newNode;
            }
        }
    
        bool empty() {
            return front == nullptr;
        }
    
        void pop() {
            if (!front) return;
            Node* temp = front;
            front = front->next;
            if (!front) rear = nullptr;
            delete temp;
        }
    
        Task& frontTask() {
            if (!front) {
                throw runtime_error("Queue is empty. No valid task to return.");
            } else {
                return front->data;
            }
        }
    
        ~TaskQueue() {
            clear();
        }
    };
    
class TaskManager {
    private:
        int id;
        TaskQueue TQ;
        TaskStack US;
        
    public:
        TaskManager() : id(0) {}

        void addTask(const string &name, const string &deadline, const string &status = "Belum Selesai") {
            Task newTask(id++, name, deadline, status);
            TQ.push(newTask);

            US.push(Action(Operation::ADD, newTask));

            cout << "Task ditambahkan: " << name << " (Deadline: " << deadline << ", Status: " << status << ")\n";
        }
    
        bool removeTask(const string &taskName) {
            TaskQueue tempQueue;
            bool found = false;
    
            while (!TQ.empty()) {
                Task currentTask = TQ.frontTask();
                TQ.pop();
    
                if (currentTask.name == taskName && !found) {
                    found = true;
                    US.push(Action(Operation::DELETE, currentTask));
                    cout << "Task dihapus: " << currentTask.name << "\n";
                } else {
                    tempQueue.push(currentTask);
                }
            }

            // Restore the remaining tasks back to the original queue
            while (!tempQueue.empty()) {
                TQ.push(tempQueue.frontTask());
                tempQueue.pop();
            }
    
            if (!found) {
                cout << "Task tidak ditemukan: " << taskName << "\n";
            }
            return found;
        }

        void showSortedTasks() {
            TaskQueue tempQueue = TQ;
            ListNode* head = nullptr;
        
            if(tempQueue.empty()){
                cout << "Tidak ada task\n";
                return;
            }

            while (!tempQueue.empty()) {
                insertToSortedList(head, tempQueue.frontTask());
                tempQueue.pop();
            }
        
            ListNode* current = head;
            while (current) {
                Task& task = current->task;
                cout << "- " << task.name << " (Deadline: " << task.deadline << ", Status: " << task.status << ")\n";
                ListNode* toDelete = current;
                current = current->next;
                delete toDelete; // clean up memory
            }
        }
    
        void showTasks() {
            TaskQueue tempQueue = TQ;

            if(tempQueue.empty()){
                cout << "Tidak ada task\n";
                return;
            }

            while (!tempQueue.empty()) {
                Task currentTask = tempQueue.frontTask();
                tempQueue.pop();
                cout << "- " << currentTask.name << " (Deadline: " << currentTask.deadline << ", Status: " << currentTask.status << ")\n";
            }
        }
    
        bool completeTask(const string &taskName) {
            while (!TQ.empty()) {
                Task currentTask = TQ.frontTask();
                TQ.pop();
                
                if (currentTask.name == taskName) {
                    currentTask.status = "Selesai";
                    US.push(Action(Operation::COMPLETE, currentTask));
                    cout << "Task telah selesai: " << currentTask.name << "\n";
                    return true;
                } else {
                    TQ.push(currentTask);
                }
            }
            cout << "Task tidak ditemukan: " << taskName << "\n";
            return false;
        }

        void importTasks(){
            ifstream file("tasks.txt");
            
            if (!file) {
                cout << "File tidak ditemukan.\n";
                return;
            }

            string line;
            while (getline(file, line)) {
                size_t firstDelim = line.find(';');
                size_t secondDelim = line.find(';', firstDelim + 1);

                string name = line.substr(0, firstDelim);
                string deadline = line.substr(firstDelim + 1, secondDelim - firstDelim - 1);
                string status = line.substr(secondDelim + 1);

                checkDeadline(deadline);
                if (status.empty()) {
                    status = "Belum Selesai";
                }

                if(status == "Selesai"){
                    continue;
                }

                Task newTask(id++, name, deadline, status);
                TQ.push(newTask);
            }
            cout << "Tasks berhasil diimport dari file:\n";
        }

        void exportTasks(){
            ofstream file("tasks.txt");
            if (!file) {
                cout << "File tidak dapat dibuat.\n";
                return;
            }

            if(TQ.empty()){
                cout << "Tidak ada task untuk di export.\n";
                return;
            }

            TaskQueue tempQueue = TQ;
            while (!tempQueue.empty()) {
                Task currentTask = tempQueue.frontTask();
                tempQueue.pop();
                file << currentTask.name << ";" << currentTask.deadline << ";" << currentTask.status << "\n";
            }
            cout << "Tasks berhasil diekspor ke file tasks.txt\n";
        }

        //The TaskManager::undoLastAction method attempts to undo the last action performed on the task manager by analyzing the type of operation (e.g., add, delete, complete, import, export) stored in the undo stack and reverting its effects accordingly. If the undo stack is empty or the operation type is unsupported, it provides appropriate feedback and returns false.
        void undoLastAction() {
            if (US.empty()) {
                cout << "Tidak ada task untuk di-undo.\n";
                // return false;
                return;
            }

            Action lastAction = US.top();
            US.pop();

            switch (lastAction.type) 
            {
                case Operation::ADD :
                {
                    TaskQueue tempQueue;
                    bool taskRemoved = false;
                
                    while (!TQ.empty()) {
                        Task currentTask = TQ.frontTask();
                        TQ.pop();
                
                        // Check for the task to be removed
                        if (!taskRemoved &&
                            currentTask.id == lastAction.task.id &&
                            currentTask.deadline == lastAction.task.deadline) 
                        {
                            taskRemoved = true;
                            cout << "Task telah dihapus: " << currentTask.name << "\n";
                            continue; // Skip adding this task to the tempQueue
                        }
                
                        // Keep all other tasks
                        tempQueue.push(currentTask);
                    }
                
                    // Restore the preserved tasks
                    TQ = tempQueue;
                
                    if (!taskRemoved) {
                        cout << "Task tidak ditemukan untuk dihapus.\n";
                        // return false;
                        return;
                    }
                
                    // return true;
                    return;
                    break;                
                }
            
            case Operation::DELETE: {
                TQ.push(lastAction.task);
                cout << "Task ditambahkan kembali: " << lastAction.task.name << "\n";
                // return true;
                return;
                break;
            }

            case Operation::COMPLETE: {
                while (!TQ.empty()) {
                    Task currentTask = TQ.frontTask();
                    TQ.pop();
                    
                    if (currentTask.id == lastAction.task.id && currentTask.deadline == lastAction.task.deadline) {
                        currentTask.status = "Belum Selesai";
                        TQ.push(currentTask);
                        cout << "Task ditandai sebagai belum selesai: " << currentTask.name << "\n";
                        // return true;
                        return;
                    }
                }
                break;
            }
                
            case Operation::IMPORT: {
                TQ.empty();
                return;
                break;
            }

            case Operation::EXPORT: {

                cout << "Fitur Export belum di implementasi.\n";
                // return true;
                return;
                break;
            }
                
            default:{

                // return false;
                return;
                break;
                }
            }
        }
    };

void printBanner() {
    cout << CYAN << "========================================================================\n";
    cout << YELLOW << R"(
     _____         _      __  __                                   
    |_   _|_ _ ___| | __ |  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
      | |/ _` / __| |/ / | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
      | | (_| \__ \   <  | |  | | (_| | | | | (_| | (_| |  __/ |   
      |_|\__,_|___/_|\_\ |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                   |___/           
)";
    cout << CYAN << "========================================================================\n\n";
    cout << RESET;
}

void printSeparator(){
    cout << WHITE;  
    cout << "------------------------------------------------------------------------\n";
    cout << RESET;
}

void welcome(){
    cout << "[1] Tambah Task\n";
    cout << "[2] Undo Aksi Terakhir\n";
    cout << "[3] Tampilkan Task berdasarkan Deadline\n";
    cout << "[4] Tampilkan Tasks\n";
    cout << "[5] Selesaikan Task\n";
    cout << "[6] Import Tasks dari File\n";
    cout << "[7] Export Tasks ke File\n";
    cout << "[0] Exit\n";
    cout << "Tolong pilih sebuah opsi: ";
}

int main() {
    TaskManager manager;
    printBanner();
    welcome();

    int option;

    bool isRunning = true;

    while(isRunning){
        cin >> option;

        switch (option)
        {
        case 0:
        {
            cls;
            cout << "Keluar Task Manager!\n";
            isRunning = false;
            break;
        }
        case 1:
        {
            cls;
            printBanner();
            cout << "TAMBAH TASK\n";
            string name, deadline;
            cout << "Masukkan nama tugas: ";
            cin.ignore(); 
            getline(cin, name);
            cout << "Masukkan Deadline task menggunakan format (yyyy-mm-dd): ";
            getline(cin, deadline);
            
            if(checkDeadline(deadline)){
                manager.addTask(name, deadline);
                cout << "\n";
            }else {
                cout << "Format tanggal tidak valid. Harap masukkan dalam format (yyyy-mm-dd).\n";
            }
            printSeparator();
            welcome();
            break;
        }
        case 2:
        {
            cls;
            printBanner();
            cout << "UNDO TASK TERAKHIR\n";
            manager.undoLastAction();
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        case 3:
        {
            cls;
            printBanner();
            cout << "SORT TASKS BERDASARKAN DEADLINE\n";
            manager.showSortedTasks();
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        case 4:
        {
            cls;
            printBanner();
            cout << "TAMPILKAN TASKS\n";
            manager.showTasks();
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        case 5:
        {
            cls;
            printBanner();
            cout << "DAFTAR TASK\n";
            manager.showTasks();
            cout << "SELESAIKAN TASK\n\n";
            string taskName;
            cout << "Masukkan nama task yang telah diselesaikan: ";
            cin.ignore();
            getline(cin, taskName);
            manager.completeTask(taskName);
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        case 6:
        {
            cls;
            printBanner();
            cout << "IMPORT TASKS DARI FILE\n";
            manager.importTasks();
            manager.showTasks();
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        case 7:
        {
            cls;
            printBanner();
            cout << "EXPORT TASKS KE FILE\n";
            manager.exportTasks();
            cout << "\n";
            printSeparator();
            welcome();
            break;
        }
        default:
        {
            cls;
            printBanner();
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            printSeparator();
            welcome();
            break;
        }
        }
    }
    
    return 0;
}
