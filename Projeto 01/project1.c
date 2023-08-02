/*
 * File:  project1.c
 * Author:  Patrícia Gameiro
 * Description: Public transport career management system.
 * The program has four main functions: adding or listing lines, adding or   
 * listing stops, listing links between stops, and listing intersections 
 * between lines.
 */

#include "project1.h"

#include <stdio.h>
#include <string.h>

/* Global Variables */

Line _lines[MAX_LINES];
int _num_lines = 0;

Stop _stops[MAX_STOPS];
int _num_stops = 0;


int main() {
	char c;

	while ((c = getchar()) != EOF)
		switch (c) {
			case 'q':
				/* stop the program */
				return 0;
			case 'c':
				handleLinesCommand();
				break;
			case 'p':
				handleStopsCommand();
				break;
			case 'l':
				handleLinksCommand();
				break;
			case 'i':
				handleIntersectionsCommand();
				break;
			default:
				/* handles unknown commands */
				break;
		}
	return 0;
}

/* Lines */

/*
 * Handles the 'c' command.
 * Adds or lists lines depending on the input.
 */
void handleLinesCommand() {
	Line line;
	int id_line, state = 0;
	char parameter[INVERSE];

	if ((state = readInput(line.name, MAX_LINE_NAME))) {
		/* check if the line already exists */
		if (findLine(line.name, &id_line)) {
			/* if there was a parameter written after the line name */
			if (state != NEWLINE) {
				readInput(parameter, INVERSE);
				if (checkParameter(parameter))
					listPath(id_line, TRUE);
				else
					printf(LINE_ERR_INVALID);
			}
			else
				listPath(id_line, FALSE);
		}
		/* if the line does not already exist, add it */
		else
			addLine(line.name);
	}
	else
		listAllLines();
}

/*
 * Adds a new line to the system.
 */
void addLine(char line_name[]) {
	strcpy(_lines[_num_lines].name, line_name);
	_lines[_num_lines].total_stops = 0;
	_lines[_num_lines].total_cost = 0;
	_lines[_num_lines++].total_duration = 0;
}

/*
 * Prints information about all lines in the system.
 */
void listAllLines() {
	int i;

	for (i = 0; i < _num_lines; i++) {
		/* check if the current line has no stops */
		if (_lines[i].total_stops == 0)
			printf("%s %d %.2f %.2f\n", _lines[i].name, _lines[i].total_stops,
				_lines[i].total_cost, _lines[i].total_duration);
		else
			printf("%s %s %s %d %.2f %.2f\n", _lines[i].name, 
				_lines[i].origin.name, _lines[i].dest.name,
				 _lines[i].total_stops, _lines[i].total_cost, 
				 	_lines[i].total_duration);
	}			
}

/*
 * Prints the stops along a given line in either forward or reverse order.
 */
void listPath(int id_line, int inverse) {
	int i, j, len;
	len = _lines[id_line].total_stops - 1;

	/* if the line has no stops, return without printing anything */
	if (_lines[id_line].total_stops == 0)
		return;
	else if (inverse == FALSE) {  /* forward order */
		for (i = 0; i < len; i++) {
			j = _lines[id_line].id_stops[i];
			printf("%s, ", _stops[j].name);
		}
		j = _lines[id_line].id_stops[i];
		printf("%s\n", _stops[j].name);
	}
	else {  /* reverse order */
		for (i = len; i > 0; i--) {
			j = _lines[id_line].id_stops[i];
			printf("%s, ", _stops[j].name);
		}
		j = _lines[id_line].id_stops[i];
		printf("%s\n", _stops[j].name);
	}			
}

/*
 * Updates an existing line with the given origin and destination stops.
 * The direction parameter determines whether the new destination stop is added
 * to the end of the line or at the beginning.
 */
void updateLine(int id_line, int id_origin, int id_dest, int direction) { 
	int i;
	/* if the line has no stops yet, 
	 * add the origin and destination stops to it */
	if (_lines[id_line].total_stops == 0) {
		strcpy(_lines[id_line].origin.name, _stops[id_origin].name);
		strcpy(_lines[id_line].dest.name, _stops[id_dest].name);
		_lines[id_line].id_stops[_lines[id_line].total_stops++] = id_origin;
		_lines[id_line].id_stops[_lines[id_line].total_stops++] = id_dest;
		_stops[id_origin].total_lines++;
		_stops[id_dest].total_lines++;
	}
	/* if the line has stops and the direction is forward */
	else if (direction == TRUE) {
		strcpy(_lines[id_line].dest.name, _stops[id_dest].name);
		_lines[id_line].id_stops[_lines[id_line].total_stops++] = id_dest;
		/* checks if line is circular */
		if (strcmp(_lines[id_line].origin.name, _lines[id_line].dest.name))
			_stops[id_dest].total_lines++;
	}
	/* if the line has stops and the direction is backward */
	else {
		/* shifts the id_stops array to the right */
		for (i = _lines[id_line].total_stops; i > 0; i--)       
    		_lines[id_line].id_stops[i] = _lines[id_line].id_stops[i-1];
		strcpy(_lines[id_line].origin.name, _stops[id_origin].name);
		_lines[id_line].id_stops[0] = id_origin;
		_lines[id_line].total_stops++;
		/* checks if line is circular */
		if (strcmp(_lines[id_line].origin.name, _lines[id_line].dest.name))
			_stops[id_origin].total_lines++;
	}
}

