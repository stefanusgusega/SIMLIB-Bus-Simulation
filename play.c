#include "simlib.h"
#include <stdio.h>

#define EVENT_BUS_ARRIVAL_TERMINAL_1 1
#define EVENT_BUS_ARRIVAL_TERMINAL_2 2
#define EVENT_BUS_ARRIVAL_CAR_RENTAL 3
#define EVENT_BUS_DEPARTURE_TERMINAL_1 4
#define EVENT_BUS_DEPARTURE_TERMINAL_2 5
#define EVENT_BUS_DEPARTURE_CAR_RENTAL 6
#define EVENT_LOAD_PASSENGER_TERMINAL_1 7
#define EVENT_LOAD_PASSENGER_TERMINAL_2 8
#define EVENT_LOAD_PASSENGER_CAR_RENTAL 9
#define EVENT_UNLOAD_PASSENGER_TERMINAL_1 10
#define EVENT_UNLOAD_PASSENGER_TERMINAL_2 11
#define EVENT_UNLOAD_PASSENGER_CAR_RENTAL 12
#define EVENT_PASSENGER_ARRIVAL_CAR_RENTAL 13
#define EVENT_PASSENGER_ARRIVAL_TERMINAL_1 14
#define EVENT_PASSENGER_ARRIVAL_TERMINAL_2 15
#define EVENT_ENDOFTIME 16

#define DESTINATION_TERMINAL_1 1
#define DESTINATION_TERMINAL_2 2
#define DESTINATION_CAR_RENTAL 3

#define LIST_QUEUE_TERMINAL_1 1
#define LIST_QUEUE_TERMINAL_2 2
#define LIST_QUEUE_CAR_RENTAL 3
#define LIST_QUEUE_BUS_DEST_TERMINAL_1 4
#define LIST_QUEUE_BUS_DEST_TERMINAL_2 5
#define LIST_QUEUE_BUS_DEST_CAR_RENTAL 6

#define STREAM_INTERARRIVAL_TERMINAL_1 1
#define STREAM_INTERARRIVAL_TERMINAL_2 2
#define STREAM_INTERARRIVAL_CAR_RENTAL 3
#define STREAM_UNLOAD_PASSENGER 4
#define STREAM_LOAD_PASSENGER 5
#define STREAM_DESTINATION 6

#define MAX_PASSENGERS 20

#define VARIABLE_LIST_QUEUE_TERMINAL_1 1
#define VARIABLE_LIST_QUEUE_TERMINAL_2 2
#define VARIABLE_LIST_QUEUE_CAR_RENTAL 3
#define VARIABLE_DELAY_TERMINAL_1 4
#define VARIABLE_DELAY_TERMINAL_2 5
#define VARIABLE_DELAY_CAR_RENTAL 6
#define VARIABLE_LIST_QUEUE_BUS 7
#define VARIABLE_BUS_STOP_TIME_TERMINAL_1 8
#define VARIABLE_BUS_STOP_TIME_TERMINAL_2 9
#define VARIABLE_BUS_STOP_TIME_CAR_RENTAL 10
#define VARIABLE_BUS_LOOP 11
#define VARIABLE_PERSON_SYSTEM 12

#define ARRIVED 1
#define DESTINATION 2

void init_model();
int random_destination();
double time_spent_to_next_place(double distance);
void arrive_car_rental();
void depart_car_rental();
void arrive_terminal_1();
void depart_terminal_1();
void arrive_terminal_2();
void depart_terminal_2();
void passenger_arrival_car_rental();
void passenger_arrival_terminal_1();
void passenger_arrival_terminal_2();
void load_passenger();
void unload_passenger();
double interarrival_time_car_rental();
double interarrival_time_terminal_1();
double interarrival_time_terminal_2();
void report();

double mean_interarrival_car_rental, mean_interarrival_terminal_1, mean_interarrival_terminal_2;
double min_load, max_load, min_unload, max_unload, min_wait, time_end, bus_arrival = 0.0, bus_last_departure_from_car_rental = 0.0;

