#ifndef GAME_INFO_H
#define GAME_INFO_H

enum e_game_info {
    tile_num,
    piece_name,
    tile_num_click,
    piece_color
};

struct game_info {
    int tileNum, tileNum_click, pieceColor;
    char pieceName;
};

#endif // GAME_INFO_H
