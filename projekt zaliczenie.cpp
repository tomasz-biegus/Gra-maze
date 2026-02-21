#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <time.h>

#define MAX_LEVELS 5
#define MAP_SIZE 16
#define PLAYER 'P'
#define WALL '#'
#define EXIT 'E'


typedef struct {
    int x;
    int y;
    int color;
} Player;

Player player;

int currentLevel = 0;
char map[MAX_LEVELS][MAP_SIZE][MAP_SIZE];
int wallColor = 7;

char** dynamicMap;
int dynamicMapWidth, dynamicMapHeight;

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int cWidth = 190;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        cWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return cWidth;
}

void setRandomPlayerColor() {
    player.color = (rand() % 14) + 1;
}

void printCentered(const char* text) {
    int consoleWidth = getConsoleWidth();
    int textLength = strlen(text);

    if (textLength < consoleWidth) {
        int padding = (consoleWidth - textLength) / 2;
        for (int i = 0; i < padding; i++) {
            printf(" ");
        }
    }
    printf("%s\n", text);
}

void welcome() {
    printCentered("Co robisz?\n");
    Sleep(2000);
    printCentered("Jak tu trafiłeś?\n");
    Sleep(2000);
    printCentered("Wyglądasz jak...\n");
    Sleep(1000);
    printCentered("P\n");
    Sleep(2000);
    printCentered("Ale wokół są ściany! Patrz ->   #\n");
    Sleep(2000);
    printCentered("Gdzie jest wyjście?\n");
    Sleep(2000);
    printCentered("To chyba tam ->   E\n");
    Sleep(2000);
    system("cls");
    printCentered("Dasz radę się wydostać?!\n");
    Sleep(3000);
    system("cls");
    printCentered("  __  __    _     __________ \n");
    printCentered(" |  \\/  |  / \\   |__  / ____|\n");
    printCentered(" | |\\/| | / _ \\    / /|  _|  \n");
    printCentered(" | |  | |/ ___ \\  / /_| |___ \n");
    printCentered(" |_|  |_/_/   \\_\\/____|_____|");
    Sleep(5000);
}



void loadMaps() {
    FILE* file;
    errno_t err;
    err = fopen_s(&file, "plansza1.txt", "r");
    if (err != 0 || file == NULL) {
        printf("Błąd otwarcia pliku");
        exit(1);
    }

    for (int level = 0; level < MAX_LEVELS; level++) {
        for (int i = 0; i < MAP_SIZE; i++) {
            fgets(map[level][i], MAP_SIZE + 2, file);
        }
        fgetc(file);
    }
    fclose(file);
}

void loadDynamicMap() {
    FILE* file;
    errno_t err;

    err = fopen_s(&file, "plansza2.txt", "r");
    if (err != 0 || file == NULL) {
        printf("Błąd otwarcia pliku");
        exit(1);
    }

    char line[100];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Błąd wczytywania wymiarów planszy");
        exit(1);
    }
    sscanf_s(line, "%d %d", &dynamicMapHeight, &dynamicMapWidth);

    dynamicMap = (char**)malloc(dynamicMapHeight * sizeof(char*));
    for (int i = 0; i < dynamicMapHeight; i++) {
        dynamicMap[i] = (char*)malloc((dynamicMapWidth + 1) * sizeof(char));
    }

    for (int i = 0; i < dynamicMapHeight; i++) {
        if (fgets(dynamicMap[i], dynamicMapWidth + 2, file) == NULL) {
            printf("Błąd wczytywania linii planszy %d", i + 1);
            exit(1);
        }
        dynamicMap[i][strcspn(dynamicMap[i], "\n")] = '\0';
    }
    fclose(file);
}

void freeDynamicMap() {
    if (dynamicMap != NULL) {
        for (int i = 0; i < dynamicMapHeight; i++) {
            free(dynamicMap[i]);
        }
        free(dynamicMap);
        dynamicMap = NULL;
    }
}

void displayMap() {
    system("cls");
    printf("Poziom: %d\n\n", currentLevel + 1);

    int i, j;
    if (currentLevel < MAX_LEVELS) {
        for (i = 0; i < MAP_SIZE; i++) {
            for (j = 0; j < MAP_SIZE; j++) {
                if (i == player.y && j == player.x) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player.color);
                    printf("%c", PLAYER);
                }
                else if (map[currentLevel][i][j] == WALL) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wallColor);
                    printf("%c", WALL);
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    printf("%c", map[currentLevel][i][j]);
                }
            }
            printf("\n");
        }
    }
    else {
        for (i = 0; i < dynamicMapHeight; i++) {
            for (j = 0; j < dynamicMapWidth; j++) {
                if (i == player.y && j == player.x) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player.color);
                    printf("%c", PLAYER);
                }
                else if (dynamicMap[i][j] == WALL) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wallColor);
                    printf("%c", WALL);
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    printf("%c", dynamicMap[i][j]);
                }
            }
            printf("\n");
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void createCustomMap() {
    int width, height;
    FILE* customMap;
    errno_t err;

    system("cls");
    printf("Podaj szerokość planszy: ");
    scanf_s("%d", &width);
    printf("Podaj wysokość planszy: ");
    scanf_s("%d", &height);

    if (width <= 2 || height <= 2) {
        printf("Nieprawidłowe wymiary planszy.\n");
        return;
    }

    err = fopen_s(&customMap, "plansza2.txt", "w");
    if (err != 0 || customMap == NULL) {
        printf("Błąd otwarcia pliku do zapisu.\n");
        return;
    }

    fprintf(customMap, "%d %d\n", height, width);

    char* row = (char*)malloc((width + 2) * sizeof(char));

    system("cls");
    printf("Wprowadź planszę, używając '#' dla ścian, spacji dla pustych miejsc i 'E' dla wyjścia.\n");
    printf("Każdy wiersz musi mieć dokładnie %d znaków:\n", width);

    for (int i = 0; i < height; i++) {
        while (1) {
            printf("Wiersz %d: ", i + 1);
            scanf_s(" %[^\n]", row, width + 1);

            if (strlen(row) == width) {
                fprintf(customMap, "%s\n", row);
                break;
            }
            else {
                printf("Nieprawidłowa liczba znaków! Wprowadź dokładnie %d znaków.\n", width);
            }
        }
    }

    free(row);
    fclose(customMap);
    printf("Plansza została zapisana do pliku plansza2.txt.\n");
}

