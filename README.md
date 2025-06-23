# File System Navigator

A simple command-line file system navigator written in C++ that simulates basic file system operations in memory.

## What is this program?

This program creates a virtual file system that exists only in your computer's memory (RAM). You can create directories, files, navigate between folders, and search for items - just like using a real file system, but everything is temporary and disappears when you close the program.

Think of it like a sandbox where you can practice file system commands without affecting your real files!

## Features

- **Create directories** - Make new folders
- **Create files** - Make new empty files  
- **Navigate directories** - Move between folders
- **List contents** - See what's in the current folder
- **Find files/folders** - Search for items by name
- **Show current location** - Display your current path

## How to Build and Run

### Prerequisites
- A C++ compiler (like g++, Visual Studio, or MinGW)
- C++14 standard support or higher

### Compilation
Open a terminal/command prompt in the project directory and run:

```bash
g++ -std=c++14 -o navigator.exe navigator.cpp
```

### Running the Program
```bash
./navigator.exe
```

## Available Commands

Once the program starts, you can use these commands:

| Command | Description | Example |
|---------|-------------|---------|
| `ls` | List contents of current directory | `ls` |
| `pwd` | Print current directory path | `pwd` |
| `mkdir <name>` | Create a new directory | `mkdir photos` |
| `touch <name>` | Create a new empty file | `touch document.txt` |
| `cd <path>` | Change to a different directory | `cd photos` |
| `find <name>` | Search for files/directories | `find document.txt` |
| `help` | Show command list | `help` |
| `exit` | Exit the program | `exit` |

## Usage Examples

### Basic Navigation
```
fs/> ls                    # List current directory contents
home/

fs/> cd home              # Go into the home directory
fs/home> pwd              # Show current path
/home

fs/home> ls               # List contents of home directory
user/
readme.txt
```

### Creating Files and Directories
```
fs/> mkdir projects       # Create a new directory called "projects"
fs/> cd projects          # Go into the projects directory
fs/projects> touch app.cpp # Create a new file called "app.cpp"
fs/projects> ls           # List contents
app.cpp
```

### Advanced Navigation
```
fs/projects> cd ..        # Go up one directory (to parent)
fs/> cd /                 # Go to root directory
fs/> cd /home/user        # Go to specific path
fs/home/user> 
```

### Searching
```
fs/> find readme.txt      # Find all files named "readme.txt"
/home/readme.txt

fs/> find user            # Find all items named "user"
/home/user
```

## Sample Directory Structure

When you start the program, it creates this sample structure for you to explore:

```
/
├── home/
│   ├── readme.txt
│   └── user/
│       ├── Documents/
│       │   └── report.docx
│       ├── Downloads/
│       └── profile.txt
```

## How It Works (Technical Details)

### For Beginners
The program uses a tree-like structure to represent folders and files:
- Each folder can contain other folders and files
- Files are like leaves on a tree (they can't contain anything)
- Folders are like branches (they can contain other items)

### For Programmers
- **Data Structure**: Uses a tree of `Node` objects, each representing a file or directory
- **Memory Management**: Uses C++ smart pointers (`std::unique_ptr`) for automatic memory cleanup
- **Navigation**: Implements path parsing and traversal algorithms
- **Design Pattern**: Follows object-oriented design with encapsulation

### Key Classes
- `Node`: Represents a single file or directory
- `FileSystem`: Manages the entire file system and operations
- Helper functions handle common tasks like path validation and navigation

## Limitations

- **Temporary**: Everything is lost when you exit the program
- **No file content**: Files are empty placeholders (no actual content storage)
- **Memory only**: Nothing is saved to your real hard drive
- **Simple paths**: No support for complex path operations like `~` (home directory)
- **No permissions**: No file permission system implemented

## Educational Value

This program is great for:
- Learning basic file system concepts
- Understanding tree data structures
- Practicing command-line interfaces
- Exploring C++ memory management
- Understanding how real file systems work at a basic level

## Possible Extensions

You could enhance this program by adding:
- File content storage and editing
- File permissions (read/write/execute)
- File size simulation
- Copy and move operations
- Save/load file system state to disk
- Tab completion for commands
- Command history

## Troubleshooting

**Program crashes when using pipes (like `echo "ls" | ./navigator.exe`)**
- This is now fixed! The program properly handles EOF and exits gracefully.

**Can't create files/directories with certain names**
- Names cannot contain the '/' character (this is by design)

**"Path not found" errors**
- Make sure the directory exists before trying to navigate to it
- Use `ls` to see what's available in the current directory

## Contributing

Feel free to modify and extend this program! Some ideas:
- Add new commands
- Improve error messages
- Add file content support
- Create a GUI version

---

**Happy navigating!** 🚀
