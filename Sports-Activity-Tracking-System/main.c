#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "activities.txt"

typedef struct {
    char type[30];
    float duration; // in min
    float distance; // in km
    char date[12];  // DD.MM.YYYY
} Activity;

time_t start_time = 0;

void check_and_write() {
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        // if file doesn't exist, we create it with an example entry
        f = fopen(FILE_NAME, "w");
        if (f) {
            fprintf(f, "Running 30.50 5.20 01.04.2026\n");
            fclose(f);
            printf("Notice: System initialized with an example data.\n");
        }
    } else {
        fclose(f);
    }
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

void pause_and_clear() {
    printf("\nPress Enter to return...");
    getchar(); 
    if (start_time == 0) getchar(); 
    clear_screen();
}

void start_session() {
    if (start_time != 0) {
        printf("\nA session is already running!\n");
        return;
    }
    start_time = time(NULL);
    printf("\nTimer started! Go crush those goals.\n");
}

void stop_session() {
    if (start_time == 0) {
        printf("\nNo active session to stop.\n");
        return;
    }

    time_t end_time = time(NULL);
    double seconds = difftime(end_time, start_time);
    float minutes = (float)seconds / 60.0;

    Activity a;
    printf("\nSession stopped. Duration: %.2f minutes.\n", minutes);
    printf("Enter activity type (Ex. Running): ");
    scanf("%s", a.type);
    printf("Enter distance covered (km): ");
    scanf("%f", &a.distance);

    struct tm *tm_info = localtime(&end_time);
    strftime(a.date, 12, "%d.%m.%Y", tm_info);

    FILE *f = fopen(FILE_NAME, "a");
    if(f) {
        fprintf(f, "%s %.2f %.2f %s\n", a.type, minutes, a.distance, a.date);
        fclose(f);
        printf("Session saved successfully!\n");
    }
    start_time = 0;
}

void add_manual() {
    Activity a;
    printf("\n--- Manual Activity Entry ---\n");
    printf("Activity type (Ex. Running): ");
    scanf("%s", a.type);
    printf("Duration (min): ");
    scanf("%f", &a.duration);
    printf("Distance (km): ");
    scanf("%f", &a.distance);
    printf("Date (DD.MM.YYYY): ");
    scanf("%s", a.date);

    FILE *f = fopen(FILE_NAME, "a");
    if(f) {
        fprintf(f, "%s %.2f %.2f %s\n", a.type, a.duration, a.distance, a.date);
        fclose(f);
        printf("Activity recorded manually.\n");
    }
}

void display_stats() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf("\nNo data found. Please add an activity first.\n");
        return;
    }

    Activity a;
    int count = 0;
    float total_dur = 0, total_dist = 0;

    printf("\n%-15s | %-10s | %-10s | %-12s\n", "Type", "Min", "KM", "Date");
    printf("----------------------------------------------------------\n");

    // Reading logic (The core requirement)
    while (fscanf(f, "%s %f %f %s", a.type, &a.duration, &a.distance, a.date) != EOF) {
        printf("%-15s | %-10.2f | %-10.2f | %-12s\n", a.type, a.duration, a.distance, a.date);
        count++;
        total_dur += a.duration;
        total_dist += a.distance;
    }

    if (count > 0) {
        printf("----------------------------------------------------------\n");
        printf("Total Workouts: %d\n", count);
        printf("Average Duration: %.2f min\n", total_dur / count);
        printf("Total Distance: %.2f km\n", total_dist);
    } else {
        printf("File is empty.\n");
    }
    fclose(f);
}

void settings_menu() {
    int choice;
    do {
        clear_screen();
        printf("=== SETTINGS SUBMENU ===\n");
        printf("1. Delete All Records\n");
        printf("0. Back to Main Menu\n");
        printf("Selection: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char confirm;
            printf("Are you sure? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                remove(FILE_NAME);
                printf("History cleared.\n");
            }
            pause_and_clear();
        }
    } while (choice != 0);
    clear_screen();
}

void main_menu() {
    int option;
    do {
        printf("--- SPORTS ACTIVITY TRACKER ---\n");
        printf("1 - Start Live Session (Timer)\n");
        printf("2 - Stop & Save Session\n");
        printf("3 - Add Activity Manually\n");
        printf("4 - View Statistics\n");
        printf("5 - Settings (Delete/Manage)\n");
        printf("0 - Exit\n");
        printf("-------------------------------\n");
        printf("Select option: ");

        if (scanf("%d", &option) != 1) {
            while(getchar() != '\n');
            clear_screen();
            continue;
        }

        switch(option) {
            case 1: start_session(); break;
            case 2: stop_session(); pause_and_clear(); break;
            case 3: add_manual(); pause_and_clear(); break;
            case 4: 
                clear_screen(); 
                printf("=== ACTIVITY STATISTICS ===\n");
                display_stats(); 
                pause_and_clear(); 
                break;
            case 5: settings_menu(); break;
            case 0: printf("Exiting... Goodbye!\n"); break;
            default: printf("Invalid option.\n");
        }
    } while(option != 0);
}

int main() {
    clear_screen();
    check_and_write();
    main_menu();
    return 0;
}