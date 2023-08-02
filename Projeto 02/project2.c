/*
 * File:  project2.c
 * Author:  Patrícia Gameiro
 * Description: Public transport career management system.
 * The program has seven main functions: adding or listing lines, adding or   
 * listing stops, listing links between stops, listing intersections 
 * between lines, removing a line, removing a stop, and restarting the system.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project2.h"

int main() {
	System global_data = {0};
    
    initSystem(&global_data);
	/* execute program until the user sends the 'q' command */
	while (handleCommand(&global_data)) {
	}
    /* free all memory allocated */
    deleteSystem(&global_data);
	return 0;
}

/*
 * Reads a character from standard input and executes the corresponding command
 */
int handleCommand(System *global_data) {
	char c = getchar();

	switch (c) {
		case 'q':
            /* stop the program */
			return FALSE;
		case 'c':
			handleLinesCommand(global_data);
			return TRUE;
		case 'p':
			handleStopsCommand(global_data);
			return TRUE;
		case 'l':
			handleLinksCommand(global_data);
			return TRUE;
		case 'i':
			handleIntersectionsCommand(global_data);
			return TRUE;
        case 'r':
            handleRemoveLineCommand(global_data);
            return TRUE;
        case 'e':
            handleRemoveStopCommand(global_data);
            return TRUE;
        case 'a':
            restartSystem(global_data);
            return TRUE;
		default:
			/* handles unknown commands */
			return TRUE;
	}
}

/* Commands */

/*
 * Handles the 'c' command.
 * Adds or lists lines depending on the input.
 */
void handleLinesCommand(System* global_data) {
    Line* line;
	int endOfLine= readSpaces();
	char *line_name, *parameter;

	if (!endOfLine) {
        listAllLines(global_data);
        return;
    }
    line_name = readName(global_data);
    endOfLine = readSpaces();
    line = searchLinkedLines(global_data->lines, line_name);
    if (!endOfLine) {
        /* if the line does not already exist, add it */
        if (line == NULL) {
            addLine(global_data, line_name);}
        else 
            listPath(global_data, line_name, FALSE);
    }
    /* if there was a parameter written after the line name */
    else {
        parameter = readName(global_data);
        if (line != NULL && checkParameter(parameter)) 
            listPath(global_data, line_name ,TRUE);
        else if (line != NULL) {
            printf(LINE_ERR_INVALID);
        }
        free(parameter);
    }
}

/*
 * Handles the 'p' command.
 * Adds or lists stops depending on the input.
 */
void handleStopsCommand(System* global_data) {
    char *stop_name;
    int endOfLine = readSpaces(), add = FALSE;
    Stop *stop;
    
    /* if no stop name is given, list all stops */
    if (!endOfLine) {
        listAllStops(global_data);
        return;
    }
    stop_name = readName(global_data);
    endOfLine = readSpaces();
    if (!endOfLine) {
        stop = searchLinkedStops(global_data->stops, stop_name);
        /* if the stop does not exist and no coordinates are given */
        if (stop == NULL)
            printf("%s: %s", stop_name, STOP_ERR_NO_SUCH_STOP);
        else        
            printf("%16.12f %16.12f\n", stop->latitude, stop->longitude);
    }
    else {
        double latitude, longitude;
        /* if the stop does not exist and coordinates are given */
        if (searchLinkedStops(global_data->stops, stop_name) == NULL &&
            scanf("%lf %lf", &latitude, &longitude) == 2) {
            add = TRUE;
            addStop(global_data, stop_name, latitude, longitude);
        }
        else
            printf("%s: %s", stop_name, STOP_ERR_DUPLICATE);
    }
    if (!add)
        free(stop_name);
}

/*
 * Handles the 'l' command.
 * Adds lines to the system.
 */
void handleLinksCommand(System* global_data) {
    double cost, duration;
    char *line_name, *orig_name, *dest_name;
    Line* line;
    Stop* orig, *dest;

	line_name = readName(global_data);
	orig_name = readName(global_data);
	dest_name = readName(global_data);
	if (scanf(" %lf %lf", &cost, &duration) != 2)
		return;
    /* check if the input is valid, and get the ids of the line, origin stop,
	 * and destination stop */
    if ((line = searchLinkedLines(global_data->lines, line_name)) == NULL)
        printf("%s: %s", line_name, LINK_ERR_NO_SUCH_LINE);
    else if ((orig = searchLinkedStops(global_data->stops, orig_name)) == NULL)
        printf("%s: %s", orig_name, STOP_ERR_NO_SUCH_STOP);
    else if ((dest = searchLinkedStops(global_data->stops, dest_name)) == NULL)
        printf("%s: %s", dest_name, STOP_ERR_NO_SUCH_STOP);
    else if (cost < 0.0 || duration < 0.0)
        printf("%s", LINK_ERR_NEGATIVE);
    else {
        addLink(global_data, line, orig, dest, cost, duration);
    }
    free(line_name);
    free(orig_name);
    free(dest_name);
}

/*
 * Handles the 'i' command.
 * Finds the intersections of the lines in the system.
 */
