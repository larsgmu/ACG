#include "requests.h"
#include "unity.h"


Elevator elev;
void setUp(void) {
    // Initialize a elevator on 3nd floor standing still without orders
    elev.floor = 3;
    elev.dirn = D_Stop;
    elev.behaviour = EB_Idle;
    for(Button btn = 0; btn < N_BUTTONS; btn++){
      for(int floor = 0; floor < N_FLOORS; floor++){
          elev.requests[floor][btn] = 0;
      }
    }
}

void tearDown(void) {
    // clean stuff up here
}

void test_requests_above(void) {
  /*
  Scenario:
  - Elevator in floor 0
  - Initially 0 orders in queue
  - Check if orders above (false)
  - Add request in 2nd floor
  - Check if orders above (true)
  */

  elev.floor = 0;
  TEST_ASSERT_EQUAL(0, requests_above(elev));
  elev.requests[2][1] = 1;
  TEST_ASSERT_EQUAL(1, requests_above(elev));
}

void test_requests_below(void) {
  /*
  Scenario:
  - Elevator in floor 2
  - Initially 0 orders in queue
  - Check if orders below (false)
  - Add request in 1st floor
  - Check if orders below (true)
  */

  elev.floor = 2;
  TEST_ASSERT_EQUAL(0, requests_below(elev));
  elev.requests[0][1] = 1;
  TEST_ASSERT_EQUAL(1, requests_below(elev));
}

void test_requests_chooseDirection(void) {
    /*

    requests_chooseDirection decides if the elevator
    should continue with the direction it's currently
    in, or change it's direction to stop or the opposite.
    The function is called everytime the elevator
    is at a new floor.

    Five scenarios based on the truth table, where scenario 1
    represent the leftmost column.
    */

    // Scenario 1:
    elev.floor = 1; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[2][0] = 1; Dirn dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Up,  dir);

    // Scenario 2:
    elev.floor = 1; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[2][0] = 0;
    elev.requests[0][0] = 1; dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Down,  dir);

    // Scenario 3:
    elev.floor = 1; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[0][0] = 1; dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Down,  dir);

    // Scenario 4:
    elev.floor = 1; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[0][0] = 0;
    elev.requests[2][0] = 1; dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Up,  dir);

    // Scenario 5:
    elev.floor = 1; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[2][0] = 0; dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Stop,  dir);
}

void test_requests_shouldStop() {
    /*
        The function shouldStop() returns true
        if based on the current state of the
        elevator when it passes a floor says it's
        reasonable to stop.

        Eight scenarios based on the truth table, where scenario 1
        represent the leftmost column.

    */

    int shouldStop = 0;


    // Scenario 1:
    elev.floor = 2; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[2][1] = 1; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 2:
    elev.floor = 2; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[2][1] = 0; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 3:
    elev.floor = 2; elev.dirn = D_Down; elev.behaviour = EB_Moving;
    elev.requests[1][1] = 1; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(0,  shouldStop);

    // Scenario 4:
    elev.floor = 2; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[1][1] = 0; // Reset from previous scenario
    elev.requests[2][0] = 1; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 5:
    elev.floor = 2; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[2][0] = 0; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 6:
    elev.floor = 2; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[3][1] = 1; shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(0,  shouldStop);

    // Scenario 7:
    elev.floor = 2; elev.dirn = D_Up; elev.behaviour = EB_Moving;
    elev.requests[3][1] = 0; // Reset from previous scenario
    elev.requests[2][2] = 1; // Cab order on current floor
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 8:
    elev.floor = 2; elev.dirn = D_Stop; elev.behaviour = EB_Idle;
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

}

void test_requests_clearAtCurrentFloor() {
  /*
    When this function is called, it removes
    all orders on the floor the elevator currently
    is at.

    Scenario: Fill the queue with arbitrary orders, both on
    current floor and on floors above and below. The function
    should only remove the orders on the current floor.
  */

  // Create a expected output queue with no orders
  int expected_queue[N_FLOORS][N_BUTTONS];
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        expected_queue[floor][btn] = 0;
    }
  }

  // Scenario 1:
  elev.floor = 2; // Set current floor and add orders above and below
  elev.requests[3][0] = elev.requests[2][0] = elev.requests[1][1] = 1;
  expected_queue[3][0] = expected_queue[1][1] = 1;
  elev = requests_clearAtCurrentFloor(elev);
  expected_queue[elev.floor][0] = 0; // Clear current floor in expected queue
  // Iterate through all orders and see that the queues are the same
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        TEST_ASSERT_EQUAL(expected_queue[floor][btn],elev.requests[floor][btn]);
    }
  }


}
