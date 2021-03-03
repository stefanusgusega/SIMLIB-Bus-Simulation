#include "simlib.h"
#include <stdio.h>

#define EVENT_ARRIVAL_TERMINAL_1 1
#define EVENT_ARRIVAL_TERMINAL_2 2
#define EVENT_ARRIVAL_CAR_RENTAL 3
#define EVENT_LOAD_PASSENGER 4
#define EVENT_UNLOAD_PASSENGER 5
#define LIST_QUEUE_TERMINAL_1 1
#define LIST_QUEUE_TERMINAL_2 2
#define LIST_QUEUE_CAR_RENTAL 3
#define LIST_QUEUE_BUS 4
#define STREAM_INTERARRIVAL_TERMINAL_1 1
#define STREAM_INTERARRIVAL_TERMINAL_2 2
#define STREAM_INTERARRIVAL_CAR_RENTAL 3
#define STREAM_UNLOAD_PASSENGER 4
#define STREAM_LOAD_PASSENGER 5
#define STREAM_DESTINATION 6
#define MAX_PASSENGERS 20

void arrive_car_rental();
void depart_car_rental();
void arrive_terminal_1();
void depart_terminal_1();
void arrive_terminal_2();
void depart_terminal_2();
int random_destination();
void load_passenger();
void unload_passenger();

int mean_interarrival_car_rental, mean_interarrival_terminal_1, mean_interarrival_terminal_2;
double min_load, max_load, min_unload, max_unload, min_wait, prob_distrib_dest[26]; 

int main() {
  maxatr = 4;

  init_simlib();
  printf("%d\n", random_destination());

  return 0;
}

int random_destination() {
  prob_distrib_dest[0] = 0.583;
  prob_distrib_dest[1] = 0.417;
  prob_distrib_dest[2] = 1.0;
  return random_integer(prob_distrib_dest, STREAM_DESTINATION);
}


void arrive_car_rental() {
  double current_time = 0;
  // Schedule next arrival
  event_schedule(sim_time + expon(mean_interarrival_car_rental, STREAM_INTERARRIVAL_CAR_RENTAL), EVENT_ARRIVAL_CAR_RENTAL);
  
  // Schedule terminal location
  // event_schedule(sim_time + , )

  // Unload bus
  while (list_size[LIST_QUEUE_BUS] > 0) {
    double unload_time = uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER);
    event_schedule(sim_time + unload_time, EVENT_UNLOAD_PASSENGER);
    current_time += unload_time;
  }
  // Load bus
  while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
    double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
    event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
    current_time += load_time;
  }
  
  // Check whether 5 minutes have passed
  while (current_time < 300) {
    while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
      double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
      event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
      current_time += load_time;
    }
    current_time++;
    // to-do : gmn biar ga infinite loop, ngitung curr_time
  }

  // Schedule departure from car rental
  // event_schedule(sim_time, )

}
void depart_car_rental();
void arrive_terminal_1();
void depart_terminal_1();
void arrive_terminal_2();
void depart_terminal_2();
void load_passenger();
void unload_passenger();