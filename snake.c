#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct SnakePart{ // Head or Tail
    int posX;
    int posY;
    struct SnakePart* prev;
    struct SnakePart* next;
} SnakePart;
SnakePart* head = NULL;


SnakePart* lastBodyPart(){
    SnakePart* temp = head;
    if(temp->prev == NULL){
	return temp;
    }

    while((temp = temp->prev)->prev != NULL){}

    return temp;
}

void main() {
    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    head = malloc(sizeof(SnakePart));
    head->posX = 0;
    head->posY = 0;
    head->prev = NULL;
    head->next = NULL;
    int foodX = rand() % 20;
    int foodY = rand() % 20;
    int dirX = 1;
    int dirY = 0;
    while (true) {
        int pressed = wgetch(win);
        if (pressed == KEY_LEFT) {
            dirX = -1;
            dirY = 0;
        }
        if (pressed == KEY_RIGHT) {
            dirX = 1;
            dirY = 0;
        }
        if (pressed == KEY_UP) {
            dirX = 0;
            dirY = -1;
        }
        if (pressed == KEY_DOWN) {
            dirX = 0;
            dirY = 1;
        }

	for(SnakePart* temp = lastBodyPart(); temp->next != NULL; temp = temp->next){
	    temp->posX = temp->next->posX;
	    temp->posY = temp->next->posY;
	}
        head->posX += dirX;
        head->posY += dirY;
	for(SnakePart* temp = head->prev; temp != NULL; temp = temp->prev){
	    if(head->posX == temp->posX && head->posY == temp->posY)
		return; // Game over
	}
        erase();
        mvaddstr(head->posY, head->posX, "*");
	SnakePart* temp = head;
	while((temp = temp->prev) != NULL){
	    mvaddstr(temp->posY, temp->posX, "*");
	}
        mvaddstr(foodY, foodX, "&");
        if (foodX == head->posX && foodY == head->posY) {
            foodX = rand() % 20;
            foodY = rand() % 20;

	    // Add new snake part
	    SnakePart* last = lastBodyPart();
	    SnakePart* newPart = malloc(sizeof(SnakePart));
	    newPart->posX = last->posX - dirX;
	    newPart->posY = last->posY - dirY;
	    newPart->prev = NULL;
	    newPart->next = last;
	    last->prev = newPart;
        }
        usleep(100000);
    }

    endwin();
}
