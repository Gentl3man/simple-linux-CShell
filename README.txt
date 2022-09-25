Giorgos Dovas 


The basic idea of the sheel is that it stores everything the user types
and then executes the commands and ingores the rest

There is a small problem with pipes, in general they work fine (multiple pipes supported)
but there is a problem with build-ins and pipes althought they work, the shell doesnt
recognize other commands after that, and i dont know exactly why.

After some testing it seems there is a problem with getline() in some cases, 
or i forgot to put an exit and the shell waits forever

Anyway, this is how it works:
    1. Stores input and then a parser splits it to 2 arrays
        array with commands and array with arguments and in some 'special'
        places it holds some flags for Redirection and Pipelining

    2.  In case of Redirection there is a separate array that holds the name
        of the input/output file

    3. After the parser is done Execute_Command() takes place and is responsible to
        execute the given commands.
            -First checks if there is any pipes if there is then another function is 
            responsible  for the current commands

            -Then check for Redirections and simply does a fork() nothing special

            -Finally executes the commands

    4. Pipes work as ONE command this is why a different function
        handles them (Execute_pipeline)
        And this is how the multipipe works, it acts like its one command

    5. Then the buffer resets and its ready for another input from the user            


SUPPORTS:

-Multiple commands supported
-Multiple pipes supported (for external commands only)
-Redirection supported