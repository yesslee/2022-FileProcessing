//
// Report3: B-Tree 삽입, 삭제 구현 [삽입: 성공, 삭제: 성공] (성공 추정. 내부 구조 완전한지 불확실)
// 20181667 이예슬
// 실행 환경: macOS 12.6, g++(Apple clang version 14.0.0) 컴파일
//
// Created by yesslee31 on 2022/11/24.
//
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

struct Node { // Node
    int n; // 노드가 가지고 있는 키의 개수
    vector<int> K; // 노드가 가지고 있는 키들을 담고있는 배열
    vector<Node*>P; // 노드가 가지고 있는 포인터들(자식노드) 담고있는 배열
};

class BT { // tree
public:
    Node *root;

    BT() {
        root = nullptr;
    }
};

Node *getBTNode(int m) { // B-Tree에서의 차수 m
    Node *newNode = new Node();

    newNode->n = 0;
    newNode->K.resize(m - 1, 0);
    newNode->P.resize(m, nullptr);

    return newNode;
}

bool searchPath(Node *T, int m, int key, stack<Node*> &s) {
    Node *x = T;
    int i;

    do {
        i = 0;
        while ((i < x->n) && (key > x->K[i]))
            i++;

        // 삽입할 키 발견 못함 = 삽입 진행
        s.push(x);

        // 삽입할 키 발견함 = 이미 존재하는 값이므로 삽입 X
        if ((i < x->n) && (key == x->K[i])) {
            return true;
        }
    }
    while ((x = x->P[i]) != nullptr);

    return false;
}

// --- insert ---
void insertKey(BT *T, int m, Node *x, Node *y, int newKey) { // x는 키 담는 배열?, y는 다음 포인터가 가리킬 노드
    // newKey보다 큰 키들 오른쪽으로 한 칸씩 이동
    int i = x->n - 1;
    while ((i >= 0) && (newKey < x->K[i])) {
        x->K[i + 1] = x->K[i];
        x->P[i + 2] = x->P[i + 1];
        i--;
    }

    // newKey 삽입
    x->K[i + 1] = newKey;
    x->P[i + 2] = y;
    x->n++;
}

Node* splitNode(BT *T, int m, Node *x, Node *y, int &newKey) { // y는 다음 포인터가 가리킬 노드
    // overflow 위한 임시 노드(x, newKey 저장)
    Node *tempNode = getBTNode(m + 1);
    // copy x to tempNode
    tempNode->n = x->n;
    for (int i = 0; i < x->n; i++)
        tempNode->K[i] = x->K[i];
    for (int i = 0; i <= x->n; i++)
        tempNode->P[i] = x->P[i];
    insertKey(T, m, tempNode, y, newKey);

    // 노드 분할 기준이 되는 중앙값
    int centerKey;
    centerKey = tempNode->K[tempNode->n / 2];
    newKey = centerKey;

    // centerKey 이전 값(중앙값 보다 작은 값들)을 노드 x로 복사
    x->n = 0;
    int i = 0;
    while (tempNode->K[i] < centerKey) {
        x->K[i] = tempNode->K[i];
        x->P[i] = tempNode->P[i];
        i++;
        x->n++;
    }
    x->P[i] = tempNode->P[i];

    // centerKey 이후 값(중앙값 보다 큰 값들)을 newNode로 복사
    Node *newNode = getBTNode(m);
    i++;
    while (i < tempNode->n) {
        newNode->K[newNode->n] = tempNode->K[i];
        newNode->P[newNode->n] = tempNode->P[i];
        i++;
        newNode->n++;
    }
    newNode->P[newNode->n] = tempNode->P[i];

    delete tempNode;

    return newNode; // split 후 부모노드에 삽입되어야 할 키 값과 분리된 새로운 노드를 반환
}

