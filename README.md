# Activity 10: Examining Assembly Language Code
## Put your name(s) here

In this activity, you will see your first example of compiling C source code into assembly. You will:
- Compile a C program so that you can view the generated assembly code
- Learn to use the `objdump` terminal command to rebuild assembly code from executable
- Modify the C program to demonstrate assembly code for different kinds of data
- Learn new Makefile tools

## Provided Code

This has a simple library code file that has a swap function defined in it, with
a more complex Makefile used to generate assembly code automatically:
- `Makefile`
    - a makefile to automate the compilation process
- `swap.c`
    - a library file that only contains a `swap` function
- `swap_main.c`
    - a simple program that tests the `swap` function
 
## VS Code Users

There is an extension in the marketplace for viewing assembly files. Go to the marketplace by choosing it along the 
left side (below the bug icon), and search for **assembly**. Then pick the popular one for x86 and x86_64.

## Your Tasks

Before starting the tasks, examine the contents of `swap.c` and `swap_main.c`. 
Make sure that the C code makes sense to you, and everything happening is 
familiar. If necessary, review how pointers work!


### Task 1: Compiling and Generating Assembly in the Terminal

You are first going to look at two methods for generating assembly code by trying the compiler commands in the terminal, so
that when you look at the Makefile, it will make more sense.

**Method one:** The first approach asks the compiler to produce assembly code rather than an executable. 
We want to simplify the assembly code, so we will use compiler flags (optional arguments) to make that happen

New compiler options (also called flags):
- The `-Og` flag (dash capital O, lowercase g): tells the compiler to use the lowest level of optimization; this will make the assembly code easier to read
- The `-S` flag (dash S): tells the compiler to generate assembly code in a .s file; if the code file is `swap.c` then the new file will be `swap.s`

Try the following command in the terminal:

    gcc -Og -S swap.c

Examine the contents of `swap.s`. The main assembly language instructions for the function are between the `.LFB0` and
the `.LFE0` labels. We will come back to reading and understanding this assembly code in a later section.

_The Makefile for this repository can generate assembly in this way for any code file we like._

**Method two:** The second approach, described in your textbook, is to generate an executable and then use the
`objdump` utility to _disassemble_ the executable and reproduce the assembly code that would have created the
executable. This gives us more information, including mapping assembly to the actual machine code. But we must have
a complete program that can generate a functional executable in order to use this method. That is what `swap_main.c` is for.

Try the following two commands in the terminal:

    gcc -Og -g swap.c swap_main.c -o swap
    objdump -d swap > swap.d

New compiler options:
- The `-g` flag (dash lowercase g): tells the compiler to include information useful for debugging (and also perhaps for `objdump`)

Examine the contents of `swap.d`. There is a lot there, but you can find the portions that relate to the `swap` and `main` functions
if you scroll down far enough. We'll get into the details of this later on, as well.

Run the executable as well, to see that the `swap` function works as it should.

_The Makefile for this repository can generate assembly in this way for any code file we like._

### Task 2: Understanding the Makefile

Open up the Makefile and read through it. It looks more complicated than our previous Makefiles. We'll break it down:

**Definitions:** At the top of the Makefile are a series of definitions that map variables to specific strings: for 
example, `CC` is defined to be `gcc`. Later in the Makefile, the tag `$(CC)`  is replaced by `gcc` before the 
compilation begins for a target. All variables are substituted similarly before the command is run. By convention 
these variables are all capital letters. Note careflly where each variable is used (and where it is not used!).

New compiler options:
- The `-Wall` option is commonly used to activate all compiler warnings, so that 
messages are generated at compile-time
- The `-m64` option ensures that the compiler targets a 64-bit machine (leaving this out may work just fine)

**Pattern-matching rules:** Makefiles allow us to specify pattern-matching rules, that apply to any files that
match the given pattern. Pattern rules look just like regular rules, except they contain the special symbol %
that is a placeholder for any string. A typical pattern might look like this:
	%.a : %.b

This means that if any file ending in `.b` has changed, use this rule to generate a target file with the same base name,
but ending in  `.a`.

In the body of the rule, we use a set of special codes to refer to the target filename, the prerequisite files, and the 
root name:
- `$@` refers to the target name, `%.a` (with % replaced by the actual root name)
- `$<` refers to the first prerequisite name `%.b` (with % replaced by the actual root name)
- `$*` refers to the root name

In our Makefile, we have two pattern rules: one for generating assembly code with the compiler from a C code file:

    %.s: %.c

and one that generates assembly code by first creating an executable, and then disassembling it to build a full
assembly program:

    %.d: %.c


The first rule will trigger any time a `.c` file is modified, **if** the `.s` file is a prerequisite somewhere. The
rule uses the -S compiler option to force the generation of a `.s` file, using the `.c` fileas the source

