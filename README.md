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
 gcc -o a.out ex22.c  
 ./a.out /home/os2021/conf.txt  
  
### How it works?  
    
1. **The program enters all sub-directories**, and should ignore all files that aren’t directories - if exists,  and to **search in each sub-directory a c file and to compile it**.  
*notice that the program searches c files only in the path of the sub-directory itself, and not in deeper levels.*
2. The executable file of each c program in the sub-directories, should **run with the content input** that is in the input file we have in line 2 of the configuration file.  
3. The output of any c file that we have compiled and run, should be **compared to the correct_output**, that is in line 3 of the configuration file.  
  
(*we can assume that in each sub-directory there will be at most 1 c file, and of course there could be more directories and another files, but one c file at the most.  
we also assume that each c file will have a propriate suffix, which is .c.*)     
  
**The interesting part of the project** is that the executable files we got, are compared with the correct_output, by using the executable file - **comp.out**, of another repository – **Files-Comparator**.  
*comp.out actually compares between 2 files and determines if the files are identical/similar or different (the interpretations to these concepts are in Files-Comparator readme repository*).  
  
4. To summarize all that data, we **create a results.csv file** that contains the name of any user (the sub-directory name ; the student name for example) and its grade, based on the results comp.out gave us.  
    
**About the grades in results.csv**:  
+ If there is **no c file** in the sub-directory, then the grade will be **0**.  
+ if there is a **compilation error**, then the grade will be **10**.  
+ if there is **wrong answer**, i.e the files are different, then the grade will be **50**.  
+ if there is a **similar answer**, i.e the files are similar but not identical, then the grade will be **75**.  
+ if there is an **excellent answer**, i.e the files are identical, then the grade will be **100**.
    
**Example of a results.csv file**:  
Monica,100,EXCELLENT  
Phoebe,0,NO_C_FILE  
Ross,10,COMPILATION_ERROR  
Joey,50,WRONG  
Chandler,75,SIMILAR  
  
5. Moreover, we create an **errors.txt file**. Any errors in the programs we run of the c files that are located in the sub-directories (for example, the programs of the students), will be written to this file, instead of to the screen, so we have a follow up on compilation errors of student programs.  
  
## Example of the automatic checking files system running:  
** The configuration file: **  
![image](https://user-images.githubusercontent.com/83518959/193104901-0de3e2db-9791-443d-855e-5159ed7ee6b9.png)  
  
** the input and correct output files: **  
![image](https://user-images.githubusercontent.com/83518959/193104721-c676b58d-98d9-4114-9a64-2a3f05ec6d6e.png)  
  
** The results.csv and errors.txt files: **  
![image](https://user-images.githubusercontent.com/83518959/193103565-85649ff3-4b8f-4942-918c-69809ec75fad.png)  
  
** The students c programs in sub directories: **  
![image](https://user-images.githubusercontent.com/83518959/193103770-94bb29ab-cf44-41d2-a8ba-b02ff2fbbf63.png)  
  
** Example of programs of students: **  
Excellent:  
![image](https://user-images.githubusercontent.com/83518959/193105305-6dc5ecd8-bef7-4c76-9bac-34a783425780.png)  

Similar:  
![image](https://user-images.githubusercontent.com/83518959/193105470-24dd9d9b-99cf-4a92-a76f-c11ac8781e21.png)  

Compilation Error:  
![image](https://user-images.githubusercontent.com/83518959/193105577-1e45288c-830b-4d13-96c1-a22edad15a11.png)  
etc.  








