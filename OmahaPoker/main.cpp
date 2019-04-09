//
//  main.cpp
//  OmahaPoker
//
//  Created by Johnson Johnson on 2019-04-05.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include "stdlib.h"
#include <unordered_set>

using namespace std;

const string STRAIGHT_FLUSH = "Straight Flush";
const string FOUR_OF_A_KIND = "4-of-a-Kind";
const string FULL_HOUSE = "Full House";
const string FLUSH = "Flush";
const string STRAIGHT = "Straight";
const string THREE_OF_A_KIND = "3-of-a-Kind";
const string TWO_PAIR = "Two Pair";
const string ONE_PAIR = "One Pair";
const string HIGH_CARD = "High card";
const string NOT_QUALIFIED_FOR_LOW = "No hand qualified for Low";
const string IDENTICAL = "Identical";

// A ranking map with int rank values
unordered_map<string, int> high_hand_ranking_dict = {
    {STRAIGHT_FLUSH, 10},
    {FOUR_OF_A_KIND, 9},
    {FULL_HOUSE, 8},
    {FLUSH, 7},
    {STRAIGHT, 6},
    {THREE_OF_A_KIND, 5},
    {TWO_PAIR, 4},
    {ONE_PAIR, 3},
    {HIGH_CARD, 2}
};

unordered_map<int, string> high_hand_ranking_dict_int = {
    {10, STRAIGHT_FLUSH},
    {9, FOUR_OF_A_KIND},
    {8, FULL_HOUSE},
    {7, FLUSH},
    {6, STRAIGHT},
    {5, THREE_OF_A_KIND},
    {4, TWO_PAIR},
    {3, ONE_PAIR},
    {2, HIGH_CARD}
};

// convert char to int
int charToInt(char c){
    int res = 0;
    if(isnumber(c))
        res = c-'0';
    else{
        switch (c) {
            case 'T':
                res = 10;
                break;
            case 'J':
                res = 11;
                break;
            case 'Q':
                res = 12;
                break;
            case 'K':
                res = 13;
                break;
        }
    }
    return res;
}

string cardValueCount(map<char, int> dict){
    string res;
    for(auto each:dict){
        res+=to_string(each.second);
    }
    sort(res.begin(), res.end());
    return res;
}

bool isIncreasingVector(const vector<int> &V){
    return V[0]+1==V[1]&&
    V[1]+1==V[2]&&
    V[2]+1==V[3]&&
    V[3]+1==V[4];
}

// compute
vector<int> computeIntValueOfCards(string cards, string type){
    vector<int> res;
    if(type == STRAIGHT_FLUSH || type == STRAIGHT){
        for(auto c:cards){
            if(c=='A')
                res.push_back(1);
            else
                res.push_back(charToInt(c));
        }
        sort(res.begin(), res.end());
        if(isIncreasingVector(res))
            return res;
        else{
            res.clear();
            for(auto c:cards){
                if(c=='A')
                    res.push_back(14);
                else
                    res.push_back(charToInt(c));
            }
        }
        sort(res.begin(), res.end());
        return res;
    }else{
        for(auto c:cards){
            if(c=='A')
                res.push_back(14);
            else
                res.push_back(charToInt(c));
        }
        sort(res.begin(), res.end());
        return res;
    }
}

bool isStraight(const vector<string> & cards){
    string card_values;
    vector<int> card_value_int, card_value_int_A14;
    for(auto card:cards)
        card_values+=card[0];
    if(card_values.find('A')!=string::npos){
        // A->1
        for(auto c:card_values){
            if(c=='A')
                card_value_int.push_back(1);
            else
                card_value_int.push_back(charToInt(c));
        }
        //A->14
        for(auto c:card_values){
            if(c=='A')
                card_value_int_A14.push_back(14);
            else
                card_value_int_A14.push_back(charToInt(c));
        }
        sort(card_value_int.begin(), card_value_int.end());
        sort(card_value_int_A14.begin(), card_value_int_A14.end());
        if(isIncreasingVector(card_value_int)||isIncreasingVector(card_value_int_A14))
            return true;
    }else{
        for(auto c:card_values)
            card_value_int.push_back(charToInt(c));
        sort(card_value_int.begin(), card_value_int.end());
        if(isIncreasingVector(card_value_int))
            return true;
    }
    return false;
}

