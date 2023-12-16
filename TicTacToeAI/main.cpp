#include "game.hpp"

int main()
{
    bool should_continue = true;
    screen_title();
    while (should_continue)
    {
        char c = screen_menu();
        switch (c)
        {
            case '1':
                screen_game(false);
                break;
            case '2':
                screen_game(true);
                break;
            case '3':
                screen_options();

                break;
            case '4':
                screen_exit();
                should_continue = false;
                break;
            default:
                break;
        }
    }
    return 0;
} 