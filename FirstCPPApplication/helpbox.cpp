#include "stdafx.h"
#include "helpbox.h"
#include "draw_utils.h"
#include "tile.h"
#include "actors\actor.h"

void HelpBox::initialize()
{
    this->left_pad=1;
    this->right_pad=2;
    this->top_pad=1;
    this->bot_pad=2;
    this->con = NULL;
    this->selected_tile = NULL;

    this->extra_padding = 100;
};
HelpBox::HelpBox()
{
    this->initialize();
};

HelpBox::~HelpBox()
{
    if (this->con != NULL)
    {
        delete this->con;
    }
};

HelpBox::HelpBox(std::vector<std::string> messages, TCODConsole* target_con, Tile* selected_tile) 
{
    this->initialize();
    this->selected_tile = selected_tile;
    this->target_con = target_con;
    this->messages = messages;
    int max_width = 19;
    int line_width = 0;
    int line_height = messages.size()*2;
    // printf("%d\n", max_height);

    std::vector<std::string>::iterator it = messages.begin();
    for (it; it!= messages.end(); it++)
    {
        line_width = (int)std::max((double)(*it).size(), (double)line_width);
    };

    this->width = line_width+left_pad+right_pad;
    this->height = line_height+top_pad+bot_pad;
    this->con = new TCODConsole(this->width+this->extra_padding, this->height+this->extra_padding);
};

void HelpBox::draw()
{
    draw_rect(this->con, 0, 0, this->width, this->height);//, "?", "?", "?", "?", "?", "?", "?", "?");
    int y = 1;
    std::vector<std::string>::iterator it = messages.begin();
    for (it; it!= messages.end(); it++)
    {
        this->con->print(1+this->left_pad, y+this->top_pad, (*it).c_str());
        y++;
        y++;
    };

    if (!selected_tile->is_known())
    {
        this->con->setDefaultForeground(TCODColor::darkestGrey);
    };

    if (!selected_tile->is_known())
        draw_rect(this->con, 0, 0, this->width, this->height, "?", "?", "?", "?", "?", "?", "?", "?");
    else
        draw_rect(this->con, 0, 0, this->width, this->height);

    if (selected_tile->occupant != NULL)
    {
        TCODImage* img = selected_tile->occupant->get_image();
        img->blit(this->con, 9, 12);
        delete img;
    };

    TCODConsole::root->blit(this->con, 0, 0, this->width+this->extra_padding, this->height+this->extra_padding, this->target_con, 0, 23);
    
};
