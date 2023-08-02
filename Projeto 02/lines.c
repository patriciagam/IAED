/*
 * File: lines.c 
 * Author:  Patrícia Gameiro
 * Description: File contains functions that manage the lines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project2.h"

/*
 * Adds a new line to the system.
 */
void addLine(System* global_data, char *line_name) {
	Line* new_line = safeMalloc(global_data, sizeof(Line));

	new_line->name = line_name;
    new_line->first = NULL;
    new_line->last = NULL;
	new_line->total_stops = 0;
	new_line->total_cost = 0.0;
	new_line->total_duration = 0.0;
    new_line->next = NULL;
    /* inserts the new line in the linked list of lines */
    insertLinkedLines(global_data->lines, new_line);
}

/*
 * Displays information about the line.
 */
void showLine(Line* line) {
    int total_stops = line->total_stops;

    printf("%s ", line->name);
    if (total_stops > 0) {
        printf("%s %s ", line->first->origin->name,
                line->last->dest->name);
        printf("%d ", total_stops);
        printf("%.2f ", line->total_cost);
        printf("%.2f\n", line->total_duration);
    }
    else 
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/*
 * Lists all the lines in the system.
 */
void listAllLines(System* global_data) {
    Line* currentLine = global_data->lines->head;
    
    while (currentLine != NULL) {
        showLine(currentLine);
        currentLine = currentLine->next;
    }
}

/*
 * Prints the stops along a given line in either forward or reverse order.
 */
void listPath(System* global_data, char *line_name, int inverse) {
    Line* line = searchLinkedLines(global_data->lines, line_name);   
  
    if (line->total_stops == 0) {
        free(line_name);
        return;
    }
    if (inverse == FALSE) {  /* forward order */
        Link* current = line->first; 
        printf("%s, ", current->origin->name); 
        while (current->next != NULL) {
            current = current->next;
            printf("%s, ", current->origin->name);
        }
        printf("%s\n", current->dest->name);
    }
    else {  /* reverse order */
        Link* current = line->last; 
        printf("%s, ", current->dest->name);
        while (current->prev != NULL) {
            current = current->prev;
            printf("%s, ", current->dest->name); 
        }
        printf("%s\n", current->origin->name); 
    }
    free(line_name);
}

/*
 * Searches for a stop in a given line.
 */
int findStopinLine(Line* line, Stop* stop) {
    Link* current = line->first;
    int i = 0;

    while (current != NULL) {
        if (current->origin == stop)
            return i;
        current = current->next;
        i++;
    }
    current = line->last;
    if (current->dest == stop)
        return line->total_stops;
    return NOT_FOUND;
}

/*
 * Checks if a given string is a valid parameter.
 */
int checkParameter(char *parameter) {   
    char inv[] = "inverso";
    int size = strlen(parameter), i;

    if (size < 3 || size > 7)
        return FALSE;
    for (i = 0; i < size; i++)
        if (inv[i] != parameter[i])
            return FALSE;
    return TRUE;
}

/*
 * Removes a line from all the stops it passes through.
 */
void removeLinefromStop(Line* line) {
    Link* current = line->first;

    while (current != NULL) {
        Stop* origin = current->origin;
        removeIntersection(origin, line);
        current = current->next;
    }
    removeIntersection(line->last->dest, line);
}

/*
 * Returns a sorted list of the names of the lines that intersect 
 * at the given stop.
 */
char** Intersections(Stop* stop) {
    int i = 0;
    Intersection* current;
    char** list_lines = (char**) malloc(stop->total_lines * sizeof(char*));

    current = stop->line_head;
    /* inserts the names of the lines in the list */
    while (current != NULL) {
        list_lines[i] = current->line->name;
        i++;
        current = current->next;
    }
    quicksort(list_lines, 0, stop->total_lines - 1);
    return list_lines;
}


/*
 * Removes a given line from the list of lines that pass through a stop.
 */
void removeIntersection(Stop* stop, Line* line) {
    Intersection* current = stop->line_head;
    Intersection* prev = NULL;

    while (current != NULL) {
        if (current->line == line) {
            stop->total_lines--;
            if (prev == NULL)
                stop->line_head = current->next;
            else 
                prev->next = current->next;
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

/* Linked Lines */

/*
 * Initializes the linked list of lines.
 */
LinkedLines* initLinkedLines(System* global_data) {
    LinkedLines* lines = safeMalloc(global_data, sizeof(LinkedLines));

    lines->head = NULL;
    lines->tail = NULL;
    return lines;
}

/* 
 * Inserts a line in the linked list of lines.
 */
void insertLinkedLines(LinkedLines* lines, Line* node) {
    if (lines->head == NULL) {
        lines->head = node;
        lines->tail = node;
    } 
    else {
        lines->tail->next = node;
        lines->tail = node;
    }
}


/*
 * Searches for a line in the linked list of lines.
 */
Line* searchLinkedLines(LinkedLines* lines, char *name) {
    Line* current = lines->head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    /* if the line is not found */
    return NULL;
}

/*
 * Deletes a line node from a linked list of lines.
 */
void deleteLineNode(LinkedLines* lines, char *name) {
    Line* current = searchLinkedLines(lines, name);
    Line* prev = NULL;
    
    /* if the line node was not found */
    if (current == NULL) 
        return;
    /* delete the links associated with the line node */
    deleteLinks(current);
    prev = lines->head;
    while (prev != NULL) {
        if (prev->next == current) 
            break;
        prev = prev->next;
    }
    if (prev == NULL) 
        lines->head = current->next;
    else 
        prev->next = current->next;
    if (lines->tail == current) 
        lines->tail = prev;
    current->total_cost = 0;
    current->total_duration = 0;
    current->total_stops = 0;
    free(current->name);
    free(current);
}

/*
 * Deletes a linked list of links.
 */
void deleteLinks(Line* line) {
    Link* current = line->first;
    
    while (current != NULL) {
        Link* next = current->next;
        free(current);
        current = next;
    }   
}

/*
 * Deletes a linked list of lines.
 */
void deleteLinkedLines(LinkedLines* lines) {
    Line* current = lines->head;
    Line* next;

    while (current != NULL) {
        next = current->next;
        deleteLineNode(lines, current->name);
        current = next;
    }
    free(lines);
}
