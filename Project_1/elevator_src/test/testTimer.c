#include "timer.h"
#include "unity.h"
#include <unistd.h>

extern double timerEndTime;
extern int timerActive;

void setUp(void) {}

void tearDown(void) {}

void test_timer_start(void) {
    /*
      The function timer_start() uses the system clock to set the
      variable timerEndTime() to the current time
      + a duration, so to test this we need to do a call
      to the function so the variable get initialized.
      Then we run it again with the desired duration.
      It also sets the variable timerActive, so we need
      to check this aswell.
    */
    timer_start(0);
    double timerEndTimeOld = timerEndTime;
    timer_start(3);
    TEST_ASSERT_EQUAL(timerEndTimeOld+3, timerEndTime);
    TEST_ASSERT_EQUAL(1, timerActive);

}

void test_timer_stop(void) {
  /*
    This function stops the clock, which is done by setting the variable
    timerActive = 0.
  */
  timer_stop();
  TEST_ASSERT_EQUAL(0, timerActive);

}

void test_timer_timed_out(void) {
  /*
    This function checks if the timer is active and if it has
    exceeded timerEndTime it returns 1, else it returns 0.

    We have two scenarios.

    Scenario 1: It has timed out i.e. start the timer, and sleep
    the same amount as the timer.

    Scenario 2: It has not timed out. Start the timer, but sleep less
    than the timer is set to endure.
  */

  // Scenario 1:
  timer_start(3);
  sleep(3);
  TEST_ASSERT_EQUAL(1, timer_timedOut());

  // Scenario 2:
  timer_start(3);
  sleep(1);
  TEST_ASSERT_EQUAL(0, timer_timedOut());
}
