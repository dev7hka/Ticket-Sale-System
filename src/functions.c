#include "struct.h"
Person* newPerson(char* name, char* flightName, int wanted, int priority){

    Person* person = malloc(sizeof(Person));
    if(person == NULL)
        return NULL;
    person->name = malloc(strlen(name)+1);
    strcpy(person->name, name);
    person->wanted = wanted;
    person->taken = -1;
    person->priority = priority;
    person->flight = malloc(strlen(flightName)+1);
    strcpy(person->flight,flightName);
    return person;
}
void pushList(PersonList** personList, Person* person){

    if( *personList == NULL){  // If there is no person in the personList, the person is added to the head of the list

        PersonList* temp = malloc(sizeof(PersonList));
        temp->person = person;
        temp->next = NULL;
        *personList = temp;
        return;
    }
    PersonList* cur = *personList;

    while (cur->next != NULL){      // the cur goes to the end of the list
        cur = cur->next;
    }
    PersonList* temp= malloc(sizeof(PersonList));
    temp->person = person;
    temp->next = NULL;
    cur->next = temp;
}
Person* getList(PersonList* personList, char* name){

    if (personList == NULL)
        return NULL;
    PersonList* cur = personList;
    while (cur != NULL){
        if (strcmp(cur->person->name,name) == 0)    // checks if the person is already in the list
            return cur->person;
        cur = cur->next;
    }
    return NULL;
}
void pushStack(PersonStack** personStack, Person* person){

    if(*personStack == NULL){                       // If there is no person in the personStack, the person is added to the head of the Stack

        PersonStack* temp = malloc(sizeof(PersonStack));
        temp->person = person;
        temp->next = NULL;
        *personStack = temp;
        return;
    }
    PersonStack* cur = *personStack;
    while (cur->next != NULL)
        cur = cur->next;
    PersonStack* temp = malloc(sizeof(PersonStack));
    temp->person = person;
    temp->next = NULL;
    cur->next = temp;
}
void pushQueue(PersonQueue** personQueue, Person* person){

    PersonQueue* temp = malloc(sizeof(PersonQueue));
    temp->person = person;
    temp->next = NULL;

    if(*personQueue == NULL){                       // The person is added to the head of the list if list is empty
        *personQueue = temp;
        return;
    }
    PersonQueue* cur = *personQueue;
    if(cur->person->priority > temp->person->priority){       // The person is added to the head of the list if there is one person and its priority is less than the new one
        temp->next = *personQueue;
        *personQueue = temp;
        return;
    }
    while (cur->next != NULL && cur->next->person->priority <= temp->person->priority)          // The loop goes until a less prior person is found in the queue
        cur = cur->next;

    temp->next = cur->next;
    cur->next = temp;
}
int indexQueue(PersonQueue** personQueue, int class){            // Returns the number of persons for a particular class in queue (e.g how many persons wait for economy)

    if(*personQueue == NULL) {
        return 0;
    }
    int count = 0;
    PersonQueue* cur = *personQueue;
    while (cur != NULL){
        if(cur->person->wanted == class)
            ++count;
        cur = cur->next;
    }
    return count;
}
void pushFlight(FlightList** flightList, Flight* flight){          // Adds flight to the list of flights.

    FlightList* temp = malloc(sizeof(FlightList));
    temp->flight = flight;
    temp->next = NULL;

    if( *flightList == NULL){
        *flightList = temp;
        return;
    }
    FlightList* cur = *flightList;
    while (cur->next != NULL){
        cur = cur->next;
    }
    cur->next = temp;
}
Flight* getFlight(FlightList* flightList, char* name){          // Returns the address of the flight if exists, otherwise returns NULL.

    if (flightList == NULL)
        return NULL;
    FlightList* cur = flightList;
    while (cur != NULL){
        if (strcmp(cur->flight->name,name) == 0)
            return cur->flight;
        cur = cur->next;
    }
    return NULL;
}
void sellFlight(Flight* flight){

    if(flight->size == 0)
        return;
    PersonQueue* queue = flight->queue;
    PersonQueue* temp = NULL;                      // A temporary queue for the people who couldn't buy ticket for the first time

    while (queue != NULL){

        if(queue->person->wanted == BUSINESS && flight->Business->top < flight->Business->capacity){

            pushStack(&flight->Business->personStack, queue->person);
            ++flight->Business->top;
            --flight->size;
            queue->person->taken = BUSINESS;

        }else if(queue->person->wanted == ECONOMY && flight->Economy->top < flight->Economy->capacity){

            pushStack(&flight->Economy->personStack, queue->person);
            ++flight->Economy->top;
            --flight->size;
            queue->person->taken = ECONOMY;

        }else if(queue->person->wanted == STANDARD && flight->Standard->top < flight->Standard->capacity){

            pushStack(&flight->Standard->personStack, queue->person);
            ++flight->Standard->top;
            --flight->size;
            queue->person->taken = STANDARD;
        }
        else{
            pushQueue(&temp,queue->person);
        }
        queue = queue->next;
    }
    PersonQueue* temp2 = flight->queue;
    while (temp2 != NULL){
        PersonQueue* temp3 = temp2->next;
        free(temp2);
        temp2 = temp3;
    }
    flight->size = 0;
    flight->queue = NULL;
    PersonQueue* freetemp = temp;
    while (temp != NULL){

        if(flight->Standard->top < flight->Standard->capacity){
            pushStack(&flight->Standard->personStack,temp->person);
            ++flight->Standard->top;
            temp->person->taken = STANDARD;
        }
        else{
            pushQueue(&flight->queue, temp->person);
            ++flight->size;
        }
        temp = temp->next;
    }
    while (freetemp != NULL){
        PersonQueue* tmp = freetemp->next;
        free(freetemp);
        freetemp = tmp;
    }
}
Class* newClass(int class, int capacity){                     // class corresponds to the type of Class.
                                                              // 1: Business 2: Economy 3: Standard capacity: the size of person stack in class
    Class* new_class = malloc(sizeof(Class));
    new_class->class = class;
    new_class->capacity = capacity;
    new_class->top = 0;
    new_class->personStack = NULL;
    return new_class;
}
Flight* newFlight(char* name, int business, int economy, int standard){   // Creates a new flight. Business, economy and standard corresponds to the sizes of the classes.

    Flight* new_flight = malloc(sizeof(Flight));
    new_flight->name = malloc(strlen(name)+1);
    strcpy(new_flight->name, name);
    new_flight->Business = newClass(BUSINESS, business);
    new_flight->Economy = newClass(ECONOMY, economy);
    new_flight->Standard = newClass(STANDARD, standard);
    new_flight->queue = NULL;
    new_flight->size = 0;
    new_flight->isopen = 1;
    return new_flight;
}
void flightFree(Flight* flight){             // Deallocates the address of flight, the name of flight. Also calls other functions to deallocate the classes and queue

    free(flight->name);
    classFree(flight->Business);
    classFree(flight->Economy);
    classFree(flight->Standard);
    queueFree(flight->queue);
    free(flight);
}
void classFree(Class* class){               // Deallocates the address of class and calls stackFree to deallocates the stack of the class

    stackFree(class->personStack);
    free(class);
}
void stackFree(PersonStack* stack){        // Deallocates the stack

    PersonStack* temp;
    while (stack != NULL){
        temp = stack->next;
        free(stack);
        stack = temp;
    }
}
void queueFree(PersonQueue* queue){       // Deallocates the (priority)queue

    PersonQueue* temp;
    while (queue != NULL){
        temp = queue->next;
        free(queue);
        queue = temp;
    }
}