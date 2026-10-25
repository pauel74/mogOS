#include <iostream>
#include <vector>
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

void search(Folder* currentDirectory, string searchFolder) {
    if(currentDirectory->name == searchFolder) {
        cout << "found: " << currentDirectory->name << " at " << currentDirectory << endl;
        return;
    }
    for(int i = 0; i<currentDirectory->children.size(); i++) {
        Folder* child = currentDirectory->children[i];
        search(child, searchFolder);
    }
}

int main() {
    Folder root;
    root.name = "/";

    mkdir(&root, "home");
    mkdir(root.children[0], "camera");
    mkdir(root.children[0], "music");

    search(&root, "/");
    search(&root, "school");
    search(&root, "games");
    search(&root, "camera");
    search(&root, "music");

    return 0;
}