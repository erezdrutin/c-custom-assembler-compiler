# Maman 14 - Custom assembly conversion to custom machine code

### Basic Requirements
A supporting system, the project was written using CLion on MacOS Monterey and tested on Ubuntu 16.04.

The files provided when executing the code must be in the same location as the project is, and must end with the extension '.as'.
When providing the files, you **SHOULD NOT** include the '.as' ending, as the assembler assumes that the provided files ends with that extension.

### Quick Start
1. Clone the project.
2. Either compile it via the supplied makefile on your gcc / run it using another provider (such as CLion, Visual Studio, etc.).
3. Execute it using the following command:
```bash
./maman14 <file1> <file2> ...
```

#### Notes
* I added the CMakeLists.txt file for your convenience if you prefer to run the code using CLion rather than using gcc.
* Being an executable, unless there are any errors the code won't print anything. Check the FS to validate that the new ".ob", ".ent", ".ext" & ".am" files were created successfully.

#### Purpose
Showcase the skills learnt during the course C lab in the Open University.

#### Tests
* Tested on MacOS Monterey, Ubuntu 16.04.
* Tested with multiple src files.
* Tested with "broken" files / files that contains blacklisted words.
