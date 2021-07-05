#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "menu.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <windowsx.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

///Global variables
string flagCountStr;
string unclickedtilesStr;

///WinAPI variables
HINSTANCE hinst;
HWND hwnd;   // Main window handle
HMENU toolbarMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_MYMENU));
typedef void (__cdecl *MYPROC)(HWND);
//MYPROC centerWindow;


/*  Declare Windows procedure and square buttons procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK InGameSettingsDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SettingsDialogProc(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

void centerWindow (HWND hwnd)
{
    RECT rc;
    GetWindowRect ( hwnd, &rc ) ;
    int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;
    SetWindowPos( hwnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
}

int WINAPI WinMain (HINSTANCE hinst, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{

    // HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /// The Window structure
{
    wincl.hInstance = hinst;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, MAKEINTRESOURCE(ID_ICON));
    wincl.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(ID_ICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;


    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;


}

    // HMENU toolbarMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_MYMENU));

    /// Creation of window
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Minesweeper"),       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           toolbarMenu,
           hinst,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    HICON hIcon = LoadIcon(hinst, MAKEINTRESOURCE(ID_ICON));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    createMainMenu(hwnd);




    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /// Message listener
    {
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
    }
}


/*  This function is called by the Windows function DispatchMessage()  */



BOOL CALLBACK DestroyChildCallback(HWND hwnd,LPARAM lParam) // clears child windows from window
{
  if (hwnd != NULL) {
    DestroyWindow(hwnd);
  }

  return TRUE;
}


    /// Function that starts the main menu
void createMainMenu(HWND hwnd)
{
        centerWindow(hwnd);

        string dlText, bgcText;
        string wText, lText;
        ifstream SettingsFile("settings.txt");
        ifstream StatsFile("stats.txt");

        getline (SettingsFile, dlText);
        getline (SettingsFile, bgcText);
        getline (StatsFile, wText);
        getline (StatsFile, lText);

        difficultyLevel = stoi(dlText);
        bkColorRef = getBackgroundColor(stoi(bgcText));
        winCount = stoi(wText);
        lossCount = stoi(lText);

        SettingsFile.close();
        StatsFile.close();

        InvalidateRect(hwnd, NULL, TRUE); // clears window
        EnumChildWindows(hwnd, DestroyChildCallback, NULL);



        CreateWindow("BUTTON", "Play",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        200,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) PLAY_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

        CreateWindow("BUTTON", "Stats",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        275,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) ID_STATS_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

        CreateWindow("BUTTON", "Settings",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        350,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) ID_SETTINGS_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

        CreateWindow("BUTTON", "Exit",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        425,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) ID_FILE_EXIT,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);


}

    /// Function that starts the game
