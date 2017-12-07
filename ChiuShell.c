#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <windows.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <wchar.h>
#include <libgen.h>

/*
* Function: readLine
* ------------------
* reads the input of the user and stores it into a string
*
* returns: the line being read
*/
char *readLine() {

	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);

	return line;
}

/*
* Function: tokenize
* ------------------
* converts a string into an array of strings
*
* line: the input string to be separated into tokens
* delimeters: the string that contains symbols to separate line into tokens
*
* returns: an array of strings 
*/
#define TOKEN_SIZE 128
char **tokenize(char *line, char *delimeters) {
	int bufsize = TOKEN_SIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens) {
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, delimeters);
	while(token != NULL) {
		tokens[position] = token;
		position++;

		if(position >= bufsize) {
			bufsize += TOKEN_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if(!tokens) {
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, delimeters);
	}
	tokens[position] = NULL;
	return tokens;
}

/*
* Function: getDir
* ----------------
* displays the current directory 
*/
void getDir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf(cwd);
	printf(">");
}

/*
* Function: lowerStr
* ------------------
* converts a string to its lowercase version
* 
* returns: a lowercase version of the string
*/
char *lowerStr(char *str) {
	for(int i = 0; str[i]; i++){
	  str[i] = tolower(str[i]);
	}

	return str;
}

/*
* Function countSlash
* -------------------
* counts how many slashes are in the string
*
* string: the given string to be check
*
* returns: the numver of slashes in the string
*/
int countSlash(char* string) {
	int size = strlen(string);
	int slashes = 0;
	for(int i = 0; i < size; i++) {
		if(string[i] == '\\') {
			slashes++;
		}
	}
	return slashes;
}

/*
* Function: countArgs
* -------------------
* counts how many arguments there are in a token, excluding the cmd commands
*
* tokens: array of strings
*
* returns: the number of tokens -1
*/
int countArgs(char **tokens) {
	int noOfArgs = -1;
	char **temp = tokens;

	while(*temp != NULL) {
		noOfArgs++;
		temp++;
	}
	return noOfArgs;
}

/*
* Function getStrSize
* -------------------
* counts the number of characters in a string
*
* string: the string
*
* returns: the size of the string
*/
int getStrSize(char *string) {
	return sizeof(string);
}

/*
* Function getStrSize2
* --------------------
* counts the number of characters in a string
*
* string: the string
*
* returns: the size of the string
*/
int getStrSize2(char *string) {
	int i = 0;
	while(1) {
		if(string[i] != '\0') {
			i++;
		} else {
			break;
		}
	}

	return i;
}

/*
* Function: incorrect
* -------------------
* prints a statement declaring that the command is incorrect
*/
void incorrect() {
	printf("The syntax of the command is incorrect.\n");
}

/*
* Function: cantFind
* ------------------
* prints a statement declaring that the given file is nonexistent
*/
void cantFind() {
	printf("The system cannot find the file specified.\n");
}

/*
* Function: askOverwrite
* -------------------
* prints a statement confirming whether the user would want to overwrite a file
*/
void askOverwrite(char *directory) {
	char full[_MAX_PATH];
	_fullpath(full, directory, _MAX_PATH);
	printf("Overwrite %s? (Yes/No/All): ", full);
}

/*
* Function: fileType
* ------------------
* determines whether the given path is a path to a dir or a file
*
* path: string to a file or dir
*
* returns: 0 if the path is to a dir
*		   1 if the path is to a file
*/
int fileType(char *path) {
	struct stat statbuf;
	stat(path, &statbuf);
	if(S_ISDIR(statbuf.st_mode)){
	    return 0;
	}
	else{
	    return 1;
	}
}

