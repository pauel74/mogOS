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
    cout << "\ncommands:\n-mkdir\n-ls\n-cd\n-clear\n\n";
}

void mkdir(Folder* currentDirectory, string name) {
    Folder* folder = new Folder; // create object called folder and returns pointer
    folder->name = name;
    folder->parent = currentDirectory;
    currentDirectory->children.push_back(folder);
}

Folder* rm(Folder* currentDirectory, string name) {
    for(int i = 0; i < currentDirectory->children.size(); i++) {
        if(currentDirectory->children[i]->name == name) {
            delete currentDirectory->children[i];
            //erase!
        }
    }
    return currentDirectory;
}

Folder* cd(Folder* currentDirectory, string moveTo) {
    Folder* address = search(currentDirectory, moveTo);
    if(moveTo == "..") {
        return currentDirectory->parent;
    }
    if(address != nullptr) {
        return address;
    }
    else {
        return nullptr;
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

    getline(cin, input);
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
        else if(command[0] == "ls") {
            ls(currentDirectory);
        }
        else if(command[0] == "mkdir") {
            mkdir(currentDirectory, command[1]);
        }
        else if(command[0] == "cd") {
            if(currentDirectory->name == "/" && command[1] == "..") {
                cout << "cant move over root" << endl;
                return currentDirectory;
            }
            currentDirectory = cd(currentDirectory, command[1]);
            return currentDirectory;
        }
        else if(command[0] == "rm") {
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
    while(true) {
        currentDirectory = input(currentDirectory);
    }

    return 0;
}