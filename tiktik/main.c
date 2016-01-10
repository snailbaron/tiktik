#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

#define DELTA_MS 1000
#define GOLD_START 100
#define GOLD_STATIC_INCOME 1

#define GOLD_FARM_PRICE 10
#define GOLD_FARM_INCOME 1

#define RAIDER_PRICE 50
#define RAIDER_MAX_COUNT 5

#define ENEMY_START_COUNT 5

long long gold = GOLD_START;
long long gold_farms = 0;
long long raider_count = 0;

long long enemy_count = ENEMY_START_COUNT;

long long gold_income()
{
    return GOLD_STATIC_INCOME + gold_farms * GOLD_FARM_INCOME;
}

void print_state()
{
    system("cls");

    printf("Gold:       %-20lldEnemy\n", gold);
    printf("Gold farms: %-20lld\n", gold_farms);
    printf("Raiders:    %-20lld", raider_count);
    for (long long i = 0; i < enemy_count; i++) {
        printf("^");
    }
    printf("\n");
    
    printf(
        "\n"
        "Build\n"
        "\n"
        "+---+-------+  +---+----+\n"
        "| f |    10 |  | r | 50 |\n"
        "+---+-------+  +---+----+\n"
        "| Gold mine |  | Raider |\n"
        "+-----------+  +--------+\n"
    );
    printf("\n");

    if (raider_count > 0) {
        for (long long i = 0; i < raider_count; i++) {
            printf(" O  ");
        }
        printf("\n");
        for (long long i = 0; i < raider_count; i++) {
            printf("/|\\ ");
        }
        printf("\n");
        for (long long i = 0; i < raider_count; i++) {
            printf(" ^  ");
        }
        printf("\n");
        printf("\n");

        printf(
            "+---+--------+\n"
            "| a | Attack |\n"
            "+---+--------+\n"
        );
    }
}

bool parse_key(char c)
{
    switch (c)
    {
        case 'q': return false;

        case 'f':
        {
            if (gold >= GOLD_FARM_PRICE) {
                gold -= GOLD_FARM_PRICE;
                gold_farms++;
                print_state();
            }
            break;
        }

        case 'r':
        {
            if (gold >= RAIDER_PRICE && raider_count < RAIDER_MAX_COUNT) {
                gold -= RAIDER_PRICE;
                raider_count++;
                print_state();
            }
            break;
        }

        case 'a':
        {
            if (raider_count > 0) {
                long long loss = min(raider_count, enemy_count);
                raider_count -= loss;
                enemy_count -= loss;
                print_state();

                if (enemy_count == 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);

    system("cls");

    clock_t global_start = clock();

    clock_t time_start = clock() - DELTA_MS;
    bool done = false;
    while (!done) {
        // Read console input events
        DWORD evt_num;
        GetNumberOfConsoleInputEvents(input_handle, &evt_num);
        for (DWORD i = 0; i < evt_num; i++) {
            INPUT_RECORD rec;
            DWORD evt_left;
            ReadConsoleInput(input_handle, &rec, 1, &evt_left);
        
            if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown == TRUE) {
                char c = rec.Event.KeyEvent.uChar.AsciiChar;
                if (!parse_key(c)) {
                    done = true;
                }
            }
        }


        // Perform action, if time has come
        clock_t time_now = clock();
        if (time_now - time_start >= DELTA_MS) {
            gold += gold_income();

            print_state();

            time_start = clock();
        }

    }

    clock_t global_end = clock();

    clock_t time_played = global_end - global_start;

    printf("You finished the game in %d.%d seconds\n", time_played / 1000, time_played % 1000);
    system("pause");


    return 0;
}