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

    Idea: Draw scenario in latex?

    requests_chooseDirection decides if the elevator
    should continue with the direction it's currently
    in, or change it's direction to stop or the opposite.
    The function is called everytime the elevator
    is at a new floor.

    Scenario 1: Continue same direction
    - On the move upwards from floor 0
    - Has a order in floor 3
    - Function is called at floor 1
    - Expected return is direction up i.e. D_Up

    Scenario 2: Change direction
    - On the move upwards from floor 0
    - Has a order in florr 0
    - Function is called at floor 1
    - Expected return is down

    Scenario 3: Stop
    - On the move upwards from floor 0
    - Has a order on floor 1
    - Function is called when elevator enter floor 1
    - Expected return is stop.

    */

    // Scenario 1:
    elev.floor = 1;
    elev.dirn = D_Up;
    elev.behaviour = EB_Moving;
    elev.requests[2][0] = 1;
    Dirn dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Up,  dir);

    // Scenario 2:
    elev.floor = 1;
    elev.dirn = D_Up;
    elev.behaviour = EB_Moving;
    elev.requests[2][0] = 0;
    elev.requests[0][0] = 1;
    dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Down,  dir);

    // Scenario 3:
    elev.floor = 1;
    elev.dirn = D_Up;
    elev.behaviour = EB_Moving;
    elev.requests[1][0] = 1;
    elev.requests[0][0] = 0;
    dir = requests_chooseDirection(elev);
    TEST_ASSERT_EQUAL(D_Stop,  dir);

}

void test_requests_shouldStop() {
    /*
        The function shouldStop() returns true
        if based on the current state of the
        elevator when it passes a floor says it's
        reasonable to stop.

        Scenario 1: on the way down and has order
        on passing floor - True
        Scenario 2: Same but without order on
        passing floor - False

        Scenario 3: On the way up w order - True
        Scenario 4: On the way uo w/o order - False
    */

    int shouldStop = 0;

    // Scenario 1:
    elev.floor = 2;
    elev.dirn = D_Down;
    elev.behaviour = EB_Moving;
    elev.requests[2][0] = 1;
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 2:
    elev.floor = 2;
    elev.dirn = D_Down;
    elev.behaviour = EB_Moving;
    elev.requests[2][0] = 0;
    elev.requests[1][0] = 1;
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(0,  shouldStop);

    // Scenario 3:
    elev.floor = 1;
    elev.dirn = D_Up;
    elev.behaviour = EB_Moving;
    elev.requests[1][0] = 1;
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(1,  shouldStop);

    // Scenario 3:
    elev.floor = 1;
    elev.dirn = D_Up;
    elev.behaviour = EB_Moving;
    elev.requests[1][0] = 0;
    elev.requests[2][0] = 1;
    shouldStop = requests_shouldStop(elev);
    TEST_ASSERT_EQUAL(0,  shouldStop);

}

void test_requests_clearAtCurrentFloor() {
  /*
    When this function is called, it removes
    all orders on the floor the elevator currently
    is at.

    Scenario 1: Elevator only has orders on current
    floor, should return empty queue.

    Scenario 2: Elevator has orders on all floors,
    should only remove orders from current floor.

    Scenario 3: Elevator has orders on all floors
    except current floor. Should not remove anything.

  */

  // Create a expected output queue
  int expected_queue[N_FLOORS][N_BUTTONS];
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        expected_queue[floor][btn] = 0;
    }
  }



  // Scenario 1:
  elev.floor                   = 2;
  elev.requests[2][0]          = 1;
  elev = requests_clearAtCurrentFloor(elev);
  expected_queue[elev.floor][0] = 0;
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        TEST_ASSERT_EQUAL(expected_queue[floor][btn],elev.requests[floor][btn]);
    }
  }


  // Scenario 2:
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        elev.requests[floor][btn] = 1;
        if(floor != elev.floor) {
          expected_queue[floor][btn] = 1;
        }
    }
  }
  elev = requests_clearAtCurrentFloor(elev);
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        TEST_ASSERT_EQUAL(expected_queue[floor][btn],elev.requests[floor][btn]);
    }
  }

  // Scenario 3:
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){

        if(floor != elev.floor) {
          expected_queue[floor][btn] = 1;
          elev.requests[floor][btn] = 1;
        }else{
          expected_queue[floor][btn] = 0;
          elev.requests[floor][btn] = 0;
        }
    }
  }
  elev = requests_clearAtCurrentFloor(elev);
  for(Button btn = 0; btn < N_BUTTONS; btn++){
    for(int floor = 0; floor < N_FLOORS; floor++){
        TEST_ASSERT_EQUAL(expected_queue[floor][btn],elev.requests[floor][btn]);
    }
  }

}