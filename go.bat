@echo off
@REM echo Start!
echo Compiling...
g++ -c src/point.cpp -o point.o -I"include"
g++ -c src/polygon.cpp -o polygon.o -I"include"
g++ -c src/triangle.cpp -o triangle.o -I"include"
g++ -c src/logic.cpp -o logic.o -I"include" -I"SFML-2.6.0\include"
g++ -c run.cpp -o run.o -I"SFML-2.6.0\include" -I"include"
echo Linking...
g++ logic.o polygon.o triangle.o point.o run.o -o run.exe -L"SFML-2.6.0\lib" -lsfml-graphics -lsfml-window -lsfml-system
@REM -mwindows
echo Compiled!
run.exe
@REM echo Finished!