string getHighRanking(const vector<string> & cards ){
    string cardsValue_string;
    unordered_map<char, int> card_color_count;
    map<char, int> card_value_count;
    int count_of_bigger_than_8=0;
    vector<int>cardsValue_int;
    for(auto card: cards){
        cardsValue_string+=card[0];
        card_color_count[card[1]]++;
        card_value_count[card[0]]++;
        if(card[0]=='9'||card[0]=='T'||
           card[0]=='J'||card[0]=='Q'||
           card[0]=='K')
            count_of_bigger_than_8++;
    }
    
    string cardType = cardValueCount(card_value_count);
    bool straight = isStraight(cards);
    if(card_value_count.size()==5 && card_color_count.size()==1 && straight){
        return STRAIGHT_FLUSH;
    }else if(cardType=="14"){
        return FOUR_OF_A_KIND;
    }else if(cardType=="23"){
        return FULL_HOUSE;
    }else if(card_value_count.size()==5 && card_color_count.size()==1&&!straight){
        return FLUSH;
    }else if(card_value_count.size()==5 && card_color_count.size()!=1&&straight){
        return STRAIGHT;
    }else if(cardType=="113"){
        return THREE_OF_A_KIND;
    }else if(cardType=="122"){
        return TWO_PAIR;
    }else if(cardType=="1112"){
        return ONE_PAIR;
    }else{
        return HIGH_CARD;
    }
}

// two vectors, in each vector the values are different
// for high hand, the smaller the number is, the bigger its ranking is
bool compareTwoVectorForHigh(const vector<int> &A, const vector<int> &B){
    return A[4] > B[4] ||
    (A[4] == B[4] && A[3] > B[3]) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] > B[2] ) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] == B[2] && A[1] > B[1]) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] == B[2] && A[1] == B[1]&& A[0] > B[0]);
}
// two vectors, in each vector the values are different
// for low hand, the smaller the number is, the bigger its ranking is
bool compareTwoVectorForLow(const vector<int> &A, const vector<int> &B){
    return A[4] < B[4] ||
    (A[4] == B[4] && A[3] < B[3]) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] < B[2] ) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] == B[2] && A[1] < B[1]) ||
    (A[4] == B[4] && A[3] == B[3] &&  A[2] == B[2] && A[1] == B[1]&& A[0] < B[0]);
}
    

