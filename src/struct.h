#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUSINESS 1               // the type of the seat
#define ECONOMY 2                // the type of the seat
#define STANDARD 3               // the type of the seat
#define BUSINESS_DIPLOMAT 1      // the priority of the person
#define BUSINESS_NORMAL 2        // the priority of the person
#define ECONOMY_VETERAN 3        // the priority of the person
#define ECONOMY_NORMAL 4         // the priority of the person
#define NORMAL 5                 // the priority of the person

typedef struct {
    char *name;
    int wanted;
    int taken;
    int priority;
    char* flight;
}Person;                             // the struct of an individual person
typedef struct persons{
    Person* person;
    struct persons* next;
}PersonList;                        // the struct to keep the list of persons
typedef struct stack{
    Person* person;
    struct stack* next;
}PersonStack;                      // the stack to keep the persons in class
typedef struct{
    int class;
    int capacity;
    int top;
    PersonStack* personStack;
}Class;
typedef struct queue{
    Person* person;
    struct queue* next;
}PersonQueue;                     // to keep persons who wait to buy ticket
typedef struct {
    char *name;
    Class* Business;
    Class* Economy;
    Class* Standard;
    PersonQueue* queue;
    int size;
    int isopen;
}Flight;                         // the struct of an individual flight
typedef struct flights{
    Flight* flight;
    struct flights* next;
}FlightList;                    // the struct to keep the list of flights

Person* newPerson(char* name, char* flightName, int wanted, int priority);
Class* newClass(int class, int capacity);
Flight* newFlight(char* name, int business, int economy, int standard);

void pushList(PersonList** personList, Person* person);
Person* getList(PersonList* personList, char* name);

void pushStack(PersonStack** personStack, Person* person);

void pushQueue(PersonQueue** personQueue, Person* person);
int indexQueue(PersonQueue** personQueue, int class);

void pushFlight(FlightList** flightList, Flight* flight);
Flight* getFlight(FlightList* flightList, char* name);
void sellFlight(Flight* flight);

void flightFree(Flight* flight);
void classFree(Class* class);
void stackFree(PersonStack* stack);
void queueFree(PersonQueue* queue);