void createGame(HWND hwnd)
{


    if (!isGameStarted)
    {
        EnableMenuItem(toolbarMenu, 1000, MF_ENABLED);
        EnableMenuItem(toolbarMenu, 1001, MF_ENABLED);
        EnableMenuItem(toolbarMenu, 1002, MF_ENABLED);
    }

    if (difficultyLevel == 1)
    {

        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 480, 480, SWP_NOZORDER);
        xLength = xLengthEasy;
        yLength = yLengthEasy;
        bombCount = bombCountEasy;
    }

    if (difficultyLevel == 2)
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 580, 655, SWP_NOZORDER);
        xLength = xLengthMedium;
        yLength = yLengthMedium;
        bombCount = bombCountMedium;
    }

    if (difficultyLevel == 3)
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 1080, 655, SWP_NOZORDER);
        xLength = xLengthHard;
        yLength = yLengthHard;
        bombCount = bombCountHard;
    }

    unclickedTiles = xLength * yLength;

    srand (time(NULL));
    int bombCount2 = bombCount;


    while (bombCount2 > 0) // randomly generates bomb positions for squares
    {
        int x,y;
        generateRandomCoords(xLength-1, yLength-1, &x, &y);


        if (!playingBoard[x][y].containsMine)
        {
            playingBoard[x][y].containsMine = true;
            bombCount2--;
        }

    }



    for (int i=0; i<xLength; i++) // calculates neighbor mines for each square
        for (int j=0; j<yLength; j++)
            {
                if (i-1>=0) if (playingBoard[i-1][j].containsMine) playingBoard[i][j].neighborMines++;
                if (i-1>=0 && j-1>=0) if (playingBoard[i-1][j-1].containsMine) playingBoard[i][j].neighborMines++;
                if (i-1>=0 && j+1<yLength) if (playingBoard[i-1][j+1].containsMine) playingBoard[i][j].neighborMines++;

                if (i+1<xLength) if (playingBoard[i+1][j].containsMine) playingBoard[i][j].neighborMines++;
                if (i+1<xLength && j-1>=0) if (playingBoard[i+1][j-1].containsMine) playingBoard[i][j].neighborMines++;
                if (i+1<xLength && j+1<yLength) if (playingBoard[i+1][j+1].containsMine) playingBoard[i][j].neighborMines++;

                if (j+1<yLength) if (playingBoard[i][j+1].containsMine) playingBoard[i][j].neighborMines++;
                if (j-1>=0) if (playingBoard[i][j-1].containsMine) playingBoard[i][j].neighborMines++;
            }


    flagCount = bombCount;
    flagCountStr = "Flags: " + to_string(flagCount);
    unclickedtilesStr = "Unclicked tiles: " + to_string(unclickedTiles);
    HDC hDC;
    hDC = GetDC(hwnd);

    isGameStarted = true;

    drawBoard(hwnd, hDC);
    ReleaseDC(hwnd, hDC);

    InvalidateRect(hwnd, NULL, TRUE); // clears window
    UpdateWindow(hwnd);

    // su destytoju bandem parasyti dll ir is jo paimt funkcija, bet neveike tai palikom tiesiog

    // if (centerWindow != NULL) centerWindow(hwnd);

    //        hinstLib = LoadLibrary("centerWindow.dll");
    //    if (hinstLib != NULL){
    //                    centerWindow = (MYPROC) GetProcAddress(hinstLib, "centerWindow");
    //                    if (centerWindow != NULL){
    //                        centerWindow(hwnd);
    //                    }
    //                    FreeLibrary(hinstLib);
    //    }



    InvalidateRect(hwnd, NULL, TRUE); // clears window
    UpdateWindow(hwnd);

}

    /// Draws bitmaps
    /*
        Type codes:
        1 - main menu background
    */
