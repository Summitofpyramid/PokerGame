//
//  Board.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

class board{
public:
    vector<vector<int>> positions=vector<vector<int>>(8, vector<int>(8, 0));
    board(){
        for(int i=0;i<8;++i)
            for(int j=0;j<8;++j)
                if(((i&1)==0&&(j&1)==1)||
                   ((i&1)==1&&(j&1)==0))
                    positions[i][j] = 1;
    }
    
    
};