/*
* Function: timeformat
* --------------------
* checks whether the given string is written in correct time format
*
* string: the string
*
* returns: 0 if it's a wrong format
*		   the specified time in time_t format as defined by time.h
*/
int timeformat(char *string) {
	char **tokens = tokenize(string, ":");
	struct tm breakdown = {0};

	if(tokens[0] && isdigit(*tokens[0])) {
		breakdown.tm_hour = *tokens[0];
		if(tokens[1]) {
			if(isdigit(*tokens[1])) {
				breakdown.tm_min = *tokens[1];
				if(tokens[2]) {
					if(isdigit(*tokens[2])) {
						breakdown.tm_sec = *tokens[2];
					} else {
						return 0;
					}
				}
			} else {
				return 0;	
			}
		}
	} else {
		return 0;
	}

	return mktime(&breakdown);
}

/*
* Function: dateformat
* --------------------
* checks whether the given string is written in correct date format
*
* string: the string
*
* returns: 0 if it's a wrong format
*		   the specified date in time_t format as defined by time.h
*/
int dateformat(char *string) {
	char **tokens = tokenize(string, "-/");
	struct tm breakdown = {0};

	if(tokens[0] && isdigit(*tokens[0])) {
		breakdown.tm_mon = *tokens[0];
		if(tokens[1]) {
			if(isdigit(*tokens[1])) {
				breakdown.tm_yday = *tokens[1];
				if(tokens[2]) {
					if(isdigit(*tokens[2])) {
						breakdown.tm_year = *tokens[2];
					} else {
						return 0;
					}
				}
			} else {
				return 0;	
			}
		}
	} else {
		return 0;
	}

	return mktime(&breakdown);
}

/*
* Function: handlecd
* ------------------
* handles a command that starts with a 'cd' or 'chdir' keyword
* 
* returns: 1 after executing the cd command
*/
int handlecd(char **tokens) {
	int noOfArgs = countArgs(tokens);

	if(noOfArgs == 0) {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf(cwd);
		printf("\n");
	} else if(noOfArgs > 1) {
		cantFind();
	} else {
		if(chdir(tokens[1]) == -1) {
			cantFind();
		}
	}

	return 1;
}

/*
* Function: handlecls
* -------------------
* clears the screen when the user types 'cls'
*
* hConsole: a handle to the specified standard device
*
* returns: 1 after executing the cd command
*/
int handlecls(HANDLE hConsole) {
	
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize; 

	if( !GetConsoleScreenBufferInfo( hConsole, &csbi )) {
	  return 1;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, 
		coordScreen, &cCharsWritten )) {
	  return 1;
	}

	if( !GetConsoleScreenBufferInfo( hConsole, &csbi )) {
	  return 1;
	}

	if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, 
		coordScreen, &cCharsWritten )) {
	  return 1;
	}
	SetConsoleCursorPosition( hConsole, coordScreen );

	return 1;
}

/*
* Function: handlecmd
* -------------------
* displays the information of the shell
*
* returns: 1 after executing the cd command
*/
int handlecmd() {

	DWORD dwVersion = 0; 
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0; 
    DWORD dwBuild = 0;

    dwVersion = GetVersion();
 
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwVersion < 0x80000000)              
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Microsoft Windows [Version %d.%d.%d]\n",    		
                dwMajorVersion,
                dwMinorVersion,
                dwBuild);
	printf("(c) 2017 Microsoft Corporation. All rights reserved.\n");

	return 1;
}