void insertBT(BT *T, int m, int newKey) {
    // 루트 노드 생성
    if (T->root == nullptr) {
        T->root = getBTNode(m);
        T->root->K[0] = newKey;
        T->root->n = 1;

        return;
    }

    // newKey 삽입할 노드의 경로 탐색, 탐색한 경로는 스택에 저장
    stack<Node*> st;
    bool found;
    found = searchPath(T->root, m, newKey, st);

    // newKey 발견 = 이미 존재하는 값이므로 삽입 X
    if (found) {
        cout << "i " << newKey << " : The key already exists" << endl;
        return;
    }

    // newKey 발견 X = 삽입 진행
    bool finished = false;

    Node *x = st.top(); // x = pop a value from the top of stack;
    st.pop();
    Node *y = nullptr; // y는 새로 분할된 노드 담을 변수

    do {
        if (x->n < m - 1) { // overflow 발생 검사
            insertKey(T, m, x, y, newKey); // overflow 발생 X. 삽입 진행
            finished = true;
        }
        else { // overflow case
            y = splitNode(T, m, x, y, newKey); // newKey 기준으로 x를 분할

            if (!st.empty()) {
                x = st.top(); // x = pop from stack;
                st.pop();
            }
            else { // 트리 레벨 + 1
                T->root = getBTNode(m);
                T->root->K[0] = newKey;
                T->root->P[0] = x;
                T->root->P[1] = y;
                T->root->n = 1;
                finished = true;
            }
        }
    }
    while (!finished);
}

// --- delete ---
void deleteKey(BT *T, int m, Node *x, int oldKey) {
    // oldKey의 위치 i 탐색
    int i = 0;
    while (oldKey > x->K[i])
        i++;

    // oldKey 보다 큰 키들 왼쪽으로 한 칸씩 이동
    while (i < x->n) {
        x->K[i] = x->K[i + 1];
        x->P[i + 1] = x->P[i + 2];
        i++;
    }
    x->n--;
}

int bestSibling(BT *T, int m, Node *x, Node *y) {
    // y에서 x의 위치 i 탐색
    int i = 0;
    while (y->P[i] != x)
        i++;

    // 인접한 두 형제 노드 중 키 개수 더 많은 쪽을 bestSibling으로 선택
    int bestSibling;
    if (i == 0) // 왼쪽 형제 X
        bestSibling = i + 1;
    else if (i == y->n) // 오른쪽 형제 X
        bestSibling = i - 1;
    else if (y->P[i]->n >= y->P[i + 1]->n)
        bestSibling = i - 1;
    else
        bestSibling = i + 1;

    return bestSibling;
}

void redistributeKeys(BT *T, int m, Node *x, Node *y, int bestSib) {
    // y에서 x의 위치 i 탐색
    int i = 0;
    while (y->P[i] != x)
        i++;

    Node *bestNode = y->P[bestSib];
    if (bestSib < i) { // bestSibling = 왼쪽 형제 노드
        int lastKey = bestNode->K[bestNode->n - 1];
        insertKey(T, m, x, nullptr, y->K[i - 1]);
        x->P[1] = x->P[0];
        x->P[0] = bestNode->P[bestNode->n];
        bestNode->P[bestNode->n] = nullptr;
        deleteKey(T, m, bestNode, lastKey);
        y->K[i - 1] = lastKey;
    }
    else { // bestSibling = 오른족 형제 노드
        int firstKey = bestNode->K[0];
        insertKey(T, m, x, nullptr, y->K[i]);
        x->P[x->n] = bestNode->P[0];
        bestNode->P[0] = bestNode->P[1];
        deleteKey(T, m, bestNode, firstKey);
        y->K[i] = firstKey;
    }
}

void mergeNode(BT *T, int m, Node *x, Node *y, int bestSib) {
    // y에서 x의 위치 i를 탐색
    int i = 0;
    while (y->P[i] != x)
        i++;

    Node *bestNode = y->P[bestSib];
    if (bestSib > i) { // 왼쪽 기준으로 병합 이루어지게끔 교체
        int temp = i;
        i = bestSib;
        bestSib = temp;

        Node *tempNode = x;
        x = bestNode;
        bestNode = tempNode;
    }
    // merge
    bestNode->K[bestNode->n] = y->K[i - 1];
    bestNode->n++;

    int j = 0;
    while (j < x->n) {
        bestNode->K[bestNode->n] = x->K[j];
        bestNode->P[bestNode->n] = x->P[j];
        bestNode->n++;
        j++;
    }
    bestNode->P[bestNode->n] = x->P[x->n];
    deleteKey(T, m, y, y->K[i -1]);

    delete x;
}

