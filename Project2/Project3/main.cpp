#include <windows.h>
#include <commdlg.h> 

LPCSTR szWindowClass = "TrapecioApp";
LPCSTR szTitle = "Aplicación Trapecio";
HINSTANCE hInst;

HBITMAP hBitmap = NULL;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadAndDisplayImage(HWND hwnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    HWND hwnd;
    MSG msg;

    hInst = hInstance;

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szWindowClass;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    hwnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, "Error al crear la ventana.", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (hBitmap) {
            HDC hMemDC = CreateCompatibleDC(hdc);
            SelectObject(hMemDC, hBitmap);

            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);

            int x = ps.rcPaint.right - bitmap.bmWidth - 10;
            int y = (ps.rcPaint.bottom - bitmap.bmHeight) / 2;

            BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
            DeleteDC(hMemDC);
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            LoadAndDisplayImage(hwnd);
        }
        break;

    case WM_CREATE: {
        HMENU hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, 1, "Abrir BMP");
        SetMenu(hwnd, hMenu);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void LoadAndDisplayImage(HWND hwnd) {
    OPENFILENAME ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Bitmap Files\0*.BMP\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        hBitmap = (HBITMAP)LoadImage(NULL, szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!hBitmap) {
            MessageBox(hwnd, "Error al cargar el archivo BMP.", "Error", MB_ICONERROR);
        }
        else {
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }
}
