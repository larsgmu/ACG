#include "fsm.h"
#include "unity.h"
#include "elevator.h"
#include "elevator_io_device.h"

extern Elevator elevator;
extern int timerActive;
void setUp(void) {}
void tearDown(void) {}

void test_fsm_onInitBetweenFloors(void) {
  /*
    Elevator direction should be D_Down, and behaviour should be EB_Moving.
  */
  fsm_onInitBetweenFloors();
  TEST_ASSERT_EQUAL(D_Down, elevator.dirn);
  TEST_ASSERT_EQUAL(EB_Moving, elevator.behaviour);
}

void test_fsm_onRequestButtonPress(void) {
  /*
      This function responds to button presses in different
      scenarios. The scenarios are based on the truth table,
      scenario 1 is leftmost column.
  */

  // Scenario 1: Should start timer
  elevator.behaviour      = EB_DoorOpen;
  elevator.floor          = 1;
  elevator.requests[1][1] = 0;
  fsm_onRequestButtonPress(1, B_HallDown);
  TEST_ASSERT_EQUAL(1, timerActive);

  // Scenario 2: Check that request gets added to matrix
  elevator.requests[2][1] = 0; // Make sure no orders
  fsm_onRequestButtonPress(2, B_HallDown);
  TEST_ASSERT_EQUAL(1, elevator.requests[2][1]);

  // Scenario 3: Check that request gets added to matrix
  elevator.behaviour = EB_Moving;
  elevator.requests[2][1] = 0; // Make sure no orders
  fsm_onRequestButtonPress(2, B_HallDown);
  TEST_ASSERT_EQUAL(1, elevator.requests[2][1]);

  // Scenario 4: Should start  timer and set the behaviour EBDoorOpen
  elevator.behaviour = EB_Idle;
  elevator.floor          = 1;
  elevator.requests[1][1] = 0;
  fsm_onRequestButtonPress(1, B_HallDown);
  TEST_ASSERT_EQUAL(1, timerActive);
  TEST_ASSERT_EQUAL(EB_DoorOpen, elevator.behaviour);

  // Scenario 5: Should add request, set dir and behaviour to moving.
  elevator.behaviour = EB_Idle;
  elevator.floor          = 1;
  elevator.requests[2][1] = 0;
  fsm_onRequestButtonPress(2, B_HallDown);
  TEST_ASSERT_EQUAL(1, elevator.requests[2][1]);
  TEST_ASSERT_EQUAL(D_Up, elevator.dirn);
  TEST_ASSERT_EQUAL(EB_Moving, elevator.behaviour);

}

void test_fsm_onFloorArrival(void) {
  /*
    This function desides what happens to the elevator
    when it arrives to a new floor. It's all based on the
    state of the elevator.

    The scenarios are from the truth table, where scenario 1
    is the leftmos column.
  */

  // Scenario 1:
  elevator.behaviour = EB_Idle;
  elevator.floor = 1;
  fsm_onFloorArrival(2);
  TEST_ASSERT_EQUAL(2, elevator.floor);

  // Scenario 2:
  elevator.behaviour = EB_Moving;
  elevator.dirn = D_Up; // Set up s.t. shouldStop() returns 0
  elevator.floor = 1;
  elevator.requests[3][B_HallUp] = 1;
  fsm_onFloorArrival(2);
  TEST_ASSERT_EQUAL(2, elevator.floor);

  // Scenario 3
  elevator.behaviour = EB_Moving;
  elevator.dirn = D_Up; // Set up s.t. shouldStop() returns 1
  elevator.floor = 1;
  elevator.requests[2][B_HallUp] = 1;
  fsm_onFloorArrival(2);
  TEST_ASSERT_EQUAL(2, elevator.floor);
  TEST_ASSERT_EQUAL(0, elevator.requests[2][B_HallUp]);
  TEST_ASSERT_EQUAL(EB_DoorOpen, elevator.behaviour);
  TEST_ASSERT_EQUAL(1, timerActive);
}
void test_fsm_onDoorTimeout(void) {}
