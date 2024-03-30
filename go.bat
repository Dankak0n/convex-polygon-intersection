@echo off
@REM echo Start!
echo Compiling...
g++ -c src/point.cpp -o point.o -I"include"
g++ -c src/polygon.cpp -o polygon.o -I"include"
g++ -c src/triangle.cpp -o triangle.o -I"include"
g++ -DSFML_STATIC -c src/logic.cpp -o logic.o -I"include" -I"SFML-2.6.0\include"
g++ -DSFML_STATIC -c run.cpp -o run.o -I"SFML-2.6.0\include" -I"include" 
echo Linking...
g++ logic.o polygon.o triangle.o point.o run.o -o .build/run.exe -L"SFML-2.6.0\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lwinmm -lopengl32 -lfreetype -lgdi32
del point.o
del polygon.o
del triangle.o
del logic.o
del run.o
@REM -mwindows
echo Compiled!
cd ".build"
run.exe
@REM echo Finished!