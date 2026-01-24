#include<iostream>
#include<vector>
#include<string>

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
    TextElement(string img){
        this->img=img;
    }

    string render() override{
        return "[Image:"+text+"]";
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

    // Adds a tab space to the document.
    void addTabSpace() {
        document->addElement(new TabSpaceElement());
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
    editor->addTabSpace();
    editor->addText("Indented text after a tab space.");
    editor->addNewLine();
    editor->addImage("picture.jpg");

    cout << editor->renderDocument() << endl;

    editor->saveDocument();

    return 0;
}