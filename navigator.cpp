// FileSystemNavigator.cpp

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include <memory>

// Enum to distinguish between files and directories
enum class NodeType {
    FILE,
    DIRECTORY
};

// Forward declaration of FileSystem class for the Node's find_helper
class FileSystem;

// Represents a single node (file or directory) in the file system tree
class Node {
public:
    std::string name;
    NodeType type;
    Node* parent;
    std::map<std::string, std::unique_ptr<Node>> children; // Only used by directories

    // Constructor
    Node(const std::string& name, NodeType type, Node* parent = nullptr)
        : name(name), type(type), parent(parent) {}

    // Destructor: Memory management is now handled by unique_ptr automatically
    ~Node() = default;
};

// The main class that manages the file system operations
class FileSystem {
private:
    std::unique_ptr<Node> root;
    Node* currentDirectory;

    // Helper function to split a path string by '/'
    std::vector<std::string> splitPath(const std::string& path) {
        std::vector<std::string> parts;
        std::stringstream ss(path);
        std::string part;
        while (std::getline(ss, part, '/')) {
            if (!part.empty()) {
                parts.push_back(part);
            }
        }
        return parts;
    }

    // Helper function to check if a name is valid (no '/' characters)
    bool isValidName(const std::string& name) {
        return name.find('/') == std::string::npos;
    }

    // Helper function to check if a name already exists in current directory
    bool nameExists(const std::string& name) {
        return currentDirectory->children.count(name) > 0;
    }

    // Helper function to navigate to a node by path parts
    Node* navigateToPath(const std::vector<std::string>& parts, Node* startNode) {
        Node* targetNode = startNode;
        
        for (const auto& part : parts) {
            if (part == "..") {
                if (targetNode->parent != nullptr) {
                    targetNode = targetNode->parent;
                }
            } else if (part != ".") {
                if (targetNode->type != NodeType::DIRECTORY) {
                    return nullptr; // Not a directory
                }
                if (targetNode->children.count(part)) {
                    Node* child = targetNode->children[part].get();
                    if (child->type == NodeType::DIRECTORY) {
                        targetNode = child;
                    } else {
                        return nullptr; // Not a directory
                    }
                } else {
                    return nullptr; // Path not found
                }
            }
        }
        return targetNode;
    }

    // Helper for the 'find' command (recursive traversal)
    void find_helper(Node* startNode, const std::string& targetName, std::vector<std::string>& results) {
        // Check if the current node's name matches the target
        if (startNode->name == targetName) {
            results.push_back(getPath(startNode));
        }

        // If it's a directory, recurse on its children
        if (startNode->type == NodeType::DIRECTORY) {
            for (auto it = startNode->children.begin(); it != startNode->children.end(); ++it) {
                find_helper(it->second.get(), targetName, results);
            }
        }
    }


public:
    FileSystem() {
        // The root directory has no parent
        root = std::make_unique<Node>("/", NodeType::DIRECTORY, nullptr);
        currentDirectory = root.get();
    }

    ~FileSystem() {
        // Smart pointers handle cleanup automatically
    }
      // Get the full path of a given node
    std::string getPath(Node* node) {
        if (node == root.get()) {
            return "/";
        }
        // Recurse up to the parent and build the path backwards
        if (node->parent == root.get()) {
            return "/" + node->name;
        }
        return getPath(node->parent) + "/" + node->name;
    }


    // Print Working Directory (pwd)
    void pwd() {
        std::cout << getPath(currentDirectory) << std::endl;
    }    // List contents (ls)
    void ls() {
        for (auto it = currentDirectory->children.begin(); it != currentDirectory->children.end(); ++it) {
            std::cout << it->first;
            if (it->second->type == NodeType::DIRECTORY) {
                std::cout << "/";
            }
            std::cout << std::endl;
        }
    }

    // Make Directory (mkdir)
    void mkdir(const std::string& dirName) {
        if (!isValidName(dirName)) {
            std::cout << "Error: Directory name cannot contain '/'." << std::endl;
            return;
        }
        if (nameExists(dirName)) {
            std::cout << "Error: '" << dirName << "' already exists." << std::endl;
        } else {
            auto newDir = std::make_unique<Node>(dirName, NodeType::DIRECTORY, currentDirectory);
            currentDirectory->children[dirName] = std::move(newDir);
        }
    }
    