/*
* Function: handledel
* -------------------
* deletes the given file
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handledel(char **tokens) {
	int i = 1;
	char cwd[1024];

	while(tokens[i] != NULL) {
		if(remove(tokens[i]) == -1) {
			getcwd(cwd, sizeof(cwd));
			if(i == 1) {
				printf("Could Not Find %s\\%s\n", cwd, tokens[i]);
			}
		} 
		i++;
	}	

	return 1;
}

/*
* Function: handlecopy
* --------------------
* copies files
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handlecopy(char **tokens) {
	int noOfArgs = countArgs(tokens);
	int filesCopied = 0;
	FILE *fp1;
	FILE *fp2;
	char **filetokens;
	char *filename;
	int fileidx;
	char c;
	char *line;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char *dir = malloc(100);

	if(noOfArgs == 0 ) {
		incorrect();
		return 1;
	} else if(noOfArgs == 1) {
		printf("The file cannot be copied onto itself.\n");
		return 1;
	} else if (noOfArgs > 2) {
		for(int i = 1; i < noOfArgs; i++) {
			if(fileType(tokens[i]) == 0) {
				incorrect();
			} else if(fileType(tokens[i]) == 1) {
				if((fp1 = fopen(tokens[i], "r")) == NULL) {
					cantFind();
				} else {
					if(fileType(tokens[noOfArgs]) == 0) {
						filetokens = tokenize(tokens[i], "\\");
						fileidx = countArgs(filetokens);
						filename = filetokens[fileidx];
						strcpy(dir, tokens[noOfArgs]);
						strcat(dir, "\\");
						strcat(dir, filename);
						fp2 = fopen(dir, "r");
						if(fp2 == NULL) {
							fp2 = fopen(dir, "w");
							while((c = getc(fp1)) != EOF) {
								if(fputc(c, fp2) == -1) {
									filesCopied--;
									break;
								}
							}
							filesCopied++;
						} else {
							while(1) {
								askOverwrite(dir);
								chdir(cwd);
								line = lowerStr(readLine());
								if(strstr(line, "yes") != NULL ||
								   strstr(line, "y")  != NULL ||
								   strstr(line, "all") != NULL||
								   strstr(line, "a") != NULL) {
									remove(tokens[i]);
									fp2 = fopen(dir, "w");
									while((c = getc(fp1)) != EOF) {
										fputc(c, fp2);
									}
									filesCopied++;
									break;
								} else if(strstr(line, "no") != NULL ||
										  strstr(line, "n") != NULL) {
									break;
								} else {
									continue;
								}
							}	
						}
						free(dir);
						fclose(fp1);
						fclose(fp2);
					} else if(fileType(tokens[noOfArgs]) == 1) {
						incorrect();
					} else {
						incorrect();
					}
				}
					
			} else {
				incorrect();
				return 1;
			}	
		}
		free(dir);
		fclose(fp1);
		fclose(fp2);
		printf("\t%d file(s) copied.", filesCopied);
	} else {
		if((fp1= fopen(tokens[1], "r")) == NULL) {
			cantFind();
			return 1;
		} else {
			fileType(tokens[2]);
			fp2 = fopen(tokens[2], "r");
			if(fp2 == NULL) {
				if(fileType(tokens[2]) == 0) {
					strcat(tokens[2], "\\");
					strcat(tokens[2], tokens[1]);
					handlecopy(tokens);
				} else if(fileType(tokens[2]) == 1){
					fp2 = fopen(tokens[2], "w");
					while((c = getc(fp1)) != EOF) {
						if(fputc(c, fp2) == -1) {
							filesCopied--;
							break;
						}
					}
					filesCopied++;
					printf("\t%d file(s) copied.", filesCopied);
				} else {
					printf("alien ???? ???? ??? \n");
					printf("\t%d file(s) copied.", filesCopied);
				}
			} else {
				while(1) {
					askOverwrite(tokens[2]);
					chdir(cwd);
					line = lowerStr(readLine());
					if(strstr(line, "yes") != NULL ||
					   strstr(line, "y")  != NULL ||
					   strstr(line, "all") != NULL||
					   strstr(line, "a") != NULL) {
						remove(tokens[2]);
						fp2 = fopen(tokens[2], "w");
						while((c = getc(fp1)) != EOF) {
							fputc(c, fp2);
						}
						filesCopied++;
						break;
					} else if(strstr(line, "no") != NULL ||
							  strstr(line, "n") != NULL) {
						break;
					} else {
						continue;
					}
				}	
				printf("\t%d file(s) copied.", filesCopied);	
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 1;
}

/*
* Function: handledate
* --------------------
* displays the date
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handledate(char **tokens) {
	int noOfArgs = countArgs(tokens);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *line;
	if(noOfArgs == 0) {
		printf("The current date is: %d-%d-%d\n"
			, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
		printf("Enter the new date: (mm-dd-yy)");
		line = readLine();
		if(dateformat(line)) {
			printf("A required privilege is not held by the client.\n");
		} else {
			printf("The system cannot accept the date entered.\n");
		}
	} else {
		if(noOfArgs > 1) {
			printf("The system cannot accept the date entered.\n");
		} else {
			if(dateformat(tokens[1])) {
				printf("A required privilege is not held by the client.\n");
			} else {
				printf("The system cannot accept the date entered.\n");
			}
		}
	}
	return 1;
}

/*
* Function: handledir
* -------------------
* displays contents and details about the directory
*
* dp: a pointer to the directory
* opendis: the name of the directory to be opened
*
* returns: 1 after executing the cd command
*/
void handledir(DIR *dp, char *opendis) {
    struct dirent *dir;
    dp = opendir(opendis);
    struct stat attr;
    char time[20];
    char size[10];
    char *newsize;
    int length;
    int commas;
    struct tm * timeinfo;
    int files = 0;
    int dirs = 0;
    if (dp) {
    	printf("\n");
        while ((dir = readdir(dp)) != NULL) {
            stat(dir->d_name, &attr);
            timeinfo = localtime (&attr.st_mtime);
            strftime(time, sizeof(time), "%m/%d/%Y %I:%M %p", timeinfo);
            printf("%20s", time);
            if (S_ISDIR(attr.st_mode)) {
                printf("%7s", "<DIR>");
                printf("%15s", " ");
                dirs++;
            }
            else {
                printf("%10s", " ");
                itoa((unsigned)attr.st_size, size, 10);
                length = getStrSize2(size);
                if(length % 3 == 0) {
                	commas = (length / 3) - 1;
                } else {
                	int mod = length % 3;
                	commas = (length - mod) / 3;
                }
                length = commas + getStrSize2(size);

                newsize = (char *)malloc(length);

                for(int i = getStrSize2(size)-1; i >= 0; i--) {
                	if(((getStrSize2(size)-i)%3 == 0) && i != 0) {
                		newsize[i + commas] = size[i];
                		newsize[i + --commas] = ',';
                	} else {
                		newsize[i + commas] = size[i];
                	}
                }
                newsize[length] = '\0';

                printf("%10s", newsize);
                printf("%2s", " ");
                files++;
            }
            printf("%s\n", dir->d_name);
        }
        closedir(dp);
    }
    else {
    	printf("File not found.\n");
    }
}

