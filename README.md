# 2022-FileProcessing 
**20181667 - 2022년 2학기 화일처리**

**실행환경: MacBook Air M1 - macOS Monterey 12.6**

**사용 컴파일러: g++**
```bash
$ g++ -o filename filename.cpp
```

## 성적 및 채점 기준
### 성적
**과제 점수**
- **BST**: 50점/50점 
- **AVL**: 44점/50점 
- **BT**: 37점/50점 

**기말고사 점수**: 180점/200점 

**출석 점수**: 10점/10점

**최종 성적: A0**

### 과제 채점 기준
#### 과제 별 채점
- **BST** 
  - 기간내 제출 여부: 0점/10점
  - 입출력만족여부: 0점/10점
  - 정확도 (단순한 테스트 데이터 5개, 랜덤 테스트 데이터 10개): 0점, 5점, 10점, 18~30점 
- **AVL** 
  - 기간내 제출 여부: 0점/10점
  - 입출력만족여부: 0점/10점
  - 정확도 (단순한 테스트 데이터 8개, 랜덤 테스트 데이터 10개): 0점, 5점, 10점, 15~30점 
- **BT** 
  - 기간내 제출 여부: 0점/10점
  - 입출력만족여부: 0점/10점
  - 정확도 (단순한 테스트 데이터 8개, 랜덤 테스트 데이터 10개): 0점, 5점, 10점, 15~30점 
  
기간내 제출 여부는 “실행 가능한 소스코드” 를 제출한 경우에만 제출로 인정

입출력 만족 여부는 과제 pdf파일에 안내된 입력, 출력을 제대로 지켰는지를 기준으로 채점

#### 정확도 채점
- **단순한 테스트 데이터** 
  - 선형적으로 삽입/삭제가 이루어짐 (즉, 오름차순 또는 내림차순으로 중복되는 값 없이 주어짐)
  - 공지한 예제 입력을 포함
  - 예제 입력을 제외한 테스트 케이스는 10&#126;20개의 삽입/삭제 연산과 1&#126;10 사이의 키값으로 구성됨
- **랜덤 테스트 데이터** 
  - 100개의 삽입/삭제 연산이 랜덤으로 주어짐
  - 각 키값은 0~100 사이의 랜덤 값
- **점수 산출** 
  - 각 테스트 케이스의 각 명령 (삽입/삭제)에 대해 올바른 트리 구조를 가지는지 판단
  - inorder 출력 결과 뿐만 아니라, 별도로 트리 구조를 파악할 수 있도록 inorder 탐색 과정까지 출력하도록 코드를 수정하여 채점기에서 트리를 재구성함
  - **BST**
    - 왼쪽 subtree, 오른쪽 subtree 간의 대소관계 만족 여부 판단
  - **AVL**
    - 왼쪽 subtree, 오른쪽 subtree 간의 대소관계 만족 여부 판단
    - 재구성한 트리의 bf가 [-1, 1]범위를 만족하는지 판단
    - 재구성한 트리의 bf와 출력한 bf가 동일한지 판단
  - **BT**
    - 키값의 대소관계 만족 여부 판단 
    - 각 node에 들어있는 키 개수가 Btree 조건을 만족하는지 판단
  
  테스트 케이스 별로 정답 여부를 파악하려 했으나 채점 결과 점수대가 낮아서 명령 단위로 점수 부여 
  
  (각 삽입/삭제 명령 결과가 올바른지를 기준으로 채점)

#### 채점에 사용된 데이터셋: [testcase.zip](https://github.com/yesslee/2022-FileProcessing/blob/main/fileProcessing/testcases.zip)
