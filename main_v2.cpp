#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <stdlib.h>

using namespace std;

struct Node {
    unsigned char c;
    unsigned int f;
    Node *pLeft, *pRight;
};

struct HNode {
    Node data;
    HNode *pNext;
};

struct Queue {
    HNode *pHead, *pTail;
};

void Init(Queue &q){
    q.pHead = q.pTail = NULL;
}

HNode* getNode(Node* x){
    HNode *p = new HNode;
    if(p == NULL){
        cout << "can't create HNode...";
        return NULL;
    }
    p->data.c = x->c;
    p->data.f = x->f;
    p->data.pLeft = x->pLeft;
    p->data.pRight = x->pRight;
    p->pNext = NULL;
    return p;
}

bool isEmpty(Queue q){
    if(q.pHead == NULL){
        return false;
    }
    return true;
}

void AddTail(Queue &q, HNode *p){
    if(isEmpty(q) == false){
        q.pHead = q.pTail = p;
    }
    else {
        q.pTail->pNext = p;
        q.pTail = p;
    }
}

void AddHead(Queue &q, HNode *p){
    if(isEmpty(q) == false){
        q.pHead = q.pTail = p;
    }
    else {
        p->pNext = q.pHead;
        q.pHead = p;
    }
}

void AddAfterNode(Queue &q, HNode *x) {
    if(q.pHead == NULL) {
            q.pHead = q.pTail = x;
            return;
    }
    if(x->data.f < q.pHead->data.f) {
        AddHead(q, x);
        return;
    }
    else if(x->data.f > q.pTail->data.f) {
        AddTail(q, x);
        return;
    }

    HNode* temp;
    for(HNode* i = q.pHead; i != NULL; i = i->pNext){
        if(i->data.f > x->data.f) {
            x->pNext = i;
            temp->pNext = x;
            return;
        }
        temp = i;
    }
}

Node PopTop(Queue &q) {
    Node temp = q.pHead->data;
    HNode *t = q.pHead;
    q.pHead = q.pHead->pNext;
    delete t;
    return temp;
}

void freeQueue(Queue &q){
    HNode *p;
    while(q.pHead != NULL){
        p = q.pHead;
        q.pHead = q.pHead->pNext;
        delete p;
    }
}

void saveCode(Node* head, string c, string table[]) {
    if(head != NULL){
            //    http://www.cplusplus.com/forum/general/51192/
            //    http://stackoverflow.com/questions/18937892/c-string-to-binary-code-binary-code-to-string
            //    http://stackoverflow.com/questions/13823656/how-to-convert-char-to-an-array-of-bits-in-c
            //    http://stackoverflow.com/questions/23537053/storing-a-bit-in-a-bit-of-character-array-in-c-linux
            //    http://stackoverflow.com/questions/3289065/how-to-store-8-bits-in-char-using-c
            //    http://stackoverflow.com/questions/26348566/is-there-a-way-to-convert-string-to-binary-data-to-reduce-size
            //    http://stackoverflow.com/questions/19229561/storing-a-two-bit-binary-in-char
        if(head->c != '$'){
            cout << head->c << "\t" << head->f << endl;
            //char *pr;
            //long int neu = strtol(c.c_str(), &pr, 2);
            table[(int)head->c] = c;
        }
        saveCode(head->pLeft, c + "0", table);
        saveCode(head->pRight, c + "1", table);
    }
}

string readFile(ifstream &input, char* name){
    input.open(name, ifstream::binary);
    if(input){
        input.seekg(0, input.end);
        int length = input.tellg();
        input.seekg(0, input.beg);

        char *buffer = new char[length];
        input.read(buffer, length);

        input.close();

        string t(buffer);
        delete buffer;
        return t;
    }
    return NULL;
}

