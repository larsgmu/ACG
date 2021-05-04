#include "fsm.h"
#include "unity.h"
#include "elevator.h"
#include "elevator_io_device.h"

extern Elevator elevator;
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

void test_fsm_onRequestButtonPress(void) {}
void test_fsm_onFloorArrival(void) {}
void test_fsm_onDoorTimeout(void) {}
