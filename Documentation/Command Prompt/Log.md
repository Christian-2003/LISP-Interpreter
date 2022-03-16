# Logs: ![](https://img.shields.io/github/size/Christian-2003/LISP-Interpreter/Documentation/Command%20Prompt/Logs.md?label=File%20size)
> This Lisp Tutorial has been written for version _1.8_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [Enable / disable logger](#enabledisable)
2. [How to read the logs](#read)

<br/>
<br/>
<br/>

***

## Enable / disable logger: <a name="enabledisable"></a>

The logger can be enabled and disabled in order to save performance when executing resource-dependant source code. By default, the logger is disabled.

### Enabeling logger:

The [`LOG`](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Command%20Prompt/Commands.md#log) command can be used to enable the logger when passing the argument `true`.

The terminal shows the path and filename in which the respective log can be found.

Afterwards, the logger will monitor everything (including e.g. terminal commands) until it's disabled or the interpreter is closed utilizing the [`EXIT`](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Command%20Prompt/Commands.md#exit) command.

### Disabeling logger:

The [`LOG`](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Command%20Prompt/Commands.md#log) command can be used to disable the logger when passing the argument `false`.

The terminal shows the path and filename in which the respective log can be found.

Alternatively, the logger will automatically disable when the interpreter is closed utilizing the [`EXIT`](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Command%20Prompt/Commands.md#exit) command.

<br/>

***

## How to read the logs: <a name="read"></a>

Logs will something like this:

```logtalk
[16.03.2022 16:47:26] [Terminal/INFO] Begin session log...
[16.03.2022 16:47:33] [Terminal/INFO] Executing command "execute" with arguments: "code.lsp".
[16.03.2022 16:47:33] [Tokenizer/INFO] Begin lexical analysis (Tokenizer).
[16.03.2022 16:47:33] [Tokenizer/INFO] End lexical analysis (Tokenizer) successfully.
[16.03.2022 16:47:33] [Tokenizer/INFO] Begin syntactical analysis (Parser).
[16.03.2022 16:47:33] [Tokenizer/INFO] End syntactical analysis (Parser) successfully.
[16.03.2022 16:47:33] [Tokenizer/INFO] Begin code execution (Interpreter).
[16.03.2022 16:47:33] [Interpreter/FATAL ERROR] LISP main-function has too many parameters.
[16.03.2022 16:47:35] [Terminal/INFO] Executing command "exit" without arguments.
[16.03.2022 16:47:35] [Terminal/INFO] Exit terminal.
[16.03.2022 16:47:35] [Terminal/INFO] End session log.
```

The content between the first squared parenthesis (e.g. `[16.03.2022 16:47:26]`), shows the respective **date** and **time** at which the respective event was logged.

The content between the second squared parenthesis (e.g. `[Terminal/INFO]`), shows the part of the interpreter in which the logged event occured and the type of the event. You may see the available values below:

### Origin of events:

Type | Origin
---  | ---
_Terminal_ | The logged event originates within the command prompt of the interpreter.
_File System_ | The logged event originates within the interpreter's file management system.
_Tokenizer_ | The logged event originates withing the lexical analysis (tokenizer).
_Parser_ | The logged event originates within the syntactical analysis (parser).
_Interpreter_ | The logged event originates during the code execution (interpreter).

### Type of events:

Type | Event
--- | ---
_INFO_ | The logged event resembles any information. This can be ignored in most cases.
_WARNING_ | The logged event resembles a warning.
_ERROR_ | The logged event resembles any error that does not occur during the execution of LISP source code.
_FATAL ERROR_ | The logged event resembles any error that occurs during the execution of LISP source code.