void drawBitmap(HWND hwnd, int x, int y, int type, HDC hDC)
{
    switch (type)
    case 1: {HANDLE hBmp1 = LoadImage(NULL, "bitmaps/minesweeperbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); bitmapDrawHelper(hDC, hBmp1, x, y); break;}

}

void bitmapDrawHelper(HDC hdc, HANDLE image, int x, int y) // helper function which puts image from handle on window
{

    HDC dcmem = CreateCompatibleDC(NULL);
    SelectObject(dcmem, image);
    BITMAP bm;
    GetObject(image, sizeof(bm), &bm);
    BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, dcmem, 0,0, SRCCOPY);
    DeleteDC(dcmem);

}

void buttonClickBitmapChange (HWND hwnd, int x, int y) // changes square bitmap when it's clicked
{
    HDC hDC;
    hDC = GetDC(hwnd);
    bitmapDrawHelper(hDC, hBmpClicked, 32+32*x, 64+32*y);
    playingBoard[x][y].isClicked = true;
    ReleaseDC(hwnd, hDC);
}

void buttonUnclickBitmapChange (HWND hwnd) // changes square bitmap when it's unclicked
{
    HDC hDC;
    hDC = GetDC(hwnd);

    for (int i=0; i<xLength; i++)
        for (int j=0; j<yLength; j++)
        {
            if (!playingBoard[i][j].isRevealed)
            {
                if (playingBoard[i][j].isMarked) bitmapDrawHelper(hDC, hBmpFlag, 32+32*i, 64+32*j);
                else bitmapDrawHelper(hDC, hBmpUnknown, 32+32*i, 64+32*j);

            }

        playingBoard[i][j].isClicked = false;

        }

    ReleaseDC(hwnd, hDC);

}



/// draws the board in the beginning of the game

void drawBoard(HWND hwnd, HDC hDC)
{
    // 10x10 board
    // square length = 32 pixels
    // first square top-left corner coordinates: x=32, y=64

    EnumChildWindows(hwnd, DestoryChildCallback, NULL);

    for (int i=0; i<xLength; i++)
        for (int j=0; j<yLength; j++)
            bitmapDrawHelper(hDC, hBmpUnknown, 32+32*i, 64+32*j);
            //drawSquare(hwnd, 32+32*i, 64+32*j, 0, hDC);



    UpdateWindow(hwnd);

}

/// redraws the board after each click

void drawRevealedBoard(HWND hwnd, HDC hDC)
{

    for (int i=0; i<xLength; i++)
            for (int j=0; j<yLength; j++)
            {
                if (playingBoard[i][j].isRevealed)
                 {
                        if(playingBoard[i][j].containsMine) bitmapDrawHelper(hDC, hBmpBomb, 32+32*i, 64+32*j);
                        else
                        {
                            if(playingBoard[i][j].neighborMines == 0) bitmapDrawHelper(hDC, hBmpEmpty, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 1) bitmapDrawHelper(hDC, hBmp1, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 2) bitmapDrawHelper(hDC, hBmp2, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 3) bitmapDrawHelper(hDC, hBmp3, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 4) bitmapDrawHelper(hDC, hBmp4, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 5) bitmapDrawHelper(hDC, hBmp5, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 6) bitmapDrawHelper(hDC, hBmp6, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 7) bitmapDrawHelper(hDC, hBmp7, 32+32*i, 64+32*j);
                            if(playingBoard[i][j].neighborMines == 8) bitmapDrawHelper(hDC, hBmp8, 32+32*i, 64+32*j);
                        }

                 }
                 else
                 {
                     if(playingBoard[i][j].isMarked) bitmapDrawHelper(hDC, hBmpFlag, 32+32*i, 64+32*j);
                     else bitmapDrawHelper(hDC, hBmpUnknown, 32+32*i, 64+32*j);
                 }

            }
    UpdateWindow(hwnd);
}

//void centerWindow (HWND hwnd)
//{
//    RECT rc;
//    GetWindowRect ( hwnd, &rc ) ;
//    int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
//    int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;
//    SetWindowPos( hwnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
//}

void generateRandomCoords(int xLength, int yLength, int *x, int *y)
{
    *x = rand() % xLength + 1;
    *y = rand() % yLength + 1;
}

BOOL CALLBACK DestoryChildCallback(
  HWND   hwnd,
  LPARAM lParam
)
{
  if (hwnd != NULL) {
    DestroyWindow(hwnd);
  }

  return TRUE;
}

/// what happens when user clicks a square
void onSquareClick(HWND hwnd, int x, int y)
{
    HDC hDC;
    hDC = GetDC(hwnd);

    if (playingBoard[x][y].containsMine)
    {
            for (int i=0; i<xLength; i++)
                for (int j=0; j<yLength; j++)
                {
                    playingBoard[i][j].isRevealed = true;
                }



         InvalidateRect(hwnd, NULL, TRUE); // clears window
         drawRevealedBoard(hwnd, hDC);
         ReleaseDC(hwnd, hDC);
         lossCount++;

         ofstream StatsFile;
         StatsFile.open("stats.txt", ios::out | ios::trunc);
         StatsFile << winCount << endl;
         StatsFile << lossCount;
         StatsFile.close();
         MessageBox(hwnd, "Susprogote", "PepeHands", MB_OK);

    }

    else if (!playingBoard[x][y].isRevealed)
    {
        revealSquare(x,y, hwnd);
        unclickedtilesStr = "Unclicked tiles: " + to_string(unclickedTiles);

        if (unclickedTiles == bombCount)
            {
            for (int i=0; i<xLength; i++)
                for (int j=0; j<yLength; j++)
                {
                    playingBoard[i][j].isRevealed = true;
                }

            drawRevealedBoard(hwnd, hDC);
            ReleaseDC(hwnd, hDC);
            winCount++;
            ofstream StatsFile;
            StatsFile.open("stats.txt", ios::out | ios::trunc);
            StatsFile << winCount << endl;
            StatsFile << lossCount;
            StatsFile.close();
            MessageBox(hwnd, "Laimejote", "widepeepoHappy", MB_OK);
            }

        else
            {
            drawRevealedBoard(hwnd, hDC);
            ReleaseDC(hwnd, hDC);
            }
    }

}

void revealSquare(int x, int y, HWND hwnd) // algorithm that recursively reveals square and adjacent empty squares
{

        playingBoard[x][y].isRevealed = true;
        unclickedTiles--;
        if (playingBoard[x][y].isMarked)
        {
            flagCount++;
            flagCountStr = "Flags: " + to_string(flagCount);
        }


        if (playingBoard[x][y].neighborMines == 0)
        {
            if (x-1>=0 && playingBoard[x-1][y].isRevealed == false) revealSquare(x-1, y, hwnd);
            if (x-1>=0 && y-1>=0 && playingBoard[x-1][y-1].isRevealed == false) revealSquare(x-1, y-1, hwnd);
            if (x-1>=0 && y+1<yLength && playingBoard[x-1][y+1].isRevealed == false) revealSquare(x-1, y+1, hwnd);

            if (x+1<xLength && playingBoard[x+1][y].isRevealed == false) revealSquare(x+1, y, hwnd);
            if (x+1<xLength && y-1>=0 && playingBoard[x+1][y-1].isRevealed == false) revealSquare(x+1, y-1, hwnd);
            if (x+1<xLength && y+1<yLength && playingBoard[x+1][y+1].isRevealed == false) revealSquare(x+1, y+1, hwnd);

            if (y+1<yLength && playingBoard[x][y+1].isRevealed == false) revealSquare(x, y+1, hwnd);
            if (y-1>=0 && playingBoard[x][y-1].isRevealed == false) revealSquare(x, y-1, hwnd);
        }
}


void resetGame() //resets the game
{

    if (difficultyLevel == 1)
    {
        xLength = xLengthEasy;
        yLength = yLengthEasy;
        bombCount = bombCountEasy;
    }
    if (difficultyLevel == 2)
    {
        xLength = xLengthMedium;
        yLength = yLengthMedium;
        bombCount = bombCountMedium;
    }
    if (difficultyLevel == 2)
    {
        xLength = xLengthHard;
        yLength = yLengthHard;
        bombCount = bombCountHard;
    }


    unclickedTiles = xLength * yLength;

    for (int i=0; i<xLength; i++)
        for (int j=0; j<yLength; j++)
            {
             playingBoard[i][j].containsMine = false;
             playingBoard[i][j].isRevealed = false;
             playingBoard[i][j].isMarked = false;
             playingBoard[i][j].isClicked = false;
             playingBoard[i][j].neighborMines = 0;
            }
}

//Red, Blue, Green, Orange, Pink
COLORREF getBackgroundColor(int code)
{
    switch (code)
    {
        case 1: return rgbRed;
        case 2: return rgbBlue;
        case 3: return rgbGreen;
        case 4: return rgbOrange;
        case 5: return rgbPink;
        default: return rgbRed;
    }
}

int getBackGroundCode(COLORREF colorref)
{
    switch (colorref)
    {
        case 0x1E1E9C: return 1;
        case 0xCC6600: return 2;
        case 0x006600: return 3;
        case 0x0066CC: return 4;
        case 0xB266FF: return 5;
        default: return 1;
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.bottom = 60;
    rect.right = 640;

    switch (message)                  /* handle the messages */
    {

        case WM_CREATE: // loads bitmaps on window creation
            {
                hBmp1 = LoadImage(NULL, "bitmaps/1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp2 = LoadImage(NULL, "bitmaps/2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp3 = LoadImage(NULL, "bitmaps/3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp4 = LoadImage(NULL, "bitmaps/4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp5 = LoadImage(NULL, "bitmaps/5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp6 = LoadImage(NULL, "bitmaps/6.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp7 = LoadImage(NULL, "bitmaps/7.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmp8 = LoadImage(NULL, "bitmaps/8.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmpBomb = LoadImage(NULL, "bitmaps/bomb.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmpEmpty = LoadImage(NULL, "bitmaps/empty.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmpUnknown = LoadImage(NULL, "bitmaps/unknown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmpFlag = LoadImage(NULL, "bitmaps/flag.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                hBmpClicked = LoadImage(NULL, "bitmaps/clickedbutton.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

//                hinstLib = LoadLibrary("centerWindow.dll");
//                if (hinstLib != NULL){
//                                centerWindow = (MYPROC) GetProcAddress(hinstLib, "centerWindow");
//                                if (centerWindow != NULL){
//                                    centerWindow(hwnd);
//                                }
//                                FreeLibrary(hinstLib);
//                }
                break;
            }


        case WM_PAINT:
            {
                if (isGameStarted)
                {
                HDC hDC;
                PAINTSTRUCT ps;
                hDC = BeginPaint(hwnd, &ps);
                drawRevealedBoard(hwnd, hDC);
                SetBkMode(hDC, TRANSPARENT);
                TextOut(hDC, 32, 10, flagCountStr.c_str(), strlen(flagCountStr.c_str()));
                TextOut(hDC, 32, 25, unclickedtilesStr.c_str(), strlen(unclickedtilesStr.c_str()));
                EndPaint(hwnd, &ps);
                }
                else
                {
                HDC hDC;
                PAINTSTRUCT ps;
                hDC = BeginPaint(hwnd, &ps);
                drawBitmap(hwnd, 0,0, 1, hDC);
                EndPaint(hwnd, &ps);
                }

                break;
            }



        case WM_COMMAND:
            {
                switch(LOWORD(wParam)){

                    case PLAY_BUTTON:
                        EnumChildWindows(hwnd, DestroyChildCallback, NULL); // lango isvalymas
                        createGame(hwnd);
                        centerWindow(hwnd);

                        break;


                    case ID_EXIT_TO_MAIN_MENU:

                        EnableMenuItem(toolbarMenu, 1000, MF_DISABLED);
                        EnableMenuItem(toolbarMenu, 1001, MF_DISABLED);
                        EnableMenuItem(toolbarMenu, 1002, MF_DISABLED);
                        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 800, 600, SWP_NOZORDER);
                        //centerWindow(hwnd);
                        resetGame();
                        isGameStarted = false;
                        createMainMenu(hwnd);
                        break;

                    case ID_RESTART:
                        resetGame();
                        createGame(hwnd);
                        break;

                    case ID_FILE_EXIT:
                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                        break;

                    case ID_IN_GAME_SETTINGS:
                        //MessageBox(hwnd, "Work in progress!", "PepeHands", MB_OK);
                        DialogBox(NULL, MAKEINTRESOURCE(MYDIALOG), hwnd, InGameSettingsDialogProc);
                        break;

                    case ID_SETTINGS_BUTTON:
                        DialogBox(NULL, MAKEINTRESOURCE(SETTINGSDIALOG), hwnd, SettingsDialogProc);
                        break;

                    case ID_STATS_BUTTON:
                        string text = "Wins: " + to_string(winCount) + "\nLosses: " + to_string(lossCount);
                        MessageBox(hwnd, text.c_str(), "W/L record", MB_OK);
                        break;

                    }
                    break;
            }


        case WM_RBUTTONUP:  // to change flag status of unclicked tile
                            // only reason i use this instead of W_RBUTTONDOWN is because when spam clicking on tile,
                            // it doesnt register everything, while this one seems to work better
            {
                if (isGameStarted)
                {

                    int xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);
                    int xLengthPixels = xLength*32+32;
                    int yLengthPixels = yLength*32+64;

                    if (32 <= xPos && xPos < xLengthPixels)
                        if (64 <= yPos && yPos < yLengthPixels)
                            {
                            int x = (xPos-32)/32;
                            int y = (yPos-64)/32;
                            HDC hDC = GetDC(hwnd);

                            if (!playingBoard[x][y].isRevealed)
                            {
                                if (!playingBoard[x][y].isMarked)
                                    {
                                    bitmapDrawHelper(hDC, hBmpFlag, 32+32*x, 64+32*y);
                                    playingBoard[x][y].isMarked = true;
                                    flagCount--;
                                    flagCountStr = "Flags: " + to_string(flagCount);
                                    InvalidateRect(hwnd, &rect, TRUE);
                                    UpdateWindow(hwnd);
                                    ReleaseDC(hwnd, hDC);
                                    }
                                else
                                    {
                                    bitmapDrawHelper(hDC, hBmpUnknown, 32+32*x, 64+32*y);
                                    playingBoard[x][y].isMarked = false;
                                    flagCount++;
                                    flagCountStr = "Flags: " + to_string(flagCount);
                                    InvalidateRect(hwnd, &rect, TRUE);
                                    UpdateWindow(hwnd);
                                    ReleaseDC(hwnd, hDC);
                                    }
                            }


                            }

                    break;


                }
                else break;

            }

        case WM_LBUTTONDOWN:// to change bitmap of button
            {
                if (isGameStarted)
                {
                    int xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);
                    int xLengthPixels = xLength*32+32;
                    int yLengthPixels = yLength*32+64;

                    if (32 <= xPos && xPos < xLengthPixels)
                        if (64 <= yPos && yPos < yLengthPixels)
                            {
                            int x = (xPos-32)/32;
                            int y = (yPos-64)/32;

                            if (!playingBoard[x][y].isRevealed)
                                buttonClickBitmapChange(hwnd, x, y);
                            }

                    break;


                }
                else break;

            }

        case WM_LBUTTONUP:  // to reveal or unclick a button
            {
                if (isGameStarted)
                {
                    int xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);
                    int xLengthPixels = xLength*32+32;
                    int yLengthPixels = yLength*32+64;

                    if (32 <= xPos && xPos < xLengthPixels && 64 <= yPos && yPos < yLengthPixels)
                        {
                        int x = (xPos-32)/32;
                        int y = (yPos-64)/32;

                        if (playingBoard[x][y].isClicked)
                            {
                                onSquareClick(hwnd, x, y);
                                InvalidateRect(hwnd, &rect, TRUE);
                                UpdateWindow(hwnd);
                            }
                        else
                            {
                                buttonUnclickBitmapChange(hwnd);
                            }
                        }
                    else buttonUnclickBitmapChange(hwnd);


                    break;
                }
                else break;

            }

//        case WM_MOUSEMOVE: // doesnt work
//                           // i wanted to use this to unclick buttons when user goes out of program bounds with mouse
//        {
//            if (isGameStarted)
//            {
//            xMouseCoord = GET_X_LPARAM(lParam);
//            yMouseCoord = GET_Y_LPARAM(lParam);
//
//            if (difficultyLevel == 1)
//            {
//                if (0 > xMouseCoord || xMouseCoord >= 480 || 0 > yMouseCoord || yMouseCoord >= 480)
//                    buttonUnclickBitmapChange(hwnd);
//            }
//            if (difficultyLevel == 2)
//            {
//                if (0 > xMouseCoord || xMouseCoord >= 580 || 0 > yMouseCoord || yMouseCoord >= 655)
//                    buttonUnclickBitmapChange(hwnd);
//            }
//            if (difficultyLevel == 3)
//            {
//                if (0 > xMouseCoord || xMouseCoord >= 1280 || 0 > yMouseCoord || yMouseCoord >= 655)
//                    buttonUnclickBitmapChange(hwnd);
//            }
//            break;
//            }
//            break;
//
//        }

        case WM_ERASEBKGND:
            {
                if (isGameStarted)
                    {
                        HDC hdc = (HDC) wParam;
                        RECT rc;
                        GetClientRect(hwnd, &rc);
                        HBRUSH brush = CreateSolidBrush(bkColorRef);
                        FillRect(hdc, &rc, brush);

                    }
                break;
            }


        case WM_DESTROY:
            {
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
            }



        default:
            {
                                    /* for messages that we don't deal with */
                return DefWindowProc (hwnd, message, wParam, lParam);
            }




    }

    return 0;
}


INT_PTR CALLBACK InGameSettingsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            {
                CreateWindow("BUTTON", TEXT("Red"),
                             BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,
                             10, 32, 100, 20,
                             hDlg, (HMENU) IDR_RADIO1, GetModuleHandle(NULL), NULL);
                CreateWindow("BUTTON", TEXT("Blue"),
                             BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                             10, 52, 100, 20,
                             hDlg, (HMENU) IDR_RADIO2, GetModuleHandle(NULL), NULL);
                CreateWindow("BUTTON", TEXT("Green"),
                             BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                             10, 72, 100, 20,
                             hDlg, (HMENU) IDR_RADIO3, GetModuleHandle(NULL), NULL);
                CreateWindow("BUTTON", TEXT("Orange"),
                             BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                             10, 92, 100, 20,
                             hDlg, (HMENU) IDR_RADIO4, GetModuleHandle(NULL), NULL);
                CreateWindow("BUTTON", TEXT("Pink"),
                             BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                             10, 112, 100, 20,
                             hDlg, (HMENU) IDR_RADIO5, GetModuleHandle(NULL), NULL);
                return TRUE;
            }
        case WM_COMMAND:
            switch(LOWORD(wParam))
                {
                    case DIALOG_BUTTON:
                        {
                            ofstream SettingsFile;
                            SettingsFile.open("settings.txt", ios::out | ios::trunc);
                            SettingsFile << difficultyLevel << endl;
                            SettingsFile << getBackGroundCode(bkColorRef);
                            SettingsFile.close();
                            EndDialog(hDlg, 0);
                            return TRUE;
                        }
                    case IDR_RADIO1:
                        {
                            HDC hDC = GetDC(hwnd);
                            bkColorRef = getBackgroundColor(1);
                            DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)hDC, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                            ReleaseDC(hwnd, hDC);
                            return TRUE;
                        }
                    case IDR_RADIO2:
                        {
                            HDC hDC = GetDC(hwnd);
                            bkColorRef = getBackgroundColor(2);
                            DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)hDC, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                            ReleaseDC(hwnd, hDC);
                            return TRUE;
                        }
                    case IDR_RADIO3:
                        {
                            HDC hDC = GetDC(hwnd);
                            bkColorRef = getBackgroundColor(3);
                            DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)hDC, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                            ReleaseDC(hwnd, hDC);
                            return TRUE;
                        }
                    case IDR_RADIO4:
                        {
                            HDC hDC = GetDC(hwnd);
                            bkColorRef = getBackgroundColor(4);
                            DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)hDC, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                            ReleaseDC(hwnd, hDC);
                            return TRUE;
                        }
                    case IDR_RADIO5:
                        {
                            HDC hDC = GetDC(hwnd);
                            bkColorRef = getBackgroundColor(5);
                            DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)hDC, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                            ReleaseDC(hwnd, hDC);
                            return TRUE;
                        }

                }
        return TRUE;

        case WM_CLOSE:
            {
                EndDialog(hDlg, 0);
                return TRUE;
            }

        case WM_DESTROY:
            {
               EndDialog(hDlg, 0);
                return TRUE;
            }

    }
    return FALSE;
}

INT_PTR CALLBACK SettingsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            {
                return TRUE;
            }
        case WM_COMMAND:
            switch(LOWORD(wParam))
                {
                    case DIALOG_BUTTON2:
                        {
                            ofstream SettingsFile;
                            SettingsFile.open("settings.txt", ios::out | ios::trunc);
                            SettingsFile << difficultyLevel << endl;
                            SettingsFile << getBackGroundCode(bkColorRef);
                            SettingsFile.close();
                            EndDialog(hDlg, 0);
                            return TRUE;
                        }
                    case SETTINGS_RADIO1:
                        {
                            difficultyLevel = 1;
                            break;
                        }
                    case SETTINGS_RADIO2:
                        {
                            difficultyLevel = 2;
                            break;
                        }
                    case SETTINGS_RADIO3:
                        {
                            difficultyLevel = 3;
                            break;
                        }

                }
        return TRUE;

        case WM_CLOSE:
            {
                EndDialog(hDlg, 0);
                return TRUE;
            }

        case WM_DESTROY:
            {
               EndDialog(hDlg, 0);
                return TRUE;
            }

    }
    return FALSE;
}

