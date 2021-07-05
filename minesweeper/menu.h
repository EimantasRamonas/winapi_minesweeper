#define IDR_MYMENU 10001
#define ID_FILE_EXIT 10002
#define ID_RESTART 1000
#define ID_IN_GAME_SETTINGS 1001
#define ID_EXIT_TO_MAIN_MENU 1002
#define ID_STATS_BUTTON 1003
#define ID_SETTINGS_BUTTON 1004
#define PLAY_BUTTON 10005
#define MYDIALOG 20001
#define DIALOG_BUTTON 20002
#define TEXT_FIELD 20003
#define IDR_RADIO1 20004
#define IDR_RADIO2 20005
#define IDR_RADIO3 20006
#define IDR_RADIO4 20007
#define IDR_RADIO5 20008
#define SETTINGS_RADIO1 20009
#define SETTINGS_RADIO2 20010
#define SETTINGS_RADIO3 20011
#define DIALOG_BUTTON2 20012
#define SETTINGSDIALOG 20013
#define ID_ICON 20014

///Board square structure
struct Square
{
    bool containsMine = false;
    bool isRevealed = false;
    bool isMarked = false;
    bool isClicked = false;
    int neighborMines = 0;
};

int difficultyLevel = 1; // by default difficulty is set to easy

Square playingBoard[30][30]; // here we save all information about the game

int xLength = 10; // amount of squares in x axis
int yLength = 10; // amount of squares in y axis
int bombCount = 10;

const int xLengthEasy = 10;
const int yLengthEasy = 10;
const int bombCountEasy = 10;

const int xLengthMedium = 16;
const int yLengthMedium = 16;
const int bombCountMedium = 40;

const int xLengthHard = 30;
const int yLengthHard = 16;
const int bombCountHard = 99;

///Game variables
int flagCount;
bool isGameStarted = false;
int unclickedTiles;
int winCount, lossCount;
HINSTANCE hinstLib;

///Color variables for changing background
COLORREF getBackgroundColor(int code);
int getBackGroundCode(COLORREF colorref);
COLORREF bkColorRef = 0x1E1E9C;
const COLORREF rgbRed = 0x1E1E9C;
const COLORREF rgbBlue = 0xCC6600;
const COLORREF rgbGreen = 0x006600;
const COLORREF rgbOrange = 0x0066CC;
const COLORREF rgbPink = 0xB266FF;

///Variables for WM_MOUSEMOVE
LPARAM xMouseCoord;
LPARAM yMouseCoord;

///Functions
void displayGameWindow(HWND hWnd);
void createMainMenu(HWND hwnd);
//void centerWindow (HWND hwnd);
void createGame(HWND hwnd);
void resetGame();
void buttonClickBitmapChange (HWND hwnd, int x, int y);
void buttonUnclickBitmapChange (HWND hwnd);
void drawBitmap(HWND hwnd, int x, int y, int type, HDC hDC);
BOOL CALLBACK DestoryChildCallback(HWND hwnd,LPARAM lParam);

///Functions related to the game
void generateRandomCoords(int xLength, int yLength, int *x, int *y);
void drawBoard(HWND hwnd, HDC hDC);
void drawRevealedBoard(HWND hwnd, HDC hDC);
void drawSquare(HWND hwnd, int x, int y, int type, HDC hdc);
void bitmapDrawHelper(HDC hdc, HANDLE image, int x, int y);
void onSquareClick(HWND hwnd, int x, int y);
void revealSquare(int x, int y, HWND hwnd);

///Bitmap handles
HANDLE hBmp1;
HANDLE hBmp2;
HANDLE hBmp3;
HANDLE hBmp4;
HANDLE hBmp5;
HANDLE hBmp6;
HANDLE hBmp7;
HANDLE hBmp8;
HANDLE hBmpUnknown;
HANDLE hBmpEmpty;
HANDLE hBmpFlag;
HANDLE hBmpClicked;
HANDLE hBmpBomb;





