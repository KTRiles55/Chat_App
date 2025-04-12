# Chat_App
### Project Date: 04-12-2025
### Group members: Hector Velez, Kenneth Riles
## Contributions:
- Kenny: Helped Hector install and set up WSL2 for establishing a testing environment for our code on Ubuntu. Created source code files for the server, client and main (chat.c). Also set up the GitHub repository to save and keep track of our changes to the files.

- Hector:  My contribution to this project was to create the rest of the c files and/or .h files so we can have a working product at the end of the day! Lastly, it was a great experience to create a project like this and I could not have ask for a better teammate. 

## Prerequisites:
1. Install WSL2 to connect to a Linux distribution on your operating system by clicking on this link and following the steps (Note: This is exclusively for Windows users):

https://learn.microsoft.com/en-us/windows/wsl/install

2. Once you activated WSL2 on your shell, install and run the Linux Distribution of your choice (ex: Ubuntu, Fedora, Kali Linux, etc..)
3. Install VsCode on your system or virtual machine by clicking on link below.

https://code.visualstudio.com/download

4. Once you finished installing VsCode, launch it and type in the search bar on top, ">wsl" and click "Connect to WSL using Distro".

![Screenshot (325)](https://github.com/user-attachments/assets/2b2f51f5-314e-4c8a-a35e-5fad227ab65f)

5. Then, click on the distribution of your choice to connect to the IDE.

![Screenshot (326)](https://github.com/user-attachments/assets/4b2b7047-d884-4dfc-b197-4f847e610acc)

6. Once you are connected to the environment, navigate to "Explorer" and click "Open Folder" to open the project folder.

![Screenshot (327)](https://github.com/user-attachments/assets/06bc4771-7e09-4536-b170-a7ea15eefd9c)

7. Now you can execute the project.

## Steps:
1. To execute the chat application program, first, make sure that the project folder is opened.
2. Open the terminal by clicking "View" on the top bar and click "Terminal".
3. Make sure that the terminal's current path is within the project directory.
4. Compile the source code files with the command (chat.c is the main program file):

- gcc chat.c connection_manager.c Command_handler.c Server.c Client.c -o \<name of executable\> -Wall -pthread

\* Replace \<name of executable\> with whatever name you want for the executable

5. Finally, execute program by entering the command:

- ./\<name of executable\> \<port number\>

\* Replace \<port number\>  with whatever port number that you want the server to listen on
