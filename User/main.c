#include "stm32f10x.h"
#include "EIE3810_Buzzer.h"
#include "EIE3810_Clock.h"
#include "EIE3810_JOYPAD.h"
#include "EIE3810_Key.h"
#include "EIE3810_LED.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_Timer.h"
#include "EIE3810_USART.h"
#include "EIE3810_Others.h"


/* macros for timer usage */
#define ORG_FRE 72000000 // orginial frequency, will be divided
#define FIX_ARR 5000     // fixed arr for usage
#define TIM3_FRE 16      // frequency for TIM3
#define TIM4_FRE 16      // frequency for TIM4

/* screen formating info */
#define SCREEN_WIDTH 480  // screen width
#define SCREEN_HEIGHT 800 // screen height
#define LOG_HEIGHT 20     // the log height
#define LOG_WIDTH 120     // the log width
#define EDGE_SIZE 30      // top and bottom edges, for showing information

/* Player and Ball information */
#define PLAYER_A_Y (SCREEN_HEIGHT - (LOG_HEIGHT / 2 + EDGE_SIZE)) // log A's Y
#define PLAYER_B_Y (LOG_HEIGHT / 2 + EDGE_SIZE)                   // log B's Y
#define BALL_RADIUS 20                                            // ball's radius
#define UPPER_BOUND (PLAYER_B_Y + LOG_HEIGHT / 2)                 // upper bound
#define LOWER_BOUND (PLAYER_A_Y - LOG_HEIGHT / 2)                 // lower bound

/* Ploting indices */
#define INTRO_X 100 // introductin starting x
#define INTRO_Y 100 // introductin starting y
#define LEVEL_X 100 // level choosing starting x
#define LEVEL_Y 240 // level choosing starting y
#define USART_X 30  // USART starting x
#define USART_Y 300 // USART starting y
#define ROUND_X 10  // round showing starting x
#define ROUND_Y 480 // round showing starting y
#define TIME_X 10   // time showing starting x
#define TIME_Y 505  // time showing starting y

/* function prototype */
void TIM3_IRQHandler(void);
void Ball_Move(void);
void TIM4_IRQHandler(void);
void game_state_INTRO_init(void);
void game_state_DIFF_LEVEL_init(void);
void game_state_USART_init(void);
void game_state_COUNT_DOWN(void);
void game_state_PLAYING_init(void);
void game_state_GAME_OVER_init(void);
void draw_e_time(u16 color);
void draw_bounces(u16 color);
void PLAYING_update(void);
void EXTI4_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI0_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void game_state_BALL_SIZE_init(void);

enum game_state // indicating the game state
{
    INTRO = 0,      // introduce the game
    DIFF_LEVEL = 1, // select the difficulty level
    BALL_SIZE = 2,  // select the ball size
    USART = 3,      // send the USART
    COUNT_DOWN = 4, // count down page
    PREPARE = 5,    // prepare
    PLAYING = 6,    // playing
    PAUSE = 7,      // pause the game
    GAME_OVER = 8   // game over
} g_game_state = INTRO;

enum game_mode // indicating the game difficulty level
{
    EASY = 0,
    HARD = 1
} g_game_mode = EASY;

enum game_ball
{
    SMALL = 0,
    BIG = 1
} g_game_ball = SMALL;

enum game_turn // indicating which player kicks the ball
{
    A = 0,
    B = 1
} g_game_turn = A;

u32 g_TIM3_local_beat = 0; // to count the time

u32 g_e_time = 0;   // elapsed time
u32 g_bounces = 0;  // bounces count
u32 g_ball_dir = 4; // ball direction (for B reference )
u32 dirs[8][2] = {  // 8 fixed directions
    {9, 2},
    {8, 5},
    {5, 8},
    {2, 9},
    {-2, 9},
    {-5, 8},
    {-8, 5},
    {-9, 2}};

/* indices when playing */
u16 g_Player_A_X = SCREEN_WIDTH / 2;
u16 g_Player_B_X = SCREEN_WIDTH / 2;
u16 g_ball_X = SCREEN_WIDTH / 2;
u16 g_ball_Y = EDGE_SIZE + LOG_HEIGHT + BALL_RADIUS + 10;