int main() {
  maxatr = 4;

  min_load = 15.00;
  max_load = 25.00;
  min_unload = 16.00;
  max_unload = 24.00;
  
  min_wait = 300.00;

  mean_interarrival_car_rental = 24.00;
  mean_interarrival_terminal_1 = 14.00;
  mean_interarrival_terminal_2 = 10.00;
  
  time_end = 288000.00;

  init_simlib();

  init_model();

  while (next_event_type != EVENT_ENDOFTIME) {
    timing();

    switch(next_event_type) {
      case EVENT_BUS_ARRIVAL_TERMINAL_1:
        arrive_terminal_1();
        break;
      case EVENT_BUS_ARRIVAL_TERMINAL_2:
        arrive_terminal_2();
        break;
      case EVENT_BUS_ARRIVAL_CAR_RENTAL:
        arrive_car_rental();
        break;
      case EVENT_BUS_DEPARTURE_TERMINAL_1:
        depart_terminal_1();
        break;
      case EVENT_BUS_DEPARTURE_TERMINAL_2:
        depart_terminal_2();
        break;
      case EVENT_BUS_DEPARTURE_CAR_RENTAL:
        depart_car_rental();
        break;
      case EVENT_LOAD_PASSENGER_TERMINAL_1:
        load_passenger(1);
        break;
      case EVENT_LOAD_PASSENGER_TERMINAL_2:
        load_passenger(2);
        break;
      case EVENT_LOAD_PASSENGER_CAR_RENTAL:
        load_passenger(3);
        break;
      case EVENT_UNLOAD_PASSENGER_TERMINAL_1:
        unload_passenger(1);
        break;
      case EVENT_UNLOAD_PASSENGER_TERMINAL_2:
        unload_passenger(2);
        break;
      case EVENT_UNLOAD_PASSENGER_CAR_RENTAL:
        unload_passenger(3);
        break;
      case EVENT_PASSENGER_ARRIVAL_CAR_RENTAL:
        passenger_arrival_car_rental();
        break;
      case EVENT_PASSENGER_ARRIVAL_TERMINAL_1:
        passenger_arrival_terminal_1();
        break;
      case EVENT_PASSENGER_ARRIVAL_TERMINAL_2:
        passenger_arrival_terminal_2();
        break;
    }
  }

  out_sampst(stdout, VARIABLE_LIST_QUEUE_TERMINAL_1, VARIABLE_PERSON_SYSTEM);
  out_timest(stdout, VARIABLE_LIST_QUEUE_TERMINAL_1, VARIABLE_PERSON_SYSTEM);

  printf("Finish time: %.5f\n\n", sim_time);
  printf("=================STATISTICS=================\n");
  report();

  return 0;
}

void init_model() {
  event_schedule(sim_time + interarrival_time_car_rental(), EVENT_PASSENGER_ARRIVAL_CAR_RENTAL);
  event_schedule(sim_time + interarrival_time_terminal_1(), EVENT_PASSENGER_ARRIVAL_TERMINAL_1);
  event_schedule(sim_time + interarrival_time_terminal_2(), EVENT_PASSENGER_ARRIVAL_TERMINAL_2);

  event_schedule(sim_time + interarrival_time_car_rental(), EVENT_BUS_ARRIVAL_CAR_RENTAL);
  event_schedule(time_end, EVENT_ENDOFTIME);

  sampst(0.0, 0);
  timest(0.0, 0);
}

double time_spent_to_next_place(double distance) {
  return 120.00*distance;
}

int random_destination() {
  prob_distrib[1] = 0.583;
  prob_distrib[2] = 1.0;

  return random_integer(prob_distrib, STREAM_DESTINATION); // return 1 for terminal 1, 2 for terminal 2
}

double interarrival_time_car_rental() {
  return expon(3600.00/mean_interarrival_car_rental,STREAM_INTERARRIVAL_CAR_RENTAL);
}

double interarrival_time_terminal_1() {
  return expon(3600.00/mean_interarrival_terminal_1,STREAM_INTERARRIVAL_TERMINAL_1);
}

