#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
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
    cout << "\ncommands:\n-pwd (show current dir)\n-mkdir (create new dir)\n-cd (change dir)\n-rm (remove dir)\n-ls (list content of dir)\n-mv (move (and rename) dir)\n-tree (show filesystem structure)\n-clear (clear the screen)\n-exit (exit mogOs)\n\n";
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

void mkdir(Folder* currentDirectory, string name) {
    if(name == "/") {
        cout << "cant create second root" << endl;
        return;
    }
    if(search(currentDirectory, name) != nullptr) {
        cout << "\"" << name <<"\" already exists\n";
        return;
    }
    Folder* folder = new Folder; // create object called folder and returns pointer
    folder->name = name;
    folder->parent = currentDirectory;
    currentDirectory->children.push_back(folder);
    cout << "created \"" << name <<"\" successfully\n";
}


Folder* mv(Folder* currentDirectory, string name, string targetLocation, string newName) {

    Folder* targetPtr = search(currentDirectory, name);

    if(targetPtr == nullptr) {
        cout << "\"" << name << "\" was not found\n";
        return currentDirectory;
    }

    // create root pointer
    Folder* root = currentDirectory;
    while(root->parent != nullptr) {
        root = root->parent;
    }

    Folder* targetPosition = search(root, targetLocation);

    if(targetPosition == nullptr) {
        cout << "\"" << targetLocation << "\" was not found\n";
        return currentDirectory;
    }

    // dont move into itself
    Folder* temp = targetPosition;
    while(temp->name != "/") {
        if(temp == targetPtr) {
            cout << "Cannot move folder into itself\n";
            return currentDirectory;
        }
        temp = temp->parent;
    }

    // dont rename
    if(newName == "keep") {
        newName = name;
    }

    Folder* oldParent = targetPtr->parent;

    for(int i = 0; i < oldParent->children.size(); i++) {
        if(oldParent->children[i] == targetPtr) {
            oldParent->children.erase(oldParent->children.begin() + i);
            break;
        }
    }

    targetPtr->parent = targetPosition;
    targetPtr->name = newName;
    targetPosition->children.push_back(targetPtr);

    cout << "Moved \"" << name << "\" into \"" << newName << "\" successfully\n";

    return currentDirectory;
}

Folder* cd(Folder* currentDirectory, string moveTo) {
    int dotInStr = 0;
    for(char c : moveTo) {
        if(c == '.') {
            dotInStr++;
        }
    }
    if(moveTo == "/") {
        while(currentDirectory->name != "/") {
            currentDirectory = currentDirectory->parent;
        }
    }
    if(dotInStr>0) {
        dotInStr /= 2;
        for(int i = 0; i<dotInStr; i++) {
            if(currentDirectory->name == "/") {
                cout << "cant move over root" << endl;
                return currentDirectory;
            }
            currentDirectory = currentDirectory->parent;
        }
    }
    else if(search(currentDirectory, moveTo) == nullptr) {
        cout << "\"" << moveTo <<"\" was not found\n";
        return currentDirectory;
    }
    else {
        return search(currentDirectory, moveTo);
    }
    return currentDirectory;
}

void ls(Folder* currentDirectory, string name) {
    if(name != "") {
        if(search(currentDirectory, name) == nullptr) {
            cout << "\"" << name <<"\" was not found\n";
            return;
        }
        else {
            currentDirectory = search(currentDirectory, name);
        }
    }
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        cout << currentDirectory->children[i]->name << " ";
        if(i ==  currentDirectory->children.size()-1) {
            cout << endl;
        }
    }
}

void tree(Folder* currentDirectory, int depth) {
    if(depth == 0) {
        while(currentDirectory->name != "/") {
            currentDirectory = currentDirectory->parent;
        }
        cout << "/root" << endl;
    }
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        for(int j = 0; j<depth+1; j++) {
            cout << "  ";
        }
        cout << "└─ " << currentDirectory->children[i]->name << endl;
        tree(currentDirectory->children[i], depth+1);
    }
}

Folder* input(Folder* currentDirectory) {
    cout << "\033[0m";
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

    cout << "\033[35m";

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
            if(command.size() == 1) {
                ls(currentDirectory, "");
            }
            else if(command.size() == 2) {
                ls(currentDirectory, command[1]);
            }
            else {
                cout << "ls expects either zero or one argument\n"; 
            }
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
        else if(command[0] == "mv") {

            if(command.size() != 3 && command.size() != 4) {
                cout << "mv expects either two or three arguments\n";
                return currentDirectory;
            }

            string name = command[1];
            string target = command[2];
            string newName;
            if(command.size() == 3) {
                newName = "keep";
            }
            else {newName = command[3];}

            currentDirectory = mv(currentDirectory, name, target, newName);
        }
        else if(command[0] == "tree") {
            vector<Folder> content; // empty
            tree(currentDirectory, 0);
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
    root.parent = nullptr;

    mkdir(&root, "home");
    mkdir(root.children[0], "Documents");
    mkdir(root.children[0], "Music");
    mkdir(root.children[0], "Pictures");
    mkdir(root.children[0], "Videos");
    mkdir(root.children[0], "Downloads");

    Folder* currentDirectory = root.children[0];

    system("clear");
    cout << R"(
    ⠀⠀⠀ ⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⡼⠙⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠃⢹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢰⠃⠀⠘⣿⡄⠀⠀⠀⠀⠀⠀⢀⣿⡇⠀⠘⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡎⠀⠀⠀⢹⣿⣆⠀⠀⠀⠀⠀⣼⣿⠁⠀⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⠞⢻⣿⡆⠀⠀⠀⢰⣿⣿⡀⠀⠀⢹⡄⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡇⠀⢰⠋⠀⠈⣿⣿⡄⠀⠀⣾⣿⡇⠹⡄⠀⢨⡇⠀⠀
⠀⠀⠀⠀⠀⠀⡇⢀⡏⠀⢀⣴⣿⣿⣿⣿⣾⣿⣿⣧⡀⢳⠀⢸⡁⠀
⠀⠀⠀⠀⠀⠀⢣⠸⣅⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⡀⣸⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠘⣦⣿⣿⡿⠛⠻⢿⣿⣿⣿⣿⡟⠉⠙⢿⣿⣇⠀⠀⠀⣀⣠⡄⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣿⣿⡿⠀⠀⣿⡟⣿⣿⣿⣿⢸⣷⠀⠈⣿⣿⣤⠶⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠚⠉⠉⠉⠙⠒⠲⣿⣿⣷⠀⠀⠙⢡⣿⣿⣽⣿⣌⠁⠀⣰⣿⣿⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣠⠽⢿⣿⣿⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⠀⠀⠉⠑⠒⠠⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣀⠴⠚⠉⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⡍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧)" << endl;

    cout << "\n\033[35mwelcome to mogOS\033[0m\ntype \"help\" for help\n\n";
    
    while(true) {
        Folder* result = input(currentDirectory);
        if(result == nullptr) {
            break;
        }
        currentDirectory = result;
    }
    
    return 0;
}
