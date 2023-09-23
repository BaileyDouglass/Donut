// Based on Andy Sloane's proper donut shaped code;
// This is the code reformatted to better show the matrix reprojection
// Also added OS based directives to remove terminal flicker

// Slighty modified code by shifting donut down and drawing more lines;
// Can never have too much donut!!

// TODO: Add more donuts..

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle((DWORD)-11);  // STD_OUTPUT_HANDLE = -11
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = 0;  // FALSE
    SetConsoleCursorInfo(consoleHandle, &info);
}
void clearScreen() {
    system("cls");
}
#elif __linux__
void hideCursor() {
    printf("\e[?25l");
}
void clearScreen() {
    printf("\x1b[2J");
}
#else
void hideCursor() {}
void clearScreen() {}
#endif

void rotate_donut() {
    // Constants for the screen dimensions and other frequently used numbers
    const int screenWidth = 80;
    const int screenHeight = 26;
    const int bufferLength = screenWidth * screenHeight;
    const float twoPi = 6.28;
    const float rotationIncrementA = 0.04;
    const float rotationIncrementB = 0.02;
    const float angleStepI = 0.02;
    const float angleStepJ = 0.07;

    int bufferIndex;  // Loop counter for the rendering loop
    float angleA = 0;  // Rotation angle A
    float angleB = 0;  // Rotation angle B
    float depthBuffer[bufferLength];  // Depth buffer
    char frameBuffer[bufferLength];  // Frame buffer

    for (;;) {  // Infinite loop for continuous rendering
        // Initialize buffers
        memset(frameBuffer, 32, bufferLength * sizeof(char));
        memset(depthBuffer, 0, bufferLength * sizeof(float));

        // Loop through each angle increment
        for (float angleJ = 0; twoPi > angleJ; angleJ += angleStepJ)
            for (float angleI = 0; twoPi > angleI; angleI += angleStepI) {

                // Perform trigonometric operations for rotations
                float sinI = sin(angleI), cosJ = cos(angleJ), sinA = sin(angleA);
                float sinJ = sin(angleJ), cosA = cos(angleA);
                float cosI = cos(angleI), cosB = cos(angleB), sinB = sin(angleB);

                // Intermediate calculations for 3D transformations
                float h = cosJ + 2;
                float D = 1 / (sinI * h * sinA + sinJ * cosA + 5);
                float t = sinI * h * cosA - sinJ * sinA;

                // Project the 3D point onto 2D space
                int x = 40 + 30 * D * (cosI * h * cosB - t * sinB);
                int y = 14 + 15 * D * (cosI * h * sinB + t * cosB);

                // Compute linear index for 1D depth and frame buffers
                int linearIndex = x + screenWidth * y;

                // Calculate brightness based on surface normal
                int brightnessIndex = 8 * ((sinJ * sinA - sinI * cosJ * cosA) * cosB - sinI * cosJ * sinA - sinJ * cosA - cosI * cosJ * sinB);

                // Update depth and frame buffer if point is closer than the previous point
                if (screenHeight > y && y > 0 && x > 0 && screenWidth > x && D > depthBuffer[linearIndex]) {
                    depthBuffer[linearIndex] = D;
                    frameBuffer[linearIndex] = ".,-~:;=!*#$@"[brightnessIndex > 0 ? brightnessIndex : 0];
                }
            }

        // Render the frame
        printf("\x1b[H");
        for (bufferIndex = 0; bufferIndex < bufferLength + 1; bufferIndex++) {
            putchar(bufferIndex % screenWidth ? frameBuffer[bufferIndex] : 10);
        }

        // Update rotation angles
        angleA += rotationIncrementA;
        angleB += rotationIncrementB;
    }
}

int main() {
    std::cout << "Starting donut renderer...\n";
    hideCursor();
    clearScreen();
    
    rotate_donut();

    return 0;
}
