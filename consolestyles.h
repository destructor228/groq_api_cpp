#ifndef CONSOLESTYLES_H
#define CONSOLESTYLES_H

#include <string>

namespace ConsoleStyles {

    // Text colors
    extern const std::string RESET;        // Reset
    extern const std::string BLACK;        // Black
    extern const std::string RED;          // Red
    extern const std::string GREEN;        // Green
    extern const std::string YELLOW;       // Yellow
    extern const std::string BLUE;         // Blue
    extern const std::string MAGENTA;      // Magenta
    extern const std::string CYAN;         // Cyan
    extern const std::string WHITE;        // White
    extern const std::string BRIGHT_BLACK; // Bright black
    extern const std::string BRIGHT_RED;   // Bright red
    extern const std::string BRIGHT_GREEN; // Bright green
    extern const std::string BRIGHT_YELLOW;// Bright yellow
    extern const std::string BRIGHT_BLUE;  // Bright blue
    extern const std::string BRIGHT_MAGENTA;// Bright magenta
    extern const std::string BRIGHT_CYAN;  // Bright cyan
    extern const std::string BRIGHT_WHITE; // Bright white

    // Background colors
    extern const std::string BG_BLACK;        // Black
    extern const std::string BG_RED;         // Red
    extern const std::string BG_GREEN;       // Green
    extern const std::string BG_YELLOW;      // Yellow
    extern const std::string BG_BLUE;        // Blue
    extern const std::string BG_MAGENTA;     // Magenta
    extern const std::string BG_CYAN;        // Cyan
    extern const std::string BG_WHITE;       // White
    extern const std::string BG_BRIGHT_BLACK;// Bright black
    extern const std::string BG_BRIGHT_RED;  // Bright red
    extern const std::string BG_BRIGHT_GREEN;// Bright green
    extern const std::string BG_BRIGHT_YELLOW;// Bright yellow
    extern const std::string BG_BRIGHT_BLUE; // Bright blue
    extern const std::string BG_BRIGHT_MAGENTA;// Bright magenta
    extern const std::string BG_BRIGHT_CYAN; // Bright cyan
    extern const std::string BG_BRIGHT_WHITE;// Bright white

    // Text styles
    extern const std::string BOLD;      // Bold
    extern const std::string DIM;       // Dim
    extern const std::string ITALIC;    // Italic
    extern const std::string UNDERLINE; // Underline
    extern const std::string BLINK;     // Blink
    extern const std::string INVERT;    // Invert
    extern const std::string HIDDEN;    // Hidden
    extern const std::string STRIKETHROUGH; // Strikethrough

    // Apply style to text
    std::string apply(const std::string& text, const std::string& style);

    void allsample();
}

#endif // CONSOLESTYLES_H