double interarrival_time_terminal_2() {
  return expon(3600.00/mean_interarrival_terminal_2,STREAM_INTERARRIVAL_TERMINAL_2);
}

void passenger_arrival_car_rental() {
  event_schedule(sim_time + interarrival_time_car_rental(), EVENT_PASSENGER_ARRIVAL_CAR_RENTAL);
  list_file(LAST, LIST_QUEUE_CAR_RENTAL);
  transfer[ARRIVED] = sim_time;
  transfer[DESTINATION] = random_destination();
  timest(list_size[LIST_QUEUE_CAR_RENTAL], VARIABLE_LIST_QUEUE_CAR_RENTAL);
}

void passenger_arrival_terminal_1() {
  event_schedule(sim_time + interarrival_time_terminal_1(), EVENT_PASSENGER_ARRIVAL_TERMINAL_1);
  list_file(LAST, LIST_QUEUE_TERMINAL_1);
  transfer[ARRIVED] = sim_time;
  transfer[DESTINATION] = DESTINATION_CAR_RENTAL;
  timest(list_size[LIST_QUEUE_TERMINAL_1], VARIABLE_LIST_QUEUE_TERMINAL_1);
}

void passenger_arrival_terminal_2() {
  event_schedule(sim_time + interarrival_time_terminal_2(), EVENT_PASSENGER_ARRIVAL_TERMINAL_2);
  list_file(LAST, LIST_QUEUE_TERMINAL_2);
  transfer[ARRIVED] = sim_time;
  transfer[DESTINATION] = DESTINATION_CAR_RENTAL;
  timest(list_size[LIST_QUEUE_TERMINAL_2], VARIABLE_LIST_QUEUE_TERMINAL_2);
}

void arrive_car_rental() {
  bus_arrival = sim_time;
  
  // Unload passengers
  if (list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] > 0) {
    event_schedule(sim_time + uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER), EVENT_UNLOAD_PASSENGER_CAR_RENTAL);
  }

  // Load passengers
  if (list_size[LIST_QUEUE_CAR_RENTAL] > 0) {
    event_schedule(sim_time + uniform(min_load, max_load, STREAM_LOAD_PASSENGER), EVENT_LOAD_PASSENGER_CAR_RENTAL);
  }

  // Schedule departure
  event_schedule(sim_time + min_wait, EVENT_BUS_DEPARTURE_CAR_RENTAL);
}

void depart_car_rental() {
  if (sim_time - bus_arrival < min_wait) { // Bus has to wait at least 5 minutes
    event_schedule(bus_arrival + min_wait, EVENT_BUS_DEPARTURE_CAR_RENTAL);
  } else {
    sampst(sim_time - bus_last_departure_from_car_rental, VARIABLE_BUS_LOOP);
    bus_last_departure_from_car_rental = sim_time;
    sampst(sim_time - bus_arrival, VARIABLE_BUS_STOP_TIME_CAR_RENTAL);
    event_schedule(sim_time + time_spent_to_next_place(4.5), EVENT_BUS_ARRIVAL_TERMINAL_1);
  }
}

void arrive_terminal_1() {
  bus_arrival = sim_time;
  
  // Unload passengers
  if (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] > 0) {
    event_schedule(sim_time + uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER), EVENT_UNLOAD_PASSENGER_TERMINAL_1);
  }

  // Load passengers
  if (list_size[LIST_QUEUE_TERMINAL_1] > 0) {
    event_schedule(sim_time + uniform(min_load, max_load, STREAM_LOAD_PASSENGER), EVENT_LOAD_PASSENGER_TERMINAL_1);
  }

  // Schedule departure
  event_schedule(sim_time + min_wait, EVENT_BUS_DEPARTURE_TERMINAL_1);
}

void depart_terminal_1() {
  if (sim_time - bus_arrival < min_wait) { // Bus has to wait at least 5 minutes
    event_schedule(bus_arrival + min_wait, EVENT_BUS_DEPARTURE_TERMINAL_1);
  } else {
    sampst(sim_time - bus_arrival, VARIABLE_BUS_STOP_TIME_TERMINAL_1);
    event_schedule(sim_time + time_spent_to_next_place(1), EVENT_BUS_ARRIVAL_TERMINAL_2);
  }
}

