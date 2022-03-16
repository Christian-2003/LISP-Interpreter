# LISP Interpreter ![Code size](https://img.shields.io/github/languages/code-size/Christian-2003/LISP-Interpreter) ![Version](https://img.shields.io/github/manifest-json/v/Christian-2003/LISP-Interpreter?color=green) ![License](https://img.shields.io/badge/License-MIT-red.svg)

![](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Resources/Preview.png?raw=true)

This LISP interpreter is an improved version of my old interpreter, which can be found [here](https://github.com/Christian-2003/LispInterpreter).

Due to poorly implemented algorithms, I have decided to rebuild the interpreter in a better way, this time utilizing the mechanics and speed of the programming language C++.

<br/>

## Table of contents:
1. [Description](#description)
2. [System requirements](#requirements)
3. [Usage](#usage)
4. [First steps](#first-steps)
5. [Coding](#coding)
6. [How to contribute](#contribution)
7. [License](#license)

<br/>
<br/>
<br/>

***

## Description: <a name="description"></a>
This repository resembles an interpreter for a custom dialect of the programming language Lisp.

An interpreter is a program which is capable of evaluating and executing sourcecode. Unlike a compiler, the interpreter does not create any executable file, so the source code must be interpreted every time one wants to execute it.

Unlike you might thing, Lisp is no programming language, but rather a collection of multiple programming languages, which all have similar paradigms which makes them all feel more alike. The Lisp dialect, which this interpreter can evaluate is inspired by the much more common _STELLA_ dialect, but still different in many ways (and of course with a much smaller functional scope). For further information on my Lisp dialect, visit the [Lisp documentation](https://github.com/Christian-2003/LISP-Interpreter/tree/main/Documentation/Lisp) of this repository.

<br/>

***

### System requirements: <a name="requirements"></a>

In order for the interpreter to run on your system, you need to have at least **Windows 10** installed.

Due to changes with the [Windows Console and Terminal Ecosystem](https://docs.microsoft.com/en-us/windows/console/ecosystem-roadmap), I do not recommend to run the interpreter with the newly introduced terminal within Windows 11. Instead, I would recommend to use the old Windows 10 terminal when executing the interpreter under Windows 11. The interpreter itself will still work correctly with the new terminal, however there are some irritating occurrences with the displayment within the new terminal.

<br/>

***

## Usage: <a name="usage"></a>

### Download the executable:
In order to use the interpreter, you do not need to install any program.

You can download the newest release of the interpreter [here](https://github.com/Christian-2003/LISP-Interpreter/releases). The downloaded file resembles the developer command prompt, which you can execute. After that, skip the next paragraph and continue with your [first steps](#first-steps).

<br/>

### Download and compile the sourcecode:
If you prefer to download the sourcecode from the [code](https://github.com/Christian-2003/LISP-Interpreter)-page, feel free to do so. This interpreter is written in the programming language C++ and needs to be compiled with a respective compiler. The source code of this repository has only been compiled using Microsoft's MSVC compiler and may not be compilable with other C/C++-Compilers.

After successfully compiling the sourcecode you can continue with your [first steps](#first-steps).

<br/>

***

## First steps: <a name="first-steps"></a>
After executing the interpreter, you are welcomed with a command promt. The visual depition of the command prompt may change in different versions, but the principle of executing sourcecode should stay the same.

<br/>

### Write the sourcecode:
The first thing you need to do is to write the source code which you want to execute with the interpreter. In the following, we will write a simple _Hello World_-program in Lisp. Copy the following source code into a file, which is located in the same directory as the previously downloaded executable and name said file "sampleCode.lsp".
```Lisp
;File: sampleCode.lsp
(void main () (
    (println "Hello World")
))
```

<br/>

### Start the developer command prompt:
After successfully saving your sourcecode to the directory, start the command prompt. You should see a terminal that looks something like this:

![](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Resources/First-steps1.png?raw=true)

<br/>

### Execute the sourcecode:
After that, type `execute sampleCode.lsp` into the command prompt. If you have saved the file with the sourcecode to the same directory as the executable, the sourcecode should be executed by now and the string "Hello World" should be shown in the terminal.

![](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Resources/First-steps2.png?raw=true)

If you have saved the sourcecode to another directory, you need to provide the filepath to the interpreter. In order not to provide the filepath with every execution, you may change the path by using the `cd` command.

<br/>

### Available commands:
The command promt is capable of executing a small number of comamnds. You can type `help` in order to get further information on available commands.

<br/>

***

## Coding: <a name="coding"></a>
After you finished with your [first steps](#first-steps), you may deal with other elements of this LISP dialect.

### Elements of the LISP dialect:
This LISP dialect comes with a variety of [keywords](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Keywords.md), [control flow statements](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Control%20structures.md) and [data types](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Data%20types.md). It is even possible to implement [functions](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Functions.md) with this programming language.

For further information on arithmetic-, relational- or boolean operations, look up [this](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Operations.md) document.

### Exception handling:
Incorrectly implemented LISP source code might cause the interpreter to throw an exception. Exceptions usually display the _file_, _line_ and _token_ which caused the error. In addition to that, an error message is displayed. The following source code generates an exception:
```Lisp
;File: sampleCode.lsp
(void main (
    (int myVar "Hello World")
    ;Cannot store String in integer-variable
))
```
The corresponding exception:

![](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Resources/Exemplary-error-message.png?raw=true)

As you can see, the filename, line number and token which caused the error are mentioned. The error message `Cannot convert string to integer` implies that the String "Hello World" cannot be converted and stored in a variable of type integer.

If the provided error message does not help you to solve the error, you may look up the documentation of every possible error message [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Error%20messages.md).

<br/>

***

## How to contribute: <a name="contribution"></a>
If you want to contribute to the project, feel free to open a pull request.

If you find any bugs or errors while playing with the interpreter, please create an [issue](https://github.com/Christian-2003/LISP-Interpreter/issues).

<br/>

***

## License: <a name="license"></a>
The interpreter is licensed under the **MIT License**. See the full license [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/LICENSE.txt).
