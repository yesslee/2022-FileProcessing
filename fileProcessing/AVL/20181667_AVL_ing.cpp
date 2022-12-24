//
// Created by yesslee31 on 2022/10/31.
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
    int bf; // balance factor
    Node *left;
    Node *right;
};

// 전역변수들
stack<Node*> s;
bool isExist = false; // key 중복삽입 시 rotationType 출력 안하기 위한 flag

class AVL { // tree
public:
    Node *root;

    AVL() {
        root = nullptr;
    }
    ~AVL() {
        delete root;
    }
};

Node *getAVLNode() {
    Node *newNode = new Node();
    newNode->height = 1;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->bf = 0; // add balance factor based getNode() function

    return newNode;
}

// 트리 회전에 의해 변경된 balance factor와 그에 따른 height 업데이트하는 함수
// 기존 부분에서 추가적으로 bf 업데이트 필요함에 따라 기존 구조 변경하여 적용
Node *update_H_BF(Node *n) {
    if (n == nullptr)
        return n;

    if ((n->left != nullptr) || (n->right != nullptr)) {
        if (n->right == nullptr) { // have left subtree
            n->height = 1 + n->left->height;
            n->bf = n->left->height;
        }
        else if (n->left == nullptr) { // have right subtree
            n->height = 1 + n->right->height;
            n->bf = 0 - n->right->height;
        }
        else {
            n->height = 1 + max(n->left->height, n->right->height);
            n->bf = n->left->height - n->right->height;
        }
    }
    else if ((n->left == nullptr) && (n->right == nullptr)) { // root case
        n->height = 1;
        n->bf = 0;
    }
    return n;
}

void insertBST(AVL *T, int newKey) {
    Node *p = T->root;
    Node *q = nullptr;

    // find position to insert newKey while storing parent node on stack
    while (p != nullptr) {
        if (newKey == p->key) { // case: key is already exists
            isExist = true;
            cout << "i " << p->key << " : The key already exists" << endl;
            return;
        }

        q = p;
        s.push(q);

        if (newKey < p->key) p = p->left;
        else p = p->right;
    }

    // create new node
    Node *y = getAVLNode();
    y->key = newKey;

    // insert y as a child of q
    if (T->root == nullptr) T->root = y;
    else if (newKey < q->key) q->left = y;
    else q->right = y;

    // 기존 insertBST 함수에서 height 업데이트 부분을 update_H_BF 함수로 따로 분리
}

string checkBalance(AVL *&subT, int newKey, string &rotationType, Node *&p, Node *&q) { // rotationType을 string으로 반환, 노드 p와 q는 reference?로 리턴
    Node *x = nullptr; // 최초로 불균형 발생한 노드(bf = 2 or -2)
    Node *f = nullptr; // x's parent node

    // BF 계산
    // update height and balancing factor while popping parent node from stack
    while (!s.empty()) {
        q = s.top();
        s.pop();
        update_H_BF(q); // update height and balancing factor

        if ((1 < q->bf) || (q->bf < -1)) {
            if (x == nullptr) {
                x = q;
                if (!s.empty())
                    f = s.top();
            }
        }
    }

    // 트리 균형 여부 검사
    // if there's no problem, return
    if (x == nullptr) {
        rotationType = "NO";
        p = nullptr;
        q = nullptr;
        return rotationType;
    }

    // rebalance tree
    if (1 < x->bf) {
        if (x->left->bf < 0) {
            rotationType = "LR"; // rotate LR
            p = x;
            q = f;
        }
        else {
            rotationType = "LL"; // rotate LL
            p = x;
            q = f;
        }
    }
    else {
        if (x->right->bf > 0) {
            rotationType = "RL"; // rotate RL
            p = x;
            q = f;
        }
        else {
            rotationType = "RR"; // rotate RR
            p = x;
            q = f;
        }
    }
    return rotationType;
}

