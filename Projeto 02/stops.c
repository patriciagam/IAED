/*
 * File: stops.c
 * Author:  Patrícia Gameiro
 * Description: File contains functions that manage the stops.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project2.h"

/*
 * Lists all stops in the system with their coordinates and the number of
 * lines that pass through them.
 */
void listAllStops(System* global_data) {
    Stop* current = global_data->stops->head;

    while (current != NULL) {
        printf("%s: %16.12f %16.12f %d\n", current->name, current->latitude, 
			current->longitude, current->total_lines);
        current = current->next;
    }
}

/*
 * Adds a new stop to the system.
 */
void addStop(System* global_data, char *stop_name, double lat, double longi) {
	Stop* new_stop = safeMalloc(global_data, sizeof(Stop));

	new_stop->name = stop_name;
    new_stop->latitude = lat;
    new_stop->longitude = longi;
    new_stop->total_lines = 0;
    new_stop->next = NULL;
    new_stop->line_head = NULL;
    /* inserts the new stop in the linked list of stops */
    insertLinkedStops(global_data->stops, new_stop);
}

/*
 * Adds a line to the list of lines that stop at a particular stop.
 */
void addLinetoStop(System* global_data, Stop* stop, Line* line) {
    Intersection* current = stop->line_head;
    Intersection* new_intersection = safeMalloc(global_data, sizeof
                                                (Intersection));

    while (current != NULL) {
        if (current->line == line) {
            free(new_intersection);
            return;
        }
        current = current->next;
    }
    new_intersection->line = line;
    new_intersection->next = stop->line_head;
    stop->line_head = new_intersection;
    stop->total_lines++;
}

/*
 * Removes a  line from the list of lines that stop at a particular stop.
 */
void removeStop(System* global_data, Stop* stop) {
    Intersection* current = stop->line_head;
    Intersection* next;

    while (current != NULL) {
        next = current->next;
        deleteStopfromLine(global_data, current->line, stop);
        current = next;
    }
}

/*
 * Deletes a stop from a specific line.
 */
void deleteStopfromLine(System* global_data, Line* line, Stop* stop) {
    if (line->total_stops == 0) 
        return;
    if (line->total_stops == 2 && (line->first->origin == stop || 
                                   line->first->dest == stop)) {    
        deleteSingleLink(line, stop);
        return;
    }
    if (line->first->origin == stop) {
        deleteFirstLink(line);
        deleteStopfromLine(global_data, line, stop);
        return;
    }
    if (line->last->dest == stop) {
        deleteLastLink(line);
        deleteStopfromLine(global_data, line, stop);
        return;
    } 
    /* if the stop is in the middle of the line */
    modifyLinks(global_data, line, stop);
}

/*
 * Deletes a single link from the line, removing it completely.
 * Removes the corresponding intersection from the stop it was connected to.
 */
void deleteSingleLink(Line* line, Stop* stop) {
    Link* current = line->first;

    if (line->first->origin != stop) 
        removeIntersection(line->first->origin, line);
    else if (line->first->dest != stop) 
        removeIntersection(line->first->dest, line);
    line->first = NULL;
    line->last = NULL;
    line->total_stops = 0;
    line->total_cost = 0;
    line->total_duration = 0;
    free(current);
}

/*
 * Deletes the first link of the line.
 */
void deleteFirstLink(Line* line) {
    Link* current = line->first;

    line->first = current->next;
    line->first->prev = NULL;
    line->total_cost -= current->cost;
    line->total_duration -= current->duration;
    line->total_stops--;
    free(current);  
}

/*
 * Deletes the last link of the line.
 */
void deleteLastLink(Line* line) {
    Link* current = line->last;

    line->last = current->prev;
    line->last->next = NULL;
    line->total_cost -= current->cost;
    line->total_duration -= current->duration;
    line->total_stops--;
    free(current);       
}

/*
 * Modify all links that include the stop in the given line.
 */
void modifyLinks(System* global_data, Line* line, Stop* stop) {
    Link* current = line->first;
    Link* next;
    Link* prev;

    while (current != NULL) {
        next = current->next;
        prev = current->prev;
        if (current->origin == stop) {
            /* create a new link with the same destination as the current one */
            Link* new_link = createLink(global_data,line, prev->origin, 
                                        current->dest,      
                                        current->cost + prev->cost, 
                                        current->duration + prev->duration);
            new_link->prev = prev->prev;
            new_link->next = next;
            if (prev->prev != NULL)
                prev->prev->next = new_link;
            else
                line->first = new_link;
            if (next != NULL)
                next->prev = new_link;
            else
                line->last = new_link;
            free(current);
            free(prev);
            line->total_stops--;
        }
        current = next;
    }
}

/* Linked Stops */

/*
 * Initializes the linked list of stops.
 */
LinkedStops* initLinkedStops(System* global_data) {
    LinkedStops* stops = safeMalloc(global_data, sizeof(LinkedStops));

    stops->head = NULL;
    stops->tail = NULL;
    return stops;
}

/*
 * Inserts a stop in the linked list of stops.
 */
void insertLinkedStops(LinkedStops* stops, Stop* node) {
    if (stops->head == NULL) {
        stops->head = node;
        stops->tail = node;
    } 
    else {
        stops->tail->next = node;
        stops->tail = node;
    }
}

/*
 * Searches for a stop in the linked list of stops.
 */
Stop* searchLinkedStops(LinkedStops* stops, char *name) {
    Stop* current = stops->head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    /* if the stop is not found */
    return NULL;
}

/*
 * Deletes a stop node  from a linked list of stops.
 */
void deleteStopNode(LinkedStops* stops, char *name) {
    Stop* current = searchLinkedStops(stops, name);
    Stop* prev = NULL;

    /* if the stop is not found */
    if (current == NULL) 
        return;
    /* delete intersections associated with the stop node */
    deleteIntersections(current);
    prev = stops->head;
    while (prev != NULL) {
        if (prev->next == current) 
            break;
        prev = prev->next;
    }
    if (prev == NULL) 
        stops->head = current->next;
    else 
        prev->next = current->next;
    if (stops->tail == current) 
        stops->tail = prev;
    free(current->name);
    free(current);
}

/*
 * Deletes all intersections associated with a stop.
 */
void deleteIntersections(Stop* stop) {
    Intersection* current = stop->line_head;

    while (current != NULL) {
        Intersection* next = current->next;
        free(current);
        current = next;
    }   
}

/*
 * Deletes a linked list of stops.
 */
void deleteLinkedStops(LinkedStops* stops) {
    Stop* current = stops->head;
    Stop* next;

    while (current != NULL) {
        next = current->next;
        deleteStopNode(stops, current->name);
        current = next;
    }
    free(stops);
}
