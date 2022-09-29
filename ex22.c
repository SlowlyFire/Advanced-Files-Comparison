// Gal Giladi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#define OPEN_ERROR "Error in: open"
#define CLOSE_ERROR "Error in: close"
#define READ_ERROR "Error in: read"
#define WRITE_ERROR "Error in: write"
#define FORK_ERROR "Error in: fork"
#define EXECVP_ERROR "Error in: execvp"
#define CHDIR_ERROR "Error in: chdir"
#define OPENDIR_ERROR "Error in: opendir"
#define STAT_ERROR "Error in: stat"

int isDirectory(char* path, char* pathToStudentDirectory) {
    if (chdir(pathToStudentDirectory) == -1) {
        perror(CHDIR_ERROR);
        exit(-1);
    }

    struct stat buf;
    if (stat(path, &buf) < 0) {
        perror(STAT_ERROR);
        exit(-1);
    }
    int returnVal = S_ISDIR(buf.st_mode);
     if (chdir("..") == -1) {
        perror(CHDIR_ERROR);
        exit(-1);
    }
    if (chdir("..") == -1) {
        perror(CHDIR_ERROR);
        exit(-1);
    }

    return returnVal;
}

int compileFile(char* pathToCFile, char* cFileWithOutSuffix, char* pathToStudentDirectory, int fdErrorFile) {
    pid_t pid;
    // here we compile the c file, with given path to the file
    // we should use fork and child proccess
    char* command = "gcc";
    char* commandArgs[5];
    commandArgs[0] = "gcc";
    commandArgs[1] = pathToCFile;
    commandArgs[2] = "-o";
    commandArgs[3] = cFileWithOutSuffix;
    commandArgs[4] = NULL;
    // redirection
    dup2(fdErrorFile, 2);
    // Forking a child, fork returns pid of the son
	if((pid = fork()) == -1) {
        perror(FORK_ERROR);
        exit(-1);
	// if fork=0 im in son proccess
	} else if (pid == 0) {
        if (chdir(pathToStudentDirectory) == -1) {
            perror(CHDIR_ERROR);
            exit(-1);
        }
		if (execvp(command, commandArgs) < 0) {
			perror(EXECVP_ERROR);
            exit(-1);
		}
	} else {
		// waiting for child to terminate
		int status;
        waitpid(pid, &status, 0);
        int result = WEXITSTATUS(status);
        return result;
	}
}

