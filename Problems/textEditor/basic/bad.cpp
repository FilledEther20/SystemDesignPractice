#include<iostream>
#include<string>   
#include<fstream> 
using namespace std;

// Everything loaded in a single class DocumentEditor 
class DocumentEditor{
private:
    vector<string> documents;
    string renderedDoc;

public:
    void addText(string st){
        documents.push_back(st);
    }

    void addImage(string path){
        documents.push_back(path);
    }

    string renderer(){
        if(renderedDoc.empty()){
            string result;
            for(auto &element:documents){
                if (element.size() > 4 && (element.substr(element.size() - 4) == ".jpg" ||
                 element.substr(element.size() - 4) == ".png")) {
                    result += "[Image: " + element + "]" + "\n";
                } else {
                    result += element + "\n";
                }
            }
            renderedDoc=result;
        }
        return renderedDoc;
    }
    void saveToFile(){
         ofstream file("document.txt");
        if (file.is_open()) {
            file << renderer();
            file.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

int main(){
    DocumentEditor editor;
    editor.addText("Hello, world!");
    editor.addImage("picture.jpg");
    editor.addText("This is a document editor.");

    cout << editor.renderer() << endl;

    editor.saveToFile();
    
    return 0;
}