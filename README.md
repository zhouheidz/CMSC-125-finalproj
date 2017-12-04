# ChiuShell
This file is written in Markup language

A simulation of a Windows command line interpreter written in C.

Final project in CMSC 125, Operating Systems

<br><br>

## Files

### ChiuShell.c
The source code, written in C. It contains all implementations of selected Windows command line interpreter commands

### ChiuShell.exe
The executable file, emulating selected processes and behaviors of a Windows command line interpreter

<br><br>

## Compilation

Type in the following text in the shell. 
```
gcc ChiuShell.c -o ChiuShell
```

<br><br>

## Required Commands

### CD
Changes the current directory

###### Syntax
```
CD [..]
```
```
CD [drive:][path]
```
###### Example
```
cd D:/College_stuff/cmsc 125/cmsc-125-finalproj 
```

### CHDIR
Changes the current directory

###### Syntax
```
CHDIR [..]
```
```
CHDIR [drive:][path]
```
###### Example
```
chdir D:/College_stuff/cmsc 125/cmsc-125-finalproj 
```

### CLS
Clears the screen

###### Syntax
```
cls
```

### CMD
Starts a new instance of the command interpreter

###### Syntax
```
cmd
```

### COPY
Copies one or more files to another location

###### Syntax
```
copy [source][destination]
```
where source is the file to be copied,
destination may be a directory or a new filename for the file to be copied
```
copy [source1][source2][...] [destination]
```
where source1, source2, and etc. are multiple files that will be copied,
destination is strictly a directory
###### Example
```

```

### DATE
Displays or sets the date

###### Syntax
```

```
###### Example
```

```

### DEL
Deletes one or more files

###### Syntax
```

```
###### Example
```

```

### DIR
Displays a list of files and subdirectories in a directory

###### Syntax
```

```
###### Example
```

```

### MKDIR
Creates a directory

###### Syntax
```

```
###### Example
```

```

### MOVE
Moves one or more files from one directory to another directory

###### Syntax
```

```
###### Example
```

```

### RENAME
Renames a file or files

###### Syntax
```

```
###### Example
```

```

### RMDIR
Removes a directory

###### Syntax
```

```
###### Example
```

```

### TIME
Displays or sets the system time

###### Syntax
```

```
###### Example
```

```

### TYPE
Displays the contents of a text file

###### Syntax
```

```
###### Example
```

```

<br><br>

## Additional Commands

### EXIT
Closes the shell

###### Syntax
```

```
###### Example
```

```
