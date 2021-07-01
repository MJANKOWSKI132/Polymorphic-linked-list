#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// This linked list can store the following primitive data types
enum Type {INTEGER = 0, STRING = 1, FLOAT = 2, CHAR = 3};

// Each node must have an accompanying type
struct node{
    void* value;
    struct node* next;
    enum Type type;
};

// Function prototypes
void printLinkedList(struct node* head);
void clear(struct node** head, void (*freeFunction)(void*));
void clearTest();
void addToEnd(struct node** head, struct node* insertNode);
void addToEndTest();
void addToStart(struct node** head, struct node* insertNode);
void addToStartTest();
struct node* createNode(void* value, enum Type type);
void createNodeTest();
void printNode(struct node* node);
int getLength(struct node* head);
void getLengthTest();
struct node* getHead(struct node** head, bool remove);
void getHeadTest();
struct node* getFoot(struct node** head, bool remove);
void getFootTest();
struct node* get(struct node** head, int index, bool remove);
void getTest();
bool isEmpty(struct node* head);
void isEmptyTest();
bool isHomogeneous(struct node* head);
void isHomogeneousTest();
struct node* findFirstIter(struct node* head, bool (*predicate)(void*));
void findFirstIterTest();
struct node* findFirstRec(struct node* head, bool (*predicate)(void*));
void findFirstRecTest();
struct node* findAllIter(struct node* head, bool (*predicate)(void*));
void findAllIterTest();
struct node* findAllRecUtility(struct node* head, bool (*predicate)(void*), struct node* newHead);
struct node* findAllRec(struct node* head, bool(*predicate)(void*));
void findAllRecTest();
bool isEqual(struct node* firstNode, struct node* secondNode);
void isEqualTest();
void reverse(struct node** head);
void reverseTest();
struct node* map(struct node* head, void* (*f)(void*));
void mapTest();
void forEach(struct node* head, void (*f)(void*));
void forEachTest();
struct node* filter(struct node* head, bool (*predicate)(void*));
void filterTest();

bool isInteger(struct node* node){
    return node -> type == INTEGER;
}

void main(){
    clearTest();
    createNodeTest();
    addToEndTest();
}

// A function to print each node
// This function works by identifying the node's type and copying to a variable of the same type's memory
void printNode(struct node* node){
    if(node -> type == INTEGER){
        int val;
        memcpy(&val, &(node -> value), sizeof(int));
        printf("%d\n", val);
    }else if(node -> type == STRING){
        char *val;
        val = (char*)(node -> value);
        printf("%s\n", val);
    }else if(node -> type == FLOAT){
        float val;
        memcpy(&val, &(node -> value), sizeof(float));
        printf("%f\n", val);
    }else if(node -> type == CHAR){
        char val;
        memcpy(&val, &(node -> value), sizeof(char));
        printf("%c\n", val);
    }
}

// A function to print the entire linked list using the printNode function
void printLinkedList(struct node* head){
    struct node* currNode = head;
    while(currNode){
        printNode(currNode);
        currNode = currNode -> next;
    }
}

// A function to free the entire linked list
void clear(struct node** head, void (*freeFunction)(void*)){
    struct node* currNode = *head;
    struct node* prevNode = NULL;
    // Iterate over each node freeing it, ensuring that you store the next node before freeing the current
    while(currNode){
        prevNode = currNode;
        currNode = currNode -> next;
        freeFunction(prevNode);
    }
    *head = NULL;
}

void clearTest(){
    struct node* head = (struct node*)malloc(sizeof(struct node));
    head = NULL;
    int NUMBERS_LENGTH = 5;
    int numbers[5] = {1, 2, 3, 4, 5};
    struct node** nodes = (struct node**)malloc(NUMBERS_LENGTH * sizeof(struct node*));
    for(int i = 0; i < NUMBERS_LENGTH; i ++){
        struct node* insertNode = createNode(&numbers[i], INTEGER);
        addToEnd(&head, insertNode);
    }
    clear(&head, free);
    assert(head == NULL);
}


// A function to create a node based on a value and a specified type
struct node* createNode(void* value, enum Type type){
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node -> next = NULL;
    node -> type = type;
    size_t size;
    if(type == INTEGER){
        size = sizeof(int);
    }else if(type == STRING){
        char* s;
        s = (char*)value;
        // String contains n + 1 characters, the extra character being the end character
        size = (strlen(s) + 1) * sizeof(char);
    }else if(type == FLOAT){
        size = sizeof(float);
    }else if(type == CHAR){
        size = sizeof(char);
    }else{
        return NULL;
    }
    node -> value = malloc(size);
    memcpy(&(node -> value), value, size);
    return node;
}

