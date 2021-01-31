
// Created by kerim on 28.11.2019.

/*
 * My project consist of 2 c file and 1 header file.
 * The declarations of functions are in header file.
 * The functions are in functions.c file
 * The main function is in this file.
 */

#include "struct.h"
#define MAXLINE 150        // The maximum length of a single line for input file

int main(int argc, char *argv[]) {

    if(argc != 3){
        printf("few arguments error\n");
        return -1;
    }
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");

    if (inputFile == NULL || outputFile == NULL){
        printf("error\n");
        return -1;
    }

    PersonList *personList = NULL;                                      // Every person is collected in this list
    FlightList* flightList = NULL;                                      // Every flight is collected in this list

    char* tempLine = malloc(sizeof(char)* MAXLINE);

    while (fgets(tempLine, MAXLINE, inputFile) != NULL){

        char *del = " \n\r\t";
        char *line1 = malloc(sizeof(char) * strlen(tempLine)+1);
        strcpy(line1, tempLine);

        int count_input = 0;
        char *token = strtok(line1, del);
        while (token != NULL) {
            token = strtok(NULL, del);                       // counts the arguments of the line to get how many argument is given.
            ++count_input;
        }
        char *line2 = malloc(strlen(tempLine)+1);
        strcpy(line2, tempLine);

        char **inputs = malloc(sizeof(char *) * count_input);
        char *token2 = strtok(line2, del);
        int i = 0;

        while (token2 != NULL) {
            *(inputs + i) = malloc(strlen(token2) + 1);
            strcpy(*(inputs + i), token2);                 //  takes the arguments of the line and puts them to char**inputs
            token2 = strtok(NULL, del);
            ++i;
        }
        if(strcmp(*inputs,"addseat") == 0){

            if(count_input != 4)
                fprintf(outputFile, "error\n");
            else if(getFlight(flightList, *(inputs+1)) != NULL){  // If flight already exists, this code block works

                Flight* flight = getFlight(flightList, *(inputs+1));
                if(flight->isopen == 0)
                    fprintf(outputFile,"error\n");
                else{
                if(strcmp(*(inputs+2), "business") == 0)
                    flight->Business->capacity+= atoi(*(inputs+3));
                else if(strcmp(*(inputs+2), "economy") == 0)
                    flight->Economy->capacity+= atoi(*(inputs+3));
                else if(strcmp(*(inputs+2), "standard") == 0)
                    flight->Standard->capacity+= atoi(*(inputs+3));
                fprintf(outputFile,"addseats %s %d %d %d\n",flight->name,flight->Business->capacity,
                        flight->Economy->capacity,flight->Standard->capacity);
                }
            }
            else{
                Flight* flight;
                if (strcmp(*(inputs+2), "business") == 0)
                    flight = newFlight(*(inputs+1),atoi(*(inputs+3)),0,0);
                else if (strcmp(*(inputs+2), "economy") == 0)
                    flight = newFlight(*(inputs+1),0,atoi(*(inputs+3)),0);
                else if (strcmp(*(inputs+2), "standard") == 0)
                    flight = newFlight(*(inputs+1),0,0,atoi(*(inputs+3)));
                else {
                    flight = NULL;
                    fprintf(outputFile,"error\n");
                }
                if(flight != NULL){
                    pushFlight(&flightList, flight);
                    fprintf(outputFile,"addseats %s %d %d %d\n",flight->name,flight->Business->capacity,
                            flight->Economy->capacity,flight->Standard->capacity);
                }
            }
        }else if(strcmp(*inputs,"enqueue") == 0){

            if(count_input < 4 || count_input > 5)
                fprintf(outputFile,"error\n");
            else if(strcmp(*(inputs+2),"business") != 0 && strcmp(*(inputs+2),"economy") != 0 && strcmp(*(inputs+2),"standard") != 0)
                fprintf(outputFile,"error\n");
            else if(getFlight(flightList,*(inputs+1)) == NULL)
                fprintf(outputFile, "error\n");
            else if(count_input == 5 && strcmp(*(inputs+2),"business") == 0 && strcmp(*(inputs+4),"diplomat") != 0)
                fprintf(outputFile,"error\n");
            else if(count_input == 5 && strcmp(*(inputs+2),"economy") == 0 && strcmp(*(inputs+4),"veteran") != 0)
                fprintf(outputFile,"error\n");
            else if(count_input == 5 && strcmp(*(inputs+2),"standard") == 0)
                fprintf(outputFile,"error\n");
            else{
                Flight* flight = getFlight(flightList,*(inputs+1));
                if(flight->isopen == 0)
                    fprintf(outputFile,"error\n");
                else if(getList(personList,*(inputs+3)) != NULL)
                    fprintf(outputFile,"error\n");
                else{
                    int class = -1;
                    int priority = NORMAL;
                    if(strcmp(*(inputs+2),"business") == 0) {
                        class = BUSINESS;
                        priority = BUSINESS_NORMAL;
                    }
                    else if(strcmp(*(inputs+2),"economy") == 0){
                        class = ECONOMY;
                        priority = ECONOMY_NORMAL;
                    }
                    else if(strcmp(*(inputs+2),"standard") == 0){
                        class = STANDARD;
                        priority = NORMAL;
                    }
                    if(count_input == 5){
                        if(strcmp(*(inputs+4),"diplomat")  == 0)
                            priority = BUSINESS_DIPLOMAT;
                        else if(strcmp(*(inputs+4),"veteran") == 0)
                            priority = ECONOMY_VETERAN;
                    }
                    if(count_input == 5 && priority == NORMAL)
                        fprintf(outputFile,"error\n");
                    else{
                        Person* person = newPerson(*(inputs+3),*(inputs+1),class,priority);
                        pushList(&personList,person);
                        pushQueue(&flight->queue, person);
                        ++flight->size;
                        fprintf(outputFile,"queue %s %s ",flight->name,person->name);
                        switch (person->wanted){
                            case 1:
                                fprintf(outputFile,"business ");
                                break;
                            case 2:
                                fprintf(outputFile,"economy ");
                                break;
                            case 3:
                                fprintf(outputFile,"standard ");
                            default:
                                break;
                        }
                        fprintf(outputFile,"%d\n",indexQueue(&flight->queue, person->wanted));
                    }
                }
            }
        }else if(strcmp(*inputs,"sell") == 0){

            if(count_input != 2)
                fprintf(outputFile,"error\n");
            else if(getFlight(flightList, *(inputs+1)) == NULL)
                fprintf(outputFile, "error\n");
            else {
                Flight *flight = getFlight(flightList, *(inputs + 1));
                if (flight->isopen != 0) {
                    sellFlight(flight);
                    fprintf(outputFile, "sold %s %d %d %d\n", flight->name, flight->Business->top, flight->Economy->top, flight->Standard->top);
                }
                else
                    fprintf(outputFile,"error\n");
            }
        }else if(strcmp(*inputs,"close") == 0){

            if(count_input != 2)
                fprintf(outputFile,"error\n");
            else {
                Flight *flight = getFlight(flightList, *(inputs + 1));
                if (flight == NULL)
                    fprintf(outputFile, "error\n");
                else if (flight->isopen == 0)
                    fprintf(outputFile, "error\n");
                else {
                    flight->isopen = 0;
                    fprintf(outputFile, "closed %s %d %d\n", flight->name,
                            (flight->Business->top + flight->Economy->top + flight->Standard->top), flight->size);
                    PersonQueue *temp = flight->queue;
                    while (temp != NULL) {
                        fprintf(outputFile, "waiting %s\n", temp->person->name);
                        temp = temp->next;
                    }
                }
            }
        }else if(strcmp(*inputs,"report") == 0){

            if(count_input != 2)
                fprintf(outputFile, "error\n");

            else if(getFlight(flightList, *(inputs+1)) != NULL){

                Flight* flight = getFlight(flightList, *(inputs+1));
                fprintf(outputFile,"report %s\n",flight->name);
                PersonStack* temp = flight->Business->personStack;
                fprintf(outputFile,"business %d\n",flight->Business->top);
                for ( i = 0; i < flight->Business->top && temp != NULL; ++i) {
                    fprintf(outputFile,"%s\n", temp->person->name);
                    temp = temp->next;
                }
                temp = flight->Economy->personStack;
                fprintf(outputFile,"economy %d\n",flight->Economy->top);
                for ( i = 0; i < flight->Economy->top && temp != NULL; ++i) {
                    fprintf(outputFile,"%s\n", temp->person->name);
                    temp = temp->next;
                }
                temp = flight->Standard->personStack;
                fprintf(outputFile,"standard %d\n",flight->Standard->top);
                for ( i = 0; i < flight->Standard->top && temp != NULL; ++i) {
                    fprintf(outputFile,"%s\n", temp->person->name);
                    temp = temp->next;
                }
                fprintf(outputFile,"end of report %s\n",flight->name);
            }
            else{
                fprintf(outputFile,"error\n");
            }
        }else if(strcmp(*inputs,"info") == 0){

            if(count_input != 2)
                fprintf(outputFile,"error\n");

            else if(getList(personList, *(inputs+1)) != NULL){

                Person* person = getList(personList,*(inputs+1));
                fprintf(outputFile,"info %s %s ", person->name,person->flight);
                switch (person->wanted){
                    case BUSINESS:
                        fprintf(outputFile,"business ");
                        break;
                    case ECONOMY:
                        fprintf(outputFile,"economy ");
                        break;
                    case STANDARD:
                        fprintf(outputFile,"standard ");
                        break;
                    default:
                        break;
                }
                switch (person->taken){
                    case BUSINESS:
                        fprintf(outputFile,"business\n");
                        break;
                    case ECONOMY:
                        fprintf(outputFile,"economy\n");
                        break;
                    case STANDARD:
                        fprintf(outputFile,"standard\n");
                        break;
                    default:
                        fprintf(outputFile,"none\n");
                        break;
                }
            }
            else
                fprintf(outputFile,"error\n");
        }
        else
            fprintf(outputFile, "error\n");
        for ( i = 0; i < count_input ; ++i)
            free(*(inputs+i));
        free(inputs);
        if(line1 != NULL)
            free(line1);
        if(line2 != NULL)
            free(line2);
        if(token != NULL)
            free(token);
        if(token2 != NULL)
            free(token2);
    }
    fclose(inputFile);
    fclose(outputFile);
    free(tempLine);
    while (personList != NULL){
        PersonList* temp = personList->next;
        free(personList->person->name);
        free(personList->person->flight);
        free(personList->person);
        free(personList);
        personList = temp;
    }
    while (flightList != NULL){
        FlightList* temp = flightList->next;
        flightFree(flightList->flight);
        free(flightList);
        flightList = temp;
    }
    free(flightList);
}