void arrive_terminal_2() {
  bus_arrival = sim_time;
  
  // Unload passengers
  if (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2] > 0) {
    event_schedule(sim_time + uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER), EVENT_UNLOAD_PASSENGER_TERMINAL_2);
  }

  // Load passengers
  if (list_size[LIST_QUEUE_TERMINAL_2] > 0) {
    event_schedule(sim_time + uniform(min_load, max_load, STREAM_LOAD_PASSENGER), EVENT_LOAD_PASSENGER_TERMINAL_2);
  }

  // Schedule departure
  event_schedule(sim_time + min_wait, EVENT_BUS_DEPARTURE_TERMINAL_2);
}

void depart_terminal_2() {
  if (sim_time - bus_arrival < min_wait) { // Bus has to wait at least 5 minutes
    event_schedule(bus_arrival + min_wait, EVENT_BUS_DEPARTURE_TERMINAL_2);
  } else {
    sampst(sim_time - bus_arrival, VARIABLE_BUS_STOP_TIME_TERMINAL_2);
    event_schedule(sim_time + time_spent_to_next_place(1), EVENT_BUS_ARRIVAL_CAR_RENTAL);
  }
}

void load_passenger(int loc) {
  list_remove(FIRST, loc);
  double load_time = uniform(min_load, max_load, STREAM_LOAD_PASSENGER);
  if (loc == 3) { // car rental
    timest(list_size[LIST_QUEUE_CAR_RENTAL], VARIABLE_LIST_QUEUE_CAR_RENTAL);
    sampst(sim_time-transfer[ARRIVED], VARIABLE_DELAY_CAR_RENTAL);
    if (transfer[DESTINATION] == DESTINATION_TERMINAL_1) {
      list_file(LAST, LIST_QUEUE_BUS_DEST_TERMINAL_1);
      // kl masi ada jadwalin lg
      if (list_size[LIST_QUEUE_CAR_RENTAL] > 0 && (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2]) < MAX_PASSENGERS) {
        event_schedule(sim_time+load_time, EVENT_LOAD_PASSENGER_CAR_RENTAL);
      }
    }
    else {
      list_file(LAST, LIST_QUEUE_BUS_DEST_TERMINAL_2);
      // kl masi ada jadwalin lg
      if (list_size[LIST_QUEUE_CAR_RENTAL] > 0 && (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2]) < MAX_PASSENGERS) {
        event_schedule(sim_time+load_time, EVENT_LOAD_PASSENGER_CAR_RENTAL);
      }
    }
  }
  else {
    list_file(LAST, LIST_QUEUE_BUS_DEST_CAR_RENTAL);

    if (loc == 1) {
      timest(list_size[LIST_QUEUE_TERMINAL_1], VARIABLE_LIST_QUEUE_TERMINAL_1);
      sampst(sim_time-transfer[ARRIVED], VARIABLE_DELAY_TERMINAL_1);
      if (list_size[LIST_QUEUE_TERMINAL_1] > 0 && list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] < MAX_PASSENGERS) {
        event_schedule(sim_time+load_time, EVENT_LOAD_PASSENGER_TERMINAL_1);
      }
    }
    else if (loc == 2) {
      timest(list_size[LIST_QUEUE_TERMINAL_2], VARIABLE_LIST_QUEUE_TERMINAL_2);
      sampst(sim_time-transfer[ARRIVED], VARIABLE_DELAY_TERMINAL_2);
      if (list_size[LIST_QUEUE_TERMINAL_2] > 0 && list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] < MAX_PASSENGERS) {
        event_schedule(sim_time+load_time, EVENT_LOAD_PASSENGER_TERMINAL_2);
      }
    }
  }
  timest(list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2], VARIABLE_LIST_QUEUE_BUS);
}

