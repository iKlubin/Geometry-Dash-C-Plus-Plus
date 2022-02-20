#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <conio.h>

#define JUMP ' '

using namespace std;

void SetWindow(int Width, int Height)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;
    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

class Cube
{
private:
    int x, y;
public:
    Cube()
    {
        x = 20;
        y = 20;
    }
    void moveC(int m)
    {
        y += m;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};

class Spikes
{
private:
    int x, y;
public:
    Spikes()
    {
        x = 74;
        y = 20;
    }
    void moveS()
    {
        --x;
    }
    void setX()
    {
        x = 74;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};

void screenPrint(wchar_t* screen, int w, Cube c, Spikes s)
{
    for (int i = 0; i < 75; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            screen[i + j * w] = ' ';
        }
    }
    wstring gameName = { L"╔══════════════════════════╗║>><<  GEOMETRY  DASH  >><<║╚══════════════════════════╝" };
    int j = 1;
    int i = 24;
    for (int n = 0; n < 84; n++, i++)
    {
        if (n == 28 or n == 56)
        {
            j++;
            i = 24;
        }
        screen[i + j * w] = gameName[n];
    }
    for (int x = 0; x < 75; x++)
    {
        screen[x + 20 * w] = L'─';
    }
    for (int i = c.getX() - 3; i < c.getX() + 1; i++)
    {
        for (int j = c.getY() - 2; j < c.getY() + 1; j++)
        {
            screen[i + j * w] = '#';
        }
    }
    for (int i = s.getX() - 2; i < s.getX() + 1; i++)
    {
        for (int j = 20 - 1; j < 20 + 1; j++)
        {
            if (!((i == s.getX() - 2 and j == 20 - 1) or (i == s.getX() and j == 20 - 1)))
            {
                screen[i + j * w] = L'█';
            }
        }
    }
}

bool collusionCube(Cube c, Spikes s)
{
    for (int i = c.getX() - 3; i < c.getX() + 3; i++)
    {
        for (int j = c.getY() - 2; j < c.getY() + 2; j++)
        {
            if (i == s.getX() and j == s.getY())
            {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    int w = 75;
    int h = 25;
    SetWindow(h, w);
    wchar_t* screen = new wchar_t[h * w];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    Cube c;
    Spikes s;
    bool play = true;
    bool jump = false;
    int n = 8;
    int key = 0;
    int x = 0, t = 0;
    while (play)
    {
        if (_kbhit())
        {
            key = _getch();
            if (key == JUMP and c.getY() == 20)
            {
                jump = true;
                n = 15;
            }
        }
        if (jump)
        {
            if (n > 9)
            {
                c.moveC(-1);
                n--;
            }
            else if (n > 7)
            {
                n--;
            }
            else if (n > 5)
            {
                c.moveC(-1);
                n--;
            }
            else if (n > 3)
            {
                n--;
            }
            else if (n > -5)
            {
                c.moveC(1);
                n--;
            }
            else if (n == 1)
            {
                n = 15;
                jump = false;
            }
        }
        if (collusionCube(c, s))
        {
            play = false;
        }
        s.moveS();
        if (x % 75 == 0)
        {
            s.setX();
            if (x % 150 == 0)
            {
                t++;
            }
        }
        screenPrint(screen, w, c, s);
        WriteConsoleOutputCharacter(hConsole, screen, h * w, { 0, 0 }, &dwBytesWritten);
        Sleep(20 - t);
        x++;
    }
    wstring gameScore = { L"╔══════════════════════════╗║>><<     SCORE:       >><<║╚══════════════════════════╝" };
    int j = 10;
    int i = 24;
    for (int n = 0; n < 84; n++, i++)
    {
        if (n == 28 or n == 56)
        {
            j++;
            i = 24;
        }
        screen[i + j * w] = gameScore[n];
    }
    string strS = to_string((int)(x / 75));
    i = 42;
    for (int n = 0; n < size(strS); n++, i++)
    {
        screen[i + 11 * w] = strS[n];
    }
    WriteConsoleOutputCharacter(hConsole, screen, h * w, { 0, 0 }, &dwBytesWritten);
    Sleep(1000);
    return 0;
}
