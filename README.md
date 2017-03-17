## CSCI 3753 - Programming Assignment 3

### DNS Name Resolution Engine

#### by Gannon Faul

#### Friday, March 17, 2017


**Files Submitted**

1. multi-lookup.c: Primary program file that contains my main function, and ...my helper functions resolve and request. The main function creates ...the threads and queue and error checks the input arguments to ...align with the lab specifications. It also joins the threads and ...cleans up the queue and mutexes once the threads are finished ...running. The request function handles the threads responsible ...for reading from the inputted text files and push hostnames ...to the queue. The resolve function is responsible handling the ...threads that pop hostnames off of the queue and write them to the ...output file.

2. multi-lookup.h: Header file for multi-lookup.c. This file contains the ...definitions of the constants that I use in multi-lookup.c, as well as ...the function prototypes for request() and resolve().

3. Makefile: This file tells the kernel how to compile my code, and creates ...executables for multi-lookup.c as well as several other helper ...files. This file includes a clean option to get rid of my previous ...executables and output files.

4. README.md: Markdown file with a write-up explaining my submitted files ...and how to build my code.


**Building the Program**

1. Navigate to the local file containing multi-lookup.c and input files.

2. > make clean

3. > make

4. > ./multi-lookup names*.txt results.txt
..* You may include however many input files for names*.txt as needed

5. > gedit results.txt



 