/*
 * Finds a line in the array of lines and returns a value indicating whether or
 * not the line was found. If the line is found, the id of the line is passed
 * back through the id_line pointer.
 */
int findLine(char line_name[], int *id_line) {
	int i;

	for (i = 0; i < _num_lines; i++)
		if (strcmp(_lines[i].name, line_name) == 0) {
			*id_line = i;
            return TRUE; 
		}	
	return FALSE;
}

/*
 * Checks if a given string is a valid parameter.
 */
int checkParameter(char parameter[]) {   
	char inverse[] = {'i', 'n', 'v', 'e', 'r', 's', 'o', '\0'};
	int i = 0, len = strlen(parameter);

	while (i < len && inverse[i] == parameter[i]) 
		i++;
	return (i == len && i > 2);
}

/* Stops */

/*
 * Handles the 'p' command.
 * Adds or lists stops depending on the input.
 */
void handleStopsCommand() {
	Stop stop;
	double latitude, longitude;
	int id_stop, in_list;

	if (readInput(stop.name, MAX_STOP_NAME)) {
		/* check if the stop already exists */
		in_list = findStop(stop.name, &id_stop);
        if (scanf("%lf %lf", &latitude, &longitude) == 2) {
			if (in_list == FALSE) 
        		addStop(stop.name, latitude, longitude);
			else 
				printf("%s: %s", stop.name, STOP_ERR_DUPLICATE);
		}
		else {
			/* if the stop does not exist and no coordinates are given */
			if (in_list == FALSE) 
				printf("%s: %s", stop.name,STOP_ERR_NOT_FOUND);
			else
				printf("%16.12f %16.12f\n",_stops[id_stop].latitude, 
					_stops[id_stop].longitude);
		}
	}
	/* if no stop name is given, list all stops */
	else
		listAllStops();
}

/*
 * Lists all stops in the system with their coordinates and the number of
 * lines that pass through them.
 */
void listAllStops() {
	int i;

	for (i = 0; i < _num_stops; i++) 
		printf("%s: %16.12f %16.12f %d\n", _stops[i].name, _stops[i].latitude, 
			_stops[i].longitude, _stops[i].total_lines);
}

/*
 * Adds a new stop to the system.
 */
void addStop(char name[], double latitude, double longitude) {
	strcpy(_stops[_num_stops].name, name);
	_stops[_num_stops].latitude = latitude;
	_stops[_num_stops].longitude = longitude;
	_stops[_num_stops++].total_lines = 0;
}

/*
 * Finds a stop in the array of stops and returns a value indicating whether or
 * not the stop was found. If the stop is found, the id of the stop is passed
 * back through the id_stop pointer.
 */
int findStop(char stop_name[], int *id_stop) {
	int i;

	for (i = 0; i < _num_stops; i++)
		if (strcmp(_stops[i].name, stop_name) == 0) {
			*id_stop = i;
			return TRUE; 
		}
	return FALSE;
}

/* Links */

/*
 * Handles the 'l' command.
 * Adds lines to the system.
 */
void handleLinksCommand() {
	Link link; 
	int id_line, id_origin, id_destination, forward = FALSE;

	readInput(link.line.name, MAX_LINE_NAME);
	readInput(link.origin.name, MAX_STOP_NAME);
	readInput(link.dest.name, MAX_STOP_NAME);
	if (scanf(" %lf %lf", &link.cost, &link.duration) != 2)
		return;
	/* check if the input is valid, and get the ids of the line, origin stop,
	 * and destination stop */
	if (errorCheckLink(link, &id_line, &id_origin, &id_destination)) {
		_lines[id_line].total_cost += link.cost;
		_lines[id_line].total_duration += link.duration;
		/* check if the link goes forward or backward */
		forward = checkDirection(link, id_line);
		updateLine(id_line, id_origin, id_destination, forward);
	}
}	

/*
 * Checks if the link goes forward or backward  in relation 
 * to the line direction.
 */
int checkDirection(Link link, int id_line) {
	int forward = FALSE;  
	
	/* check if the link goes from origin to destination (forward) */
	if (strcmp(_lines[id_line].dest.name, link.origin.name) == 0) 
		forward = TRUE;
	return forward;
} 

/*
 * Check if the link is valid.
 */
