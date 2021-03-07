#include "simlib.h"
#include <stdio.h>

#define EVENT_ARRIVAL_TERMINAL_1 1
#define EVENT_ARRIVAL_TERMINAL_2 2
#define EVENT_ARRIVAL_CAR_RENTAL 3
#define EVENT_LOAD_PASSENGER 4
#define EVENT_UNLOAD_PASSENGER 5
#define EVENT_ENDOFTIME 6
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


// TO DO:
// 1. simulasiin delaynya (nunggu di queuenya), coba liat di tandem.c
// 2. solve masalah infinite loop buat hitung sampai 300 s kalau gaada aktivitas
// 3. schedule arrival orang di setiap lokasi
// 4. berbagai macam perhitungan statistik (prosedur report)
void init_model();
int random_destination();
double time_spent_to_next_place(double distance);
void arrive_car_rental();
// void depart_car_rental();
void arrive_terminal_1();
// void depart_terminal_1();
void arrive_terminal_2();
// void depart_terminal_2();
void load_passenger();
void unload_passenger();
double interarrival_time_car_rental();
double interarrival_time_terminal_1();
double interarrival_time_terminal_2();

int mean_interarrival_car_rental, mean_interarrival_terminal_1, mean_interarrival_terminal_2;
double min_load, max_load, min_unload, max_unload, min_wait, time_end;

int terminal_1_destination, terminal_2_destination;

int main() {
  maxatr = 4;

  min_load = 15;
  max_load = 25;
  min_unload = 16;
  max_unload = 24;
  
  // ini buat  yg pas nungguin penumpang kan?
  min_wait = 300;

  mean_interarrival_car_rental = 24;
  mean_interarrival_terminal_1 = 14;
  mean_interarrival_terminal_2 = 10;
  
  time_end = 288000;

  init_simlib();

  init_model();

  return 0;
}

void init_model() {
  terminal_1_destination = 0;
  terminal_2_destination = 0;

  event_schedule(sim_time + interarrival_time_car_rental(), EVENT_ARRIVAL_CAR_RENTAL);
  event_schedule(time_end, EVENT_ENDOFTIME);

  // init jml penumpang dibus ada 20
  list_size[LIST_QUEUE_BUS] = 20;
}

double time_spent_to_next_place(double distance) {
  return 120.00*distance;
}

int random_destination() {
  double destination = uniform(0, 1, STREAM_DESTINATION);

  return destination <= 0.583; // if true, destination is terminal 1. else, terminal 2.
}

double interarrival_time_car_rental() {
  double raw = expon(mean_interarrival_car_rental,STREAM_INTERARRIVAL_CAR_RENTAL);
  return 3600.00/raw;
}

double interarrival_time_terminal_1() {
  double raw = expon(mean_interarrival_terminal_1, STREAM_INTERARRIVAL_TERMINAL_1);
  return 3600.00/raw;
}

double interarrival_time_terminal_2() {
  double raw = expon(mean_interarrival_terminal_2, STREAM_INTERARRIVAL_TERMINAL_2);
  return 3600.00/raw;
}


void arrive_car_rental() {
  double current_time = 0;
  // Schedule next arrival
  event_schedule(sim_time + interarrival_time_car_rental(), EVENT_ARRIVAL_CAR_RENTAL);
  // harusnya insert orang
  list_file(LAST, LIST_QUEUE_CAR_RENTAL);

  // Determine the destination of the newly arrived passenger
  if (random_destination()) terminal_1_destination++;
  else terminal_2_destination++;

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
  while (current_time < min_wait) {
    int is_loaded = 0;
    while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
      double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
      event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
      is_loaded = 1;
      current_time += load_time;
    }
    if (!is_loaded) current_time++;
    // to-do : gmn biar ga infinite loop, ngitung curr_time
  }

  // Schedule arrival to terminal 1
  event_schedule(sim_time + time_spent_to_next_place(4.5), EVENT_ARRIVAL_TERMINAL_1);
}

void arrive_terminal_1() {
  double current_time = 0;
  // Schedule next arrival
  event_schedule(sim_time + interarrival_time_terminal_1(), EVENT_ARRIVAL_TERMINAL_1);

  list_file(LAST, LIST_QUEUE_TERMINAL_1);

  // Unload bus
  while (terminal_1_destination > 0) {
    double unload_time = uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER);
    event_schedule(sim_time + unload_time, EVENT_UNLOAD_PASSENGER);
    terminal_1_destination--;
    current_time += unload_time;
  }
  // Load bus
  while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_TERMINAL_1] > 0) {
    double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
    event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
    current_time += load_time;
  }
  
  // Check whether 5 minutes have passed
  while (current_time < min_wait) {
    int is_loaded = 0;
    while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
      double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
      event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
      is_loaded = 1;
      current_time += load_time;
    }
    if (!is_loaded) current_time++;
    // to-do : gmn biar ga infinite loop, ngitung curr_time
  }

  // Schedule arrival to terminal 2
  event_schedule(sim_time + time_spent_to_next_place(1), EVENT_ARRIVAL_TERMINAL_2);
}

void arrive_terminal_2() {
  double current_time = 0;
  // Schedule next arrival
  event_schedule(sim_time + interarrival_time_terminal_2(), EVENT_ARRIVAL_TERMINAL_2);
  list_file(LAST, LIST_QUEUE_TERMINAL_2);
   
   
    // Unload bus
  while (terminal_2_destination > 0) {
    double unload_time = uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER);
    event_schedule(sim_time + unload_time, EVENT_UNLOAD_PASSENGER);
    terminal_2_destination--;
    current_time += unload_time;
  }
  // Load bus
  while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_TERMINAL_2] > 0) {
    double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
    event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
    current_time += load_time;
  }
  
  // Check whether 5 minutes have passed
  while (current_time < min_wait) {
    int is_loaded = 0;
    while (list_size[LIST_QUEUE_BUS] < MAX_PASSENGERS || list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
      double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
      event_schedule(sim_time + load_time, EVENT_LOAD_PASSENGER);
      is_loaded = 1;
      current_time += load_time;
    }
    if (!is_loaded) current_time++;
    // to-do : gmn biar ga infinite loop, ngitung curr_time
  }

  // Schedule arrival to car rental
  event_schedule(sim_time + time_spent_to_next_place(1), EVENT_ARRIVAL_CAR_RENTAL);
}

// void depart_terminal_2() {
  
// }

void load_passenger() {
  list_file(LAST, LIST_QUEUE_BUS);
}

void unload_passenger() {
  list_remove(FIRST, LIST_QUEUE_BUS);
}

void report() {
  
}
