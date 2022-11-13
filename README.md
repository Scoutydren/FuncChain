# FuncChain
function chain reaction machine

## How to run
1. In the file directory, right click on FuncChain.uproject and select "Generate visual studio project files"
2. Open the project in Visual Studio
3. Build project in Visual Studio
4. Open the project in unreal engine
5. On the bottom right corner of unreal engine, compile.
6. Basic level is currently in Minimal_Default.
7. Go to Project Settings and search for Modes.
8. Change Default Pawn Class to None.
9. Search for "Game instance".
10. Change game instance to CustomGameInstance.
11. Search for gravity and change to -490/3

## Fixes
* If the scale seems off, go to project settings -> engine -> user interface. Under DPI Scaling, set application scale to 1.0 (or whatever feels comfortable).
