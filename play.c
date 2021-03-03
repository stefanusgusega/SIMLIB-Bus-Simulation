#include "simlib.h"

#define EVENT_ARRIVAL_TERMINAL_1 1
#define EVENT_UNLOAD_PASSENGER_TERMINAL_1 2
#define EVENT_LOAD_PASSENGER_TERMINAL_1 3
#define EVENT_ARRIVAL_TERMINAL_2 4
#define EVENT_UNLOAD_PASSENGER_TERMINAL_2 5
#define EVENT_LOAD_PASSENGER_TERMINAL_2 6
#define EVENT_ARRIVAL_CAR_RENTAL 7
#define EVENT_UNLOAD_PASSENGER_CAR_RENTAL 8
#define EVENT_LOAD_PASSENGER_CAR_RENTAL 9
#define LIST_QUEUE_TERMINAL_1 1
#define LIST_TERMINAL_1 2
#define LIST_QUEUE_TERMINAL_2 3
#define LIST_TERMINAL_2 4
#define LIST_QUEUE_CAR_RENTAL 5
#define LIST_CAR_RENTAL 6
#define STREAM_INTERARRIVAL_TERMINAL_1 1
#define STREAM_INTERARRIVAL_TERMINAL_2 2
#define STREAM_INTERARRIVAL_CAR_RENTAL 3
#define STREAM_UNLOAD_PASSENGER 4
#define STREAM_LOAD_PASSENGER 5
#define STREAM_DESTINATION 6

void arrive_car_rental();
void depart_car_rental();
void arrive_terminal_1();
void depart_terminal_1();
void arrive_terminal_2();
void depart_terminal_2();

int main() {
  maxatr = 4;

  init_simlib();

  return 0;
}