void unload_passenger(int loc) {
  double next_unload_time = uniform(min_unload, max_unload, STREAM_UNLOAD_PASSENGER);
  if (loc == 1) {
    list_remove(FIRST, LIST_QUEUE_BUS_DEST_TERMINAL_1); 

    if (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] > 0) {
      event_schedule(sim_time + next_unload_time, EVENT_UNLOAD_PASSENGER_TERMINAL_1);
    }
  }
  else if (loc == 2) {
    list_remove(FIRST, LIST_QUEUE_BUS_DEST_TERMINAL_2); 

    if (list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2] > 0) {
      event_schedule(sim_time + next_unload_time, EVENT_UNLOAD_PASSENGER_TERMINAL_2);
    }
  }
  else {
    list_remove(FIRST, LIST_QUEUE_BUS_DEST_CAR_RENTAL); 

    if (list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] > 0) {
      event_schedule(sim_time + next_unload_time, EVENT_UNLOAD_PASSENGER_CAR_RENTAL);
    }
  }
  sampst(sim_time-transfer[ARRIVED],VARIABLE_PERSON_SYSTEM);
  timest(list_size[LIST_QUEUE_BUS_DEST_CAR_RENTAL] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_1] + list_size[LIST_QUEUE_BUS_DEST_TERMINAL_2], VARIABLE_LIST_QUEUE_BUS);
}

void report() {
  timest(0,-VARIABLE_LIST_QUEUE_TERMINAL_1);
  printf("(a) Average and maximum number in queue: \n     1. Terminal 1:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[2]);
  timest(0,-VARIABLE_LIST_QUEUE_TERMINAL_2);
  printf("     2. Terminal 2:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[2]);
  timest(0,-VARIABLE_LIST_QUEUE_CAR_RENTAL);
  printf("     3. Car Rental:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[2]);
  
  sampst(0,-VARIABLE_DELAY_TERMINAL_1);
  printf("(b) Average and maximum delay in queue: \n     1. Terminal 1:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[3]);
  sampst(0,-VARIABLE_DELAY_TERMINAL_2);
  printf("     2. Terminal 2:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[3]);
  sampst(0,-VARIABLE_DELAY_CAR_RENTAL);
  printf("     3. Car Rental:\n       -Average: %.3lf\n       -Maximum: %.3lf\n", transfer[1], transfer[3]);

  printf("(c) Average and maximum number on the bus \n");
  timest(0,-VARIABLE_LIST_QUEUE_BUS);
  printf("     -Average: %.3lf\n     -Maximum: %.3lf\n", transfer[1], transfer[2]);
  
  sampst(0,-VARIABLE_BUS_STOP_TIME_TERMINAL_1);
  printf("(d) Average, maximum, and minimum time bus stopped at each location \n     1. Terminal 1:\n       -Average: %.3lf\n       -Maximum: %.3lf\n       -Minimum: %.3lf\n", transfer[1], transfer[3], transfer[4]);
  sampst(0,-VARIABLE_BUS_STOP_TIME_TERMINAL_2);
  printf("     2. Terminal 2:\n       -Average: %.3lf\n       -Maximum: %.3lf\n       -Minimum: %.3lf\n", transfer[1], transfer[3], transfer[4]);
  sampst(0,-VARIABLE_BUS_STOP_TIME_CAR_RENTAL);
  printf("     3. Car Rental:\n       -Average: %.3lf\n       -Maximum: %.3lf\n       -Minimum: %.3lf\n", transfer[1], transfer[3], transfer[4]);

  sampst(0,-VARIABLE_BUS_LOOP);
  printf("(e) Average, maximum, and minimum time bus making a loop \n     -Average: %.3lf\n     -Maximum: %.3lf\n     -Minimum: %.3lf\n", transfer[1], transfer[3], transfer[4]);
  
  sampst(0,-VARIABLE_PERSON_SYSTEM);
  printf("(f) Average, maximum, and minimum time person in the system \n     -Average: %.3lf\n     -Maximum: %.3lf\n     -Minimum: %.3lf\n", transfer[1], transfer[3], transfer[4]);
}