/*
* Function: handlemkdir
* ---------------------
* creates a new directory
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handlemkdir(char **tokens) {
	int noOfArgs = countArgs(tokens);

	if(noOfArgs < 1) {
		incorrect();
	} else {
		for(int i = 1; i <= noOfArgs; i++) {
			if(mkdir(tokens[i]) == -1) {
				printf("A subdirectory or file %s already exists.\n", tokens[i]);
				printf("Error occurred while processing: %s.\n", tokens[i]);
			}
		}
	}

	return 1;
}

/*
* Function: handlemove
* --------------------
* moves files
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handlemove(char **tokens) {
	int noOfArgs = countArgs(tokens);
	FILE *fp1;
	FILE *fp2;
	char **filetokens;
	char *filename;
	int fileidx;
	int filesCopied = 0;
	char c;
	char *line;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	int strsize;
	char *dir = malloc(100);
	char *ptr;

	if(noOfArgs < 2) {
		incorrect();
	} else {
		for(int i = 1; i < noOfArgs; i++) {
			if(fileType(tokens[i]) == 0) {
				incorrect();
			} else {
				if(fileType(tokens[noOfArgs]) == 0) {
					filetokens = tokenize(tokens[i], "\\");
					fileidx = countArgs(filetokens);
					filename = filetokens[fileidx];
					strcpy(dir, tokens[noOfArgs]);
					strcat(dir, "\\");
					strcat(dir, filename);
					if((fp1 = fopen(tokens[i], "r")) == NULL) {
						cantFind();
					} else {
						fp2 = fopen(dir, "r");
						if(fp2 == NULL) {
							fp2 = fopen(dir, "w");
							while((c = getc(fp1)) != EOF) {
								if(fputc(c, fp2) == -1) {
									filesCopied--;
									break;
								}
							}
							filesCopied++;
							fclose(fp1);
							fclose(fp2);
							remove(tokens[i]);
						} else {
							while(1) {
								askOverwrite(dir);
								chdir(cwd);
								line = lowerStr(readLine());
								if(strstr(line, "yes") != NULL ||
								   strstr(line, "y")  != NULL ||
								   strstr(line, "all") != NULL||
								   strstr(line, "a") != NULL) {
									remove(tokens[i]);
									fp2 = fopen(dir, "w");
									while((c = getc(fp1)) != EOF) {
										fputc(c, fp2);
									}
									filesCopied++;
									fclose(fp1);
									fclose(fp2);
									remove(tokens[i]);
									break;
								} else if(strstr(line, "no") != NULL ||
										  strstr(line, "n") != NULL) {
									break;
								} else {
									continue;
								}
							}	
							
						}
					}
				} else {
					incorrect();
				}
			}
			free(dir);
		}
	}
	printf("\t%d file(s) moved.", filesCopied);
	return 1;
}

/*
* Function: handlerename
* ----------------------
* renames files
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handlerename(char **tokens) {
	
	int noOfArgs = countArgs(tokens);
	int filesCopied = 0;
	FILE *fp1;
	FILE *fp2;
	char c;
	char *line;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	if(noOfArgs != 2) {
		incorrect();
	} else {
		fp1 = fopen(tokens[1], "r");
		if(fp1 == NULL) {
			cantFind();
		} else {
			fileType(tokens[2]);
			fp2 = fopen(tokens[2], "r");
			if(fp2 == NULL) {
				if(fileType(tokens[2]) == 0) {
					printf("A duplicate file name exists, or the file\n cannot be found.\n");
				} else if(fileType(tokens[2]) == 1){
					if(countSlash(tokens[2]) > 0) {
						incorrect();
						return 1;
					}
					fp2 = fopen(tokens[2], "w");
					while((c = getc(fp1)) != EOF) {
						if(fputc(c, fp2) == -1) {
							filesCopied--;
							incorrect();
							return 1;
						}
					}
					filesCopied++;
					fclose(fp1);
					fclose(fp2);
					remove(tokens[1]);
				} else {
					printf("alien ???? ???? ??? \n");
				}
			} else {
				printf("A duplicate file name exists, or the file\n cannot be found.\n");
			}
		}
	}
	fclose(fp1);
	fclose(fp2);

	return 1;
}

/*
* Function: handlermdir
* ---------------------
* removes directory
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handlermdir(char **tokens) {
	int i = 1;

	while(tokens[i] != NULL) {
		if(rmdir(tokens[i]) == -1) {
			cantFind();
		} 
		i++;
	}	

	return 1;
}

/*
* Function: handletime
* --------------------
* displays the time
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handletime(char **tokens) {
	int noOfArgs = countArgs(tokens);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *line;
	char **tokens2;

	if(noOfArgs == 0) {
		printf("The current time is: %d:%d:%d\n", 
			tm.tm_hour, tm.tm_min, tm.tm_sec);
		printf("Enter the new time: ");
		line = readLine();
		if(timeformat(line)) {
			printf("A required privilege is not held by the client.\n");
		} else {
			printf("The system cannot accept the time entered.\n");
		}
	} else {
		if(noOfArgs > 1) {
			printf("The system cannot accept the time entered.\n");
		} else {
			if(timeformat(tokens[1])) {
				printf("A required privilege is not held by the client.\n");
			} else {
				printf("The system cannot accept the time entered.\n");
			}
		}
	}

	return 1;
}

/*
* Function: handletype
* --------------------
* displays the contents of a file
*
* tokens: an array of strings
*
* returns: 1 after executing the cd command
*/
int handletype(char **tokens) {
	int noOfArgs = countArgs(tokens);
	int i = 1;
	int c;

	FILE *fp;

	while(tokens[i] != NULL) {
		fp = fopen(tokens[i], "r");
		if(fp == NULL) {
			cantFind();
			if(noOfArgs > 1) {
				printf("Error occurred while processing: %s\n", tokens[i]);
			}
		} else {
			printf("\n%s\n\n\n", tokens[i]);
			while((c = getc(fp)) != EOF) {
				printf("%c", c);
			}
		}
		i++;
	}
	fclose(fp);
	return 1;
}