void handleIntersectionsCommand(System* global_data) {
    int i;
    char** list_lines;
    Stop* current = global_data->stops->head;

    while (current != NULL) {
        if (current->total_lines > 1) {
            printf("%s %d: ", current->name, current->total_lines);
            /* get the list of lines that pass through the stop, in
             * alphabetical order */
            list_lines = Intersections(current);
            for (i = 0; i < current->total_lines - 1; i++) 
                printf("%s ", list_lines[i]);
            printf("%s\n", list_lines[i]);
            free(list_lines);
        }
        current = current->next;
    }
}

/*
 * Handles the 'r' command.
 * Removes a line from the system.
 */
void handleRemoveLineCommand(System* global_data) {
    char* line_name = readName(global_data);
    Line* line = searchLinkedLines(global_data->lines, line_name);

    if (line == NULL)
        printf("%s: %s", line_name, LINK_ERR_NO_SUCH_LINE);
    else {
        if (line->total_stops > 0) {
            removeLinefromStop(line);
        }
        deleteLineNode(global_data->lines, line_name);
    }
    free(line_name);
}

/*
 * Handles the 'e' command.
 * Removes a stop from the system.
 */
void handleRemoveStopCommand(System* global_data) {
    char* stop_name = readName(global_data);
    Stop* stop = searchLinkedStops(global_data->stops, stop_name);

    if (stop == NULL)
        printf("%s: %s", stop_name, STOP_ERR_NO_SUCH_STOP);
    else {
        if (stop->total_lines > 0)
            removeStop(global_data, stop);
        deleteStopNode(global_data->stops, stop_name);
    }
    free(stop_name);
}

/*
 * Handles the 'a' command.
 * Restarts the program.
 */
void restartSystem(System* global_data) {
    deleteSystem(global_data);
    initSystem(global_data);
}

/* Initialization and Deletion */

/*
 * Initializes the system.
 */
void initSystem(System* global_data) {
    global_data->stops = initLinkedStops(global_data);
    global_data->lines = initLinkedLines(global_data);
}

/*
 * Deletes the system.
 */
void deleteSystem(System* global_data) {
    deleteLinkedStops(global_data->stops);
    deleteLinkedLines(global_data->lines);
}

/* Auxiliary Functions */

/*
 * Reads spaces. Returns 0 if end of line is reached or 1 otherwise.
 */
int readSpaces() {
    int c;

    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return FALSE;
    ungetc(c, stdin);
    return TRUE;
}

/*
 * Reads a string from standard input and returns a dynamically allocated array
 * with the input. It dynamically increases the buffer size as needed to 
 * prevent buffer overflow. 
 */
char* readName(System* global_data) {
    int i = 0, c, buf_size = 15;
    enum {NORMAL, QUOTES} state = NORMAL;
    char* s = safeMalloc(global_data ,buf_size);

    /* skip leading spaces */
    while ((c = getchar()) == ' ');
    while (c != EOF && c != '\t' && c != '\n') {
        /* toggles QUOTES state on and off */
        if ( c == '\"') 
            state ^= QUOTES;
        else if (state == NORMAL && c == ' ')
            break;
        else {
            if (i == buf_size - 1) {
                buf_size *= 2;
                s = safeRealloc(global_data, s, buf_size);
            }
            s[i++] = c;
        }
        c = getchar();
    }
    ungetc(c, stdin);     
    s[i] = '\0';
    return s;
}

/*
 * Swaps two strings.
 */
void swapStrings(char** array, int i, int j) {
    char* temp = array[i];
    
    array[i] = array[j];
    array[j] = temp;
}

/*
 * Partitions the array in two parts, one with elements smaller than the pivot
 * and one with elements greater than the pivot.
 */
int partition(char** array, int start, int end) {
    char* pivot = array[start];
    int i = start - 1;
    int j = end + 1;
    
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
void quicksort(char** array, int start, int end) {
	int idpivot;

    if (start < end) { 
		/* partition the array and get the index of the pivot */
        idpivot = partition(array, start, end);
        quicksort(array, start, idpivot);
        quicksort(array, idpivot + 1, end);
    }
}

/*
 * Allocates memory using the standard library function malloc() and returns a 
 * void pointer to the allocated memory.
 * If memory allocation fails, it prints an error message and exits the program.
 */ 
void* safeMalloc(System* global_data, int size) {
	void* p = malloc(size);

	if (p != NULL) 
        return p;
	printf(ERR_NO_MEMORY);
    deleteSystem(global_data);
	exit(EXIT_CODE_NO_MEMORY);
	return NULL;
}

/*
 * Reallocates memory for the given pointer p, to the size specified by size, 
 * using the standard library function realloc(). Returns a void pointer to the
 * reallocated memory.
 * If memory allocation fails, it prints an error message and exits the program.
 */ 
void* safeRealloc(System* global_data ,void* p, int size) {
    p = realloc(p, size);

    if (p != NULL)
        return p;
    printf(ERR_NO_MEMORY);
    deleteSystem(global_data);
    exit(EXIT_CODE_NO_MEMORY);
    return NULL;
}