string highHandcompareWithinSameRanking(const vector<string> &A, const vector<string> &B, string ranking){
    string str_A, str_B;
    vector<int> value_A, value_B;
    map<int,int>value_count_A, value_count_B;
    map<int,vector<int>>count_value_A, count_value_B;
    for(int i=0;i<5;++i){
        str_A+=A[i][0];
        str_B+=B[i][0];
    }
    value_A = computeIntValueOfCards(str_A, ranking);
    value_B = computeIntValueOfCards(str_B, ranking);
    for(int j=0;j<5;++j){
        value_count_A[value_A[j]]++;
        value_count_B[value_B[j]]++;
    }
    for(auto element: value_count_A){
        count_value_A[element.second].push_back(element.first);
    }
    for(auto element: value_count_B){
        count_value_B[element.second].push_back(element.first);
    }
    
    if(ranking == STRAIGHT_FLUSH){
        if(value_A[0]==value_B[0])
            return IDENTICAL;
        else if (value_A[0]>value_B[0])
            return "A";
        else
            return "B";
    }
    else if(ranking == FOUR_OF_A_KIND){
        if(value_A==value_B)
            return IDENTICAL;
        else if((count_value_A[4][0] > count_value_B[4][0]) ||
                (count_value_A[4][0] == count_value_B[4][0] && count_value_A[1][0] > count_value_B[1][0])
                )
            return "A";
        else
            return "B";
    }
    else if(ranking == FULL_HOUSE){
        if(value_A==value_B)
            return IDENTICAL;
        else if((count_value_A[3][0] > count_value_B[3][0]) ||
                (count_value_A[3][0] == count_value_B[3][0] && count_value_A[2][0] > count_value_B[2][0])
                )
            return "A";
        else
            return "B";
    }
    else if(ranking == FLUSH){
        if(value_A == value_B)
            return IDENTICAL;
        if(compareTwoVectorForHigh(value_A, value_B))
            return "A";
        else
            return "B";
    }
    else if(ranking == STRAIGHT){
        if(value_A == value_B)
            return IDENTICAL;
        else if(value_A[0]>value_B[0])
            return "A";
        else
            return "B";
    }
    else if(ranking == THREE_OF_A_KIND){
        if(value_A == value_B)
            return IDENTICAL;
        else if(count_value_A[3] > count_value_B[3])
            return "A";
        else{
            // save the extra two values except the 3-same
            vector<int>a,b;
            for(auto element:count_value_A){
                if(element.first==3)
                    continue;
                else{
                    a.insert(a.begin(),element.second.begin(), element.second.end());
                }
            }
            
            //
            for(auto element:count_value_B){
                if(element.first==3)
                    continue;
                else{
                    b.insert(b.begin(),element.second.begin(), element.second.end());
                }
            }
            if(max(a[0],a[1])>max(b[0],b[1]) ||
               (max(a[0],a[1])==max(b[0],b[1])&&min(a[0],a[1])>min(b[0],b[1])))
                return "A";
            else
                return "B";
        }
    }
    else if(ranking == TWO_PAIR){
        vector<int>a,b;
        int a_1=-1,b_1=-1;
        for(auto element:count_value_A){
            if(element.first==1)
                a_1 = element.second[0];
            else{
                a.insert(a.begin(), element.second.begin(), element.second.end());
            }
        }
        
        //
        for(auto element:count_value_B){
            if(element.first==1)
                b_1 = element.second[0];
            else{
                b.insert(b.begin(), element.second.begin(), element.second.end());
            }
        }
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        if(value_A == value_B)
            return IDENTICAL;
        else if(a[1]>b[1] ||
                (a[1]==b[1]&&a[0]>b[0]) ||
                (a[1]==b[1]&&a[0]==b[0]&&a_1>b_1))
            return "A";
        else
            return "B";
    }
    else if(ranking == ONE_PAIR){
        int a_pair = -1, b_pair = -1;
        vector<int>a,b;
        for(auto element:count_value_A){
            if(element.first==2)
                a_pair = element.second[0];
            else{
                a.insert(a.begin(), element.second.begin(), element.second.end());
            }
        }
        
        //
        for(auto element:count_value_B){
            if(element.first==2)
                b_pair = element.second[0];
            else{
                b.insert(b.begin(), element.second.begin(), element.second.end());
            }
        }
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        if(value_A == value_B)
            return IDENTICAL;
        else if ((a_pair > b_pair)||
                 ((a_pair == b_pair)&&a[2]>b[2])||
                 ((a_pair == b_pair)&&a[2]==b[2]&&a[1]>b[1])||
                 ((a_pair == b_pair)&&a[2]==b[2]&&a[1]==b[1]&&a[0]>b[0]))
            return "A";
        else
            return "B";
    }
    else{
        if(value_A == value_B)
            return IDENTICAL;
        if(compareTwoVectorForHigh(value_A, value_B))
            return "A";
        else
            return "B";
    }
}

string highHandCheck(const vector<string> &A, const vector<string> &B){
    string rankingA = getHighRanking(A);
    string rankingB = getHighRanking(B);
    if(high_hand_ranking_dict[rankingA]>high_hand_ranking_dict[rankingB])
        return "A";
    else if(high_hand_ranking_dict[rankingA]<high_hand_ranking_dict[rankingB])
        return "B";
    else{
        // same high ranking
        return highHandcompareWithinSameRanking(A, B, rankingA);
    }
}
    
bool qualifiedForLow(const vector<string> &cards){
    unordered_set<char> card_values;
    int count_of_bigger_than_8 = 0;
    for(auto card:cards){
        card_values.insert(card[0]);
        if(card[0]=='9'||card[0]=='T'||
           card[0]=='J'||card[0]=='Q'||
           card[0]=='K')
            count_of_bigger_than_8++;
    }
    return card_values.size() == 5 && count_of_bigger_than_8 == 0;
}

string compareLowHand(const vector<string> &A, const vector<string> &B){
    vector<int> value_A, value_B;
    for(int i=0;i<5;++i){
        if(A[i][0]=='A')
            value_A.push_back(1);
        else
            value_A.push_back(A[i][0]-'0');
        
        if(B[i][0]=='A')
            value_B.push_back(1);
        else
            value_B.push_back(B[i][0]-'0');
    }
    sort(value_A.begin(),value_A.end());
    sort(value_B.begin(),value_B.end());

    if(value_A == value_B)
        return IDENTICAL;
    if(compareTwoVectorForLow(value_A, value_B))
        return "A";
    else
        return "B";
}

