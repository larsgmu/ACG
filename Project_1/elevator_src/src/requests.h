#pragma once

#include "elevator.h"
#include "elevator_io_types.h"

int requests_above(Elevator e);
int requests_below(Elevator e);

Dirn requests_chooseDirection(Elevator e) __attribute__((pure));

int requests_shouldStop(Elevator e) __attribute__((pure));

Elevator requests_clearAtCurrentFloor(Elevator e) __attribute__((pure));
