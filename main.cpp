#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

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

int Round(double x)
{
    return (int)(x + 0.5);
}

void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc-a, color);

    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc-b, yc-a, color);

    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-b, yc+a, color);

    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc+b, yc+a, color);

}

void DrawCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=R,y=0;
    double theta = 0 ,dtheta=1.0/R;
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x>y){
        theta+=dtheta;
        x = Round(R*cos(theta));
        y = Round(R*sin(theta));
        Draw8Points(hdc,xc,yc,round(x),round(y),color);
    }
}

void DrawQuarter(HDC hdc, int xc, int yc,int R,int xf,int yf)
{
    int x1=R,y1=0;
    double theta = 0 ,dtheta=1.0/R;
    while(x1>y1){
        theta+=dtheta;
        x1 = Round(R*cos(theta));
        y1 = Round(R*sin(theta));
        if(xc<xf && yc>yf){
            SetPixel(hdc, xc+x1, yc-y1, RGB(255,0,0));
            SetPixel(hdc, xc+y1, yc-x1, RGB(255,0,0));
        }else if(xc>xf && yc>yf){
            SetPixel(hdc, xc-x1, yc-y1, RGB(255,0,0));
            SetPixel(hdc, xc-y1, yc-x1, RGB(255,0,0));
        }else if(xc>xf && yc<yf){
            SetPixel(hdc, xc-x1, yc+y1, RGB(255,0,0));
            SetPixel(hdc, xc-y1, yc+x1, RGB(255,0,0));
        }else if(xc<xf && yf>yc){
            SetPixel(hdc, xc+x1, yc+y1, RGB(255,0,0));
            SetPixel(hdc, xc+y1, yc+x1, RGB(255,0,0));
        }
    }
}

void fillWithCircles(HDC hdc, int xc, int yc, int x1, int y1,int xf, int yf) {
	int dx = x1 - xc;
	int dy = y1 - yc;
	for (double t = 0; t < 1; t += 0.01) {
		int x = xc + (dx * t);
		int y = yc + (dy * t);
		int r = sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
		DrawQuarter(hdc, xc, yc, r,xf,yf);
	}
}

/*  This function is called by the Windows function DispatchMessage()  */

int xcenter,ycenter,x2,y2,r,clickCounter=0,x,y;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)                  /* handle the messages */
    {
        HDC hdc;
        case WM_LBUTTONDOWN:
            hdc=GetDC(hwnd);
            if(clickCounter==0){
                xcenter=LOWORD(lParam);//x of center point of circle
                ycenter=HIWORD(lParam);//y of center point of circle
                clickCounter++;
            }
            else if(clickCounter==1){
                x2=LOWORD(lParam);// x of point on circle
                y2=HIWORD(lParam);// y of point on circle
                r=sqrt(pow(x2-xcenter, 2 ) + pow(y2-ycenter, 2 )); // radius
                DrawCircle(hdc,xcenter,ycenter,r,RGB(0,0,0));
                clickCounter++;
            }else if(clickCounter==2){
                x=LOWORD(lParam);
                y=HIWORD(lParam);
                if(pow(x-xcenter,2)+pow(y-ycenter,2)< pow(r,2)){//check if point is inside the circle
                    fillWithCircles(hdc,xcenter,ycenter,x2,y2,x,y);
                }
                clickCounter=0;
            }
            ReleaseDC(hwnd,hdc);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
