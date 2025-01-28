# IDE

Since this is a micro-controller project it completely makes sense to use PlatformIO within Visiual Studio Code (aka VSCode). 
The Arduino IDE is a possibile environment, but it is now really just for beginners and it doesn't offer the capabilities of a real IDE.

Simply go to https://platformIO.org, download VSCode, run it, and then load PlatformIO as an add on.

## MacOS issues
On my particular x86 MacBook Pro I kept encountering an error where file <iostream> was not found. Installing latest version of XCode (where MacOs maintains C++ compiler) did not help. The fix was to create a .vscode/tasks.json configuration file that specified an args of:
```
"-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1",
```
so that the compiler could find the include files. To determine the proper directory name I searched the entire file system for "iostream" using the command:
```
$ sudo find / -name iostream 2>&1 | grep -v "not permitted" | grep -v "No such file or directory"
```
