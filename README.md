# Tic tac toe

Small project with educational purposes.

## Building

- Run `generate_sln.bat` on a terminal
- Open `build\TicTacToe.sln` with Visual Studio
- Build and run the project

## Adding your AI implementation

- Create corresponding header and source file implementations `Player<YourName>.<h/cpp>` under `app\source`.
- Regenerate the project running `generate_sln.bat`.
- Inherit from `IPlayer` and override the appropriate methods.
- Create an instance of your player class in `PlayerListing.cpp` and add it to the supplied competition in `setupPlayers` function.
- Build and run the simulation.
