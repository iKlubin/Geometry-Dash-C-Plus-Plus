#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <string>

#define PI 3.14159f

using namespace std;

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
    wstring gameName;
    gameName += L"╔══════════════════════════╗";
    gameName += L"║>><<  GEOMETRY  DASH  >><<║";
    gameName += L"╚══════════════════════════╝";
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

bool collisionCube(Cube c, Spikes s)
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
    int w = 75;
    int h = 25;
    wchar_t* screen = new wchar_t[w * h];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    Cube c;
    Spikes s;
    bool play = true;
    bool jump = false;
    int n = 8;
    int x = 0, t = 0;
    int timeS = 20;

    while (play)
    {
        if (timeS > 0)
        {
            timeS = 20 - t;
        }
        if (GetAsyncKeyState((unsigned short)' ') and c.getY() == 20)
        {
            jump = true;
            n = 15;
        }
        if (jump)
        {
            if (n > 8)
            {
                c.moveC(-1);
                n--;
            }
            else if (n > 7)
            {
                n--;
            }
            else if (n == 6)
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
        if (collisionCube(c, s))
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
        Sleep(timeS);
        x++;
    }
    wstring gameScore;
    gameScore += L"╔══════════════════════════╗";
    gameScore += L"║>><<     SCORE:       >><<║";
    gameScore += L"╚══════════════════════════╝";
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
    Sleep(1500);
    return 0;
}