void resetPlayerPosition(Player* p) {
    int i, j;
    if (currentLevel < MAX_LEVELS) {
        for (i = 0; i < MAP_SIZE; i++) {
            for (j = 0; j < MAP_SIZE; j++) {
                if (map[currentLevel][i][j] == ' ') {
                    p->x = j;
                    p->y = i;
                    return;
                }
            }
        }
    }
    else {
        for (i = 0; i < dynamicMapHeight; i++) {
            for (j = 0; j < dynamicMapWidth; j++) {
                if (dynamicMap[i][j] == ' ') {
                    p->x = j;
                    p->y = i;
                    return;
                }
            }
        }
    }
}


void resetPlayerPosition() {
    resetPlayerPosition(&player);
}

void changeColorOptions() {
    system("cls");
    printf("Zmiana kolorów:\n1. Kolor gracza\n2. Kolor ścian\nWybierz opcję: ");
    int choice;
    scanf_s("%d", &choice);
    if (choice == 1) {
        system("cls");
        printf("Wybierz nowy kolor gracza (0-15): ");
        printf("\nLista dostępnych kolorów:\n");
        printf(" 0: Czarny\n 1: Niebieski\n 2: Zielony\n 3: Błękitny\n");
        printf(" 4: Czerwony\n 5: Fioletowy\n 6: Żółtozielony\n 7: Jasnoszary\n");
        printf(" 8: Szary\n 9: Jasnoniebieski\n 10: Jasnozielony\n 11: Jasnobłękitny\n");
        printf("12: Jasnoczerwony\n13: Jasnofioletowy\n14: Żółty\n15: Biały\n");
        scanf_s("%d", &player.color);
    }
    else if (choice == 2) {
        system("cls");
        printf("Wybierz nowy kolor ścian (0-15): ");
        printf("\nLista dostępnych kolorów:\n");
        printf(" 0: Czarny\n 1: Niebieski\n 2: Zielony\n 3: Błękitny\n");
        printf(" 4: Czerwony\n 5: Fioletowy\n 6: Żółtozielony\n 7: Jasnoszary\n");
        printf(" 8: Szary\n 9: Jasnoniebieski\n 10: Jasnozielony\n 11: Jasnobłękitny\n");
        printf("12: Jasnoczerwony\n13: Jasnofioletowy\n14: Żółty\n15: Biały\n");
        scanf_s("%d", &wallColor);
    }
}

void mainMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("MENU GŁÓWNE\n");
        printf("1. Start gry\n2. Opcje\n3. Dodaj Planszę\n4 Wyjście\nWybierz opcję: ");
        scanf_s("%d", &choice);

        if (choice == 1) {
            break;
        }
        else if (choice == 2) {
            changeColorOptions();
        }
        else if (choice == 3) {
            createCustomMap();
        }
        else if (choice == 4) {
            system("cls");
            printCentered("Dziękujemy za grę!\n");
            Sleep(2000);
            exit(0);
        }
        else {
            printf("Błędny wybór!");
            Sleep(100);
        }
    }
}

void playGame() {
    char move;
    resetPlayerPosition();

    while (1) {
        displayMap();
        printf("WSAD - Ruch | M - Wyjście\n");
        move = _getch();

        int newX = player.x;
        int newY = player.y;

        switch (move) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        case 'm': return;
        default: continue;
        }

        if (newX >= 0 && newX < ((currentLevel < MAX_LEVELS) ? MAP_SIZE : dynamicMapWidth) &&
            newY >= 0 && newY < ((currentLevel < MAX_LEVELS) ? MAP_SIZE : dynamicMapHeight)) {

            char target = (currentLevel < MAX_LEVELS) ? map[currentLevel][newY][newX] : dynamicMap[newY][newX];

            if (target == WALL) {
                printf("Uderzyłeś w ścianę!\n");
                Sleep(1000);
                system("cls");
                printCentered("Koniec gry.");
                Sleep(2000);
                exit(0);
            }

            player.x = newX;
            player.y = newY;

            if (target == EXIT) {
                printf("Brawo! Przeszedłeś poziom %d\n", currentLevel + 1);
                Sleep(1000);
                currentLevel++;

                if (currentLevel == MAX_LEVELS) {
                    loadDynamicMap();
                }
                else if (currentLevel > MAX_LEVELS) {
                    freeDynamicMap();
                    system("cls");
                    printCentered("Wygrałeś całą grę!\n");
                    Sleep(2000);
                    exit(0);
                }
                resetPlayerPosition();
            }
        }
        else {
            printf("Ruch poza granice planszy.\n");
        }
    }
}

int main() {
    setlocale(LC_CTYPE, "Polish");
    loadMaps();
    welcome();
    srand(time(NULL));
    setRandomPlayerColor();
    mainMenu();
    playGame();
    freeDynamicMap();
    return 0;
}