/* indices record */
u16 g_Player_A_X_pre = SCREEN_WIDTH / 2;
u16 g_Player_B_X_pre = SCREEN_WIDTH / 2;
u16 g_ball_X_pre = SCREEN_WIDTH / 2;
u16 g_ball_Y_pre = EDGE_SIZE + LOG_HEIGHT + BALL_RADIUS + 10;

/* further option */
u32 g_Player_A_speed = 10;
u32 g_Player_B_speed = 10;
u32 g_ball_speed = 1;
u32 g_ball_size = 15;

/* main function */
int main(void)
{
    /* initialization */
    EIE3810_clock_tree_init();
    EIE3810_TFTLCD_Init();       // TFTLCD
    EIE3810_JOYPAD_Init();       // clock tree
    EIE3810_Buzzer_Init();       // buzzer
    EIE3810_FourKeys_EXTIInit(); // four keys interrupt

    EIE3810_TIM3_Init(FIX_ARR - 1, ORG_FRE / (TIM3_FRE * FIX_ARR) - 1); // TIM3
    EIE3810_TIM4_Init(FIX_ARR - 1, ORG_FRE / (TIM4_FRE * FIX_ARR) - 1); // TIM4

    EIE3810_USART1_init(72, 21340);   // USART1
    EIE3810_USART1_EXTIInit();        // USART1 interrupt
                                      // JOYPAD
    EIE3810_NVIC_SetPriorityGroup(5); // priority group
    Delay(1000000);                   // delay

    while (1)
    {

        g_game_state = INTRO;    // introduction state
        game_state_INTRO_init(); // init introduction screen

        while (g_game_state == INTRO) // wait until not intro
            ;
        Delay(1000000);
        game_state_DIFF_LEVEL_init(); // select difficulty level

        while (g_game_state == DIFF_LEVEL) // wait unit not level
            ;
        Delay(1000000);

        g_game_state = BALL_SIZE;
        game_state_BALL_SIZE_init(); // select ball size

        while (g_game_state == BALL_SIZE) // wait unit not level
            ;
        Delay(1000000);
        game_state_USART_init(); // send USART

        while (g_game_state == USART) // wait until not USART
            ;
        Delay(10000000);
        game_state_COUNT_DOWN(); // count down

        while (g_game_state == COUNT_DOWN) // wait until count down finish
            ;
        game_state_PLAYING_init(); // init playing

        while (g_game_state != GAME_OVER) // wait until game over
            ;
        Delay(1000000);
        game_state_GAME_OVER_init(); // init game over screen
        Delay(30000000);
    }
}

/* TIM3 interrupt handler
 * to count the time, move the ball, and update the screen
 */
void TIM3_IRQHandler(void)
{
    if (TIM3->SR & 1 << 0)
    {
        if (g_game_state == PLAYING) // when playing
        {
            /* count the elapsed time */
            g_TIM3_local_beat++;
            if (g_TIM3_local_beat == TIM3_FRE)
            {
                g_TIM3_local_beat = 0;
                g_e_time++;
            }
            /* move the ball */
            Ball_Move();
            /* judge game state if GAME_OVER */
            if (g_game_state != GAME_OVER)
            {
                /* update the screen */
                PLAYING_update();
                /* record the previous indices */
                g_Player_A_X_pre = g_Player_A_X;
                g_Player_B_X_pre = g_Player_B_X;
                g_ball_X_pre = g_ball_X;
                g_ball_Y_pre = g_ball_Y;
            }
        }
    }
    TIM3->SR &= ~(1 << 0);
}

