MiREPL
====

## Descriton
A simple REPL made in C++ that runs a simple stack based programming language.

## Syntax Guide
### IO
```
MiRepl> "Hello World!" .out
Hello World!
MiRepl> 10 "%i" .outf
10
```
### Methods
```
MiRepl> %name *your code* end
```
---
### Variables
#### Local
```
MiRepl> v$
MiRepl> 10 =v
MiRepl> v "%i" .outf
10
```
#### Global
```
MiRepl> $v$
MiRepl> 10 =$v
MiRepl> $v "%i" .outf
10
```
---
