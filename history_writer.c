/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "history_writer.h"

#include "observer_server.h"
#include "observer.h"
#include "svg_file.h"
#include "array_list.h"
#include "geometry.h"
#include "saved_action.h"

action_start* action_start_create(point* start, int id)
{
	action_start* dw = malloc(sizeof(action_start));
	dw->start = start;
	dw->id = id;
}

int compare_by_id(void* a, int key)
{
	action_start* ac = (action_start*) (a);
	return ac->id == key;
}

void compute_svg_background(svg* file, int nb_process)
{
	for (int i = 0; i < nb_process; i++) {
		int y = 1000 * (i + 1) / (nb_process + 2);
		svg_draw_line(file, 0, y, 1000, y, BLACK);
	}
}

int get_max_process()
{
	int max = 0;
	for (int i = 0; i < action_history->size; i++) {
		saved_action* s_action = (saved_action*) (list_at(action_history, i));
		if (s_action->process > max) {
			max = s_action->process;
		}
	}
	return max;
}

void compute_svg()
{
	printf("start computing\n\n\n");
	svg* file = svg_create("report.html", "w+");
	int nb_process = get_max_process();
	compute_svg_background(file, nb_process);
	int max_stamp = ((saved_action*) list_last(action_history))->stamp;
	list* actions = list_create();
	for (int i = 0; i < action_history->size; i++) {
		saved_action* s_action = (saved_action*) (list_at(action_history, i));
		int x = s_action->stamp * 500 / ((int) max_stamp + 1);
		int y = 1000 * (s_action->process + 1) / (nb_process + 2);
		int must_add = 0;
		color ac_color;
		int starting_id = -1;
		switch (s_action->action_type) {
		case REPORT_SNDMSG:
		case REPORT_SNDREQ:
		case REPORT_SNDREP:
		case REPORT_SNDREL:
		case REPORT_CSCBGN:
			must_add = 1;
			break;
		case REPORT_RCVREQ:
			starting_id = s_action->action_id;
			ac_color = YELLOW;
			break;
		case REPORT_RCVREP:
			starting_id = s_action->action_id;
			ac_color = ORANGE;
			break;
		case REPORT_RCVREL:
			starting_id = s_action->action_id;
			ac_color = TURQUOISE;
			break;
		case REPORT_RCVMSG:
			starting_id = s_action->action_id;
			ac_color = GREEN;
			break;
		case REPORT_LCLACT:
			ac_color = RED;
			break;
		case REPORT_END:
			ac_color = BLACK;
			break;
		case REPORT_CSCEND:
			starting_id = s_action->action_id;
			break;
		default:break;
		}
		if (starting_id == -1 && must_add) {
			action_start* ac = action_start_create(point_create(x, y), s_action->action_id);
			list_add(actions, ac);
		} else if (starting_id != -1) {
			int found = list_find_by_key(actions, starting_id, &compare_by_id);
			if (found != -1) {
				action_start* ac_found = ((action_start*) (list_at(actions, found)));
				svg_draw_arrow(file, ac_found->start->x, ac_found->start->y, x, y, ac_color);
			}
		} else svg_draw_circle(file, x, y, 20, ac_color);
	}
	svg_end(file);
}

void write_logs()
{
	printf("start logs\n\n\n");
	FILE* file = fopen("logs.txt", "w+");
	fprintf(file, "max process : %d \n", get_max_process());
	fprintf(file, "max stamp : %d \n", ((saved_action*) list_last(action_history))->stamp);
	for (int i = 0; i < action_history->size; i++) {
		saved_action* s_action = (saved_action*) (list_at(action_history, i));
		fprintf(file, "action  \n");
		fprintf(file, "    id      : %d \n ", s_action->action_id);
		fprintf(file, "    type    : %d \n ", s_action->action_type);
		fprintf(file, "    process : %d \n ", s_action->process);
		fprintf(file, "    stamp   : %d \n ", s_action->stamp);
	}
	fclose(file);
}