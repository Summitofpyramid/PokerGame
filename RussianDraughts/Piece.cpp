//
//  Piece.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#include <stdio.h>

class piece{

    bool normal;
    int x, y;
    bool belongToLight;
public:
    piece(int _x, int _y, bool _normal, bool _belongToLight){
        x = _x;
        y = _y;
        normal = _normal;
        belongToLight = _belongToLight;
    }
    
    int getX(){return x;}
    int getY(){return y;}
    void setX(int _x){x = _x;}
    void setY(int _y){y = _y;}
    bool getNormal(){return normal;}
    bool getIsBelongingToLight(){return belongToLight;}
    
    void changeToQueen(){
        normal = false;
    }

};