    // Create a file (touch)
    void touch(const std::string& fileName) {
        if (!isValidName(fileName)) {
            std::cout << "Error: File name cannot contain '/'." << std::endl;
            return;
        }
        if (nameExists(fileName)) {
            std::cout << "Error: '" << fileName << "' already exists." << std::endl;
        } else {
            auto newFile = std::make_unique<Node>(fileName, NodeType::FILE, currentDirectory);
            currentDirectory->children[fileName] = std::move(newFile);
        }
    }
    // Change Directory (cd)
    void cd(const std::string& path) {
        if (path == "/") {
            currentDirectory = root.get();
            return;
        }

        Node* targetNode = (path[0] == '/') ? root.get() : currentDirectory;
        std::vector<std::string> parts = splitPath(path);

        targetNode = navigateToPath(parts, targetNode);
        if (targetNode != nullptr) {
            currentDirectory = targetNode;
        } else {
            std::cout << "Error: Invalid path '" << path << "'." << std::endl;
        }
    }    // Find a file or directory by name
    void find(const std::string& name) {
        std::vector<std::string> results;
        find_helper(root.get(), name, results);

        if (results.empty()) {
            std::cout << "No file or directory named '" << name << "' found." << std::endl;
        } else {
            for (const auto& path : results) {
                std::cout << path << std::endl;
            }
        }
    }

    // Get the current directory node (for prompt display)
    Node* getCurrentDirectory() {
        return currentDirectory;
    }
};

// --- Main function to run the command-line interface ---
void show_help() {
    std::cout << "File System Navigator Commands:\n"
              << "  ls          - List contents of the current directory\n"
              << "  mkdir <name>- Create a new directory\n"
              << "  touch <name>- Create a new empty file\n"
              << "  cd <path>   - Change directory (e.g., 'cd /', 'cd ..', 'cd my_folder')\n"
              << "  pwd         - Print the current working directory path\n"
              << "  find <name> - Search for a file or directory from the root\n"
              << "  help        - Show this help message\n"
              << "  exit        - Exit the navigator\n"
              << std::endl;
}

int main() {
    FileSystem fs;
    std::string line;
    std::string command;
    std::string argument;

    // Create a sample directory structure for demonstration
    fs.mkdir("home");
    fs.cd("home");
    fs.mkdir("user");
    fs.touch("readme.txt");
    fs.cd("user");
    fs.mkdir("Documents");
    fs.mkdir("Downloads");
    fs.touch("profile.txt");
    fs.cd("Documents");
    fs.touch("report.docx");
    fs.cd("/"); // Go back to root

    std::cout << "Welcome to the C++ File System Navigator!" << std::endl;
    show_help();    while (true) {
        std::cout << "fs" << fs.getPath(fs.getCurrentDirectory()) << "> ";
        std::cout.flush(); // Ensure prompt is displayed immediately
        
        if (!std::getline(std::cin, line)) {
            // EOF reached (e.g., when using echo | program) or input error
            std::cout << std::endl; // Print newline for clean exit
            break;
        }
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        std::stringstream ss(line);
        ss >> command >> argument;

        if (command == "exit") {
            break;
        } else if (command == "pwd") {
            fs.pwd();
        } else if (command == "ls") {
            fs.ls();
        } else if (command == "mkdir") {
            if (argument.empty()) std::cout << "Usage: mkdir <name>" << std::endl;
            else fs.mkdir(argument);
        } else if (command == "touch") {
            if (argument.empty()) std::cout << "Usage: touch <name>" << std::endl;
            else fs.touch(argument);
        } else if (command == "cd") {
            if (argument.empty()) std::cout << "Usage: cd <path>" << std::endl;
            else fs.cd(argument);
        } else if (command == "find") {
            if (argument.empty()) std::cout << "Usage: find <name>" << std::endl;
            else fs.find(argument);
        } else if (command == "help") {
            show_help();
        } else if (!command.empty()) {
            std::cout << "Unknown command: '" << command << "'. Type 'help' for a list of commands." << std::endl;
        }
        
        command.clear();
        argument.clear();
    }

    std::cout << "Exiting File System Navigator." << std::endl;
    return 0;
}