#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

class TCODConsole;
void draw_rect(TCODConsole* con, int x, int y, int w, int h, 
        std::string top_c = "-", std::string right_c = "|", std::string bot_c="-", std::string left_c="|",
        std::string tr_c="\\", std::string br_c="/", std::string bl_c="\\", std::string tl_c="/" );

#endif
