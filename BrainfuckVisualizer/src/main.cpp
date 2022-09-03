#include <iostream>
#include <fstream>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"


std::string LoadFile(const char* path)
{
    static const std::string sg_ValidChars = "-+<>[],.";
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open file [" << path << "]\n";
        return std::string();
    }

    std::string content;
    content.reserve(static_cast<size_t>(ifs.tellg()));
    for (std::istreambuf_iterator<char> it = ifs; it != std::istreambuf_iterator<char>(); ++it)
    {
        if (sg_ValidChars.find(*it) != std::string::npos)
            content += *it;
    }
    return content;
}


int main()
{
    std::string sCode = LoadFile("hello.txt");
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Rectangle rect = { 20, 60, 600, 25 };
        //GuiButton(rect, "Hello World");
        GuiTextBox(rect, const_cast<char*>(sCode.c_str()), sCode.size(), false);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}