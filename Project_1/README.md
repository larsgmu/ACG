# ACG - Project 1
This Project 1 in the course ACG by Knut Vaagnes Eriksen and Lars Gjardar Musaeus.

### How to use
The *src/* folder and *unity_scripts/* are needed to run the unity tests. In the *elevator_src/* folder are all the source code and tests for the elevator project, included the compiled tests **testFsm.out**, **testRequests.out** and **testTimer.out**. By running the makefile in *elevator_src/* these executables get recompiled. To run the tests you need to have a instance of the elevator simulator running. To run the simulator we have added it in the folder *simulator/* for both Windows, Linux and OSX. Simply run it from you perferred shell.

### Example
Example run of one of the tests, with the simulator running in a different shell:
```console
foo@bar:~/ACG/Project_1/elevator_src$ ./testRequests.out
test/testRequests.c:22:test_requests_above:PASS
test/testRequests.c:38:test_requests_below:PASS
test/testRequests.c:54:test_requests_chooseDirection:PASS
test/testRequests.c:95:test_requests_shouldStop:PASS
test/testRequests.c:155:test_requests_clearAtCurrentFloor:PASS

-----------------------
5 Tests 0 Failures 0 Ignored
OK
```
