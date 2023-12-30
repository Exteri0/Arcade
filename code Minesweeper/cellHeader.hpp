#include <bits/stdc++.h>
using namespace std;
class Cell
{
    bool flagged, mined, open;
    unsigned char count;
    unsigned char mouseHover;
    unsigned char xIndex;
    unsigned char yIndex;

public:
    Cell(unsigned char xInput, unsigned char yInput);

    bool getFlag();
    bool checkMine();
    bool getOpen();
    bool open(vector<Cell> &cells);

    unsigned char get_effect_timer();
    unsigned char get_mines_around();
    unsigned char get_mouse_state();

    void count_mines_around(std::vector<Cell> &cells);
    void flag();
    void reset();
    void set_effect_timer(unsigned char i_effect_timer);
    void set_mine();
    void set_mouse_state(unsigned char i_mouse_state);
};