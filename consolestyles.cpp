#include "consolestyles.h"

#include <iostream>

namespace ConsoleStyles {

    // text colors
    const std::string RESET = "\033[0m";
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";

    // background colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    const std::string BG_BRIGHT_BLACK = "\033[100m";
    const std::string BG_BRIGHT_RED = "\033[101m";
    const std::string BG_BRIGHT_GREEN = "\033[102m";
    const std::string BG_BRIGHT_YELLOW = "\033[103m";
    const std::string BG_BRIGHT_BLUE = "\033[104m";
    const std::string BG_BRIGHT_MAGENTA = "\033[105m";
    const std::string BG_BRIGHT_CYAN = "\033[106m";
    const std::string BG_BRIGHT_WHITE = "\033[107m";

    // text styles
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string ITALIC = "\033[3m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK = "\033[5m";
    const std::string INVERT = "\033[7m";
    const std::string HIDDEN = "\033[8m";
    const std::string STRIKETHROUGH = "\033[9m";

    // Check if text is color
    bool isColor(const std::string& text) 
    {
        return false;
    }
    // Apply a style to a text
    std::string apply(const std::string& text, const std::string& style) {
        return style + text + RESET;
    }

    void allsample()
    {
        // Examples of text color application
        std::cout << apply("Black text", BLACK) << std::endl;
        std::cout << apply("Red text", RED) << std::endl;
        std::cout << apply("Green text", GREEN) << std::endl;
        std::cout << apply("Yellow text", YELLOW) << std::endl;
        std::cout << apply("Blue text", BLUE) << std::endl;
        std::cout << apply("Magenta text", MAGENTA) << std::endl;
        std::cout << apply("Cyan text", CYAN) << std::endl;
        std::cout << apply("White text", WHITE) << std::endl;

        // Examples of bright text color application
        std::cout << apply("Bright black text", BRIGHT_BLACK) << std::endl;
        std::cout << apply("Bright red text", BRIGHT_RED) << std::endl;
        std::cout << apply("Bright green text", BRIGHT_GREEN) << std::endl;
        std::cout << apply("Bright yellow text", BRIGHT_YELLOW) << std::endl;
        std::cout << apply("Bright blue text", BRIGHT_BLUE) << std::endl;
        std::cout << apply("Bright magenta text", BRIGHT_MAGENTA) << std::endl;
        std::cout << apply("Bright cyan text", BRIGHT_CYAN) << std::endl;
        std::cout << apply("Bright white text", BRIGHT_WHITE) << std::endl;

        // Examples of background color application
        std::cout << apply("Text with black background", BG_BLACK) << std::endl;
        std::cout << apply("Text with red background", BG_RED) << std::endl;
        std::cout << apply("Text with green background", BG_GREEN) << std::endl;
        std::cout << apply("Text with yellow background", BG_YELLOW) << std::endl;
        std::cout << apply("Text with blue background", BG_BLUE) << std::endl;
        std::cout << apply("Text with magenta background", BG_MAGENTA) << std::endl;
        std::cout << apply("Text with cyan background", BG_CYAN) << std::endl;
        std::cout << apply("Text with white background", BG_WHITE) << std::endl;

        // Examples of bright background color application
        std::cout << apply("Text with bright black background", BG_BRIGHT_BLACK) << std::endl;
        std::cout << apply("Text with bright red background", BG_BRIGHT_RED) << std::endl;
        std::cout << apply("Text with bright green background", BG_BRIGHT_GREEN) << std::endl;
        std::cout << apply("Text with bright yellow background", BG_BRIGHT_YELLOW) << std::endl;
        std::cout << apply("Text with bright blue background", BG_BRIGHT_BLUE) << std::endl;
        std::cout << apply("Text with bright magenta background", BG_BRIGHT_MAGENTA) << std::endl;
        std::cout << apply("Text with bright cyan background", BG_BRIGHT_CYAN) << std::endl;
        std::cout << apply("Text with bright white background", BG_BRIGHT_WHITE) << std::endl;

        // Examples of text style application
        std::cout << apply("Bold text", BOLD) << std::endl;
        std::cout << apply("Dim text", DIM) << std::endl;
        std::cout << apply("Italic text", ITALIC) << std::endl;
        std::cout << apply("Underlined text", UNDERLINE) << std::endl;
        std::cout << apply("Blinking text", BLINK) << std::endl;
        std::cout << apply("Inverted text", INVERT) << std::endl;
        std::cout << apply("Hidden text", HIDDEN) << std::endl;
        std::cout << apply("Strikethrough text", STRIKETHROUGH) << std::endl;

        // Examples of style and color combinations
        std::cout << apply("Bold red text", BOLD + RED) << std::endl;
        std::cout << apply("Underlined green text", UNDERLINE + GREEN) << std::endl;
        std::cout << apply("Italic blue text", ITALIC + BLUE) << std::endl;
        std::cout << apply("Blinking yellow text", BLINK + YELLOW) << std::endl;
        std::cout << apply("Bold text with red background", BOLD + BG_RED) << std::endl;
        std::cout << apply("Underlined text with blue background", UNDERLINE + BG_BLUE) << std::endl;
        std::cout << apply("Inverted magenta text", INVERT + MAGENTA) << std::endl;
        std::cout << apply("Hidden green text", HIDDEN + GREEN) << std::endl;
        std::cout << apply("Strikethrough white text", STRIKETHROUGH + WHITE) << std::endl;
    }


}