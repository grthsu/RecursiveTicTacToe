This repository introduces the world's first example of Recursive Tic Tac Toe (to my knowledge).

The game of Tic Tac Toe is known to be algorithmically broken.  However, the game of Ultimate Tic Tac Toe,
where one player's actions affect the other player, is not known to be broken (at least to me).  With this in mind,
I introduce Recursive Tic Tac Toe, which takes the idea of using a tic tac toe board of tic tac toe boards to its
logical conclusion: infinite recursion.

Warnings:
This repository is not able to use more than 4 recursions, because Windows does not actually have enough handles
after that point.
This repository does not save the game; all progress on close will be lost.

Setup Instructions:
Similar instructions for different versions may work, but these instructions are known to work.
Other setups are certainly possible, but these instructions are the only officially supported setups.

Source Code:

C++:
Partially adapted from wxWidgets setup documentation

Windows:

1. Download Visual Studio Community 2019 (https://visualstudio.microsoft.com)
2. Install the module "Desktop development with C++" in the Visual Studio Installer
3. Clone this repository locally
4. Download wxWidgets (https://www.wxwidgets.org/downloads/ , select Download Windows Binaries)
5. Select Visual Studio 2015/2017/2019/2022
6. Download Header Files, 32-Bit (x86)/Development Files 64-Bit (x86_64)/Development Files to a known location (defined as $wx in these instructions)
7. Install 7-Zip 19.00 for Windows: https://www.7-zip.org/a/7z1900-x64.exe
8. In folder $wx, use 7-Zip to extract the files from the downloaded .7z files into $wx
9. Open Visual Studio Community 2019
10. Select Open a project or solution
11. Select the file cpp\RecursiveTicTacToe\RecursiveTicTacToe.sln in this repository.
12. [Coming soon]

Linux:

1. [Coming soon]