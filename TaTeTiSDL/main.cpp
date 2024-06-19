//
//  main.cpp
//  TaTeTiSDL
//
//  Created by Adrian Narducci on 19/06/2024.
//

#include <iostream>
#include "SDL2/SDL.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 200;

enum Player { NONE = 0, PLAYER_X, PLAYER_O };

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Player board[3][3];
Player currentPlayer = PLAYER_X;

bool initSDL();
void closeSDL();
void drawBoard();
void handleClick(int x, int y);
Player checkWinner();
void renderText(const std::string& text, int x, int y);

int main(int argc, char* args[]) {
    if (!initSDL()) {
        std::cout << "Failed to initialize SDL\n";
        return -1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleClick(x, y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        drawBoard();

        Player winner = checkWinner();
        if (winner != NONE) {
            std::string winnerText = (winner == PLAYER_X) ? "Player X wins!" : "Player O wins!";
            renderText(winnerText, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
        }

        SDL_RenderPresent(renderer);
    }

    closeSDL();
    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = NONE;

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    SDL_Quit();
}

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 1; i < 3; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = i * CELL_SIZE;
            int y = j * CELL_SIZE;

            if (board[i][j] == PLAYER_X) {
                SDL_RenderDrawLine(renderer, x, y, x + CELL_SIZE, y + CELL_SIZE);
                SDL_RenderDrawLine(renderer, x + CELL_SIZE, y, x, y + CELL_SIZE);
            } else if (board[i][j] == PLAYER_O) {
                SDL_Rect rect = { x + 10, y + 10, CELL_SIZE - 20, CELL_SIZE - 20 };
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void handleClick(int x, int y) {
    int i = x / CELL_SIZE;
    int j = y / CELL_SIZE;

    if (board[i][j] == NONE) {
        board[i][j] = currentPlayer;
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}

Player checkWinner() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != NONE)
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != NONE)
            return board[0][i];
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != NONE)
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != NONE)
        return board[0][2];

    return NONE;
}

void renderText(const std::string& text, int x, int y) {
    // Placeholder for text rendering
    // You would normally use SDL_ttf to render text, but that is out of scope for this example.
    std::cout << text << " at (" << x << ", " << y << ")\n";
}



