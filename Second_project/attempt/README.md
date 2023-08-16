# Game of Thrones II: House of Algorithms (got2)

King Albert I, ruling four years after his ascent to the Throne of Swords, faces a myriad of political plots against him. With the goal to regain control over the nobles and maintain balance in his kingdom, he organizes a banquet. Utilizing his network of informants, the King is aware of the existing alliances amongst the nobles. To ensure order during the banquet, he devises a strategy to reorganize these alliances, dictating where each noble sits.

## Objective:
The main objective of this project is to determine the minimum number of manipulations required to maintain harmony during the banquet. These manipulations involve:
1. Establishing new alliances.
2. Dissolving existing ones.

## How It Works:
1. The solution reads the existing alliances from the `input` directory.
2. It then processes the graph, determining which edges (alliances) to add or remove to ensure every noble sits with only their allies.
3. The outputs, which represent the new and dissolved alliances, are written to the `output.txt` file.

## Usage:
1. Place input files in the `input` directory.
2. Run the `compare.sh` script to compile and compare the outputs of the `got2` and `got2Against` programs.
3. Alternatively, use the `execute.sh` script to compile and run the `got2` program separately.

## Repository Contents:

1. **compare.sh**: A Bash script that compiles the `got2` and `got2Against` C++ programs and compares their outputs for files in the `input` directory.
2. **execute.sh**: A Bash script designed to compile and run the `got2` program.
3. **got2.cpp**: The main program containing the logic to manage and restructure the alliances.
4. **got2.h**: A helper header containing utility functions related to timing and handling timeouts.
5. **got2Against.cpp**: An alternative approach or baseline solution to the same problem.
6. **grader.c** and **grader.cpp**: Implementations that handle grading functionalities, ensuring programs don't exceed specified runtime constraints.