/* move the ball, do the collision and judge the game state */
void Ball_Move(void)
{
    /* getting the increment */
    u32 go_x = dirs[g_ball_dir][0];
    u32 go_y = dirs[g_ball_dir][1];
    if (g_game_turn == A)
        go_y = -go_y; // if A, then reverse the vertical direction
    /* changing the indices */
    g_ball_X += go_x * g_ball_speed;
    g_ball_Y += go_y * g_ball_speed;

    if (g_ball_Y <= BALL_RADIUS + UPPER_BOUND)
    { // upper bound
        if (((g_ball_X - g_Player_B_X) < (LOG_WIDTH / 2)) && ((g_ball_X - g_Player_B_X) > (-LOG_WIDTH / 2)))
        { // successfully catch the ball
            g_ball_Y = BALL_RADIUS + UPPER_BOUND;
            g_game_turn = B; // change the turn
            g_bounces += 1;
        }
        else
        { // lose the game
            g_game_state = GAME_OVER;
            return;
        }
    }
    else if (g_ball_Y >= LOWER_BOUND - BALL_RADIUS)
    { // lower bound
        if (((g_ball_X - g_Player_A_X) < (LOG_WIDTH / 2)) && ((g_ball_X - g_Player_A_X) > (-LOG_WIDTH / 2)))
        { // successfully catch the ball
            g_ball_Y = LOWER_BOUND - BALL_RADIUS;
            g_game_turn = A; // change the turn
            g_bounces += 1;
        }
        else
        { // lose the game
            g_game_state = GAME_OVER;
            return;
        }
    }
    else if (g_ball_X <= BALL_RADIUS)
    { // left bound
        g_ball_X = BALL_RADIUS;
        g_ball_dir = 7 - g_ball_dir; // change direction
    }
    else if (g_ball_X >= SCREEN_WIDTH - BALL_RADIUS)
    { // right bound
        g_ball_X = SCREEN_WIDTH - BALL_RADIUS;
        g_ball_dir = 7 - g_ball_dir;
    }
    else
    {
        return;
    }
    /* touching bound causes buzzer */
    EIE3810_Toggle_Buzzer();
    Delay(100000);
    EIE3810_Toggle_Buzzer();
}

// void Ball_Move_New(void)
// {
//     /* getting the increment */
//     u32 go_x = dirs[g_ball_dir][0];
//     u32 go_y = dirs[g_ball_dir][1];
//     if (g_game_turn == A)
//         go_y = -go_y; // if A, then reverse the vertical direction
    
//     /* changing the indices */
//     g_ball_X += go_x * g_ball_speed;
//     g_ball_Y += go_y * g_ball_speed;

//     /* Collision detection and game state updates */
//     if (g_ball_Y <= g_ball_radius + UPPER_BOUND)
//     { // upper bound
//         if (((g_ball_X - g_Player_B_X) < (LOG_WIDTH / 2)) && ((g_ball_X - g_Player_B_X) > (-LOG_WIDTH / 2)))
//         { // successfully catch the ball
//             g_ball_Y = g_ball_radius + UPPER_BOUND;
//             g_game_turn = B; // change the turn
//             g_bounces += 1;
//         }
//         else
//         { // lose the game
//             g_game_state = GAME_OVER;
//             return;
//         }
//     }
//     else if (g_ball_Y >= LOWER_BOUND - g_ball_radius)
//     { // lower bound
//         if (((g_ball_X - g_Player_A_X) < (LOG_WIDTH / 2)) && ((g_ball_X - g_Player_A_X) > (-LOG_WIDTH / 2)))
//         { // successfully catch the ball
//             g_ball_Y = LOWER_BOUND - g_ball_radius;
//             g_game_turn = A; // change the turn
//             g_bounces += 1;
//         }
//         else
//         { // lose the game
//             g_game_state = GAME_OVER;
//             return;
//         }
//     }
//     else if (g_ball_X <= g_ball_radius)
//     { // left bound
//         g_ball_X = g_ball_radius;
//         g_ball_dir = 7 - g_ball_dir; // change direction
//     }
//     else if (g_ball_X >= SCREEN_WIDTH - g_ball_radius)
//     { // right bound
//         g_ball_X = SCREEN_WIDTH - g_ball_radius;
//         g_ball_dir = 7 - g_ball_dir; // change direction
//     }
//     else
//     {
//         return;
//     }

//     /* touching bound causes buzzer */
//     EIE3810_Toggle_Buzzer();
//     Delay(100000);
//     EIE3810_Toggle_Buzzer();
// }


/* Log moving control
 * Control the JOYPAD (Player B)
 * Control the KEY0 and KEY2 (Player A)
 */
