#include <iostream>
#include <string>
#include <stack>
#include <vector>

// Define a Task structure
struct Task {
    int id;
    std::string description;
    bool completed;
    
    Task(int _id, const std::string& _desc) : 
        id(_id), description(_desc), completed(false) {}
};

// Define possible operations for undo functionality
enum class Operation {
    ADD,
    REMOVE,
    COMPLETE
};

// Define an Action structure to track changes for undo
struct Action {
    Operation type;
    Task task;
    
    Action(Operation _type, const Task& _task) : 
        type(_type), task(_task) {}
};

class TaskManager {
private:
    std::vector<Task> tasks;
    std::stack<Action> undoStack;
    int nextId;
    
public:
    TaskManager() : nextId(1) {}
    
    // Add a new task
    void addTask(const std::string& description) {
        Task newTask(nextId++, description);
        tasks.push_back(newTask);
        
        // Add this action to the undo stack
        undoStack.push(Action(Operation::ADD, newTask));
        
        std::cout << "Task added: " << description << std::endl;
    }
    
    // Remove a task by id
    bool removeTask(int id) {
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (it->id == id) {
                // Push to undo stack before removing
                undoStack.push(Action(Operation::REMOVE, *it));
                
                tasks.erase(it);
                std::cout << "Task #" << id << " removed" << std::endl;
                return true;
            }
        }
        std::cout << "Task #" << id << " not found" << std::endl;
        return false;
    }
    
    // Mark a task as completed
    bool completeTask(int id) {
        for (auto& task : tasks) {
            if (task.id == id) {
                // Save the current state to the undo stack before changing
                undoStack.push(Action(Operation::COMPLETE, task));
                
                task.completed = true;
                std::cout << "Task #" << id << " marked as completed" << std::endl;
                return true;
            }
        }
        std::cout << "Task #" << id << " not found" << std::endl;
        return false;
    }
    
    // Undo the most recent action
    bool undo() {
        if (undoStack.empty()) {
            std::cout << "Nothing to undo" << std::endl;
            return false;
        }
        
        // Get the last action
        Action lastAction = undoStack.top();
        undoStack.pop();
        
        // Reverse the action based on its type
        switch (lastAction.type) {
            case Operation::ADD:
                // Remove the added task
                for (auto it = tasks.begin(); it != tasks.end(); ++it) {
                    if (it->id == lastAction.task.id) {
                        tasks.erase(it);
                        std::cout << "Undid addition of task #" << lastAction.task.id << std::endl;
                        return true;
                    }
                }
                break;
                
            case Operation::REMOVE:
                // Add back the removed task
                tasks.push_back(lastAction.task);
                std::cout << "Undid removal of task #" << lastAction.task.id << std::endl;
                return true;
                
            case Operation::COMPLETE:
                // Revert completion status
                for (auto& task : tasks) {
                    if (task.id == lastAction.task.id) {
                        task.completed = false;
                        std::cout << "Undid completion of task #" << lastAction.task.id << std::endl;
                        return true;
                    }
                }
                break;
        }
        
        std::cout << "Failed to undo action" << std::endl;
        return false;
    }
    
    // Display all tasks
    void displayTasks() const {
        if (tasks.empty()) {
            std::cout << "No tasks available" << std::endl;
            return;
        }
        
        std::cout << "\n----- TASK LIST -----" << std::endl;
        for (const auto& task : tasks) {
            std::cout << "Task #" << task.id << ": " << task.description 
                      << " [" << (task.completed ? "Completed" : "Pending") << "]" << std::endl;
        }
        std::cout << "--------------------\n" << std::endl;
    }
};

int main() {
    TaskManager manager;
    
    // Example usage
    manager.addTask("Finish coding assignment");
    manager.addTask("Buy groceries");
    manager.addTask("Call dentist");
    manager.displayTasks();
    
    manager.completeTask(2);
    manager.displayTasks();
    
    manager.removeTask(3);
    manager.displayTasks();
    
    // Undo last operation (removing task #3)
    manager.undo();
    manager.displayTasks();
    
    // Undo one more operation (completing task #2)
    manager.undo();
    manager.displayTasks();
    
    // Undo another operation (adding task #3)
    manager.undo();
    manager.displayTasks();
    
    return 0;
}