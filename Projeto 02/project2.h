/*
 * File:  project2.h
 * Author:  Patrícia Gameiro
 * Description: File contains defining constants, data types, and function      
 * prototypes for a public transportation management system.
 */

#ifndef MAIN_H
#define MAIN_H

#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1

#define INVERSE 8

#define ERR_NO_MEMORY "No memory.\n"
#define EXIT_CODE_NO_MEMORY -2

/* Lines */

#define LINE_ERR_INVALID "incorrect sort option.\n"

/* Stops */

#define STOP_ERR_DUPLICATE "stop already exists.\n"
#define STOP_ERR_NO_SUCH_STOP "no such stop.\n"

/* Links */

#define LINK_ERR_NO_SUCH_LINE "no such line.\n"  
#define LINK_ERR_INVALID "link cannot be associated with bus line.\n"
#define LINK_ERR_NEGATIVE "negative cost or duration.\n"

/**************/
/* Data Types */
/**************/

typedef struct Line Line;

typedef struct Stop Stop;

typedef struct Link Link;

typedef struct Intersection Intersection;

struct Line {
	char *name;
	Link* first;
	Link* last;
	int total_stops;
	double total_cost;
    double total_duration;
    Line *next;
};

struct Stop {
	char *name; 
	double latitude; 
    double longitude;
	int total_lines;
    Intersection* line_head;
    Stop* next;
};

struct Link {
	Line *line;   
	Stop *origin;
    Stop *dest;
	double cost;
    double duration;
    Link* next;
    Link* prev;
};

struct Intersection {
    Line* line;
    Intersection* next;
};

typedef struct {
    Stop* head;
    Stop* tail;
} LinkedStops;

typedef struct {
    Line* head;
    Line* tail;
} LinkedLines;

typedef struct {
    LinkedStops* stops;
    LinkedLines* lines;
} System;

/***********************/
/* Function Prototypes */
/***********************/

/* project2.c */

/* Commands */

int handleCommand(System* global_data);

void handleLinesCommand(System* global_data);

void handleStopsCommand(System* global_data);

void handleLinksCommand(System* global_data);

void handleIntersectionsCommand(System* global_data);

void handleRemoveLineCommand(System* global_data);

void handleRemoveStopCommand(System* global_data);

void restartSystem(System* global_data);

/* Initialization and Deletion */

void initSystem(System* global_data);

void deleteSystem(System* global_data);

/* Auxiliary Functions */

int readSpaces();

char* readName(System* global_data);

void swapStrings(char** array, int i, int j);

int partition(char** array, int start, int end);

void quicksort(char** array, int start, int end);

void* safeMalloc(System* global_data, int size);

void* safeRealloc(System* global_data ,void* p, int size);

/* lines.c */

void addLine(System* global_data, char *line_name);

void showLine(Line* line);

void listAllLines(System* global_data);

void listPath(System* global_data, char *line_name, int inverse);

int findStopinLine(Line* line, Stop* stop);

int checkParameter(char *parameter);

void removeLinefromStop(Line* line);

char** Intersections(Stop* stop);

void removeIntersection(Stop* stop, Line* line);

/* Linked Lines */

LinkedLines* initLinkedLines(System* global_data);

void insertLinkedLines(LinkedLines* lines, Line* node);

Line* searchLinkedLines(LinkedLines* lines, char *name);

void deleteLineNode(LinkedLines* lines, char *name);

void deleteLinks(Line* line);

void deleteLinkedLines(LinkedLines* lines) ;

/* stops.c */

void listAllStops(System* global_data);

void addStop(System* global_data, char *stop_name, double lat, double longi);

void addLinetoStop(System* global_data, Stop* stop, Line* line);

void removeStop(System* global_data, Stop* stop);

void deleteStopfromLine(System* global_data, Line* line, Stop* stop);

void deleteSingleLink(Line* line, Stop* stop);

void deleteFirstLink(Line* line);

void deleteLastLink(Line* line);

void modifyLinks(System* global_data, Line* line, Stop* stop);

/* Linked Stops */

LinkedStops* initLinkedStops(System* global_data);

void insertLinkedStops(LinkedStops* stops, Stop* node);

Stop* searchLinkedStops(LinkedStops* stops, char *name);

void deleteStopNode(LinkedStops* stops, char *name);

void deleteIntersections(Stop* stop);

void deleteLinkedStops(LinkedStops* stops);

/* links.c */

Link* createLink(System* global_data, Line* line, Stop* origin, Stop* dest, 
                 double cost, double duration);

void addLink(System* global_data, Line* line, Stop* orig, Stop* dest, double    
             cost, double duration);

void addFirstLink(System* global_data, Line* line, Stop* orig, Stop* dest,
                  double cost, double duration);

void addLinkStart(Line* line, Link* link);

void addLinkEnd(Line* line, Link* link);

#endif
