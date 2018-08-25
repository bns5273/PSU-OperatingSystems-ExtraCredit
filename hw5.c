// bns5273
// hw05, an extension of the lab03 Stallman assignment

#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main() {
    
    char stallman[10000][100];      int s = 0;
    char notstallman[10000][100];   int n = 0;
    char nomanpage[10000][100];     int m = 0;
    char nocommand[10000][100];     int c = 0;
    
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    idlok(stdscr, TRUE);
    setscrreg(100, 0);
    
    int row, col;
    int line_0 = 1;
    int line_n = 5;
    int line = line_0;

    char* menu[line_n];
    menu[0] = "STEP 1: Run stallman.sh";
    menu[1] = "View commands written by Stallman";
    menu[2] = "View commands with man pages";
    menu[3] = "View other commands";
    menu[4] = "View invalid commands";
    
    
    printw("  _________ __         .__  .__                         \n /   _____//  |______  |  | |  |   _____ _____    ____  \n \\_____  \\\\   __\\__  \\ |  | |  |  /     \\\\__  \\  /    \\ \n /        \\|  |  / __ \\|  |_|  |_|  Y Y  \\/ __ \\|   |  \\\n/_______  /|__| (____  /____/____/__|_|  (____  /___|  /\n        \\/           \\/                \\/     \\/     \\/ \n");
    refresh();
    sleep(1);
    clear();
    
    printw("Menu Screen (q to quit)\n");
    int sc;
    for(sc = 0; sc < line_n; sc++){
        printw("  %s\n", menu[sc]);
    }
    mvprintw(1, 0, "->");

    int choice = NULL;
    while(choice != 'q'){
        choice = getch();
        if(choice == KEY_UP && line > line_0){
            mvprintw(line, 0, "  ");
            line -= 1;
            move(line, 0);
            printw("->");
        }
        else if(choice == KEY_DOWN && line < line_n){
            mvprintw(line, 0, "  ");
            line += 1;
            move(line, 0);
            printw("->");
        }
        else if(choice == 10){ // this worked better than KEY_ENTER
            clear();
            int sel = line - line_0;
            int e, max, y, x;
            char* index;

            if(sel == 1){
                max = s;
                for(e = 0; e < max; e++){
                    getyx(stdscr, y, x);
                    index = strchr(stallman[e],':');
                    index[0] = '\0';
                    
                    mvprintw(y, 0, "%i", e);
                    mvprintw(y, 7, "| %s", stallman[e]);
                    mvprintw(y, 20, "| %s\n", index+2);
                    
                    index[0] = ':'; // prevent seg fault on recall of menu item
                    refresh();
                    usleep(200000);
                }
            }
            else if(sel == 2){
                max = n;
                for(e = 0; e < max; e++){
                    getyx(stdscr, y, x);
                    index = strchr(notstallman[e],':');
                    index[0] = '\0';
                    
                    mvprintw(y, 0, "%i", e);
                    mvprintw(y, 7, "| %s", notstallman[e]);
                    mvprintw(y, 20, "| %s\n", index+2);
                    
                    index[0] = ':'; // prevent seg fault on recall of menu item
                    refresh();
                    usleep(200000);
                }
            }
            else if(sel == 3){
                max = m;
                for(e = 0; e < max; e++){
                    getyx(stdscr, y, x);
                    index = strchr(nomanpage[e],':');
                    index[0] = '\0';
                    
                    mvprintw(y, 0, "%i", e);
                    mvprintw(y, 7, "| %s", nomanpage[e]);
                    mvprintw(y, 20, "| %s\n", index+2);
                    
                    index[0] = ':'; // prevent seg fault on recall of menu item
                    refresh();
                    usleep(200000);
                }
            }
            else if(sel == 4){
                max = c;
                for(e = 0; e < max; e++){
                    getyx(stdscr, y, x);
                    index = strchr(nocommand[e],':');
                    index[0] = '\0';
                    
                    mvprintw(y, 0, "%i", e);
                    mvprintw(y, 7, "| %s", nocommand[e]);
                    mvprintw(y, 20, "| %s\n", index+2);
                    
                    index[0] = ':'; // prevent seg fault on recall of menu item
                    refresh();
                    usleep(200000);
                }
            }
            else if(sel == 0){
                pid_t syscall, child_pid, wpid;
                int status = 0;
                int a = 0;
                char loading[] = "-\\|/";
                
                if ((syscall = fork()) == 0) {      // child for system call
                    system("./stallman.sh 1>/dev/null 2>/dev/null > data.dat");
                    exit(0);
                }
                else if((child_pid = fork()) == 0){ // child for animation
                    clear();                        // animation plays for first 10 seconds of script run time
                    int a = 0;
                    char loading[] = "-\\|/";
                    for(a = 0; a < 20; a++){
                        mvprintw(0, 0, "%c loading...", loading[a % 4]);
                        refresh();
                        usleep(500000);
                    }
                    exit(0);
                }
                
                // parent waits for both children to exit
                // the bash script is the limiting factor here.
                while ((wpid = wait(&status)) > 0); 
                
                
                // output file processing
                FILE *in = fopen("data.dat", "r");
                if (in == NULL) return;
                
                char buf[100];
                printw("%s", buf);
                int i = 0;
                while((c = getc(in)) != EOF){
                    if(c != '\n'){
                        buf[i] = c;
                        i++;
                    }
                    // reached endline
                    else{
                        // manpage exerpt. skip
                        if(buf[0] == ' ');
                        
                        // written by stallman
                        else if(buf[strlen(buf)-1] == '!'){
                            strcpy(stallman[s], buf);
                            s++;
                        }
                        
                        // not written by stallman
                        else if(buf[strlen(buf)-1] == 'n'){
                            strcpy(notstallman[n], buf);
                            n++;
                        }
                        
                        // no manpage
                        else if(buf[strlen(buf)-1] == 'y'){
                            strcpy(nomanpage[m], buf);
                            m++;
                        }
                        
                        // no command
                        else if(buf[strlen(buf)-1] == 'd'){
                            strcpy(nocommand[c], buf);
                            c++;
                        }
                        
                        i = 0;
                        memset(buf, 0, sizeof(buf));
                    }
                }
                fclose(in);
            }
            
            
            printw("Process completed.\n\nPress any key to continue...");
            getch();
            clear();
            move(0, 0);
            printw("Menu Screen (q to quit)\n");
            int sc;
            for(sc = 0; sc < line_n; sc++){
                printw("  %s\n", menu[sc]);
            }
            mvprintw(1, 0, "->");
            line = line_0;
        }
        
        else if(choice == 'q'){
            clear();
            mvprintw(0, 0, "  _________ __         .__  .__                         \n /   _____//  |______  |  | |  |   _____ _____    ____  \n \\_____  \\\\   __\\__  \\ |  | |  |  /     \\\\__  \\  /    \\ \n /        \\|  |  / __ \\|  |_|  |_|  Y Y  \\/ __ \\|   |  \\\n/_______  /|__| (____  /____/____/__|_|  (____  /___|  /\n        \\/           \\/                \\/     \\/     \\/ \n");
            refresh();
            sleep(1);
        }
    }
    
    endwin();
    return;
}