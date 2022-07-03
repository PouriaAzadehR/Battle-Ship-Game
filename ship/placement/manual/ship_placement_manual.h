#ifndef SEA_BATTLE_SHIP_PLACEMENT_MANUAL_H
#define SEA_BATTLE_SHIP_PLACEMENT_MANUAL_H

#include "../../../auto_import.h"

Player handleManualShipPlacement(User user);

Player putShipsManually(Player player);


Ship *putShip(Ship *head, int count, int width, char map[10][10]);


bool shipCanBePlacedInCoordinates(int shipWidth, char map[10][10], Coordinate start, Coordinate end);

Ship *addShipToList(Ship *head, int width, Coordinate start, Coordinate end);

Ship *findTale(Ship *head);

void updateMap(char map[10][10], Coordinate start, Coordinate end);

bool shipIsHorizontal(Coordinate start, Coordinate end);

bool shipIsVertical(Coordinate start, Coordinate end);


#endif //SEA_BATTLE_SHIP_PLACEMENT_MANUAL_H
