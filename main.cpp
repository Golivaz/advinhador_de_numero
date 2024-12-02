#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>

// Variáveis globais para o número secreto e as tentativas

int numero_secreto;
int tentativas = 0;

// Função para gerar um número aleatório
void inicializarJogo()
{
    srand(static_cast<unsigned int>(time(0)));
    numero_secreto = rand() % 100 + 1;
    tentativas = 0;
}

// Função para verificar o palpite do jogador
std::string verificarPalpite(int palpite)
{
    tentativas++;
    if (palpite > numero_secreto)
    {
        return "Muito alto!";
    }
    else if (palpite < numero_secreto)
    {
        return "Muito baixo!";
    }
    else
    {
        return "Parabéns! Você adivinhou em " + std::to_string(tentativas) + " tentativas.";
    }
}

// Função de callback para manipulação dos eventos da janela
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static HWND hEdit, hButton, hOutput;
    char buffer[256];

    switch (msg)
    {
    case WM_CREATE:
        // Criar um campo de texto para entrada do palpite
        hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 20, 100, 25, hwnd, NULL, NULL, NULL);

        // Criar um botão para o jogador enviar o palpite
        hButton = CreateWindow("BUTTON", "Adivinhar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 20, 80, 25, hwnd, (HMENU)1, NULL, NULL);

        // Criar um campo de texto para mostrar a saída
        hOutput = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 20, 60, 200, 25, hwnd, NULL, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wp) == 1)
        {
            // Quando o botão é pressionado, pegamos o texto do campo de entrada
            GetWindowText(hEdit, buffer, 256);
            int palpite = atoi(buffer); // Convertemos a string para inteiro

            // Verificamos o palpite e mostramos o resultado
            std::string resultado = verificarPalpite(palpite);
            SetWindowText(hOutput, resultado.c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// Função principal do programa que cria a janela
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    inicializarJogo(); // Inicializar o jogo com um número secreto

    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "Adivinhacao";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc))
        return -1;

    // Criar a janela principal
    HWND hwnd = CreateWindow("Adivinhacao", "Jogo de Adivinhação", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 300, 200, NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
