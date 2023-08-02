/* iaed-23 - ist1107245 - project1 */

/*
 * File:  project1.h
 * Author:  Patrícia Gameiro
 * Description: File contains defining constants, data types, and function      * prototypes for a public transportation management system.
 */

#ifndef MAIN_H
#define MAIN_H

#define TRUE 1
#define FALSE 0

#define NEWLINE 2

#define INVERSE 8

/* Lines */

#define MAX_LINES 200
#define MAX_LINE_NAME 21

#define LINE_ERR_INVALID "incorrect sort option.\n"

/* Stops */

#define MAX_STOPS 10000
#define MAX_STOP_NAME 51

#define STOP_ERR_DUPLICATE "stop already exists.\n"
#define STOP_ERR_NOT_FOUND "no such stop.\n"

/* Links */

#define LINK_ERR_NO_SUCH_LINE "no such line.\n"  
#define LINK_ERR_INVALID "link cannot be associated with bus line.\n"
#define LINK_ERR_NEGATIVE "negative cost or duration.\n"

/* Data Types */

typedef struct {
	char name[MAX_STOP_NAME];
	double latitude;
	double longitude;
	int total_lines;
} Stop;

typedef struct {
	char name[MAX_LINE_NAME];
	Stop origin;
	Stop dest;
	int id_stops[MAX_STOPS];
	int total_stops;
	double total_cost;
	double total_duration;
} Line;

typedef struct {
	Line line;
	Stop origin;
	Stop dest;
	double cost;
	double duration;
} Link;

/* project.c */

void handleLinesCommand();

void addLine(char lineName[]);

void listAllLines();

void listPath(int id_line, int inverse);

void updateLine(int id_line, int id_origin, int id_destination, int direction);

int findLine(char lineName[], int *id_line);

int checkParameter(char parameter[]);

void handleStopsCommand();

void listAllStops();

void addStop(char name[], double latitude, double longitude);

int findStop(char name[], int *id_stop);

void handleLinksCommand();

int checkDirection(Link link, int id_line);

int errorCheckLink(Link link, int *id_line, int *id_origin, int *id_dest);

void handleIntersectionsCommand();

int findIntersections(int id_stop, char intersections[][MAX_LINE_NAME]);

int readInput(char array[], int length);

void swapStrings(char array[][MAX_LINE_NAME], int i, int j);

int partition(char array[][MAX_LINE_NAME], int start, int end);

void quicksort(char array[][MAX_LINE_NAME], int start, int end);

#endif