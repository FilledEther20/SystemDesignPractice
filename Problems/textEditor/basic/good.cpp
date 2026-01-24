#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;
class DocumentElement{
public:
    virtual string render()=0;
};


class TextElement: public DocumentElement{
private:
    string text;

public:
    TextElement(string txt){
        this->text=txt;
    }

    string render() override{
        return text;
    }
};

class ImageElement: public DocumentElement{
private:
    string img;

public:
    ImageElement(string img){
        this->img=img;
    }

    
    string render() override{
        return "[Image:"+img+"]";
    }

};
class NewLineElement : public DocumentElement {
public:
    string render() override {
        return "\n";
    }
};

class Document{
private:
    vector<DocumentElement*> documentElements;

public:
    void addElement(DocumentElement* element){
        this->documentElements.push_back(element);
    }
    string render() {
        string result;
        for (auto element : documentElements) {
            result += element->render();
        }
        return result;
    }
};

class Persistence{
public:
    virtual void save(string data)=0;
};

class DBPersistence: public Persistence{
    void save(string data) override{
        // SAVED TO DB
    }
};

class FileStorage: public Persistence{
    void save(string data) override{
        ofstream outFile("document.txt");
        if (outFile) {
            outFile << data;
            outFile.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

class DocumentEditor {
private:
    Document* document;
    Persistence* storage;
    string renderedDocument;

public:
    DocumentEditor(Document* document, Persistence* storage) {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text) {
        document->addElement(new TextElement(text));
    }

    void addImage(string imagePath) {
        document->addElement(new ImageElement(imagePath));
    }

    // Adds a new line to the document.
    void addNewLine() {
        document->addElement(new NewLineElement());
    }

    string renderDocument() {
        if(renderedDocument.empty()) {
            renderedDocument = document->render();
        }
        return renderedDocument;
    }

    void saveDocument() {
        storage->save(renderDocument());
    }
};

int main() {
    Document* document = new Document();
    Persistence* persistence = new FileStorage();

    DocumentEditor* editor = new DocumentEditor(document, persistence);

    editor->addText("Hello, world!");
    editor->addNewLine();
    editor->addText("This is a real-world document editor example.");
    editor->addNewLine();
    editor->addText("Indented text after a tab space.");
    editor->addNewLine();
    editor->addImage("picture.jpg");

    cout << editor->renderDocument() << endl;

    editor->saveDocument();

    return 0;
}