string lowHandCheck(const vector<string> &A, const vector<string> &B){
    if(qualifiedForLow(A) && qualifiedForLow(B)){
        return compareLowHand(A,B);
    }else if(!qualifiedForLow(A) && qualifiedForLow(B)){
        return "B";
    }else if(qualifiedForLow(A) && !qualifiedForLow(B)){
        return "A";
    }else {
        return NOT_QUALIFIED_FOR_LOW;
    }
}

vector<string> splitString(string s){
    vector<string> res;
    string tmp;
    for(auto c:s){
        if(c!='-'){
            tmp+=c;
        }else{
            res.push_back(tmp);
            tmp.clear();
        }
    }
    if(!tmp.empty())
        res.push_back(tmp);
    return res;
}
//choose M elements from N elements, return the indices of
// chosen elements
vector<vector<int>> chooseMElementsFromN(size_t N, int M){
    vector<vector<int>> res;
    if(N<M)
        return res;
    vector<int> indices;
    int i=0;
    while(i<N-M){
        indices.push_back(0);
        i++;
    }
    while(i<N){
        indices.push_back(1);
        i++;
    }
    do {
        res.push_back(indices);
    } while (next_permutation(indices.begin(), indices.end()));
    return res;
}

//choose the corresponding cards
vector<string> getCorresponding(const vector<string> & hand, const vector<int> & indice){
    vector<string> res;
    for(int i=0;i<indice.size();++i)
        if(indice[i]&1)
            res.push_back(hand[i]);
    return res;
}

pair<int, vector<string>> getBestHighHandPossible(const vector<string> & hand, const vector<string> & board,
                                                  const vector<vector<int>> & hand_indices, const vector<vector<int>> & board_indices){
    pair<int, vector<string>> res;
    
    for(int i=0;i<hand_indices.size();++i){
        for(int j=0;j<board_indices.size();++j){
            vector<string> chosen_hand = getCorresponding(hand,hand_indices[i]);
            vector<string> chosen_board = getCorresponding(board, board_indices[j]);
            chosen_hand.insert(chosen_hand.end(), chosen_board.begin(),chosen_board.end());
            string currentBestRanking = getHighRanking(chosen_hand);
            if(high_hand_ranking_dict[currentBestRanking]>res.first){
                res.first = high_hand_ranking_dict[currentBestRanking];
                res.second = chosen_hand;
            }else if(high_hand_ranking_dict[currentBestRanking]==res.first){
                string cmpResult = highHandcompareWithinSameRanking(res.second, chosen_hand, currentBestRanking);
                if(cmpResult=="B")
                    res.second = chosen_hand;
            }
        }
    }
    return res;
}


vector<string> getBestLowHandPossible(const vector<string> & hand, const vector<string> & board,
                                                const vector<vector<int>> & hand_indices, const vector<vector<int>> & board_indices){
    vector<string> res;
    for(int i=0;i<hand_indices.size();++i){
        for(int j=0;j<board_indices.size();++j){
            vector<string> chosen_hand = getCorresponding(hand,hand_indices[i]);
            vector<string> chosen_board = getCorresponding(board, board_indices[j]);
            chosen_hand.insert(chosen_hand.end(), chosen_board.begin(),chosen_board.end());
            if(qualifiedForLow(chosen_hand)){
                if(res.empty())
                    res = chosen_hand;
                else{
                    string cmpResult = compareLowHand(res, chosen_hand);
                    if(cmpResult=="B")
                        res = chosen_hand;
                }
            }
        }
    }
    return res;
}

string getTheLowHandStr(vector<string> strs){
    string digits;
    for(string s:strs){
        if(s[0]!='A')
            digits+=s[0];
    }
    sort(digits.begin(),digits.end(), greater<int>());
    return digits.size()==5?digits:digits+'A';
}