int errorCheckLink(Link link, int *id_line, int *id_origin, int *id_dest) {
	if (!findLine(link.line.name, *&id_line)) {
		printf("%s: %s", link.line.name, LINK_ERR_NO_SUCH_LINE);
		return FALSE;
	}
	if (!findStop(link.origin.name, *&id_origin)) {
		printf("%s: %s", link.origin.name, STOP_ERR_NOT_FOUND );
		return FALSE;
	}
	if (!findStop(link.dest.name, *&id_dest)) {
		printf("%s: %s", link.dest.name, STOP_ERR_NOT_FOUND );
		return FALSE;
	}
	/* check if the link is valid based on the existing stops of the line */
	if (strcmp(_lines[*id_line].origin.name, link.dest.name) != 0 && 
			strcmp(_lines[*id_line].dest.name, link.origin.name) != 0 && 
				_lines[*id_line].total_stops != 0) {
		printf(LINK_ERR_INVALID);
		return	FALSE;
	}
	if (link.cost < 0 || link.duration < 0) {
		printf(LINK_ERR_NEGATIVE);
		return	FALSE;
	}
	return TRUE;
}

/* Intersections */

/*
 * Handles the 'i' command.
 * Finds the intersections of the lines in the system.
 */
void handleIntersectionsCommand() {
	int i, j, num_intersections = 0;
	char intersections[MAX_LINES][MAX_LINE_NAME];

	for (i = 0; i < _num_stops; i++)
		if (_stops[i].total_lines > 1) {
			num_intersections = findIntersections(i, intersections);
			printf("%s %d:", _stops[i].name, _stops[i].total_lines);
			for (j = 0; j < num_intersections ; j++) 
				printf(" %s", intersections[j]);
			printf("\n");
		}	
}

/*
 * Finds the lines that pass through a given stop and stores their names in
 * intersections array in alphabetical order.
 * Returns the number of intersections.
 */
int findIntersections(int id_stop, char intersections[][MAX_LINE_NAME]) {
	int i, j, num_intersections = 0;

	for (i = 0; i < _num_lines; i++) 
		for (j = 0; j < _lines[i].total_stops; j++) 
			if (_lines[i].id_stops[j] == id_stop) {
				strcpy(intersections[num_intersections], _lines[i].name);
            	num_intersections++;
				break;
			}
	/* sorts intersections array in alphabetical */
	quicksort(intersections, 0, num_intersections - 1);
	return num_intersections;
}

/* Auxiliary Functions */

/*
 * Reads from the standard input, up to a maximum of length.
 * The input is stored in array. If the input is a single newline
 * character, returns NEWLINE. If the input is empty, returns FALSE. 
 * Otherwise, returns TRUE.
 */
int readInput(char array[], int length) {
    int c, i = 0;
    enum {NORMAL, QUOTES} state = NORMAL;

	/* skip leading spaces */
	while ((c = getchar()) == ' ');
    while (c != EOF && c != '\n' && i < length) {
		/* toggles QUOTES state on and off */
        if ( c == '\"') 
            state ^= QUOTES;
        else if (state == NORMAL && c == ' ')
            break;
        else
            array[i++] = c;
		c = getchar();
    }     
	if (i == 0)
		return FALSE;
	array[i] = '\0';
	if (c == '\n') 
        return NEWLINE;
	return TRUE;
}

/*
 * Swaps two strings.
 */
void swapStrings(char array[][MAX_LINE_NAME], int i, int j) {
    char temp[MAX_LINE_NAME];
    memmove(temp, array[i], sizeof(temp));
    memmove(array[i], array[j], sizeof(array[i]));
    memmove(array[j], temp, sizeof(temp));
}

/*
 * Partitions the array in two parts, one with elements smaller than the pivot
 * and one with elements greater than the pivot.
 */
int partition(char array[][MAX_LINE_NAME], int start, int end) {
    char pivot[MAX_LINE_NAME];
    int i = start - 1;
    int j = end + 1;

    strcpy(pivot, array[start]);
    while (TRUE) {
		/* find the first element from the left that is greater than
		 * or equal to the pivot */
        do 
            i++;
        while (strcmp(array[i], pivot) < 0);
		/* find the first element from the right that is less than 
		 * or equal to the pivot */
        do 
            j--;
        while (strcmp(array[j], pivot) > 0);
        if (i >= j) 
            return j;
        swapStrings(array, i, j);
    }
}

/*
 * Sorts a two-dimensional array in alphabetical order using
 * the quicksort algorithm.
 */
void quicksort(char array[][MAX_LINE_NAME], int start, int end) {
	int idpivot;
    if (start < end) { 
		/* partition the array and get the index of the pivot */
        idpivot = partition(array, start, end);
        quicksort(array, start, idpivot);
        quicksort(array, idpivot + 1, end);
    }
}