void deleteBT(BT* T, int m, int oldKey) {
    // oldKey가 있던 노드의 경로 탐색, 탐색한 경로는 스택에 저장
    stack<Node*> st;
    bool found;
    found = searchPath(T->root, m, oldKey, st);

    // oldKey 발견 X = 이미 삭제한 값이므로 삭제 진행 X
    if (!found) {
        cout << "d " << oldKey << " : The key does not exist" << endl;
        return;
    }

    Node *x = st.top();
    st.pop();
    Node *y = nullptr;

    // oldKey 위치 단말인지 내부인지 확인
    bool inTerminal = false;
    for (int i = 0; i < x->n; i++) {
        if (x->P[i] == nullptr)
            inTerminal = true;
    }

    if (!inTerminal) { // oldKey가 단말에 존재하지 않는 경우 = oldKey가 내부 노드에 존재하는 경우
        Node *internalNode = x;
        int i; // i = index of oldKey in internalNode
        for (i = 0; i < x->n; i++) {
            if (x->K[i] == oldKey) // 내부 노드에서 oldKey 발견
                break;
        }

        st.push(x);
        searchPath(x->P[i + 1], m, x->K[i], st); // oldKey의 후행키 경로 탐색(일치하는 키는 찾지 못하지만 경로는 탐색)

        x = st.top(); // x는 후행키가 존재하는 단말 노드
        st.pop();

        // oldKey와 후행키 교체
        int temp = internalNode->K[i];
        internalNode->K[i] = x->K[0];
        x->K[0] = temp;
    }

    bool finished = false;
    deleteKey(T, m, x, oldKey); // 노드 x에서 oldKey 삭제

    if (!st.empty()) {
        y = st.top();
        st.pop();
    }

    do {
        if ((T->root == x) || (x->n >= (m - 1) / 2))
            finished = true;
        else { // underflow case
            int bestSib = bestSibling(T, m, x, y); // 키 재분배 & merge에 적합한 노드 선택

            if (y->P[bestSib]->n > (m - 1) / 2) { // 키 재분배
                redistributeKeys(T, m, x, y, bestSib);
                finished = true;
            }
            else { // node merge
                mergeNode(T, m, x, y, bestSib);
                x = y;
                if (!st.empty()) {
                    y = st.top();
                    st.pop();
                }
                else
                    finished = true;
            }
        }
    }
    while (!finished);

    if ((y != nullptr) && (y->n == 0)) { // 트리 레벨 - 1
        T->root = y->P[0];

        delete y;
    }
}

void inorderN(Node *node) {
    if (node == nullptr) return;
    else {
        for (int i = 0; i < node->n; i++) {
            if (node->P[i] != nullptr)
                inorderN(node->P[i]);
            cout << node->K[i] << ' ';
        }
        if (node->P[node->n] != nullptr)
            inorderN(node->P[node->n]);
    }
}

void inorderBT(BT *tree, int m) {
    if (tree == nullptr) return;
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

    BT *t = new BT();

    for (int i = 3; i <= 4; i++) { // i = m 값으로 넣어서 출력 반복
        /*
        // CLion에서 실행시 파일 경로지정
        filesystem::path p("../BT-input.txt");
        path = filesystem::canonical(p);
        inputfile.open(path);
         */

        inputfile.open("BT-input.txt");
        if (inputfile.fail()) {
            cerr << "file can't found" << endl;
            exit(1);
        }
        else {
            while (!inputfile.eof()) {
                inputfile >> optstr >> inputkey;

                if (inputfile.eof()) break;

                if (optstr == "i") insertBT(t, i, inputkey);
                if (optstr == "d") deleteBT(t, i, inputkey);
                inorderBT(t, i);
            }
        }
        inputfile.close();
    }
}