The second rule will trigger any time a `.c` file is modified, **if** the `.d` file is listed as a prerequisite somewhere.
The rule  does three things: it compiles the program, it runs `objdump` to 
generate the disassembled assembly file, and it removes the executable 
(you could remove that line if you want to be able to run the executable).

**Limitations:** As it is, this won't work for our code example, because we need 
to give two `.c` files to the compiler. But we'll fix that later.

**The `files` target:** This target makes the specified assembly code files by 
triggering other targets above it. Use this as your main `make` target. Right 
now, it only seeks to make `swap.s`, but we'll 
modify it later to make other files.

**The `clean` target:** We've seen the `clean` target before; in this case it 
removes all backup files, all .s files, all .d files, and any .64d files that 
might be created along the way.


### Task 3: Reading Assembly

Make sure that you have run `make files` to generate `swap.s`.

The assembly code for `swap.s` corresponds to `swap.c`. Look at the section of the file `swap.s` right after these lines:

    swap:

    .LFB0:

and before the occurrence of this line:

    .LFE0:

This code is the set of 64-bit assembly instructions corresponding to the function called `swap` in the `swap.c` code.

Have at hand:
- Readings and/or slides/class notes about x86 registers and how they are used.
   - Look at conventions on registers used to hold arguments to a function
   - Look at conventions on registers that hold return values
- Readings and/or slides/class notes about the syntax for memory references in x86 assembly

**Do these:**
- Copy the contents of `swap.s` to a new file you create, `commented_swap.txt`.
- Edit the new file, and add a comment to each line between `LFB0` nd `LFE0`. Explain in detail what each line is doing.
   - Comments in the assembly code can be written like this: `/* a comment */`
- In the Makefile, add `swap_main.s` to the prerequisites for the `files` target. Repeat the process 
above of copying the contents of `swap_main.s` and adding comments 
to the main lines. Can you make some sense of what is happening there?


### Task 4: Comparing assembly for other number types

Now we will look at how the assembly differs when the type of data is a different size.

- Copy `swap.c` to a new file, `swap_int.c`  
- Edit the code to change `long` to `int` 
- Add `swap_int.s` to the `files` target and make it
- Look at the difference between `swap.s` and `swap_int.s`
- Discuss with your teammates/neighbors what is different, and why
- If you have time, experiment with other data sizes

### Task 5: Makefile generating .d files

As it is, the Makefile can generate a .d file, but only when (1) it is given a complete program including a `main` function,
and (2) when all the code for that program is contained in a single .c file. We had not set up our code
that way, because we wanted the **Method one** assembly to be as simple as possible.

- Modify the existing code files to make the Makefile work
    - Copy the definition of `swap` into the `swap_main.c` code file
- Add `swap_main.d` to the `files` target
- Call `make files` and look at the .d file that is generated as a result: just look at the parts labeled `<swap>` and `<main>`. Compare them to the .s files from earlier sections:
    - How do the instructions in the `swap` function differ between the two methods we've used to generate them?
    - Why might they differ?

- Modify the `main` function to call the function from `swap_int.c` instead; make sure it works


## References


- Assembly language resources (x86, 64-bit)
    - [x86 Assembly/X86 Instructions](http://en.wikibooks.org/wiki/X86_Assembly/X86_Instructions)
    - [X86 Opcode and Instruction Reference](http://ref.x86asm.net/coder64.html)
    - [x86 assembly language](http://en.wikipedia.org/wiki/X86_assembly_language)
    - [x86 instruction listings](http://en.wikipedia.org/wiki/X86_instruction_listings)
- gcc compiler flag references
    - [Compiler Option Summary](https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
    - [Index of gcc compiler options](https://gcc.gnu.org/onlinedocs/gcc/Option-Index.html)
- `objdump` resources
    - [Overview of the `objdump` command](https://www.thegeekstuff.com/2012/09/objdump-examples/)
- Makefile guides
  - [An Introduction to Makefiles](https://www.gnu.org/software/make/manual/html_node/Introduction.html), by GNU
  - [Makefile Tutorials and Examples to Build From](https://earthly.dev/blog/make-tutorial/), by Aniket Bhattacharyea
  - [makefile basics - anthony explains](https://www.youtube.com/watch?v=20GC9mYoFGs)
  - [Defining and Redefining Pattern Rules](https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html)
- Printf formatting codes
  - [printf format specifier reference from cplusplus.com](http://www.cplusplus.com/reference/cstdio/printf/).
  [_Format Specifiers in C_](https://www.thecrazyprogrammer.com/2016/10/format-specifiers-c.html) by The Crazy Programmer
