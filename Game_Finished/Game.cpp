#include "Engine.h"
#include "GameObject.h"
#include "windows.h"
#include <vector>
#include <stdlib.h>
#include <memory.h>

//constants

CONST INT BRICK_ROW = 20;
CONST INT BRICK_COLUMN = 7;

CONST DOUBLE PLATFORM_SPEED = 7;
CONST DOUBLE BALL_SPEED = 2;
CONST INT BALL_COUNT = 2;

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()


std::vector<Ball> balls;
int ballsInGame;
int bricksLeft;
Platform platform = Platform(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2+300, 100, 10, 0);
std::vector<Brick> bricks;

// initialize game data in this function
void initialize()
{
    // spawn bricks
    for (int i = 0; i < BRICK_ROW; i++) {
        for (int j = 0; j < BRICK_COLUMN; j++) {
            bricks.push_back({ i * 50.0, j * 50.0, (int)45.0, (int)45.0, false });
        }
    }

    // spawn balls
    for (int i = 0; i < BALL_COUNT; i++)
    {
        balls.push_back({ SCREEN_WIDTH / 2 + i * 20.0, SCREEN_HEIGHT / 2 + i * 20.0, 10, 10, BALL_SPEED, BALL_SPEED, false});
    }
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    // slow the game down
    Sleep(300*dt);

    // quit game
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    // stop platform if no key is pressed
    if (!(is_key_pressed(VK_RIGHT) && is_key_pressed(VK_LEFT)))
        platform.speed = 0;

    // check right key and check platform bounds
    if (is_key_pressed(VK_RIGHT) && platform.x < SCREEN_WIDTH - platform.width)
        platform.speed = PLATFORM_SPEED;

    // check left key and check platform bounds
    if (is_key_pressed(VK_LEFT) && platform.x >= 0)
        platform.speed = -PLATFORM_SPEED;

    // move platform
    platform.x += platform.speed;

    // reset the amount of balls and bricks in game to count
    ballsInGame = 0;
    bricksLeft = 0;

    // check balls collisions 
    for (auto& ball : balls) {

        // check horizontal bounds
        if (!(ball.x >= 0 && ball.x < SCREEN_WIDTH - ball.width))
            ball.dx *= -1;

        // check vertical bound
        if (!(ball.y >= 0))
            ball.dy *= -1;

        // check collision with bottom border
        if (!(ball.y < SCREEN_HEIGHT - ball.height))
            ball.outOfBounds = true;
        else ballsInGame++;

        // check collision with platform
        if (ball.y + ball.height == platform.y && ball.x + ball.width >= platform.x && ball.x <= platform.x + platform.width)
            ball.dy *= -1;

        // check collision with bricks
        for (auto& brick : bricks) {
            if (!brick.destroyed && ball.y - ball.height <= brick.y + brick.height && ball.x + ball.width >= brick.x && ball.x - ball.width <= brick.x + brick.width) {
                ball.dy *= -1;
                brick.destroyed = true;
            }
        }

        // move the ball
        ball.x += ball.dx;
        ball.y += ball.dy;

    }

    for (auto& brick : bricks)
    {
        if (!brick.destroyed)
            bricksLeft++;
    }

    // end the game if no balls are left or all bricks have been destroyed
    if (ballsInGame == 0 || bricksLeft == 0) {
        schedule_quit_game();
    }

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear buffer
    clear_buffer();

  // draw the ball
  for (const auto& ball : balls) {
      for (int i = 0; i < ball.height; i++) {
          for (int j = 0; j < ball.width; j++) {
              int x = ball.x + j;
              int y = ball.y + i;
              if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                  buffer[y][x] = 0xFFFFFFFF; // white color
              }
          }
      }
  }


  // draw the platform
  for (int i = 0; i < platform.height; i++) {
      for (int j = 0; j < platform.width; j++) {
          int x = platform.x + j;
          int y = platform.y + i;
          if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
              buffer[y][x] = 0xFF2FFFFF; // blueish color
          }
      }
  }

  // draw the bricks
  for (const auto& brick : bricks) {
      if (!brick.destroyed) {
          for (int i = 0; i < brick.height; i++) {
              for (int j = 0; j < brick.width; j++) {
                  int x = brick.x + j;
                  int y = brick.y + i;
                  if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                      buffer[y][x] = 0xFFFF0000; // red color
                  }
              }
          }
      }
  }
}


// free game data in this function
void finalize()
{
}

