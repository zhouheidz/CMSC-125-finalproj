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
copy text1.txt text2.txt
```
```
copy text1.txt folder\subfolder
```
```
copy folder\text1.txt subfolder\text1copy.txt
```

### DATE
Displays or sets the date. The user cannot set the date due to administrator rights.

###### Syntax
```
date [date]
```
###### Example
```
date 12/07/2017
```

### DEL
Deletes one or more files

###### Syntax
```
del [file1][...]
```
###### Example
```
del text1.txt folder\text1.txt
```

### DIR
Displays a list of files and subdirectories in a directory

###### Syntax
```
dir [path]
```
###### Example
```
dir folder\subfolder
```

### MKDIR
Creates a directory

###### Syntax
```
mkdir [dirname]
```
###### Example
```
mkdir newfolder
```

### MOVE
Moves one or more files from one directory to another directory

```
move [source][destination]
```
where source is the file to be moved,
destination may be a directory or a new filename for the file to be moved
```
move [source1][source2][...] [destination]
```
where source1, source2, and etc. are multiple files that will be moved,
destination is strictly a directory
###### Example
```
move text1.txt text2.txt
```
```
move text1.txt folder\subfolder
```
```
move folder\text1.txt subfolder\text1copy.txt
```

### RENAME
Renames a file

###### Syntax
```
rename [oldname][newname]
```
###### Example
```
rename text1.txt newtext1.txt
```

### RMDIR
Removes a directory

###### Syntax
```
rmdir [dirname]
```
###### Example
```
rmdir folder
```

### TIME
Displays or sets the system time. The user cannot set the time due to administrator rights.

###### Syntax
```
time [time]
```
###### Example
```
time 11:11:11
```

### TYPE
Displays the contents of a text file

###### Syntax
```
type [filename]
```
###### Example
```
type text1.txt
```

<br><br>

## Additional Commands

### EXIT
Closes the shell

###### Syntax
```
exit
```