string processSingleLine(const string &line){
    string res;
    string handA, handB, board;
    size_t A_start = line.find("HandA:");
    size_t B_start = line.find("HandB:");
    size_t Board_start = line.find("Board:");
    handA = line.substr(A_start+6, 11);
    handB = line.substr(B_start+6, 11);
    board = line.substr(Board_start+6, 14);
    vector<string> handA_vec =splitString(handA);
    vector<string> handB_vec =splitString(handB);
    vector<string> board_vec =splitString(board);
    
    vector<vector<int>> combos_of_choosing_two_from_A = chooseMElementsFromN(handA_vec.size(),2);
    vector<vector<int>> combos_of_choosing_two_from_B = chooseMElementsFromN(handB_vec.size(),2);
    vector<vector<int>> combos_of_choosing_three_from_board = chooseMElementsFromN(board_vec.size(),3);
    
    pair<int, vector<string>> hand_A = getBestHighHandPossible(handA_vec, board_vec,
                                                               combos_of_choosing_two_from_A, combos_of_choosing_three_from_board);
    pair<int, vector<string>> hand_B = getBestHighHandPossible(handB_vec, board_vec,
                                                               combos_of_choosing_two_from_B, combos_of_choosing_three_from_board);
    
    if(hand_A.first > hand_B.first){
        res = "HandA wins Hi ("+high_hand_ranking_dict_int[hand_A.first]+");";
    }else if(hand_A.first < hand_B.first){
        res = "HandB wins Hi ("+high_hand_ranking_dict_int[hand_B.first]+");";
    }else{
        string cmpResult = highHandcompareWithinSameRanking(hand_A.second, hand_B.second, high_hand_ranking_dict_int[hand_A.first]);
        if(cmpResult=="A")
            res = "HandA wins Hi ("+high_hand_ranking_dict_int[hand_A.first]+");";
        else if(cmpResult =="B")
            res = "HandB wins Hi ("+high_hand_ranking_dict_int[hand_B.first]+");";
        else
            res = "Split Pot Hi (" +high_hand_ranking_dict_int[hand_A.first]+");";
    }
    
    vector<string> hand_A_low = getBestLowHandPossible(handA_vec, board_vec,
                           combos_of_choosing_two_from_A, combos_of_choosing_three_from_board);
    vector<string> hand_B_low = getBestLowHandPossible(handB_vec, board_vec,
                           combos_of_choosing_two_from_B, combos_of_choosing_three_from_board);
    if(hand_A_low.empty()&&hand_B_low.empty())
        res+=" No hand qualified for Low";
    else if(!hand_A_low.empty()&&hand_B_low.empty()){
        res+=" HandA wins Lo ("+getTheLowHandStr(hand_A_low)+")";
    }
    else if(hand_A_low.empty()&&!hand_B_low.empty()){
        res+=" HandB wins Lo ("+getTheLowHandStr(hand_B_low)+")";
    }
    else{
        string cmpResult = compareLowHand(hand_A_low, hand_B_low);
        if(cmpResult=="A"){
            res+=" HandA wins Lo ("+getTheLowHandStr(hand_A_low)+")";
        }else if(cmpResult =="B"){
            res+=" HandB wins Lo ("+getTheLowHandStr(hand_B_low)+")";
        }else{
            res+=" Split Pot Lo (" +getTheLowHandStr(hand_A_low)+")";
        }
    }
    return res;
}

// given the input and output file path, read a line from input file
// process it then write the evaluation to the output file
void parseInput(const string & input_path, const string & output_path){
    ifstream ifile;
    ofstream ofile;
    ifile.open(input_path);
    ofile.open(output_path);
    string line;
    while(getline(ifile,line)){
        cout<<line<<endl;
        string evaluation = processSingleLine(line);
        cout<<evaluation<<endl<<endl;
        ofile<<line<<endl;
        ofile<<"=> "<<evaluation<<endl;
        ofile<<endl;
    }
    ifile.close();
    ofile.close();
}

int main(int argc, const char * argv[]) {
    string inputPath, outputPath;
    if(argc!=3){
        cout<<"3 arguments are needed, the default arguments will be used!"<<endl;
        inputPath = "/Users/JohnsonJohnson/Documents/XcodeProjects/OmahaPoker/OmahaPoker/input.txt";
        outputPath = "/Users/JohnsonJohnson/Documents/XcodeProjects/OmahaPoker/OmahaPoker/output.txt";
        cout<<"input file: "<<inputPath<<endl;
        cout<<"output file: "<<outputPath<<endl;
        cout<<endl;
    }
    else{
        inputPath = argv[1];
        outputPath = argv[2];
    }
    parseInput(inputPath, outputPath);
    return 0;
}