void TIM4_IRQHandler(void)
{
    if (TIM4->SR & 1 << 0) // Check if the TIM4 interrupt flag is set (indicating timer overflow or event)
    {
        if (g_game_state == DIFF_LEVEL) // If the game is in "Difficulty Level" selection state
        {
            int joypad_index = EIE3810_JOYPAD_GetIndex(); // Get the current input from the JOYPAD

            switch (joypad_index)
            {
            case 4: // If Joypad UP button is pressed, set to EASY mode
                g_game_mode = EASY; // Set the game mode to EASY
                // Display "Easy" on the screen
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Easy", WHITE, BLUE, 1);
                // Display "Hard" on the screen, indicating the other option
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Hard", BLUE, WHITE, 1);
                g_ball_speed = 1;
                Delay(1000000); // Add a delay for visual feedback
                break;

            case 5: // If Joypad DOWN button is pressed, set to HARD mode
                g_game_mode = HARD; // Set the game mode to HARD
                // Display "Easy" with BLUE color (indicating it's not selected)
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Easy", BLUE, WHITE, 1);
                // Display "Hard" with WHITE text on BLUE background (indicating it's selected)
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Hard", WHITE, BLUE, 1);
                g_ball_speed = 2;
                Delay(1000000); // Add a delay for visual feedback
                break;

            case 2: // If Joypad START/SELECT button is pressed, move to the game state (USART)
                g_game_state = USART; // Change the game state to USART (indicating transition to playing)
                Delay(1000000); // Add a delay for visual feedback
                break;

            default:
                break; // No action for other buttons
            }
        }

        else if (g_game_state == BALL_SIZE)
        {
            int joypad_index = EIE3810_JOYPAD_GetIndex(); // Get the current input from the JOYPAD

            switch (joypad_index)
            {
            case 4: // If Joypad UP button is pressed, set to EASY mode
                g_game_ball = SMALL; // Set the game mode to EASY
                // Display "Easy" on the screen
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Small", WHITE, BLUE, 1);
                // Display "Hard" on the screen, indicating the other option
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Big", BLUE, WHITE, 1);
                // g_ball_size = 20;
                Delay(1000000); // Add a delay for visual feedback
                break;

            case 5: // If Joypad DOWN button is pressed, set to HARD mode
                g_game_ball = BIG; // Set the game mode to HARD
                // Display "Easy" with BLUE color (indicating it's not selected)
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Small", BLUE, WHITE, 1);
                // Display "Hard" with WHITE text on BLUE background (indicating it's selected)
                EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Big", WHITE, BLUE, 1);
                // g_ball_size = 30;
                Delay(1000000); // Add a delay for visual feedback
                break;

            case 2: // If Joypad START/SELECT button is pressed, move to the game state (USART)
                g_game_state = USART; // Change the game state to USART (indicating transition to playing)
                Delay(1000000); // Add a delay for visual feedback
                break;

            default:
                break; // No action for other buttons
            }
        }


        else if (g_game_state == PLAYING) // If the game is in PLAYING state
        {
            /* Handle Player B's movement using JOYPAD */
            int idx = EIE3810_JOYPAD_GetIndex(); // Get the current input from JOYPAD
            switch (idx)
            {
            case 3: // If START button is pressed on Joypad, pause the game
                g_game_state = PAUSE; // Change the game state to PAUSE
                Delay(1000000); // Add a delay for visual feedback
                break;

            case 6: // If LEFT button is pressed on Joypad, move Player B left
                if (g_Player_B_X > LOG_WIDTH / 2)
                {
                    g_Player_B_X -= g_Player_B_speed; // Move Player B left
                }
                else
                {
                    g_Player_B_X = LOG_WIDTH / 2; // Prevent Player B from moving off-screen
                }
                break;

            case 7: // If RIGHT button is pressed on Joypad, move Player B right
                if (g_Player_B_X < SCREEN_WIDTH - LOG_WIDTH / 2)
                {
                    g_Player_B_X += g_Player_B_speed; // Move Player B right
                }
                else
                {
                    g_Player_B_X = SCREEN_WIDTH - LOG_WIDTH / 2; // Prevent Player B from moving off-screen
                }
                break;

            default:
                break; // No action for other buttons
            }

            /* Handle Player A's movement using KEY0 and KEY2 */
            int move_right_sgn = EIE3810_Read_Key0_IDR(); // Read KEY0 status (right movement for Player A)
            int move_left_sgn = EIE3810_Read_Key2_IDR(); // Read KEY2 status (left movement for Player A)
            if (move_right_sgn == 0) // If KEY0 is pressed (move right for Player A)
            {
                if (g_Player_A_X < SCREEN_WIDTH - LOG_WIDTH / 2)
                {
                    g_Player_A_X += g_Player_B_speed; // Move Player A to the right
                }
                else
                {
                    g_Player_A_X = SCREEN_WIDTH - LOG_WIDTH / 2; // Prevent Player A from moving off-screen
                }
            }
            else if (move_left_sgn == 0) // If KEY2 is pressed (move left for Player A)
            {
                if (g_Player_A_X > LOG_WIDTH / 2)
                {
                    g_Player_A_X -= g_Player_B_speed; // Move Player A to the left
                }
                else
                {
                    g_Player_A_X = LOG_WIDTH / 2; // Prevent Player A from moving off-screen
                }
            }
        }
        else if (g_game_state == PAUSE) // If the game is in PAUSE state
        {
            int idx = EIE3810_JOYPAD_GetIndex(); // Get the current input from JOYPAD
            switch (idx)
            {
            case 3: // If START button is pressed on Joypad, resume the game
                g_game_state = PLAYING; // Change the game state to PLAYING
                Delay(1000000); // Add a delay for visual feedback
                break;

            default:
                break; // No action for other buttons
            }
        }
    }

    TIM4->SR &= ~(1 << 0); // Clear the TIM4 interrupt flag (acknowledge the interrupt)
}



