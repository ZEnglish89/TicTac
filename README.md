# TicTac
A tictactoe game, featuring an unbeatable AI

This project was completed at the end of my third semester of college, as the final project for CSE030 Data Structures. It was completed in a group of three, using some template code provided by the professor, and took roughly three days.
Apologies for the large number of header files, because this was the final project of a Data Structures course it was designed to require as many of them as possible in some capacity by the professor.

This project features game logic that can adapt to any board size(within technical limits), both PvP and PvCPU play, as well as many flavor features(text colors, player names, etc), courtesy of Reuben Self.
The menu and framework for that game logic to sit on top of is courtesy of Victor Khatri.
The GameState struct, as well as the vast majority of the header files, were pre-existing code from elsewhere in the course, that all students had written collaboratively. They were not provided specifically for this project, they were taught
separately before the project was announced, and then incorporated into it.

My contribution to the project were the three possible AI opponents. ai_simple and ai_random were not technical challenges: The former iterates through all board spaces and plays their move in the first empty space they find, the latter appends all empty
board spaces to a list and picks a random move from that list. ai_perfect, however, uses recursion and reward-finding to look forward through all possible boardstates to find which move is the best it could make, prioritising preventing its opponent from winning.
The ai is entirely impossible to beat, at least against all opponents who have tried and all board sizes it has played on.
The ai is currently capped at a maximum number of hypothetical states it can generate, and if a boardstate requires more than that maximum number of states to fully evaluate, which happens during the opening moves of larger boardsizes, it will revert to simpler
decision-making until the board has filled enough that it's able to full evaluate it again. *This cap was put in place to prevent the program from running out of memory and killing on the hardware provided by the school.* If your hardware can handle it, feel free
to remove the cap, which is tied to the MAX_SEARCH_STATES variable. I will consider adding removal of the cap as a menu option in the future.
Currently, when the cap is in place, the ai reverts to using the ai_simple logic. This is a bug, which will be fixed shortly. It should instead be reverting to the ai_random logic.
