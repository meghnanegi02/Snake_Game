#include <iostream>
#include <list>
#include <conio.h>
#include <windows.h>
using namespace std;

class Snake {
public:
    enum Face { EAST = 1, NORTH = 2, WEST = 3, SOUTH = 4 }; // 4 directions
    int max_x, max_y, is_alive;
    Face face;
    list<int> cells;
    Snake() { is_alive = 1; }
    Snake(int w, int h) : max_x(w), max_y(h) { is_alive = 1; }
    bool having(int a_cell) {
        return this->cells.end() != find(cells.begin(), cells.end(), a_cell);
    }
    void set_face(Face f) { if (2 != abs(face - f)) { face = f; } }
    int head() { return this->cells.back(); }
    int next_head() {
        int x = this->head() % max_x, y = this->head() / max_x;
        switch (this->face) { // calculate new head on the current face direction
            case EAST: x = (x + 1) % max_x; break;
            case WEST: x = (x - 1 + max_x) % max_x; break;
            case NORTH: y = (y - 1 + max_y) % max_y; break;
            case SOUTH: y = (y + 1) % max_y; break;
            default: break;
        }
        return y * max_x + x;
    }
}; // .class Snake =============================================================

class SnakeLogic {
    list<int> field, blanks;
    Snake snake;
    int score, food_cell, max_delay_ms;
    void get_input() {
        unsigned int waited_MS = 0, input_wait_MS = 20;
        do {
            if (kbhit()) { // Keyboard hit!
                switch (getch()) { // Get input char
                    case 'a': case 'A': snake.set_face(Snake::WEST); break;
                    case 's': case 'S': snake.set_face(Snake::SOUTH); break;
                    case 'd': case 'D': snake.set_face(Snake::EAST); break;
                    case 'w': case 'W': snake.set_face(Snake::NORTH); break;
                    case 'X': snake.is_alive = 0; break; // Stop Game
                    default: break;
                }
            }
            waited_MS += input_wait_MS;
            Sleep(input_wait_MS); // wait for the next keyboard hit
        } while (max_delay_ms > waited_MS);
    }
    void generate_food() {
        blanks.clear(); // clear old field on the previous calculation
        int i = 0;
        do {
            if (!snake.having(i) && i % snake.max_x != 0 && i % snake.max_x != snake.max_x - 1 &&
                i >= snake.max_x && i < snake.max_x * (snake.max_y - 1)) {
                blanks.push_back(i);
            }
        } while (i++ < field.size()); // Finish finding blank cells

        if (!blanks.empty()) {
            list<int>::iterator it = blanks.begin(); // iterator at the beginning
            advance(it, rand() % blanks.size()); // Go to a random blank cell
            food_cell = *(it); // assign new food position
        } else {
            // No blank cells left, game over
            snake.is_alive = 0;
        }
    }
    void update() {
        int next_head = snake.next_head();

        // Check if the next head is within the game boundaries
        if (next_head < 0 || next_head >= snake.max_x * snake.max_y ||
            next_head % snake.max_x == 0 || next_head % snake.max_x == snake.max_x - 1 ||
            next_head < snake.max_x || next_head >= snake.max_x * (snake.max_y - 1)) {
            snake.is_alive = 0; // Snake collided with the boundary, end the game
            return;
        }

        if (snake.having(next_head)) {
            snake.is_alive = 0; // die on collision with itself
        } else {
            // if there's food, keep the tail; else, cut the tail
            if (next_head == food_cell) {
                generate_food();
                score++; // make new food & increase score
            } else {
                snake.cells.erase(snake.cells.begin());
            }
            snake.cells.push_back(next_head); // move ahead
        }
    }
    void re_draw() {
        system("cls");
        cout << "SNAKE CPP. Controls: A-left, S-down, D-right, W-up" << endl;
        int width = snake.max_x;

        // Draw top boundary
        for (int i = 0; i < width + 2; i++) {
            cout << "~";
        }
        cout << endl;

        for (int y = 0; y < snake.max_y; y++) {
            // Draw left boundary
            cout << "~";

            for (int x = 0; x < snake.max_x; x++) {
                int current_cell = y * snake.max_x + x;

                if (x == 0) {
                    cout << "~"; // Draw left boundary
                }

                if (x != 0 && x != snake.max_x - 1) {
                    if (current_cell == food_cell) {
                        cout << "@"; // Draw food
                    } else if (snake.having(current_cell)) {
                        cout << "x"; // Draw snake body
                    } else if (current_cell == snake.head()) {
                        cout << "o"; // Draw snake head
                    } else {
                        cout << " "; // Draw empty space
                    }
                }

                if (x == snake.max_x - 1) {
                    cout << "~"; // Draw right boundary
                }
            }

            cout << endl;
        }

        // Draw bottom boundary
        for (int i = 0; i < width + 2; i++) {
            cout << "~";
        }
        cout << endl;

        cout << "SCORE: " << score << endl;
   
