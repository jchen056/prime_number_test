Synopsis

This project asks you to implement a simple primality test. Recall that a number n > 1 is prime if the only divisors of n are 1 and itself (we don’t consider 1 to be prime).

Here’s what you can expect to learn from this:

    Looping and conditional constructs.
    How to make programs that play well with others (cf. the unix philosophy).
    A little bit about some nifty unix tools like awk and bash.

Important Notes on Grading

As always,

    Code that does not compile will receive a 0.
    Please follow the submission instructions carefully!

Details

This time around, your program will not be so chatty. The program will just wait for you to type an integer, and then print a 0 or 1 to indicate whether or not that integer was prime. Here’s some sample output of an interactive session (assuming primetest is the name of the executable1):

$ ./primetest
11
1
12
0

After 11 the response is 1, indicating primality; after 12, the response is 0, meaning it is not prime. You can end the interactive session by hitting Ctrl-D, or by entering something that is not an integer, like the character a. Important note: your program is not supposed to print any messages! It is just supposed to read from standard input, and write zeros and ones to standard output. The next section will explain why this is a good idea.
Aside: Interaction With the Shell

You may be thinking that this seems like a lousy, user-unfriendly interface, and perhaps in some situations you’d be right. But it really depends on your point of view. What we intend to build here is not a product for the masses, but a tool for the competent. As I hope to demonstrate shortly, adding a bunch of chatty output messages would only serve to diminish the utility of this program as a tool.2 (And if we’re honest, who would really expect a program about prime numbers to be designed for mass-consumption?)

All right, so let’s take a look at a few examples of how someone with a little bit of knowledge about the shell might use your program. To begin, we could use the shell (bash) to print out a list showing which integers are prime:

$ echo {1..10} | ./primetest
0
1
1
0
1
0
1
0
0
0

Not so exciting, but notice the use of the pipe | operator, which takes the output of echo and sends it to our program through standard input. So, the cin statements in our code will pick up the output from echo, as if we just sat there typing all the numbers in from the shell. (By the way – the command echo {1..10} by itself will just print the list of integers from 1 to 10 with spaces in between – try it out.)

Let’s try something a little more interesting. Suppose we wanted to generate a list of all the prime numbers less than 100. Brace expansion gives us an easy way to get the list of consecutive integers ({1..100}), and then we can use awk with our program to filter the list:

$ echo {1..100} | ./primetest | awk '/1/ {print NR}'
2
3
...
97

Note the repeated use of the pipe operator | to chain the output of one program to the input of the next. Since awk was also designed to read standard input, we can send our cout statements right to it using the pipe! The awk stuff might look a little cryptic, but it is simple: in plain English, it says something like “if a line has a 1, print out the line number”. The quoted string is the entire awk program containing those instructions. The interested reader can follow up here.

Now let’s say you wanted, for some reason, to find only the primes less than 100 which end in the digit 9. Just a small change does it:

$ echo {1..100} | ./primetest | awk '/1/ && NR % 10 == 9 {print NR}'
19
29
59
79
89

# alternative, faster method (filtering is done before testing primality)
$ echo {9..100..10} | ./primetest | awk '/1/ {print (NR-1)*10 + 9}'

Or we could store a list of primes less than 1000 in a bash array, and see how many primes we have:

$ primes=($(echo {1..1000} | ./primetest | awk '/1/ {print NR}'))
$ echo ${primes[@]}
2 3 5 7 11 13 17 19 23 ...
... 911 919 929 937 941 947 953 967 971 977 983 991 997
$ echo ${#primes[@]}
168

Or even print a nice formatted list in html:

$ echo {1..1000} | ./primetest | awk 'BEGIN{print "<html><body><ol>\n"} '\
  '/1/ {print "<li>"NR"</li>"} END{print "</ol></body></html>"}' \
  > primeslist.html

The file primelist.html would then display something like

        2
        3
        5
        7…

And so on. Try it!!

The list of possibilities goes on and on… Hopefully this gives you an idea of the capabilities of the unix shell, and perhaps will generate some ideas for you down the line. Note that I don’t expect you to understand every detail right away – in fact, I would be surprised if that were the case. So don’t be worried. But I encourage you to go read about it at some point. These are powerful tools, and you’d be doing yourself a great service to learn more about them. Here’s a nice introduction to awk.

Important: once you’re done with the homework, come back through here and try out all of the examples above.
Hints

Regarding the algorithm: There are a lot of very clever primality tests out there. I don’t want you to write any of those. Keep it simple – just do trial division by odd integers until you either find a divisor, or you run out of candidates. I.e., to figure out if an odd number n is prime, we can check to see if any of {3, 5, 7, ..., n − 2} divide n. And of course if n is even, then it is prime only if n = 2. One thing to think about: I claim we can stop checking long before n − 2. See if you can figure out how many checks are really required, and modify your program to only check as many as necessary.

Regarding the programming: You may want to start with a template that looks something like this:

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <cmath>

int main()
{

}

The #include <cmath> is there in case you figure out the efficiency improvement I mentioned earlier. The sqrt function might be of use. Whoops. I gave away another hint.

To read from standard input, you’ll want to use cin, but it may not be so obvious how to decide when to stop, and we don’t want to display a menu, as we mentioned earlier. The following should work pretty well:

unsigned long n;
while(cin >> n) {
    //do stuff.
}

You can think of the expression cin >> n evaluating to false as a boolean when the user enters a non-integer, or when the end of an input stream is encountered, and true if it successfully read a number into n. At first, you may find it a little strange to interpret a cin statement as a boolean, but do you remember our if(x=23) {...} example from class where we accidentally left out an equals sign? This is much the same. As with (almost3) any other expression in C++, cin >> n has a datatype and a value, and any expression that can be converted to an integer or boolean can be used for conditional statements like while(...).

One more thing that may be of use is the break statement. This is a convenient way to stop a loop, rather than explicitly manipulating the variables in the condition that it tests. For example, once you find a non-trivial divisor of the number you’re testing, you may want to immediately end the inner loop, since you know at that point that the number isn’t prime.

Makefiles: I gave you a makefile this time. Using the tutorials (here and here), try to reverse engineer it if you have time. It is overly-simplistic, but pretty easy to follow. Reminder: to compile primetest.cpp into executable code named primetest, just run the command:

$ make

Testing + Debugging Your Code

To make things easier for you, a test procedure comes with the homework. The file somenumbers contains the numbers 1, …, 50, followed by a list of “more interesting” test inputs, and the file rightanswers has a precomputed list of the corresponding answers. If you run make && ./sanitycheck.sh from within the directory, it will tell you if anything goes wrong. Note: the test is “fragile” in some sense – make sure you don’t print anything besides a 0/1 for each number read from stdin. Any extra messages, or even extra blank lines, could make the test fail.

One more note about cool stuff you can do with the shell: you could make something about as good as my test program by piecing together standard utilities. If you finish the project early, see if you can figure this one out, which gives you essentially the same information as sanitycheck.sh:

$ ./primetest <somenumbers | paste somenumbers - | \
    sdiff -s - <(paste somenumbers rightanswers)

If for example your program failed to recognize 5 and 197 as primes, the output of that command would look like this (your program’s answers are on the left):

5       0          | 5       1
197     0          | 197     1

Debugging

Should something go wrong and your output is different than what it is supposed to be, don’t panic and don’t get frustrated! Here are some (fairly general) steps you can take to fix your program. Think of what follows as part II of the gdb tutorial (which you should read first!).

    Find an input (preferably as small as possible) where your code fails. Let’s say the smallest number your code mislabels is 9.
    Make sure your program is compiled with -g (you can get the makefile to do this for you by running make -B debug).
    Make a note of the “interesting” variables that you use, like the variable to hold the input, and maybe another that runs through potential divisors. In my program, these are named n and k respectively.
    Open another terminal (this will be for your program’s output4) and from this new terminal run $ tty. You should see something like /dev/pts/3. I’ll use this value – adjust as needed in what follows.
    Start gdb with $ gdb -tty /dev/pts/3 primetest, and then enter the following gdb commands:

(gdb) tui enable
(gdb) break main
(gdb) run <<<"9"

NOTE: the <<<"9" gives your program the string "9" as stdin, as if you had typed it. Remember to change it to whatever inputs fail for you. You can test more numbers by adding them to that string (e.g., <<<"9 16 25"), or storing them in a file and using run < filewithnumbers instead.

You should now have your code at the top of the window, stopped at the first line of main(). Now let’s set gdb to print some of your interesting variables each time the program is paused (see automatic display in the gdb documentation).

(gdb) display n
(gdb) display k

You can also display expressions which might be useful, e.g. display n%k. Now we can step through the program and with any luck, see why / how it is broken. Just use the next command and keep hitting enter, and you can walk through your code one statement at a time, printing the “interesting” variable contents as you go.

(gdb) next
1: n = 9
2: k = 3
3: n%k = 0
...

Remember: when you hit a cout statement, what you print will appear in that other terminal, not in the gdb window. NOTE: you can save a little space by wrapping those variables into an array (if they all have the same datatype!). Try this:

(gdb) undisplay
Delete all auto-display expressions? (y or n) y
(gdb) display {n,k,n%k}
4: {n,k,n%k} = {9, 3, 0}
(gdb) next
4: {n,k,n%k} = {9, 3, 0}
...

Hopefully this will help you figure it out!
