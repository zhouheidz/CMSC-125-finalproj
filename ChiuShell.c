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

char *readLine() {

	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);

	return line;
}

#define TOKEN_SIZE 64
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

char getDir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf(cwd);
	printf(">");
}

char *lowerStr(char *str) {
	for(int i = 0; str[i]; i++){
	  str[i] = tolower(str[i]);
	}

	return str;
}

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

int countArgs(char **tokens) {
	int noOfArgs = -1;
	char **temp = tokens;

	while(*temp != NULL) {
		noOfArgs++;
		temp++;
	}
	return noOfArgs;
}

int getStrSize(char *string) {
	return sizeof(string);
}

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

void incorrect() {
	printf("The syntax of the command is incorrect.\n");
}

void cantFind() {
	printf("The system cannot find the file specified.\n");
}

void askOverwrite(char *directory) {
	char full[_MAX_PATH];
	_fullpath(full, directory, _MAX_PATH);
	printf("Overwrite %s? (Yes/No/All): ", full);
}

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

int handlecopy(char **tokens) {
	int noOfArgs = countArgs(tokens);
	int filesCopied = 0;
	FILE *fp1;
	FILE *fp2;
	char c;
	char *line;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char *dir = malloc(100);

	if(noOfArgs == 0 ) {
		incorrect();
	} else if(noOfArgs == 1) {
		printf("The file cannot be copied onto itself.\n");
	} else if (noOfArgs > 2) {
		for(int i = 0; i < noOfArgs-1; i++) {
			if(fileType(tokens[i]) == 0) {
				incorrect();
			} else if(fileType(tokens[i]) == 1) {
				if(fileType(tokens[noOfArgs]) == 0) {
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
					printf("\t%d file(s) copied.", filesCopied);
				} else if(fileType(tokens[noOfArgs]) == 1) {
					incorrect();
				} else {
					incorrect();
				}
			} else {
				incorrect();
			}
			free(dir);
		}
	} else {
		fp1 = fopen(tokens[1], "r");
		if(fp1 == NULL) {
			cantFind();
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

void handledir(DIR *dp) {
    struct dirent *dir;
    dp = opendir(".");
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
			handledir(d);
		} else {
			for(int i = 1; i <= countArgs(tokens); i++) {
				d = opendir(tokens[i]);
				handledir(d);
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
		tokens = tokenize(line, " \t\r\n\a");
		keepRunning = executeCommand(tokens);
		printf("\n");
	}

	return 0;
}