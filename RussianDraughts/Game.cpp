//
//  game.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#include <stdio.h>
#include "Board.cpp"
#include "Piece.cpp"
#include <unordered_map>

class game{
    board *B;
    bool isLightTurnToMove;
    bool endGame;
    unordered_map<int,piece*> dark_normal, light_normal;
    unordered_map<int,piece*> dark_queen, light_queen;
    vector<vector<int>> dirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
    const vector<string>  names= {"Dark", "Light"};

public:
    game(){
        isLightTurnToMove = true;
        endGame = false;
        B = new board();
        isLightTurnToMove = true;
        for(int i=0;i<8;++i)
            for(int j =0;j<8;++j){
                if(i<3&&B->positions[i][j])
                    dark_normal[8*i+j] = new piece(i,j,true,false);
                else if(i>4&&B->positions[i][j])
                    light_normal[8*i+j] = new piece(i,j,true,true);
            }
    }
    ~game(){
        delete B;
    };
    
    void printPositions(){
        cout<<"DN: dark normal pieces, DQ: dark queen, LN: light normal pieces, LQ: light queen pieces"<<endl;
        cout<<"the index of the top-left of the board is [0,0], and increases from left to right and top to bottom!"<<endl;
        for(int r = 0; r<8;++r){
            for(int c=0;c<8;++c){
                if(dark_normal.count(r*8+c))
                    cout<<"DN";
                else if(light_normal.count(r*8+c))
                    cout<<"LN";
                else if(dark_queen.count(r*8+c))
                    cout<<"DQ";
                else if(light_queen.count(r*8+c))
                    cout<<"LQ";
                else
                    cout<<"##";
                cout<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    
    void start(){
        cout<<"Initial state of the board: "<<endl;
        printPositions();
        while(!endGame){
            int idx = isLightTurnToMove;
            cout<<"It is " + names[idx] + " player's turn to move."<<endl;
            if(isLightTurnToMove){
                piece* chosen = selectAPiecetoMove();
                move(chosen);
                isLightTurnToMove = false;
            }else{
                piece* chosen = selectAPiecetoMove();
                move(chosen);
                isLightTurnToMove = true;
            }
            gameStatusCheck();
            printPositions();
        }
    }
    
    piece* selectAPiecetoMove(){
        int x, y;
        cout<<"Please select a piece to move!"<<endl;
        cin>>x>>y;
        int index = x*8+y;
        if(isLightTurnToMove){
            if(light_normal.count(index))
                return light_normal[index];
            else if(light_queen.count(index))
                return light_queen[index];
            else{
                cout<<"Please select a valid light piece!"<<endl;
                return NULL;
            }
        }else{
            if(dark_normal.count(index))
                return dark_normal[index];
            else if(dark_queen.count(index))
                return dark_queen[index];
            else{
                cout<<"Please select a valid dark piece!"<<endl;
                return NULL;
            }
        }
    }
    

    bool insideTheBoard(int x, int y){
        return x>=0&&x<8&&y>=0&&y<8;
    }
    
    bool isEmpty(int x, int y){
        int cur_index = 8*x+y;
        return !light_normal.count(cur_index)&&
        !light_queen.count(cur_index)&&
        !dark_normal.count(cur_index)&&
        !dark_queen.count(cur_index);
        
    }
    
    bool belongToDifferentPlayer(piece* cur, int nextX, int nextY){
       return (cur->getIsBelongingToLight()&&(dark_normal.count(8*nextX+nextY)||dark_queen.count(8*nextX+nextY)))||(!cur->getIsBelongingToLight()&&(light_normal.count(8*nextX+nextY)||light_queen.count(8*nextX+nextY)));
    }
    
    
    bool canAQueenJumpEat(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];

            while(insideTheBoard(next_x, next_y)&&isEmpty(next_x, next_y)){
                next_x = cur->getX()+dir[0];
                next_y = cur->getY()+dir[1];
            }
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAQueenShift(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAQueenMove(piece* cur){
        return canAQueenShift(cur)||canAQueenJumpEat(cur);
    }
    
    bool canANormalPieceJumpEat(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canANormalPieceShift(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            if(insideTheBoard(next_x, next_y)&&isEmpty(next_x, next_y))
                return true;
        }
        return false;
    }
    
    bool canANormalPieceMove(piece* cur){
        return canANormalPieceShift(cur)||canANormalPieceJumpEat(cur);
    }
    
    
    void move(piece* &current_piece){
        int original_index = 8*current_piece->getX()+current_piece->getY();
        int dir_index;
        cout<<"---------Please input the index of direction--------"<<endl;
        cout<<"0: left-up, 1: right-up, 2: left-down, 3: right-down"<<endl;
        if(current_piece->getNormal()){
            
            if(canANormalPieceJumpEat(current_piece)){
                cin>>dir_index;
                int next_x =current_piece->getX()+dirs[dir_index][0];
                int next_y =current_piece->getY()+dirs[dir_index][1];
                int next_of_next_x = current_piece->getX()+2*dirs[dir_index][0];
                int next_of_next_y = current_piece->getY()+2*dirs[dir_index][1];

                if(!insideTheBoard(next_of_next_x, next_of_next_y)||
                   !isEmpty(next_of_next_x, next_of_next_y))
                    return;
                
                current_piece->setX(next_of_next_x);
                current_piece->setY(next_of_next_y);
                if(current_piece->getIsBelongingToLight()){
                    light_normal.erase(original_index);
                    light_normal[8*next_of_next_x+next_of_next_y] = current_piece;
                    int next_index = 8*next_x+next_y;
                    if(dark_normal.count(next_index))
                        dark_normal.erase(next_index);
                    else
                        dark_queen.erase(next_index);
                }else{
                    dark_normal.erase(original_index);
                    dark_normal[8*next_of_next_x+next_of_next_y] = current_piece;
                    int next_index = 8*next_x+next_y;
                    if(light_normal.count(next_index))
                        light_normal.erase(next_index);
                    else
                        light_queen.erase(next_index);
                }
            }else{
                cin>>dir_index;
                int next_x =current_piece->getX()+dirs[dir_index][0];
                int next_y =current_piece->getY()+dirs[dir_index][1];
                
                current_piece->setX(next_x);
                current_piece->setY(next_y);
                
                
                
                if(current_piece->getIsBelongingToLight()){
                    if(next_x!=0){
                        light_normal.erase(original_index);
                        int next_index = 8*current_piece->getX()+current_piece->getY();
                        light_normal[next_index] = current_piece;
                    }else{
                        current_piece->changeToQueen();
                        light_normal.erase(original_index);
                        int next_index = 8*current_piece->getX()+current_piece->getY();
                        light_queen[next_index] = current_piece;
                    }
                }else{
                    if(next_x!=7){
                        dark_normal.erase(original_index);
                        int next_index = 8*current_piece->getX()+current_piece->getY();
                        dark_normal[next_index] = current_piece;
                    }else{
                        current_piece->changeToQueen();
                        dark_normal.erase(original_index);
                        int next_index = 8*current_piece->getX()+current_piece->getY();
                        dark_queen[next_index] = current_piece;
                    }
                }
            }
            // to do: finish the moving strategy for queen
        }else{
            if(canAQueenJumpEat(current_piece)){
                
            }
            else if(canAQueenShift(current_piece)){
                
            }
        }
    }
    
    void gameStatusCheck(){
        bool light_nor = false;
        bool light_que = false;
        bool dark_nor = false;
        bool dark_que = false;
        //check if light or dark has no pieces
        if (
            (light_normal.empty()&&light_queen.empty())||
            (dark_normal.empty()&&dark_queen.empty())
            ){
            endGame = true;
            return;
        }
        // check if all light's or dark's has no place to move
        for(auto pair:light_normal){
            if(canANormalPieceMove(pair.second)){
                light_nor = true;
                break;
            }
            
        }
        for(auto pair:dark_normal){
            if(canANormalPieceMove(pair.second)){
                dark_nor = true;
                break;
            }
        }
        for(auto pair:light_queen){
            if(canAQueenMove(pair.second)){
                light_que = true;
                break;
            }
        }
        for(auto pair:dark_queen){
            if(canAQueenMove(pair.second)){
                dark_que = true;
                break;
            }
        }
        if(!(light_nor||light_que||dark_nor||dark_que)){
            endGame = true;
            return;
        }

    }

};
