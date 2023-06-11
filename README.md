C Process Manager<br />

Different Codes:<br />
- bg ./your_Program: creates a new process that runs chosen program <br />
- bglist: creates a list of codes for running processes <br />
- bgstart 'pid': resumes selected process <br />
- bgkill 'pid': terminates selected process<br />
- bgstop 'pid': pauses selected process<br />
- pstat 'pid': gives relevant stats on selected process<br />
- q: quits the process manager<br />

How to:<br />
1. In the cmd line run 'make' to compile relvant files<br />
2. Run "./pman" to start the program<br />
3. Run "bg ./your_program" to start a new process (to test run "bg ./inf a 4", read decsription in inf.c for further details)<br />
4. The program will run in the same terminal as Pman (anything you type won't be affected by what the terminal prints)<br />
5. Run "bglist" to get the "pid" of the process<br />
6. Run any relevant cmd using the pid<br />