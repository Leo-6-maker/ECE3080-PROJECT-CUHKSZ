# ECE3080 Bouncing Ball Game

A 2-player bouncing ball game developed for the ECE3080 Microprocessors and Computer Systems course at The Chinese University of Hong Kong, Shenzhen.

## 🎮 Project Overview

This project implements a complete 2-player bouncing ball game using embedded systems concepts. The game features real-time player controls, collision detection, multiple game states, and visual feedback through an LCD display.

## ✨ Features

### Core Gameplay
- **2-Player Competition**: Player A (buttons) vs Player B (joypad)
- **Real-time Ball Physics**: Ball movement with collision detection
- **Dynamic Difficulty**: Easy and Hard modes with different ball speeds
- **Customizable Ball Size**: Small and Big ball options
- **Game State Management**: Comprehensive state machine handling

### User Interface
- **Welcome Screen**: Introductory messages and instructions
- **Difficulty Selection**: Choose between Easy and Hard modes
- **Ball Size Selection**: Optional ball size customization
- **Real-time Status Display**: Game duration and bounce count
- **Pause Functionality**: Both players can pause/resume the game
- **Win/Lose Screens**: Clear victory conditions and messages

### Technical Features
- **USART Communication**: Random number generation for ball direction
- **Interrupt Handling**: Responsive button and joypad inputs
- **Timer Management**: SYSTICK for game loop timing
- **Buzzer Feedback**: Audio cues for collisions
- **Boundary Detection**: Comprehensive collision handling

## 🛠️ Technical Implementation

### Game States
```c
INTRO = 6,        // Game introduction
DIFF_LEVEL = 1,   // Difficulty selection
BALL_SIZE = 2,    // Ball size selection
USART = 3,        // USART communication
COMMT_DOWN = 4,   // Countdown
PREFERE = 5,      // Preparation
PLATINE = 6,      // Playing state
PAUSE = 7,        // Paused state
GAME_GAVE = 8     // Game over
```

### Control Scheme
**Player A (Buttons):**
- KEY0: Move right
- KEY2: Move left  
- KEY1: Pause/Resume

**Player B (Joypad):**
- LEFT/RIGHT: Move horizontally
- START: Pause/Resume
- UP/DOWN: Menu navigation

### Key Components
- **Ball Movement**: Direction-based movement with collision physics
- **Stick Control**: Real-time paddle movement with boundary checks
- **Collision Detection**: Comprehensive boundary and paddle collision
- **USART Integration**: Random direction initialization
- **State Management**: Smooth transitions between game phases

## 🎯 How to Play

1. **Startup**: System displays welcome message
2. **Difficulty Selection**: Choose Easy or Hard mode using buttons/joypad
3. **Ball Size** (Optional): Select ball size if enabled
4. **USART Initialization**: Receive random direction from PC
5. **Countdown**: 3-second preparation phase
6. **Gameplay**: 
   - Control your paddle to bounce the ball
   - Prevent the ball from reaching your boundary
   - Score by making the opponent miss
7. **Pause**: Use START button or KEY1 to pause/resume
8. **Game Over**: Victory screen displays the winner

## 📊 Experiment 2 Enhancements

The project includes an improved version with:
- **Variable Ball Size**: Dynamic ball radius selection
- **Enhanced Collision Logic**: Updated physics for different ball sizes
- **Extended Menu System**: Additional selection screens

## 🏆 Winning Conditions

- Player wins when opponent fails to catch the ball
- Ball touching upper/lower boundary ends the game
- Victory message displays the winning player

## 📝 Conclusion

This project successfully demonstrates the application of microprocessor concepts including:
- Interrupt handling and timer management
- USART communication protocols
- Real-time graphics rendering
- State machine implementation
- User input processing
- Game physics and collision detection

The implementation provides a solid foundation for embedded game development and showcases comprehensive system integration skills.

---

**Developer**: Liu Yu   
**Course**: ECE3080 Microprocessors and Computer Systems  
**Institution**: The Chinese University of Hong Kong, Shenzhen  
**Date**: December 2024