void rotateTree(AVL *&subT, string rotationType, Node *&x, Node *&f) {
    Node *xc; // x's child node

    if (rotationType == "LL") {
        if (f == nullptr) {
            xc = x->left;
            x->left = xc->right;
            xc->right = x;
            subT->root = xc;
        }
        else if (x == f->left) {
            f->left = x->left;
            x->left = x->left->right;
            f->left->right = x;
            update_H_BF(f->left);
            update_H_BF(f->right);
        }
        else { // x == f->right
            f->right = x->left;
            x->left = x->left->right;
            f->right->right = x;
            update_H_BF(f->left);
            update_H_BF(f->right);
        }
        update_H_BF(x);
    }
    else if (rotationType == "LR") {
        xc = x->left->right;
        x->left->right = xc->left;
        xc->left = x->left;
        x->left = xc->right;
        xc->right = x;

        switch (xc->bf) {
            case 0:
                update_H_BF(xc->left);
                update_H_BF(xc->right);
                break;
            case 1:
                update_H_BF(xc->right);
                update_H_BF(xc->left);
                break;
            case -1:
                update_H_BF(x->left);
                update_H_BF(x);
                break;
        }
        update_H_BF(xc);

        if (f == nullptr) subT->root = xc;
        else if (x == f->left) f->left = xc;
        else f->right = xc; // x == f->right
    }
    else {
        if (rotationType == "RR") {
            if (f == nullptr) {
                xc = x->right;
                x->right = xc->left;
                xc->left = x;
                subT->root = xc;
            }
            else if (x == f->left) {
                f->left = x->right;
                x->right = x->right->left;
                f->left->left = x;
                update_H_BF(f->right);
                update_H_BF(f->left);
            }
            else { // x == f->right
                f->right = x->right;
                x->right = x->right->left;
                f->right->left = x;
                update_H_BF(f->right);
                update_H_BF(f->left);
            }
            update_H_BF(x);
        }
        else { // rotationType == "RL"
            xc = x->right->left;
            x->right->left = xc->right;
            xc->right = x->right;
            x->right = xc->left;
            xc->left = x;

            switch (xc->bf) {
                case 0:
                    update_H_BF(xc->right);
                    update_H_BF(xc->left);
                    break;
                case 1:
                    update_H_BF(xc->left);
                    update_H_BF(xc->right);
                    break;
                case -1:
                    update_H_BF(xc->right);
                    update_H_BF(xc->left);
                    break;
            }
            update_H_BF(xc);

            if (f == nullptr) subT->root = xc;
            else if (f->left == x) f->left = xc;
            else f->right = xc; // x == f->right
        }
    }
    update_H_BF(f);
}

void insertAVL(AVL *T, int newKey) {
    // Step 1: insert BST
    insertBST(T, newKey);
    Node *p = T->root;
    Node *q = nullptr;
    string rotationType;

    // Step 2: check balance factor
    checkBalance(T, newKey, rotationType, p, q);

    // Step 3: rebalancing
    if (rotationType != "NO") {
        rotateTree(T, rotationType, p, q);
    }

    if (isExist) {
        cout << "";
        isExist = false;
    }
    else
        cout << rotationType << " ";
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

void deleteBST(AVL *T, int deleteKey) {
    Node *p = T->root;
    Node *q = nullptr;

    // find position of deleteKey while storing parent node on stack
    while ((p != nullptr) && (deleteKey != p->key)) {
        q = p;
        s.push(q);

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    if (p == nullptr) { // deleteKey was not found
        isExist = true;
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
}

void deleteAVL(AVL *T, int deleteKey) {
    // perform standard BST deletion
    // Step 1: delete BST
    deleteBST(T, deleteKey);
    Node *p = T->root;
    Node *q = nullptr; // p의 부모노드 가리키는 포인터
    string rotationType;

    // Step 2: check balance factor
    checkBalance(T, deleteKey, rotationType, p, q);

    // Step 3: rebalancing
    if (rotationType != "NO") {
        rotateTree(T, rotationType, p, q);
    }

    if (isExist) {
        cout << "";
        isExist = false;
    }
    else
        cout << rotationType << " ";
}

void inorderN(Node *node) {
    if (node == nullptr) return;
    else {
        inorderN(node->left);
        cout << '(' << node->key << ", " << node->bf << ')' << ' ';
        inorderN(node->right);
    }
}

void inorderBST(AVL *tree) {
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

    AVL *t = new AVL();

    // CLion에서 실행시 파일 경로지정
    filesystem::path p("../BST-input.txt");
    path = filesystem::canonical(p);
    inputfile.open(path);


//    inputfile.open("BST-input.txt");
    if (inputfile.fail()) {
        cerr << "file can't found" << endl;
        exit(1);
    }
    else {
        while (!inputfile.eof()) {
            inputfile >> optstr >> inputkey;

            if (inputfile.eof()) break;

            if (optstr == "i") insertAVL(t, inputkey);
            if (optstr == "d") deleteAVL(t, inputkey);
            inorderBST(t);
        }
        inputfile.close();
        t->~AVL();

        return 0;
    }
}
