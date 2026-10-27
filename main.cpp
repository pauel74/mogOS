#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
using namespace std;

struct Folder {
    string name;
    vector<Folder*> children;
    Folder* parent;
};

Folder* search(Folder* currentDirectory, string searchFolder) {
    if(currentDirectory->name == searchFolder) {
        return currentDirectory;
    }
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        Folder* child = currentDirectory->children[i];

        Folder* result = search(child, searchFolder);

        if(result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

void help() {
    cout << "\ncommands:\n-pwd\n-mkdir\n-cd\n-rm\n-ls\n-clear\n-exit\n\n";
}

void pwd(Folder* currentDirectory) {
    if(currentDirectory->name == "/") {
    cout << "/" << endl;
    return;
    }
    
    vector<string> path;
    
    while(currentDirectory->name != "/") {
        path.push_back(currentDirectory->name);
        currentDirectory = currentDirectory->parent;
    }
    
    for(int i = 0; i < path.size(); i++) {
        cout << "/" << path[(path.size() -1 -i)];
    }
    
    cout << endl;
}

void mkdir(Folder* currentDirectory, string name) {
    Folder* folder = new Folder; // create object called folder and returns pointer
    folder->name = name;
    folder->parent = currentDirectory;
    currentDirectory->children.push_back(folder);
    cout << "created \"" << name <<"\" successfully\n";
}

Folder* rm(Folder* currentDirectory, string name) {
    for(int i = 0; i < currentDirectory->children.size(); i++) {
        Folder* child = currentDirectory->children[i];
        if(currentDirectory->children[i]->name == name) { 
            while(!child->children.empty()) { // deletes all childs from memory
                rm(child, child->children[0]->name);
            }
            delete child;
            currentDirectory->children.erase( // cant just remove i because it expects iterator
                currentDirectory->children.begin() +i
            );
            cout << "removed \"" << name <<"\" successfully\n"; 
            return currentDirectory;
        }
    }
    cout << "\"" << name <<"\" was not found\n"; 
    return currentDirectory;
}

Folder* cd(Folder* currentDirectory, string moveTo) {
    if(moveTo == "..") {
        return currentDirectory->parent;
    }
    else if(search(currentDirectory, moveTo) == nullptr) {
        cout << "\"" << moveTo <<"\" was not found\n";
        return currentDirectory;
    }
    else {
        return search(currentDirectory, moveTo);
    }
}

void ls(Folder* currentDirectory) {
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        cout << currentDirectory->children[i]->name << " ";
        if(i ==  currentDirectory->children.size()-1) {
            cout << endl;
        }
    }
}

Folder* input(Folder* currentDirectory) {
    string input;
    cout << currentDirectory->name << " > ";

    if(!getline(cin, input)) {
        return nullptr;
    }
    stringstream ss(input);
    string part;
    vector<string> command;

    command.clear();

    while(getline(ss, part, ' ')) {
        command.push_back(part);
    }

    if(command.size() != 0) {
        if(command[0] == "clear") {
            system("clear");
        }
        else if(command[0] == "pwd") {
            pwd(currentDirectory);
        }
        else if(command[0] == "exit") {
            return nullptr;
        }
        else if(command[0] == "ls") {
            ls(currentDirectory);
        }
        else if(command[0] == "mkdir") {
            if(command.size() != 2) {
                cout << "mkdir expects exactly one argument\n";
                return currentDirectory;
            }
            mkdir(currentDirectory, command[1]);
        }
        else if(command[0] == "cd") {
            if(command.size() != 2) {
                cout << "cd expects exactly one argument\n";
                return currentDirectory;
            }
            else if(currentDirectory->name == "/" && command[1] == "..") {
                cout << "cant move over root" << endl;
                return currentDirectory;
            }
            currentDirectory = cd(currentDirectory, command[1]);
            return currentDirectory;
        }
        else if(command[0] == "rm") {
            if(command.size() != 2) {
                cout << "rm expects exactly one argument\n";
                return currentDirectory;
            }
            currentDirectory = rm(currentDirectory, command[1]);
        }
        else if(command[0] == "help") {
            help();
        }
        else {
            cout << "command \"" << command[0] << "\" not found" << endl;
        }
    }
    return currentDirectory;
}

int main() {
    Folder root;
    root.name = "/";

    mkdir(&root, "home");
    mkdir(root.children[0], "Documents");
    mkdir(root.children[0], "Music");
    mkdir(root.children[0], "Pictures");
    mkdir(root.children[0], "Videos");
    mkdir(root.children[0], "Downloads");

    Folder* currentDirectory = root.children[0];

    system("clear");
    cout << "welcome to mogOS\ntype \"help\" for help\n\n";
    
    while(true) {
        Folder* result = input(currentDirectory);
        if(result == nullptr) {
            break;
        }
        currentDirectory = result;
    }
    
    return 0;
}