#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "activities.txt"

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

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
        printf(RED "\nA session is already running!" RESET "\n");
        return;
    }
    start_time = time(NULL);
    printf(GREEN "\nTimer started! Go crush those goals." RESET "\n");
}

void stop_session() {
    if (start_time == 0) {
        printf(RED "\nNo active session to stop." RESET "\n");
        return;
    }

    time_t end_time = time(NULL);
    double seconds = difftime(end_time, start_time);
    float minutes = (float)seconds / 60.0;

    Activity a;
    printf("\nSession stopped. Duration: " CYAN "%.2f minutes" RESET ".\n", minutes);
    
    printf("Enter activity type " YELLOW "(Ex. Running)" RESET ": ");
    scanf("%s", a.type);
    
    printf("Enter distance covered " YELLOW "(km, Ex. 5.2)" RESET ": ");
    scanf("%f", &a.distance);

    struct tm *tm_info = localtime(&end_time);
    strftime(a.date, 12, "%d.%m.%Y", tm_info);

    FILE *f = fopen(FILE_NAME, "a");
    if(f) {
        fprintf(f, "%s %.2f %.2f %s\n", a.type, minutes, a.distance, a.date);
        fclose(f);
        printf(GREEN "Session saved successfully!" RESET "\n");
    }
    start_time = 0;
}

void add_manual() {
    Activity a;
    printf("\n--- Manual Activity Entry ---\n");
    
    printf("Activity type " YELLOW "(Ex. Running)" RESET ": ");
    scanf("%s", a.type);
    printf("Duration " YELLOW "(min, Ex. 30.5)" RESET ": ");
    scanf("%f", &a.duration);
    printf("Distance " YELLOW "(km, Ex. 5.0)" RESET ": ");
    scanf("%f", &a.distance);
    printf("Date " YELLOW "(DD.MM.YYYY)" RESET ": ");
    scanf("%s", a.date);

    FILE *f = fopen(FILE_NAME, "a");
    if(f) {
        fprintf(f, "%s %.2f %.2f %s\n", a.type, a.duration, a.distance, a.date);
        fclose(f);
        printf(GREEN "Activity recorded manually." RESET "\n");
    }
}

void display_stats() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "\nNo data found. Please add an activity first." RESET "\n");
        return;
    }

    Activity a;
    int count = 0;
    float total_dur = 0, total_dist = 0;

    printf("\n" BOLD "%-15s | %-10s | %-10s | %-12s" RESET "\n", "Type", "Min", "KM", "Date");
    printf("----------------------------------------------------------\n");

    while (fscanf(f, "%s %f %f %s", a.type, &a.duration, &a.distance, a.date) != EOF) {
        printf(CYAN "%-15s" RESET " | %-10.2f | " GREEN "%-10.2f" RESET " | %-12s\n", a.type, a.duration, a.distance, a.date);
        count++;
        total_dur += a.duration;
        total_dist += a.distance;
    }

    if (count > 0) {
        printf("----------------------------------------------------------\n");
        // Only the values are colored here as requested
        printf("Total Workouts: " BOLD "%d" RESET "\n", count);
        printf("Average Duration: " YELLOW "%.2f min" RESET "\n", total_dur / count);
        printf("Total Distance: " GREEN "%.2f km" RESET "\n", total_dist);
    }
    fclose(f);
}

void settings_menu() {
    int choice;
    do {
        clear_screen();
        printf("=== SETTINGS SUBMENU ===\n");
        printf("1. " RED "Delete All Records" RESET "\n");
        printf("0. Back to Main Menu\n");
        printf("Selection: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char confirm;
            printf("Are you sure? " YELLOW "(y/n)" RESET ": ");
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
        
        if (start_time == 0) {
            printf("1 - " GREEN "Start Live Session (Timer)" RESET "\n");
            printf("2 - Stop & Save Session\n");
        } else {
            printf("1 - Start Live Session (Timer)\n");
            printf("2 - " RED "Stop & Save Session" RESET "\n");
        }
        
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
            case 0: 
                printf(MAGENTA BOLD "Exiting... Goodbye! See you at the next workout!" RESET "\n"); 
                break;
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