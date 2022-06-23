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
4. Download wxWidgets (https://www.wxwidgets.org/downloads/ , select Windows Installer)
5. Run the downloaded installer to install the source files to $wx.
6. Open Visual Studio Community 2019
7. Select Open a project or solution
8. Select the file cpp\RecursiveTicTacToe\RecursiveTicTacToe.sln in this repository.
9. Navigate to Tools/Command Line and select Developer Command Prompt
10. Navigate to $wx\build\msw in the opened terminal
11. Run nmake /f makefile.vc
12. Run nmake /f makefile.vc BUILD=release
13. Close the terminal
14. Open the solution's Property Manager
15. Select RecursiveTicTacToe and click Add Existing Property Sheet
16. Navigate to $wx and select wxwidgets.props

You are now ready to build this repository!


Linux:

1. [Coming soon]