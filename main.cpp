#include <iostream>
#include <fstream>
#include <string>

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

HNode* getNode(Node x){
    HNode *p = new HNode;
    if(p == NULL){
        cout << "can't create HNode...";
        return NULL;
    }
    p->data = x;
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

int main()
{
    // int vars
    unsigned int f[256];
    for(int i = 0; i < 256; i++) {
        f[i] = 0;
    }
    ifstream inputFile;
    ofstream codedChars("codeChar.txt", ios::out | ios::binary);
    ofstream compressedFile;
    string comORnot;
    string fileName;
    int choice;

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

            inputFile.open(name, ios::in | ios::binary);

            if(!inputFile) {
                cout << "Cannot open file...";
                return 0;
            }

            char c;
            while(!inputFile.eof()) {
                inputFile.read(&c, 1);
                f[(int)c]++;
            }

            int co2 = 0;
            for(int i = 0; i < 256; i++){
                if(f[i] != 0) {
                    co2++;
                }
            }

            Node *huffNode = new Node[co2];
            int k_huff = 0;

            for(int i = 0; i < 256; i++){
                if(f[i] != 0) {
                    huffNode[k_huff].c = char(i);
                    huffNode[k_huff].f = f[i];
                    huffNode[k_huff].pLeft = NULL;
                    huffNode[k_huff].pRight = NULL;
                    k_huff++;
                }
            }

            // sort

            for(int i = 0; i < k_huff; i++){
                for(int j = i + 1; j < k_huff; j++){
                    if(huffNode[i].f > huffNode[j].f){
                        Node temp = huffNode[j];
                        huffNode[j] = huffNode[i];
                        huffNode[i] = temp;
                    }
                }
            }

            /*
            for(int i = 0; i < k_huff; i++){
                cout << huffNode[i].c << ": " << huffNode[i].f << endl;
            }
            */

            // make huffman tree
            int min_1 = -1, min_21= -1;
            Node **huffNode_temp = new Node*[k_huff];
            int m_huffNode_temp = 0;

            for(int i = 0; i < k_huff; i++){
                huffNode_temp[i] = new Node;
            }
            //==========================================
            Node *no1 = new Node;
            no1->c = '$';
            no1->f = huffNode[0].f + huffNode[1].f;
            no1->pLeft = &huffNode[0];
            no1->pRight = &huffNode[1];

            //==========================================

            for(int i = 0; i < m_huffNode_temp; i++){
                // find 2 mins in array
                if(m_huffNode_temp == 1){
                    min_1 = min_2 = huffNode_temp[0]->f;
                }
                else if(m_huffNode_temp == 2){
                    min_1 = huffNode_temp[0]->f;
                    min_2 = huffNode_temp[1]->f;
                }
                else {
                    if(huffNode_temp[i]->f > min_1)
                }
            }

            huffNode_temp[m_huffNode_temp] = no1;
            m_huffNode_temp++;

            Node* head = no1;
            for(int i = 2; i < k_huff; i++){
                Node *t = new Node;
                t->c = '$';
                t->f = head->f + huffNode[i].f;
                t->pLeft = &huffNode[i];
                t->pRight = head;
                head = t;
            }

            // create code

            string code = "";
            while(true) {
                if(head->pLeft->c != '$' && head->pRight->c == '$') {
                    codedChars << head->pLeft->c << ": " << code << "0" << endl;
                }
                code += "1";
                Node *del = head;

                if(head->pLeft->c != '$' && head->pRight->c != '$') {
                    codedChars << head->pLeft->c << ": " << code << "0" << endl;
                    codedChars << head->pRight->c << ": " << code << "1" << endl;
                    break;
                }
                head = head->pRight;
                delete del;
            }

            for(int i = 0; i < co2; i++){
                delete huffNode_temp[i];
            }

            delete[] huffNode_temp;
            delete no1;
            delete huffNode;
            delete name;
            break;
        }
        case 2: {
            cout << comORnot << " " << fileName;
            break;
        }
    }

    inputFile.close();
    codedChars.close();
    compressedFile.close();

    cout << endl << endl << endl;

    return 0;
}
