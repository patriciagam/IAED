/*
 * File: links.c
 * Author: Patrícia Gameiro
 * Description: File contains functions that manage the links.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project2.h"

/*
 * Creates a new link.
 */
Link* createLink(System* global_data, Line* line, Stop* origin, Stop* dest, 
                 double cost, double duration) {
    Link* new_link = safeMalloc(global_data, sizeof(Link));

    new_link->line = line;
    new_link->origin = origin;
    new_link->dest = dest;
    new_link->cost = cost;
    new_link->duration = duration;
    new_link->next = NULL;
    new_link->prev = NULL;
    return new_link;
}

/*
 * Adds a link between two stops in a given line.
 */
void addLink(System* global_data, Line* line, Stop* orig, Stop* dest, double    
             cost, double duration) {
    Link* new_link;

    /* if the line has no stops, add the link as the first one */
    if (line->total_stops == 0)
        addFirstLink(global_data, line, orig, dest, cost, duration);
    else {
        /* if the last stop in the line is the origin of the new link,
         * add it at the end */
        if (line->last->dest == orig) {
            if (findStopinLine(line, dest) == NOT_FOUND)
                addLinetoStop(global_data, dest, line);
            new_link = createLink(global_data, line, orig, dest, cost, 
                                  duration);
            addLinkEnd(line, new_link);
        }
        /* if the first stop in the line is the destination of the new link,
         * add it at the start */
        else if (line->first->origin == dest) {
            if (findStopinLine(line, orig) == NOT_FOUND)
                addLinetoStop(global_data, orig, line);
            new_link = createLink(global_data, line, orig, dest, cost, 
                                  duration);
            addLinkStart(line, new_link);
        }
        else
            printf(LINK_ERR_INVALID);
    }
}

/*
 * Adds a link as the first one in the line.
 */
void addFirstLink(System* global_data, Line* line, Stop* orig, Stop* dest,
                  double cost, double duration) {
    Link* new_link = createLink(global_data, line, orig, dest, cost, duration);

    addLinetoStop(global_data, orig, line);
    if (orig != dest)
        addLinetoStop(global_data, dest, line);
    addLinkEnd(line, new_link);
    line->total_stops++;
}

/*
 * Adds a link at the start of the line.
 */
void addLinkStart(Line* line, Link* link) {
    link->next = line->first;
    line->first->prev = link;
    line->first = link;
    line->total_cost += link->cost;
    line->total_duration += link->duration;
    line->total_stops++;
}

/*
 * Adds a link at the end of the line.
 */
void addLinkEnd(Line* line, Link* link) {
    link->prev = line->last;
    if (line->last == NULL) {
        line->first = link;
        line->last = link;
    } 
    else {
        line->last->next = link;
        link->prev = line->last;  
        line->last = link;
    }
    line->total_cost += link->cost;
    line->total_duration += link->duration;
    line->total_stops++;
}