/*
* Function: executeCommand
* ------------------------
* handles commands and calls the appropriate functions for each command
*
* tokens: an array of strings
*
* returns: 1 to continue handling commands
*		   0 to end process
*/
int executeCommand(char **tokens) {
	char *cmd = lowerStr(tokens[0]);

	if(strcmp("cd", cmd) == 0 || strcmp("chdir", cmd) == 0) {
		return handlecd(tokens);
	} else if(strcmp("cd..", cmd) == 0) {
		tokens[0] = "cd";
		tokens[1] = "..";
		return handlecd(tokens);
	}else if(strcmp("cls", cmd) == 0) {
		HANDLE hStdout;
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		return handlecls(hStdout);
	} else if(strcmp("cmd", cmd) == 0) {
		return handlecmd();
	} else if(strcmp("copy", cmd) == 0) {
		return handlecopy(tokens);
	} else if(strcmp("date", cmd) == 0) {
		return handledate(tokens);
	} else if(strcmp("del", cmd) == 0) {
		return handledel(tokens);
	} else if(strcmp("dir", cmd) == 0) {
		char cwd[1024];
		DIR *d;
		if(countArgs(tokens) == 0) {
			printf(getcwd(cwd, sizeof(cwd)));
			d = opendir(getcwd(cwd, sizeof(cwd)));
			handledir(d, ".");
		} else {
			for(int i = 1; i <= countArgs(tokens); i++) {
				d = opendir(tokens[i]);
				if(d == NULL) {
					printf("File not found\n");
				} else {
					printf("\nDirectory of %s\n", tokens[i]);
					handledir(d, tokens[i]);
				}
			}		
		}
		return 1;
	} else if(strcmp("mkdir", cmd) == 0) {
		return handlemkdir(tokens);
	} else if(strcmp("move", cmd) == 0) {
		return handlemove(tokens);
	} else if(strcmp("rename", cmd) == 0) {
		return handlerename(tokens);
	} else if(strcmp("rmdir", cmd) == 0) {
		return handlermdir(tokens);
	} else if(strcmp("time", cmd) == 0) {
		return handletime(tokens);
	} else if(strcmp("type", cmd) == 0) {
		return handletype(tokens);
	} else if(strcmp("exit", cmd) == 0) {
		return 0;
	} else if(strcmp("d:", cmd) == 0 || strcmp("c:", cmd) == 0)  {
		chdir(cmd);
		return 1;
	} else {
		printf("'%s' is not recognized as an internal or external command, operable program or batch file.\n", tokens[0]);
		return 1;
	}

}

int main() {
	char *line;
	char **tokens;
	char dir;
	int i = 0;
	int keepRunning = 1;

	while(keepRunning) {
		getDir();
		line = readLine();
		if(strlen(line) == 1) {
			printf("\n");
			continue;
		}
		tokens = tokenize(line, " \t\r\n\a");
		keepRunning = executeCommand(tokens);
		printf("\n");
	}

	return 0;
}