void createNodeTest(){
    int value = 10;
    enum Type type = INTEGER;
    struct node* node = createNode(&value, type);
    assert(node != NULL);
    int nodeValue;
    memcpy(&nodeValue, &(node -> value), sizeof(int));
    assert(nodeValue == 10);
    assert(node -> type == INTEGER);
    assert(node -> next == NULL);
    free(node);
}

// A function to add to the end (foot) of the linked list
void addToEnd(struct node** head, struct node* insertNode){
    if(!insertNode) return;
    if(!(*head)){
        // If the linked list is empty make the node being inserted the head
        *head = insertNode;
    }else{
        // Iterate through the entire linked list until arriving at the foot and insert the node after the old foot
        struct node* currNode = *head;
        while(currNode -> next){
            currNode = currNode -> next;
        }
        currNode -> next = insertNode;
    }
}

void addToEndTest(){
    struct node* head = (struct node*)malloc(sizeof(struct node));
    head = NULL;
    int LENGTH = 5;
    int integers[5] = {1, 2, 3, 4, 5};
    char characters[5] = {'a', 'b', 'c', 'd', 'e'};
    char* strings[5] = {"string1", "string2", "string3", "string4", "string5"};
    float floats[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    for(int i = 0; i < LENGTH; i ++){
        struct node* insertNode1 = createNode(&integers[i], INTEGER);
        struct node* insertNode2 = createNode(&characters[i], CHAR);
        struct node* insertNode3 = createNode(&strings[i], STRING);
        struct node* insertNode4 = createNode(&floats[i], FLOAT);
        addToEnd(&head, insertNode1);
        addToEnd(&head, insertNode2);
        addToEnd(&head, insertNode3);
        addToEnd(&head, insertNode4);
    }
    printLinkedList(head);
    clear(&head, free);
}

// A function to add to the start (head) of the linked list
void addToStart(struct node** head, struct node* insertNode){
    if(!insertNode) return;
    if(!(*head)){
        // If the linked list is empty make the node being inserted the head
        *head = insertNode;
    }else{
        // if the linked list is not empty make the head the node being inserted
        insertNode -> next = *head;
        *head = insertNode;
    }
}

// A function to get the length of the linked list
int getLength(struct node* head){
    struct node* currNode = head;
    int length = 0;
    while(currNode){
        length++;
        currNode = currNode -> next;
    }
    return length;
}

// A function to get the head of the linked list and remove it if specified (pop)
struct node* getHead(struct node** head, bool remove){
    if(remove){
        if(head == NULL) return NULL;
        struct node* newHead = (*head) -> next;
        struct node* oldHead = *head;
        *head = newHead;
        return oldHead;
    }
    return *head;
}

// A function to get the foot of the linked list and remove it if specified (dequeue)
struct node* getFoot(struct node** head, bool remove){
    if(head == NULL) return NULL;
    struct node* currNode = *head;
    struct node* prevNode = NULL;
    while(currNode -> next){
        prevNode = currNode;
        currNode = currNode -> next;
    }
    if(remove){
        prevNode -> next = NULL;
    }
    return currNode;
}

// A function to determine if a linked list is empty
bool isEmpty(struct node* head){
    return head == NULL;
}

// A function to get the node at a specified index and remove it if specified
struct node* get(struct node** head, int index, bool remove){
    if(head == NULL) return NULL;
    if(index == 0){
        // If the specified index is 0, this signifies the head being referenced and thus delegate it to the getHead function
        return getHead(head, remove);
    }
    int progressed = 1;
    struct node* currNode = (*head) -> next;
    struct node* prevNode = *head;
    // Iterate over the entire linked list until the progressed counter matches the index
    while(currNode){
        if(progressed == index){
            if(remove){
                prevNode -> next = currNode -> next;
            }
            return currNode;
        }
        progressed++;
        prevNode = currNode;
        currNode = currNode -> next;
    }
    return NULL;
}

// A function to determine if a linked list is homogeneous on type
bool isHomogeneous(struct node* head){
    struct node* currNode = head;
    while(currNode -> next){
        struct node* nextNode = currNode -> next;
        while(nextNode -> next){
            if(nextNode -> type != currNode -> type){
                return false;
            }
            nextNode = nextNode -> next;
        }
        currNode = currNode -> next;
    }
    return true;
}

// A function to determine if two nodes are equal
bool isEqual(struct node* firstNode, struct node* secondNode){
    if(firstNode == NULL && secondNode == NULL) return true;
    if(firstNode == NULL && secondNode != NULL) return false;
    if(firstNode != NULL && secondNode == NULL) return false;
    if(firstNode -> type != secondNode -> type) return false;
    switch(firstNode -> type){
        case INTEGER:
        {
            int firstVal;
            int secondVal;
            memcpy(&firstVal, &(firstNode -> value), sizeof(int));
            memcpy(&secondVal, &(secondNode -> value), sizeof(int));
            return firstVal == secondVal;
        }
        case FLOAT:
        {
            float firstVal;
            float secondVal;
            memcpy(&firstVal, &(firstNode -> value), sizeof(float));
            memcpy(&secondVal, &(secondNode -> value), sizeof(float));
            return firstVal == secondVal;
        }
        case STRING:
        {
            char* firstVal;
            char* secondVal;
            firstVal = (char*)(firstNode -> value);
            secondVal = (char*)(secondNode -> value);
            return strcmp(firstVal, secondVal) == 0;
        }
        case CHAR:
        {
            char firstVal;
            char secondVal;
            memcpy(&firstVal, &(firstNode -> value), sizeof(char));
            memcpy(&secondVal, &(secondNode -> value), sizeof(char));
            return firstVal == secondVal;
        }
    }
    return false;
}

// A function that finds the first node that satisfies the given predicate
// Utilises an iterative implementation of linear search
struct node* findFirstIter(struct node* head, bool (*predicate)(void*)){
    struct node* currNode = head;
    while(currNode){
        if(predicate(currNode)){
            return currNode;
        }
        currNode = currNode -> next;
    }
    return NULL;
}

// A function that finds the first node that satisfies the given predicate
// Utilises a recursive implementation of linear search
struct node* findFirstRec(struct node* head, bool (*predicate)(void*)){
    if(head == NULL) return NULL;
    if(predicate(head)) return head;
    return findFirstRec(head -> next, predicate);
}

// A function that finds all nodes that satisfy the given predicate and returns such nodes as a new linked list
// Utilises an iterative implementation of linear search
struct node* findAllIter(struct node* head, bool (*predicate)(void*)){
    struct node* currNode = head;
    struct node* newHead = (struct node*)malloc(sizeof(struct node));
    newHead = NULL;
    while(currNode){
        if(predicate(currNode)){
            struct node* insertNode = createNode(&(currNode -> value), currNode -> type);
            addToEnd(&newHead, insertNode);
        }
        currNode = currNode -> next;
    }
    return newHead;
}

// Recursive utility function for findAllRec
struct node* findAllRecUtility(struct node* head, bool (*predicate)(void*), struct node* newHead){
    if(head == NULL) return newHead;
    if(predicate(head)){
        struct node* insertNode = createNode(&(head -> value), head -> type);
        addToEnd(&newHead, insertNode);
    }
    return findAllRecUtility(head -> next, predicate, newHead);
}

// A function that finds all nodes that satisfy the given predicate and returns such nodes as a new linked list
// Utilises a recursive implementation of linear search
struct node* findAllRec(struct node* head, bool (*predicate)(void*)){
    struct node* newHead = (struct node*)malloc(sizeof(struct node));
    newHead = NULL;
    return findAllRecUtility(head, predicate, newHead);
}

// A function to reverse a linked list
void reverse(struct node** head){
    struct node* currNode = *head;
    struct node* prevNode = NULL;
    struct node* tmpNode = NULL;
    while(currNode){
        tmpNode = currNode -> next;
        currNode -> next = prevNode;
        prevNode = currNode;
        currNode = tmpNode;
    }
    *head = prevNode;
}

// A function that maps the values of a linked list, similar to Java's map method
struct node* map(struct node* head, void* (*f)(void*)){
    struct node* currNode = head;
    while(currNode){
        currNode -> value = f(currNode -> value);
        currNode = currNode -> next;
    }
    return head;
}

// A function that performs an operation for each item of the linked list, similar to Java's forEach method
void forEach(struct node* head, void (*f)(void*)){
    struct node* currNode = head;
    while(currNode){
        f(currNode);
        currNode = currNode -> next;
    }
}

// A function that filters a linked list based on a predicate, similar to Java's filter method
struct node* filter(struct node* head, bool (*predicate)(void*)){
    struct node* currNode = head;
    struct node* newHead = (struct node*)malloc(sizeof(struct node));
    newHead = NULL;
    while(currNode){
        if(predicate(currNode)){
            struct node* insertNode = createNode(&(currNode -> value), currNode -> type);
            addToEnd(&newHead, insertNode);
        }
        currNode = currNode -> next;
    }
    return newHead;
}