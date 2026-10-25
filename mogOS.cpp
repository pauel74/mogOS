#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
using namespace std;

struct Folder {
    string name;
    vector<Folder*> children;
};

void mkdir(Folder* currentDirectory, string name) {
    Folder* folder = new Folder; // create object called folder and returns pointer
    folder->name = name;
    currentDirectory->children.push_back(folder);
}

Folder* search(Folder* currentDirectory, string searchFolder) {
    if(currentDirectory->name == searchFolder) {
        cout << "found: " << currentDirectory->name << " at " << currentDirectory << endl;
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

void ls(Folder* currentDirectory) {
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        cout << currentDirectory->children[i]->name << " ";
    }
    cout << endl;
}

void input(Folder* currentDirectory) {
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
        else {
            cout << "command \"" << command[0] << "\" not found" << endl;
        }
    }

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

    while(true) {
        input(currentDirectory);
    }

    return 0;
}