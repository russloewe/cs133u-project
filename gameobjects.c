/*
 * file  : gameobjects.c
 * date  : 20 feb 2018
 * purpose:
 *       functions for manipulating objects in the game. Header file
 *       contains struct defs for the game objects used.
 */
 
#include "./headers/gameobjects.h"
#define GAMESQUARE 16

gamepiece * create_piece(int x, int y, enum piecetype type){
//need to check alloc
    gamepiece * newpiece = (gamepiece *)malloc(sizeof(gamepiece));

    newpiece->x = x;
    newpiece->y = y;
    newpiece->type = type;
    newpiece->sword = NULL; //init pointers to null
    newpiece->shield = NULL;
    
    return newpiece;
}

int destroy_piece(gamepiece * piece){
    if(piece == NULL){
        return 1;
    }
    //recusicly free sub structs
    if(piece->sword != NULL){
        destroy_piece(piece->sword);
    }
    if(piece->shield != NULL){
        destroy_piece(piece->shield);
    }
    free(piece);
    return 0;
}
 
/********GETTERS*************************/

int get_piece_x(gamepiece *piece){
    //interface to return a game piece's x position.
    if(piece == NULL){
        return -1;
    }
    int a = piece->x;
    return a;
}

int get_piece_y(gamepiece *piece){
    //interface to get gamepiece y coordinate
    if(piece == NULL){
        return -1;
    }
    int y = piece->y;
    return y;
}

int get_piece_val(gamepiece * item){
    //interface to access a gamepiece's val
    if(item == NULL){
        return 0;
    }
    return item->val;
}

char * get_piece_name(gamepiece * piece){
    //interface to access gamepieces name
    
    if(piece == NULL){return NULL;}
    
    return piece->name;
}

gamepiece * get_player_sword(gamepiece * piece){
    //return gamepieces sword
    //return null on failer or empty
    
    if( (get_piece_type(piece) == PLAYER_TYPE) ||
        (get_piece_type(piece) == MONSTER_TYPE) ){ //only for action character
         if(piece->sword != NULL){
             gamepiece * psword = piece->sword;
             
             //copy coords from player to sword
             set_piece_x(psword, get_piece_x(piece)); 
             set_piece_y(psword, get_piece_y(piece)); 
             return psword;
         }else{
             return NULL;
         }
    }else{
        return NULL;
    }
}

gamepiece * get_player_shield(gamepiece * piece){
    //return gamepieces shield
    //return null on failer or empty
    
    if( (get_piece_type(piece) == PLAYER_TYPE) ||
        (get_piece_type(piece) == MONSTER_TYPE) ){ //only for action character
         if(piece->shield != NULL){
             gamepiece * pshield = piece->shield;
             
             //copy coords from player to shield
             set_piece_x(pshield, get_piece_x(piece)); 
             set_piece_y(pshield, get_piece_y(piece)); 
             return pshield;
         }else{
             return NULL;
         }
    }else{
        return NULL;
    }
}

piecetype get_piece_type(gamepiece *piece){
    //super quick dirty function
    return piece->type;
 
}

/****************SETTERS**********************/

int set_piece_x(gamepiece * piece, int x){
    //interface to set the piece x coord
    if(piece == NULL){
        return 1;
    }
    if(x < 0 ){
        return -1;
    }
    piece->x = x;
    return 0;
}

int set_piece_y(gamepiece * piece, int y){
    //interface to set the piece x coord
    if(piece == NULL){
        return 1;
    }
    if(y < 0 ){
        return -1;
    }
    piece->y = y;
    return 0;
}
    
int set_piece_val(gamepiece * piece, int val){
    //interface to set item value
    if(piece == NULL){ return 1;}
    piece->val = val;
    return 0;
}

int set_player_health(gamepiece* player, int a){
    //set the health stat of a player or monster health
    
    //check for correct type first
    if( (get_piece_type(player) != PLAYER_TYPE) && (get_piece_type(player) != MONSTER_TYPE) ){
        return -1;
    }
    if(a <= 100){
        player->val = a;
    }
    if(a > 100){
        player->val = 100;
    }
    return 0;
}
    
int set_piece_name(gamepiece * piece, char * string){
    //interface to set gamepieces name
    
    if(piece == NULL){return 1;}
    
        if(strncpy(piece->name, string, 25) == NULL){
            return 1;
        }else{
            return 0;
        }
}


    
gamepiece * equip_item_to_player(gamepiece * player, gamepiece * item){
    //take an item and add it to player stuct, pop current item and return
    if( (item == NULL) || (player == NULL) ){
        return NULL;
    }
    if( (get_piece_type(player) != PLAYER_TYPE) && (get_piece_type(player) != MONSTER_TYPE) ){
        return NULL;
    }
    
    if(get_piece_type(item) == SWORD_TYPE){
        //copy the pointer to player's current shield
        gamepiece * new_sword;        
        new_sword = get_player_sword(player);  
        
        //assign incoming item to player slot
        player->sword = item;
        
        //return popped sword
        return new_sword;   
    }
    if(get_piece_type(item) == SHIELD_TYPE){
        //copy the pointer to player's current shield
        gamepiece * new_shield;        
        new_shield = get_player_shield(player);  
        
        //assign incoming item to player slot
        player->shield = item;
        
        //return popped shield
        return new_shield;   
    }
    if(get_piece_type(item) == POTION_TYPE){
        //grab values and claculate new health level
        int cur_health = get_piece_val(player);
        int potion_val = get_piece_val(item);
        
        //set health, setter function will handle healh value bounds
        set_piece_val(player, cur_health + potion_val);
        
        //free poition struct
        destroy_piece(item);
        return NULL;
    }else{
        return NULL;
    }
}        

/************ACTION ***********************/

int move_piece(gamepiece * piece, enum direction direc){
    switch(direc){
        case MVUP:
            (piece->y) -= 1;
            break;
        case MVDOWN:
            (piece->y) += 1;
            break;
        case MVLEFT:
            (piece->x) -= 1;
            break;
        case MVRIGHT:
            (piece->x) += 1;
            break;
            
        }
    return 0;
}

int attack(gamepiece *attaker, gamepiece *defender){


}