/* init introduction screen */
void game_state_INTRO_init(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    Delay(10000000);
    EIE3810_TFTLCD_ShowString(INTRO_X, INTRO_Y, "Welcome to mini Project!", WHITE, BLUE, 2);
    Delay(10000000);
    EIE3810_TFTLCD_ShowString(INTRO_X + 30, INTRO_Y + 40, "This is the Final Lab.", WHITE, RED, 1);
    Delay(10000000);
    EIE3810_TFTLCD_ShowString(INTRO_X + 30, INTRO_Y + 70, "Are you ready?", WHITE, RED, 1);
    Delay(10000000);
    EIE3810_TFTLCD_ShowString(INTRO_X + 30, INTRO_Y + 100, "OK! Let's start.", WHITE, RED, 1);
    Delay(10000000);
    g_game_state = DIFF_LEVEL;
}

/* init difficulty level screen */
void game_state_DIFF_LEVEL_init(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y, "Please select the difficulty level:", WHITE, RED, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Easy", WHITE, BLUE, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Hard", BLUE, WHITE, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 90, "Press KEY0 or SELECT to enter.", WHITE, RED, 1);
    //Delay(1000000);
    //g_game_state = BALL_SIZE;
}

/* init ball size screen */
void game_state_BALL_SIZE_init(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y, "Please select the ball size:", WHITE, RED, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Small", WHITE, BLUE, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Big", BLUE, WHITE, 1);
    EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 90, "Press KEY0 or SELECT to enter.", WHITE, RED, 1);
}

/* init USART sending screen */
void game_state_USART_init(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    EIE3810_TFTLCD_ShowString(USART_X, USART_Y, "Use USART for a random direction.", WHITE, RED, 2);
}

/* starting counting down */
void game_state_COUNT_DOWN(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    for (int i = 3; i != 0; i--)
    { // each time, show a number i
        EIE3810_TFTLCD_SevenSegment(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 70, i, RED, WHITE);
        Delay(10000000);
        EIE3810_TFTLCD_Clear(WHITE);
    }
    g_game_state = PREPARE;
    EIE3810_TFTLCD_Clear(WHITE);
}

