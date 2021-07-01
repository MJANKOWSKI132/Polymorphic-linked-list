#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
void freeLinkedList(struct node* head, void (*freeFunction)(void*));
void addToEnd(struct node** head, struct node* insertNode);
void addToStart(struct node** head, struct node* insertNode);
struct node* createNode(void* value, enum Type type);
void printNode(struct node* node);
int getLength(struct node* head);
struct node* getHead(struct node** head, bool remove);
struct node* getFoot(struct node** head, bool remove);
struct node* get(struct node** head, int index, bool remove);
bool isEmpty(struct node* head);
bool isHomogeneous(struct node* head);
struct node* findFirstIter(struct node* head, bool (*cmp)(void*));
struct node* findFirstRec(struct node* head, bool (*cmp)(void*));
struct node* findAllIter(struct node* head, bool (*cmp)(void*));
struct node* findAllRecUtility(struct node* head, bool (*cmp)(void*), struct node* newHead);
struct node* findAllRec(struct node* head, bool(*cmp)(void*));
bool isEqual(struct node* firstNode, struct node* secondNode);
void reverse(struct node** head);
struct node* map(struct node* head, void* (*f)(void*));
void forEach(struct node* head, void (*f)(void*));
struct node* filter(struct node* head, bool (*predicate)(void*));

bool isInteger(struct node* node){
    return node -> type == INTEGER;
}

void main(){
    struct node* head = (struct node*)malloc(sizeof(struct node));
    head = NULL;
    int numbers[5] = {1, 2, 3, 4, 5};
    for(int i = 0; i < 5; i ++){
        struct node* insertNode = createNode(&numbers[i], INTEGER);
        addToEnd(&head, insertNode);
    }
    struct node* filteredHead = filter(head, isInteger);
    forEach(filteredHead, printNode);
    freeLinkedList(head, free);
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
void freeLinkedList(struct node* head, void (*freeFunction)(void*)){
    struct node* currNode = head;
    struct node* prevNode = NULL;
    // Iterate over each node freeing it, ensuring that you store the next node before freeing the current
    while(currNode){
        prevNode = currNode;
        currNode = currNode -> next;
        freeFunction(prevNode);
    }
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

struct node* findFirstIter(struct node* head, bool (*cmp)(void*)){
    struct node* currNode = head;
    while(currNode){
        if(cmp(currNode)){
            return currNode;
        }
        currNode = currNode -> next;
    }
    return NULL;
}

struct node* findFirstRec(struct node* head, bool (*cmp)(void*)){
    if(head == NULL) return NULL;
    if(cmp(head)) return head;
    return findFirstRec(head -> next, cmp);
}

struct node* findAllIter(struct node* head, bool (*cmp)(void*)){
    struct node* currNode = head;
    struct node* newHead = (struct node*)malloc(sizeof(struct node));
    newHead = NULL;
    while(currNode){
        if(cmp(currNode)){
            addToEnd(&newHead, currNode);
        }
        currNode = currNode -> next;
    }
    return newHead;
}

struct node* findAllRecUtility(struct node* head, bool (*cmp)(void*), struct node* newHead){
    if(head == NULL) return newHead;
    if(cmp(head)){
        addToEnd(&newHead, head);
    }
    return findAllRecUtility(head -> next, cmp, newHead);
}

struct node* findAllRec(struct node* head, bool (*cmp)(void*)){
    struct node* newHead = (struct node*)malloc(sizeof(struct node));
    newHead = NULL;
    return findAllRecUtility(head, cmp, newHead);
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
            printf("here 1");
            addToEnd(&newHead, currNode);
            printf("here 2");
        }
        currNode = currNode -> next;
    }
    return newHead;
}