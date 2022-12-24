//
// Report1: BST 삽입, 삭제 구현 [삽입: 성공, 삭제: 성공]
// 20181667 이예슬
// 실행 환경: macOS 12.6, g++ 컴파일
//
// Created by yesslee31 on 2022/10/07.
//
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

struct Node { // node
public:
    int key;
    int height;
    Node *left;
    Node *right;
};

class BST { // tree
public:
    Node *root;

    BST() {
        root = nullptr;
    }
    ~BST() {
        delete root;
    }
};

Node *getNode() {
    Node *newNode = new Node();
    newNode->height = 0;
    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

void insertBST(BST *T, int newKey) {
    Node *p = T->root;
    Node *q = nullptr;
    stack<Node*> s;

    // find position to insert newKey while storing parent node on stack
    while (p != nullptr) {
        if (newKey == p->key) { // case: key is already exists
            cout << "i " << p->key << " : The key already exists" << endl;
            return;
        }

        q = p;
        s.push(q);

        if (newKey < p->key) p = p->left;
        else p = p->right;
    }

    // create new node
    Node *newNode = getNode();
    newNode->key = newKey;

    // insert newNode as a child of q
    if (T->root == nullptr) T->root = newNode;
    else if (newKey < q->key) q->left = newNode;
    else q->right = newNode;

    // update height while popping parent node from stack
    while (!s.empty()) {
        s.pop();
        if ((q->left != nullptr) && (q->right != nullptr))
            q->height = 1 + max(q->left->height, q->right->height);
    }
}

int height(Node *subT) { // subtree's height
    return subT->height;
}

int noNodes(Node *subT) { // subtree's total number of nodes
    if (subT != nullptr)
        return (1 + noNodes(subT->left) + noNodes(subT->right));
    else return 0;
}

Node *maxNode(Node *leftST, stack<Node*> &stk) { // left subtree's max key value node == rightmost node
    Node *p = leftST;

    while(p->right != nullptr) {
        stk.push(p);
        p = p->right;
    }
    return p;
}

Node *minNode(Node *rightST, stack<Node*> &stk) { // right subtree's min key value node = leftmost node
    Node *p = rightST;

    while(p->left != nullptr) {
        stk.push(p);
        p = p->left;
    }
    return p;
}

void deleteBST(BST *T, int deleteKey) {
    Node *p = T->root;
    Node *q = nullptr;
    stack<Node*> s;

    // find position of deleteKey while storing parent node on stack
    while ((p != nullptr) && (deleteKey != p->key)) {
        q = p;
        s.push(q);

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    if (p == nullptr) { // deleteKey was not found
        cout << "d " << deleteKey << " : The key does not exist" << endl;
        return;
    }

    // 조교님 의사코드 베이스로 작성한 코드. 강의 슬라이드(p의 차수가 2인 경우 보완) 참고해서 함수로 대체할 것
    if ((p->left != nullptr) && (p->right != nullptr)) { // case of degree 2 is reduced to case of degree 0 or case of degree 1
        s.push(p);
        Node* tempNode = p;

        /* 조교님 의사코드로 작성한 부분
        if (p->left->height <= p->right->height) { // 좌측 서브트리의 높이와 우측 서브트리의 높이가 같거나 우측이 높은 경우
            p = p->right;
            while (p->left != nullptr) { // 우측 서브트리의 최소값 노드를 찾는다
                s.push(p);
                p = p->left;
            }
        }
        else { // 좌측 서브트리의 높이가 우측 서브트리보다 높은 경우
            p = p->left;
            while (p->right != nullptr) { // 죄측 서브트리의 최대값 노드를 찾는다
                s.push(p);
                p = p->right;
            }
        }
        */

        // 위 내용을 함수로 대체
        if (height(p->left) > height(p->right)) // 좌측 서브트리의 높이가 높은 경우
            p = maxNode(p->left, s); // 좌측 서브트리의 최대값 노드를 찾는다
        else if (height(p->left) < height(p->right)) // 우측 서브트리의 높이가 높은 경우
            p = minNode(p->right, s); // 우측 서브트리의 최소값 노드를 찾는다
        else { // case: left height == right height
            if (noNodes(p->left) >= noNodes(p->right)) // 노드의 개수를 비교하여 좌측 서브트리의 노드 개수가 많거나 같은 경우
                p = maxNode(p->left, s); // 좌측 서브트리의 최대값 노드를 찾는다
            else // case: noNodes(p->left) < noNodes(p->right)
                p = minNode(p->right, s); // 우측 서브트리의 최소값 노드를 찾는다
        }

        tempNode->key = p->key;
        q = s.top();
    }

    // now degree of p is 0 or 1
    // delete p from T
    if ((p->left == nullptr) && (p->right == nullptr)) { // case of degree 0
        if (q == nullptr) T->root = nullptr; // case of root
        else if (q->left == p) q->left = nullptr;
        else q->right = nullptr;
    }
    else { // case of degree 1
        if (p->left != nullptr) {
            if (q == nullptr) T->root = T->root->left; // case of root
            else if (q->left == p) q->left = p->left;
            else q->right = p->left;
        }
        else {
            if (q == nullptr) T->root = T->root->right; // case of root
            else if (q->left == p) q->left = p->right;
            else q->right = p->right;
        }
    }

    delete p;

    // update height while popping parent node from stack
    while (!s.empty()) {
        s.pop();
        if ((q->left != nullptr) && (q->right != nullptr))
            q->height = 1 + max(q->left->height, q->right->height);
    }
}

void inorderN(Node *node) {
    if (node == nullptr) return;
    else {
        inorderN(node->left);
        cout << node->key << ' ';
        inorderN(node->right);
    }
}

void inorderBST(BST *tree) {
    if (tree->root == nullptr) return;
    else {
        inorderN(tree->root);
        cout << endl;
    }
}

int main() {
    ifstream inputfile;
    string path;

    string optstr;
    int inputkey;

    BST *t = new BST();
    /*
    // CLion에서 실행시 파일 경로지정
    filesystem::path p("../BST-input.txt");
    path = filesystem::canonical(p);
    inputfile.open(path);
    */

    inputfile.open("BST-input.txt");
    if (inputfile.fail()) {
        cerr << "file can't found" << endl;
        exit(1);
    }
    else {
        while (!inputfile.eof()) {
            inputfile >> optstr >> inputkey;

            if (inputfile.eof()) break;

            if (optstr == "i") insertBST(t, inputkey);
            if (optstr == "d") deleteBST(t, inputkey);
            inorderBST(t);
        }
        inputfile.close();
        t->~BST();

        return 0;
    }
}