/* init playing screen */
void game_state_PLAYING_init(void)
{
    /* init info */
    g_Player_A_X = SCREEN_WIDTH / 2;
    g_Player_B_X = SCREEN_WIDTH / 2;
    g_ball_X = SCREEN_WIDTH / 2;
    g_ball_Y = EDGE_SIZE + LOG_HEIGHT + BALL_RADIUS;
    g_e_time = 0;
    g_bounces = 0;
    /* clear the screen */
    EIE3810_TFTLCD_Clear(WHITE);
    /* draw initial resources */
    PLAYING_update();
    Delay(10000000);
    /* after delay, go to playing state */
    g_game_state = PLAYING;
}

/* show game over screen */
void game_state_GAME_OVER_init(void)
{
    EIE3810_TFTLCD_Clear(WHITE);
    if (g_game_turn == A)
    {
        EIE3810_TFTLCD_ShowString(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 80, "Player A WINS the game!", RED, WHITE, 2);
    }
    else if (g_game_turn == B)
    {
        EIE3810_TFTLCD_ShowString(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 80, "Player B WINS the game!", RED, WHITE, 2);
    }
}

/* draw the elapsed time */
void draw_e_time(u16 color)
{
    /* get each bit of time */
    int minutes = g_e_time / 60;
    int seconds = g_e_time % 60;
    int min_1st = minutes / 10;
    int min_2nd = minutes % 10;
    int sec_1st = seconds / 10;
    int sec_2nd = seconds % 10;
    /* draw time */
    EIE3810_TFTLCD_ShowString(TIME_X, TIME_Y, "Elapsed Time: ", color, WHITE, 1);
    EIE3810_TFTLCD_ShowChar1608(TIME_X + 14 * 8, TIME_Y, '0' + min_1st, color, WHITE);
    EIE3810_TFTLCD_ShowChar1608(TIME_X + 15 * 8, TIME_Y, '0' + min_2nd, color, WHITE);
    EIE3810_TFTLCD_ShowString(TIME_X + 16 * 8, TIME_Y, "m", color, WHITE, 1);
    EIE3810_TFTLCD_ShowChar1608(TIME_X + 17 * 8, TIME_Y, '0' + sec_1st, color, WHITE);
    EIE3810_TFTLCD_ShowChar1608(TIME_X + 18 * 8, TIME_Y, '0' + sec_2nd, color, WHITE);
    EIE3810_TFTLCD_ShowString(TIME_X + 19 * 8, TIME_Y, "s", color, WHITE, 1);
}

/* draw the bounces time */
void draw_bounces(u16 color)
{
    /* get each bit of bounces */
    int boun_1st = g_bounces / 10;
    int boun_2nd = g_bounces % 10;
    /* draw bounces */
    EIE3810_TFTLCD_ShowString(ROUND_X, ROUND_Y, "Bounces: ", color, WHITE, 1);
    EIE3810_TFTLCD_ShowChar1608(ROUND_X + 9 * 8, ROUND_Y, '0' + boun_1st, color, WHITE);
    EIE3810_TFTLCD_ShowChar1608(ROUND_X + 10 * 8, ROUND_Y, '0' + boun_2nd, color, WHITE);
}