void runFile(char* confLine2, char* cFileWithOutSuffix, int fdInputFile, int fdOutputFile) {
    pid_t pid;
    // here we run the c file, with given path to the file
    // and with given path to a file that containts the input we wish to use.
    // this input is line 2 of conf.txt
    // we should use fork and child proccess
    char command[150] = {'\0'};
    strcat(command, "./");
    strcat(command, cFileWithOutSuffix);
    char* commandArgs[3];
    commandArgs[0] = command;
    commandArgs[1] = confLine2;
    commandArgs[2] = NULL;
    // Forking a child
	if ((pid = fork()) == -1) {
        perror(FORK_ERROR);
        exit(-1);
	// if fork=0 im in son proccess
	} else if (pid == 0) {
        // redirection
        dup2(fdInputFile, 0);
        dup2(fdOutputFile, 1);
		if (execvp(command, commandArgs) < 0) {
			perror(EXECVP_ERROR);
            exit(-1);
		}
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

int compareOutputs(char* confLine3, char* pathToStudentDirectory) {
    pid_t pid;
    // compares between output files and return what comp.out gives us
    char studentOutput[150] = {'\0'};
    strcat(studentOutput, pathToStudentDirectory);
    strcat(studentOutput, "/outputFile.txt");
    char* command = "./comp.out";
    char* commandArgs[4];
    commandArgs[0] = command;
    // confLine3 is excpectedOutput
    commandArgs[1] = confLine3;
    commandArgs[2] = studentOutput;
    commandArgs[3] = NULL;
    // Forking a child
	if ((pid = fork()) == -1) {
        perror(FORK_ERROR);
        exit(-1);
	// if fork=0 im in son proccess
	} else if (pid == 0) {
		if (execvp(command, commandArgs) < 0) {
			perror(EXECVP_ERROR);
            exit(-1);
        }
	} else {
        // waiting for child to terminate
        int status;
        waitpid(pid, &status, 0);
        int result = WEXITSTATUS(status);
        return result;
	}
}

int main(int argc, char* argv[])
{
    char* tmpForUnlink;
    // creates errors.txt file
    int fdErrorFile;
    fdErrorFile = open("errors.txt", O_RDWR | O_TRUNC | O_APPEND | O_CREAT, 0777);
    if (fdErrorFile == -1) {
        perror(OPEN_ERROR);
        exit(-1);
    }

    // we should have only 1 argument to main, which is the path to 
    // configuration file
    if (argc != 2) {
        write(fdErrorFile, "Number of arguments is not valid\n", 33);
        close(fdErrorFile);
        exit(-1);
    }

    // opens configuration text path we got as an argument to main
    int fd;
    char* confPath = argv[1];
    fd = open(confPath, O_RDONLY);
    if (fd == -1) {
        perror(OPEN_ERROR);
        close(fdErrorFile);
        exit(-1);
    }

    char* confLine1;
    char* confLine2;
    char* confLine3;
    char buff[450]={'\0'};
    int resultRead;
    resultRead = read(fd, buff, 450);
    if (resultRead == -1) {
        perror(READ_ERROR);
        close(fd);
        close(fdErrorFile);
        exit(-1);
    }

    // seperates all lines in configuration file
    confLine1 = strtok(buff, "\n");
    confLine2 = strtok(NULL, "\n");
    confLine3 = strtok(NULL, "\n");

    // check if confLine3 is a valid path
    int fdGivenOutput;
    if ((fdGivenOutput = open(confLine3, O_RDONLY)) == -1) {
        perror(OPEN_ERROR);
        write(fdErrorFile, "Output file not exist\n", 22);
        write(fdErrorFile, "\n", 1);
        close(fd);
        close(fdErrorFile);
        exit(-1);
    }
    close(fdGivenOutput);

    // creates results.csv file
    int resultsFd;
    resultsFd = open("results.csv", O_RDWR | O_TRUNC | O_APPEND | O_CREAT, 0777);
    if (resultsFd == -1) {
        perror(OPEN_ERROR);
        close(fd);
        close(fdErrorFile);
        exit(-1);
    }

    // opens the directory we have got on confLine1
    DIR* dirOfAllStudents = opendir(confLine1);
    if (dirOfAllStudents == NULL) {
        perror(OPENDIR_ERROR);
        write(fdErrorFile, "Not a valid directory\n", 22);
        write(fdErrorFile, "\n", 1);
        close(resultsFd);
        close(fd);
        close(fdErrorFile);
        exit(-1);
    }

    struct dirent *studentXDir; 
    // opens one of the sub-directories inside confLine1 path (usrs) and looks for a c file.
    // while looping all sub-directories of the students.
    while ( (studentXDir = readdir(dirOfAllStudents)) != NULL ) {
        if(strcmp(studentXDir -> d_name, ".") != 0 &&
        strcmp(studentXDir -> d_name, "..") != 0) {
            
            // flagCFile is 0 when there is no c file in directory
            int flagCFile = 0;
            // now we create the path to the student directory
            char pathToStudentDirectory[150] = {'\0'};
            strcat(pathToStudentDirectory, confLine1);
            strcat(pathToStudentDirectory, "/");
            strcat(pathToStudentDirectory, studentXDir -> d_name);

            // opens sub-directory of specific student
            DIR* dirOfSpecificStudent = opendir(pathToStudentDirectory);
            if (dirOfSpecificStudent == NULL) {
                perror(OPENDIR_ERROR);
                close(resultsFd);
                close(fd);
                close(fdErrorFile);
                exit(-1);
            }
            struct dirent *insideStudentDirectory; 
            int lengthOfFileName;

            // scan files of the sub-directory of a specific student
            while ( (insideStudentDirectory = readdir(dirOfSpecificStudent)) != NULL ) {
                if(strcmp(insideStudentDirectory -> d_name, ".") != 0 &&
                strcmp(insideStudentDirectory -> d_name, "..") != 0) {
                    lengthOfFileName =  strlen(insideStudentDirectory -> d_name);
                    // we only want to conentrate about ".c" files and to compile them
                    // if there is a directory that ends with ".c" we skip
                    if((insideStudentDirectory -> d_name[lengthOfFileName-2]) == '.' 
                    && (insideStudentDirectory -> d_name[lengthOfFileName-1]) == 'c' 
                    && (isDirectory(insideStudentDirectory -> d_name, pathToStudentDirectory) == 0)) {
                        
                        // if we found a ".c" file in Student Directory, flagCFile is 1
                        flagCFile = 1;

                        // build the c file a full path
                        char pathToCFile[150] = {'\0'};
                        strcat(pathToCFile, insideStudentDirectory -> d_name);

                        // build the c file name with ".out" suffix
                        char cFileWithOutSuffix[150] = {'\0'};
                        strcat(cFileWithOutSuffix, insideStudentDirectory -> d_name);
                        cFileWithOutSuffix[lengthOfFileName-1] = '\0';
                        cFileWithOutSuffix[lengthOfFileName-2] = '\0';
                        strcat(cFileWithOutSuffix, ".out");
                            
                        // send the c file to compilation
                        int compileResult = compileFile(pathToCFile, cFileWithOutSuffix, pathToStudentDirectory, fdErrorFile);
                        // compileResult will be 0 if managed to compile, and 1 if couldn't compile
                        if (compileResult == 1) {
                            if ((write(resultsFd, studentXDir -> d_name, strlen(studentXDir -> d_name))) == -1) {
                                perror(WRITE_ERROR);
                                close(resultsFd);
                                close(fd);
                                close(fdErrorFile);
                                exit(-1);
                            }
                            if ((write(resultsFd, ",10,COMPILATION_ERROR\n", 22)) == -1) {
                                perror(WRITE_ERROR);
                                close(resultsFd);
                                close(fd);
                                close(fdErrorFile);
                                exit(-1);
                            }
                            continue;
                        }

                        // opens the input file that we have got on confLine2
                        int fdInputFile;
                        fdInputFile = open(confLine2, O_RDONLY);
                        if (fdInputFile == -1) {
                            perror(OPEN_ERROR);
                            write(fdErrorFile, "Input file not exist\n", 21);
                            write(fdErrorFile, "\n", 1);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }

                        if (chdir(pathToStudentDirectory) == -1) {
                            perror(CHDIR_ERROR);
                            close(fdInputFile);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }

                        // creates an output file to each one of the students, based on
                        // the input we had given
                        int fdOutputFile;
                        fdOutputFile = open("outputFile.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                        if (fdOutputFile == -1) {
                            perror(OPEN_ERROR);
                            close(fdInputFile);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }
                        
                        // run the compiled c file                        
                        runFile(confLine2, cFileWithOutSuffix, fdInputFile, fdOutputFile);
                        if (chdir("..") == -1) {
                            perror(CHDIR_ERROR);
                            close(fdInputFile);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }
                        if (chdir("..") == -1) {
                            perror(CHDIR_ERROR);
                            close(fdInputFile);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }
                        // close files in the end of the use of fd
                        close(fdOutputFile);
                        close(fdInputFile);
                        
                        // compare between the excpectedOutput we have given in confLine2
                        // to the output we have got on the compiled and running c file of the student
                        int compareNum;
                        compareNum = compareOutputs(confLine3, pathToStudentDirectory);
                        // append grades of students to results.csv file 
                        if ((write(resultsFd, studentXDir -> d_name, strlen(studentXDir -> d_name))) == -1) {
                            perror(WRITE_ERROR);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
                        }
                        // if files are different
                        if(compareNum == 2) {
                            if((write(resultsFd, ",50,WRONG\n", 10)) == -1) {
                                perror(WRITE_ERROR);
                                close(resultsFd);
                                close(fd);
                                close(fdErrorFile);
                                exit(-1);
                            }
                        }
                        // if files are similar
                        if (compareNum == 3) {
                            if((write(resultsFd, ",75,SIMILAR\n", 12)) == -1) {
                                perror(WRITE_ERROR);
                                close(resultsFd);
                                close(fd);
                                close(fdErrorFile);
                                exit(-1);
                            }
                        }
                        // if files are the same
                        if(compareNum == 1) {
                            if((write(resultsFd, ",100,EXCELLENT\n", 15)) == -1) {
                                perror(WRITE_ERROR);
                                close(resultsFd);
                                close(fd);
                                close(fdErrorFile);
                                exit(-1);
                            }
                        }

                        tmpForUnlink = cFileWithOutSuffix;
                    }
                }
            }
            if (chdir(pathToStudentDirectory) == -1) {
                            perror(CHDIR_ERROR);
                            close(resultsFd);
                            close(fd);
                            close(fdErrorFile);
                            exit(-1);
            }

            // delete in every student directory: outputFile.txt and name_of_file.out
            unlink("outputFile.txt");
            unlink(tmpForUnlink);

            if (chdir("..") == -1) {
                perror(CHDIR_ERROR);
                close(resultsFd);
                close(fd);
                close(fdErrorFile);
                exit(-1);
            }
            if (chdir("..") == -1) {
                perror(CHDIR_ERROR);
                close(resultsFd);
                close(fd);
                close(fdErrorFile);
                exit(-1);
            }

            // if flagCFile = 0 , then there was no ".c" file in the student directory
            if (flagCFile == 0) {
                if((write(resultsFd, studentXDir -> d_name, strlen(studentXDir -> d_name))) == -1) {
                    perror(WRITE_ERROR);
                    close(resultsFd);
                    close(fd);
                    close(fdErrorFile);
                    exit(-1);
                }
                if((write(resultsFd, ",0,NO_C_FILE\n", 13)) == -1) {
                    perror(WRITE_ERROR);
                    close(resultsFd);
                    close(fd);
                    close(fdErrorFile);
                    exit(-1);
                }
            }
            // clearing the path for the next student and close his directory
            pathToStudentDirectory[0] = '\0';
            closedir(dirOfSpecificStudent);
        }
    } 

    // finished scanning all students directories, we enter a new line in results.csv
    // as needed from us, and close all open files.
    if ((write(resultsFd, "\n", 1)) == -1) {
        perror(WRITE_ERROR);
        close(resultsFd);
        close(fd);
        close(fdErrorFile);
        exit(-1);
    }
    close(resultsFd);
    closedir(dirOfAllStudents);
    close(fd);
    close(fdErrorFile);
    return 0;
}