int main()
{
    unsigned int f[256];
    for(int i = 0; i < 256; i++) {
        f[i] = 0;
    }

    string code[256] = {""};

    ifstream input;
    ofstream codedChars("codeChar.txt", ios::out | ios::binary);
    ofstream compressedFile;
    string comORnot;
    string fileName;
    int choice;
    Queue q;
    Init(q);

    // input data
    while(true){
        cout << "Input function to do with file (compress or uncompress): ";
        cin >> comORnot;
        if(comORnot == "compress" || comORnot == "uncompress") {
            break;
        }
        cout << "\nFucntion does not exist in this project" << endl;
    }


    cout << endl;

    cout << "Input file name without space: ";
    cin >> fileName;

    // select function to do with file
    if(comORnot == "compress") {
        choice = 1;
    }
    else if(comORnot == "uncompress"){
        choice = 2;
    }

    //chau-san, aishiteiru, tsuma ni narimasu ka?

    switch (choice) {
        case 1: {
            // read binary file
            char *name = new char[fileName.length()];

            for(int i = 0; i < fileName.length(); i++){
                name[i] = fileName[i];
            }

            string str_temp = readFile(input, name);

            for(int i = 0; i < str_temp.length(); i++){
                f[(int)str_temp[i]]++;
            }

            int co2 = 0;
            for(int i = 0; i < 256; i++){
                if(f[i] != 0) {
                    co2++;
                }
            }

            Node **huffNode = new Node*[co2];
            int k_huff = 0;

            for(int i = 0; i < co2; i++) {
                huffNode[i] = new Node;
            }

            for(int i = 0; i < 256; i++){
                if(f[i] != 0) {
                    huffNode[k_huff]->c = char(i);
                    huffNode[k_huff]->f = f[i];
                    huffNode[k_huff]->pLeft = NULL;
                    huffNode[k_huff]->pRight = NULL;
                    HNode *tempHuff = getNode(huffNode[k_huff]);
                    AddAfterNode(q, tempHuff);
                    k_huff++;
                }
            }

//            for(HNode *i = q.pHead; i != NULL; i = i->pNext){
//                cout << i->data.c << ": " << i->data.f << endl;
//            }

            //create huffman tree

            while(true){
                Node t1 = PopTop(q);
                Node *no1 = new Node;
                no1->c = t1.c;
                no1->f = t1.f;
                no1->pLeft = t1.pLeft;
                no1->pRight = t1.pRight;
                Node t2 = PopTop(q);
                Node *no2 = new Node;
                no2->c = t2.c;
                no2->f = t2.f;
                no2->pLeft = t2.pLeft;
                no2->pRight = t2.pRight;
                Node *no3 = new Node;
                no3->c = '$';
                no3->f = no1->f + no2->f;
                no3->pLeft = no1;
                no3->pRight = no2;
                HNode* noH = getNode(no3);
                AddAfterNode(q, noH);
//                cout << endl << endl << endl;
//                for(HNode *i = q.pHead; i != NULL; i = i->pNext){
//                    cout << i->data.c << ": " << i->data.f << endl;
//                }
                if(q.pHead == q.pTail){
                    break;
                }
            }

            cout << endl << endl << endl;

            Node *head = &q.pHead->data;
            string c = "";
            saveCode(head, c, code);

            cout << endl << endl << endl;
            for(int i = 0; i < 256; i++){
                if(code[i] != ""){
                    cout << (char)i << ": " << code[i] << endl;
                }
            }

            string str_temp_encoded = "";

            for(int i = 0; i < str_temp.length(); i++){
                str_temp_encoded += code[(int)str_temp[i]];
            }

            cout << endl << endl << endl;
            cout << str_temp_encoded;

            for (size_t i = 0; i < str_temp_encoded.size(); ++i){
                codedChars << bitset<8>(str_temp_encoded.c_str()[i]);
            }

            for(int i = 0; i < co2; i++){
                delete huffNode[i];
            }

            delete[] huffNode;
            delete name;
            break;
        }
        case 2: {
            cout << comORnot << " " << fileName;
            break;
        }
    }

    codedChars.close();
    compressedFile.close();
    freeQueue(q);

    cout << endl << endl << endl;

    return 0;
}
