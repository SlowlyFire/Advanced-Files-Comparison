// Gal Giladi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char* argv[]) {
    int fd1, fd2;
    char ch1, ch2;
    int resultRead1, resultRead2;
    // open file 1
    fd1 = open(argv[1], O_RDONLY);
    if ( fd1 == -1 ) {
        write(2,"Error in: open", 15);
        exit(-1);
    }
    // open file 2
    fd2 = open(argv[2], O_RDONLY);
    if ( fd2 == -1 ) {
        write(2,"Error in: open", 15);
        close(fd1);
        exit(-1);
    }

    int flag1WhiteSpace = 0;
    int flag2WhiteSpace = 0;
    int flagSimilar = 0;
    // reads char by char from both files untill finished reading
    // from files or mismatch files
    while(1) {
        if (flag1WhiteSpace == 0) {
            // read one char from file 1
            resultRead1 = read(fd1, &ch1, 1);
            if (resultRead1 == -1) {
                write(2,"Error in: read", 15);
                close(fd1);
                close(fd2);
                exit(-1);
            }
        }
        if (flag2WhiteSpace == 0) {
            // read one char from file 2
            resultRead2 = read(fd2, &ch2, 1);
            if (resultRead2 == -1) {
                write(2,"Error in: read", 15);
                close(fd1);
                close(fd2);
                exit(-1);
            }
        }

        // if both files are finished and have nothing else to read, then files identical/the same
        if (resultRead1 == 0 && resultRead2 == 0) {
            close(fd1);
            close(fd2);

            // if flagSimilar is on, then files are similar, but not identical
            if (flagSimilar == 1)
                return 3;
       
            // else flagSimilar is off, then files are identical
            else 
                return 1;
            
        }

        // if file 1 is finished and have nothing else to read, but file 2
        // still have chars to read (enters, white-spaces or visible chars)
        if (resultRead1 == 0 && resultRead2 != 0) {
            flagSimilar = 1;
            // if ch2 is enter or white space
            if (ch2 == '\n' || ch2 == ' ') {
                // keep reading from file 2
                flag2WhiteSpace = 0;
                // stop reading from file 1
                flag1WhiteSpace = 1;
                continue;
            }
            else 
                // else, ch2 is visible (not white space nor enter), then files are different
                return 2;
            
        }

        // if file 2 is finished and have nothing else to read, but file 1
        // still have chars to read (enters, white-spaces or visible chars)
        if (resultRead2 == 0 && resultRead1 != 0) {
            flagSimilar = 1;
            // if ch1 is enter or white space
            if (ch1 == '\n' || ch1 == ' ') {
                // keep reading from file 1
                flag1WhiteSpace = 0;
                // stop reading from file 2
                flag2WhiteSpace = 1;
                continue;
            }
            else 
                // else, ch1 is visible (not white space nor enter), then files are different
                return 2;
            
        }

        // if both chars are equal, continue looping to next char
        if (ch1 == ch2) {
            flag1WhiteSpace = 0;
            flag2WhiteSpace = 0;
            continue;
        }
        // if there is a mismatch between both chars, but ch1 is a white space (0x20)
        if (ch1 != ch2 && ch1 == ' ') {
            flagSimilar = 1;
            // ch1 is white space, so we want to read one more char from
            // file 1, and to pause the reading from file 2
            flag2WhiteSpace = 1;
            continue;
        }

        // if there is a mismatch between both chars, but ch2 is a white space (0x20)
        if (ch1 != ch2 && ch2 == ' ') {
            flagSimilar = 1;
            // ch2 is white space, so we want to read one more char from
            // file 2, and to pause the reading from file 1
            flag1WhiteSpace = 1;
            continue;
        }

        // if there is a mismatch between both chars, but ch1 is enter (0x0D)
        if (ch1 != ch2 && ch1 == '\n') {
            flagSimilar = 1;
            flag1WhiteSpace = 0;
            // ch1 is enter, so we want to read one more char from
            // file 1, and to pause the reading from file 2
            flag2WhiteSpace = 1;
            continue;
        }

        // if there is a mismatch between both chars, but ch2 is enter (0x0D)
        if (ch1 != ch2 && ch2 == '\n') {
            flagSimilar = 1;
            flag2WhiteSpace = 0;
            // ch2 is enter, so we want to read one more char from
            // file 2, and to pause the reading from file 1
            flag1WhiteSpace = 1;
            continue;
        }

        // if there is a mismatch between readen chars at files
        if (ch1 != ch2) {
            // if ch1 is upper-case of ch2
            if (ch1 >= 'A' && ch1 <= 'Z' && ch2 >= 'a' && ch2 <= 'z') {
                    // converting ch1 to lower-case
                    ch1 = ch1 + 32;
                    if (ch1 == ch2) {
                        flagSimilar= 1;
                        flag1WhiteSpace = 0;
                        flag2WhiteSpace = 0;
                        continue;
                    }
            }

            // if ch2 is upper-case of ch1
            if (ch2 >= 'A' && ch2 <= 'Z' && ch1 >= 'a' && ch1 <= 'z') {
                    // converting ch2 to lower-case
                    ch2 = ch2 + 32;
                    if (ch1 == ch2) {
                        flagSimilar= 1;
                        flag1WhiteSpace = 0;
                        flag2WhiteSpace = 0;
                        continue;
                    }
            }

            // else there is a mismatch between files, break and return 2
            close(fd1);
            close(fd2);
            return 2;
        }
    }
}