/* update the screen when playing */
void PLAYING_update(void)
{
    /* BALL local clear */
    EIE3810_TFTLCD_DrawRectangle(g_ball_X_pre - 1.5 * BALL_RADIUS, BALL_RADIUS * 3, g_ball_Y_pre - 1.5 * BALL_RADIUS, BALL_RADIUS * 3, WHITE);
    /* BALL local update */
    EIE3810_TFTLCD_DrawCircle(g_ball_X, g_ball_Y, BALL_RADIUS, 1, RED);

    /* Player A log update method */
    EIE3810_TFTLCD_DrawRectangle(0, g_Player_A_X - LOG_WIDTH / 2, PLAYER_A_Y - LOG_HEIGHT / 2, LOG_HEIGHT, WHITE);
    EIE3810_TFTLCD_DrawRectangle(g_Player_A_X - LOG_WIDTH / 2, LOG_WIDTH, PLAYER_A_Y - LOG_HEIGHT / 2, LOG_HEIGHT, BLACK);
    EIE3810_TFTLCD_DrawRectangle(g_Player_A_X + LOG_WIDTH / 2, SCREEN_WIDTH - (g_Player_A_X + LOG_WIDTH / 2), PLAYER_A_Y - LOG_HEIGHT / 2, LOG_HEIGHT, WHITE);

    /* Player B log update method */
    EIE3810_TFTLCD_DrawRectangle(0, g_Player_B_X - LOG_WIDTH / 2, PLAYER_B_Y - LOG_HEIGHT / 2, LOG_HEIGHT, WHITE);
    EIE3810_TFTLCD_DrawRectangle(g_Player_B_X - LOG_WIDTH / 2, LOG_WIDTH, PLAYER_B_Y - LOG_HEIGHT / 2, LOG_HEIGHT, BLACK);
    EIE3810_TFTLCD_DrawRectangle(g_Player_B_X + LOG_WIDTH / 2, SCREEN_WIDTH - (g_Player_B_X + LOG_WIDTH / 2), PLAYER_B_Y - LOG_HEIGHT / 2, LOG_HEIGHT, WHITE);

    /* info update */
    draw_e_time(BLACK);
    draw_bounces(BLACK);
}

/* KEY0, step the state */
void EXTI4_IRQHandler(void)
{
    if (g_game_state == DIFF_LEVEL)
    { // in diff leve selection state, go to USART state
        g_game_state = USART;
        Delay(1000000);
    }
    else if (g_game_state == BALL_SIZE)
    { // in diff leve selection state, go to USART state
        g_game_state = USART;
        Delay(1000000);
    }
    EXTI->PR = 1 << 4; // Clear the pending status of EXTI4
}

/* KEY1 select HARD level, pause the game */
void EXTI3_IRQHandler(void)
{
    if (g_game_state == DIFF_LEVEL)
    { // in diff leve selection state, select HARD level
        g_game_mode = HARD;
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Easy", BLUE, WHITE, 1);
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Hard", WHITE, BLUE, 1);
    }
    else if (g_game_state == BALL_SIZE)
    {
        g_game_mode = BIG;
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Small", BLUE, WHITE, 1);
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Big", WHITE, BLUE, 1);
    }
    
    else if (g_game_state == PLAYING)
    { // in playing state, pause the game
        g_game_state = PAUSE;
        Delay(1000000);
    }
    else if (g_game_state == PAUSE)
    { // in pause state, continue the game
        g_game_state = PLAYING;
        Delay(1000000);
    }
    EXTI->PR = 1 << 3; // Clear the pending status of EXTI3
}

/* Key_Up, select the easy mode */
void EXTI0_IRQHandler(void)
{
    if (g_game_state == DIFF_LEVEL)
    { // in diff leve selection state, select EASY level
        g_game_mode = EASY;
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Easy", WHITE, BLUE, 1);
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Hard", BLUE, WHITE, 1);
        Delay(1000000);
    }
    else if (g_game_state == BALL_SIZE)
    {
        g_game_mode = SMALL;
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 30, "Small", WHITE, BLUE, 1);
        EIE3810_TFTLCD_ShowString(LEVEL_X, LEVEL_Y + 60, "Big", BLUE, WHITE, 1);
        Delay(1000000);
    }
    EXTI->PR = 1;// Clear the pending status of EXTI0
}

void EXTI2_IRQHandler(void){
		if (g_game_state == PLAYING){
			}
			EXTI -> PR =1<<2;
}

/* USART1, get the random number */
void USART1_IRQHandler(void)
{
    u32 num; // to store the received number
    if (USART1->SR & (1 << 5))
    {
        if (g_game_state == USART)
        {                     // If read data register is not empty
            num = USART1->DR; // Read the received data character
            g_ball_dir = num;
            EIE3810_TFTLCD_ShowString(USART_X + 40, USART_Y + 40, "The random number received is: ", WHITE, RED, 1);
            EIE3810_TFTLCD_ShowChar1608(USART_X + 40 + 31 * 8, USART_Y + 40, num + '0', WHITE, RED);
            g_game_state = COUNT_DOWN;
            Delay(1000000);
        }
    }
}
