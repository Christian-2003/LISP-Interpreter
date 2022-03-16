# Commands: ![](https://img.shields.io/github/size/Christian-2003/LISP-Interpreter/Documentation/Command%20Prompt/Commands.md?label=File%20size)
> The Lisp Tutorials have been written for version _alpha-0.4_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [HELP](#help)
2. [EXIT](#exit)
3. [EXECUTE](#execute)
4. [CLS](#cls)
5. [CD](#cd)
6. [LOG](#log)

<br/>
<br/>
<br/>

***

## HELP-Command: <a name="help"></a>

The `HELP` command displays a list of commands, which can be used with the command prompt.

### Syntax:
`HELP [command]`

### Arguments:
_command **(optional)**_
<br/>
The name of a command, allows the display of specific information about the passed command.

<br/>

***

## EXIT-Command: <a name="exit"></a>

The `EXIT` command exits the command prompt.

### Syntax:
`EXIT`

<br/>

***

## EXECUTE-Command: <a name="execute"></a>

The `EXECUTE` command is used to execute sourcecode which is stored in a file.

### Syntax:
`EXECUTE [file] [debugMode]`

### Arguments:
_file_
<br/>
The name of the file which contains the sourececode. If the file is not stored in the same directory as the executed command prompt, the path must be given as well.

_debugMode **(optional)**_
<br/>
Indicates wether additional debug-information should be shown while executing the soureceocde. Enter `1` to show said information or `0` in order not to show these information.

<br/>

***

### CLS-Command: <a name="cls"></a>

The `CLS` command is used to clear the content of the terminal.

### Syntax:
`CLS`

<br/>

***

## CD-Command: <a name="cd"></a>

The `CD` command is used to change the filepath of the directory, the program searches for a file which is passed when using the `EXECUTE` command.

### Syntax:
`CD [path]`

### Arguments:
_path_
<br/>
Path of a directory in which you want the interpreter to search for the file you pass when using `EXECUTE`.

<br/>

***

## LOG-Command: <a name="log"></a>

The `log` command is used to display whether the interpreter is logging the current session to a log. If the optional argument is passed, the logging can be enabled or disabled. By default, this is deactivated, so that you have to enter this command before the interpreter begins logging the session.

### Syntax:
`LOG [log]`

### Arguments:
_log **(optional)**_
<br/>
Indicates whether the logging should be enabled or disabled.
