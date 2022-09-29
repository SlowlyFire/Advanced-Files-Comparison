# Advanced-Files-Comparison
### In this project we take the files comparison to the next level.  
**This project is basically an automatic file checking system for teachers, so they can check all the program assignments they give to CS studesnt, with a configuration text of their own, which contains a path to the c program of the student, an input of its teacher choice, and a correct output that should be recieved.**    
   
*notice that in this project, we also use the executable file of the Files-Comparator repository. This executable file is called comp.out.*  
  
In this project we have a **configuration file**, which we get as an argument to the main, that contains **3 lines**:
1. **A path to a directory that contains sub-directories** (student names for example). Each sub-directory represents a user, and suppose to contain a c file.  
2. A path to a file that contains the **input** (for example, any input that comes to mind, so the teacher can check the program randomly).  
3. A path to a file that contains the **output**, according to the given input in line 2 (the correct output that should recived from the program of the student with the input of the teacher for example).  
  
**Example of the configuration.txt file**:  
/home/os2021/students  
/home/os2021/io/input.txt  
/home/os2021/io/output.txt  

**Example of an input.txt file**:  
1  
5 4  
4  
  
**Example of a correct_output.txt file**:  
Please enter an operation  
Please enter two numbers  
The sum is 9  
Please enter an operation  
Bye  

**Example of running the compiled program with the configuration file**:  
 ./a.out /home/os2021/conf.txt  
  
**The program enters all sub-directories**, and should ignore all files that aren’t directories - if exists,  and to **search in each sub-directory a c file and to compile it**.  
*notice that the program searches c files only in the path of the sub-directory itself, and not in deeper levels.*
- The executable file (a.out as default) that we have got from the compilation process, should **run with the content input** that is in the input file we have in line 2 of the configuration file.  
- The output of any c file that we have compiled and run, should be **compared to the correct_output**, that is in line 3 of the configuration file.  
* we can assume that in each sub-directory there will be at most 1 c file, and of course there could be more directories and another files, but one c file at the most.  
* we also assume that each c file will have a propriate suffix, which is .c.  
  
The interesting part of the project is that the executable files we got, are compared with the correct_output, by using the executable file of another repository – **comp.out**.  
*comp.out actually compares between 2 files and determines if the files are identical/similar or different (the interpretations to these concepts are in Files-Comparator readme repository*.  
- To summarize all that data, we **create a results.csv file** that contains the name of any user (the sub-directory name) and its grade, based on the results comp.out gave us.  
    
About the grades in results.csv:  
+ If there is no c file in the sub-directory, then the grade will be 0.  
+ if there is a compilation problem, then the grade will be 10.  
+ if there is wrong answer, i.e the files are different, then the grade will be 50.  
+ if there is a similar answer, i.e the files are similar but not identical, then the grade will be 75.  
+ if there is an excellent answer, i.e the files are identical, then the grade will be 100.
  
Example of a results.csv file:
Monica,100,EXCELLENT
Phoebe,0,NO_C_FILE
Rachel,20,TIMEOUT
Ross,10,COMPILATION_ERROR
Joey,50,WRONG
Chandler,75,SIMILAR

- Moreover, we create an **errors.txt file**. Any errors in the programs we run of the c files that are located in the sub-directories, will be written to this file, instead of to the screen.


