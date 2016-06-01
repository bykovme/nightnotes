## Night Notes (QT, C++)

You are welcome to the simpliest and fastest markdown notes manager in the world :)   
It is free and it will give you even more freedom than you can imagine 

----- 

Main important features:
- notes are stored as usual text or markdown files, they can be opened and edited by any other text editor, you are not limited with Night Notes, you still have free choice how to work with your information, no strings attached
- you can automatically store your info in your favorite cloud service if you select appropriate folder (for example DropBox/Night Notes)
- if you already have text notes in files, it is very easy to start working with night notes app

----

The app uses the C library discount to translate the text from markdown format, some useful features added on top of this library

1. Feature 1: copy to clipboard
If you write the text in the following format
```
[Secure password](copy:Password123)
```
The app will copy the password "Password123" into system clipboard

2. Feature 2: other md files
if the text in the link ends with ".md" the app will try to open the file from the same folder or, if the file does not exist, it will propose to create such file. Use this feature to link to other notes. Sample is below
```
[link to another md file](another file.md)
```
if you click the link above, it will try to open the file "another file.md" in the same folder

-----

I'm using the app only on Mac but it can be easily compiled on Windows or Linux, as it uses QT, if you adapt the app and compile it for Win or Linux, create a merge request to me with appropriate configuration in .pro file.

You can install the app on Mac from here: [binaries here](https://gitlab.com/bykovme/nightnotes/tree/master/Downloads/Mac)

It is my first (and probably last :) ) experieance with QT & C++, so, there are surely many things to improve in the project, feel free to do this and send a merge request to me.

Thanks and enjoy.


