
/*************************************************************
Author: Rebecca Kennedy
Date: 3/7/18
Filename: maps.c
This is the room module for the final project in CS133u.
***************************************************************/

#include "./headers/maps.h"



/*************GAMESTATE VARIABLES**********/
room * _map[7] = {NULL};
static int current_room_index = 0;
static room * _current_room;        //index of the current room
int _monster_iter_index = 0;
int _max_room = 5;
/**************INIT************************/

room * create_room(void) {

//need to check alloc
    room * newroom = (room *)malloc(sizeof(room));

     struct room room_tmp = {.walls = {NULL}, .monsters = {NULL}, .bounty = {NULL}};
     memcpy(newroom, &room_tmp, sizeof(struct room));

        set_current_room(newroom);
        room * current_room = newroom;          //set current room pointer to room1

        //make some walls
        for(int i = 0; i < get_max_x(); i++){
            add_wall_to_current_room(create_piece(i, 0, "wall", 1, WALL_TYPE));
        }
        for(int i = 0; i < get_max_y(); i++){        
            add_wall_to_current_room(create_piece(0, i, "wall", 1, WALL_TYPE));
        }
        for(int i = 0; i < get_max_x(); i++){
            add_wall_to_current_room(create_piece(i, get_max_y(), "wall", 1, WALL_TYPE));
        }
        for(int i = 0; i < get_max_y()+1; i++){
            add_wall_to_current_room(create_piece(get_max_x(), i, "wall", 1, WALL_TYPE));
        }
        
        int rnmbr = current_room_index + 1;
        //spawn some monsters
        gamepiece * monster;
        for(int i = 1; i <= rnmbr; i++){
            if(i%2 == 0){
                monster = create_piece(random_x(), random_y(), "goblin", (rnmbr) * 10, MONSTER_TYPE);
                set_piece_icon(monster, MONSTER_ICO_2);
            }else{
                monster = create_piece(random_x(), random_y(), "big goblin", (rnmbr) * 15, MONSTER_TYPE);
                set_piece_icon(monster, MONSTER_ICO_3);
            }
            gamepiece * sword = create_piece(random_x(), random_y(), "Dagger", rnmbr*2+rand()%(10),  SWORD_TYPE);
            equip_item_to_player(monster, sword);
            add_monster_to_current_room(monster);
                
        }
        monster = create_piece(random_x(), random_y(), "Room Boss", (rnmbr+1) * 15, MONSTER_TYPE);
        gamepiece * shield = create_piece(random_x(), random_y(), "Iron Shield", rnmbr*3+rand()%(10), SHIELD_TYPE);
        gamepiece * sword = create_piece(random_x(), random_y(), "Dagger", rnmbr*3+rand()%(10),  SWORD_TYPE);
        equip_item_to_player(monster, sword);
        equip_item_to_player(monster, shield);
        add_monster_to_current_room(monster);
        
        
        if(current_room_index == _max_room-1){  //add goblin king
            monster = create_piece(random_x(), random_y(), "Goblin King", 100, KING_TYPE);
            gamepiece * shield = create_piece(random_x(), random_y(), "Iron Shield", 10+rand()%(20), SHIELD_TYPE);
            gamepiece * sword = create_piece(random_x(), random_y(), "Sword", rnmbr*2+rand()%(10),  SWORD_TYPE);
            equip_item_to_player(monster, sword);
            equip_item_to_player(monster, shield);
           add_monster_to_current_room(monster);
        }
        //add some items 
        add_item_to_current_room(create_piece(random_x(), random_y(), "potion", 40, POTION_TYPE));
       
        //make 2 swords
        if(current_room_index == 0){
            add_item_to_current_room(create_piece(random_x(), random_y(), "Sword", 10+rand()%(50), SWORD_TYPE)); //
        //add_item_to_current_room(create_piece(random_x(), random_y(), "Dagger", rand()%(rnmbr*3),  SWORD_TYPE));
    }
        //make two shields
        add_item_to_current_room(create_piece(random_x(), random_y(), "Iron Shield", rnmbr*5+rand()%(10), SHIELD_TYPE));
      //  add_item_to_current_room(create_piece(random_x(), random_y(), "Bronze Shield", 40, SHIELD_TYPE));       
        //add the two doors;        

        newroom->doors[0] = create_piece(0, 10, "door", 1, DOOR_TYPE);
        if(current_room_index != _max_room-1){ //exclude a door on the last room
            newroom->doors[1] = create_piece(get_max_x(), 5, "door", 1, DOOR_TYPE); 
        }


        return newroom;
}

int create_map(){
    //generate 5 rooms and add them to the map
    current_room_index = 0;
    for(int i = 0; i < _max_room; i++){
        _map[i] = create_room();
        current_room_index++;
    }
    current_room_index = 0; //reset room index
    _current_room = _map[current_room_index];
    return 0;
}

int init_monster_iter(){
    _monster_iter_index = 1;
    return 0;
}

/*****************GETTERS*******************/
room * get_current_room(){
    //interface to get room
    return _current_room;
}

int get_room_index(){
    return current_room_index;
}
int load_next_room(){
    //advance index and copy that room pointer _current_room
    gamepiece * player = get_player();
    if(current_room_index > 7){
        return 1;
    }
    current_room_index++;
    _current_room = _map[current_room_index];
    
    _current_room->monsters[0] = player;     // got to move the player to next room
    set_piece_x(player, 1 + get_piece_x(_current_room->doors[0]));
    set_piece_y(player, get_piece_y(_current_room->doors[0]));
    return 0;
}

int load_prev_room(){
    //advance index and copy that room pointer _current_room
    gamepiece * player = get_player();
    if(current_room_index <= 0){
        return 1;
    }
    current_room_index--;
    _current_room = _map[current_room_index];
    
    _current_room->monsters[0] = player;     // got to move the player to next room
    set_piece_x(player, (-1) + get_piece_x(_current_room->doors[1]));
    set_piece_y(player, get_piece_y(_current_room->doors[1]));
    return 0;
}

gamepiece * get_player(){
    //super quick interface
    return get_current_room()->monsters[0];
}

gamepiece * grab_item_reference(int x, int y){
    //look for item at x,y and return pointer, leave in in the room
    //i made a mess trying to expand this function so o juat copied the internals 3 times
    
        //door - do this first so the "this door is locked" message appears
    for(int i = 0; i < 2; i++){
        gamepiece * temp = get_current_room()->doors[i];
        if( temp != NULL){
            int it_x = get_piece_x(temp);
            int it_y = get_piece_y(temp);
            
            if( (it_x == x) && (it_y == y) ){
                return temp;
            }
        }
    }   
    
    //items
    for(int i = 0; i < 10; i++){
        gamepiece * temp = get_current_room()->bounty[i];
        if( temp != NULL){
            int it_x = get_piece_x(temp);
            int it_y = get_piece_y(temp);
            
            if( (it_x == x) && (it_y == y) ){
                return temp;
            }
        }
    }
    //monster
    for(int i = 1; i < 10; i++){
        gamepiece * temp = get_current_room()->monsters[i];
        if( temp != NULL){
            int it_x = get_piece_x(temp);
            int it_y = get_piece_y(temp);
            
            if( (it_x == x) && (it_y == y) ){
                return temp;
            }
        }
    }
    //wall
    for(int i = 0; i < 300; i++){
        gamepiece * temp = get_current_room()->walls[i];
        if( temp != NULL){
            int it_x = get_piece_x(temp);
            int it_y = get_piece_y(temp);
            
            if( (it_x == x) && (it_y == y) ){
                return temp;
            }
        }
    }

    return NULL;
}


gamepiece * get_adjacent_item(gamepiece * ref_piece, enum direction dir){
    //get base coords
    int x = get_piece_x(ref_piece);
    int y = get_piece_y(ref_piece);
    
    gamepiece * item = NULL;
    
    //attempt to pick up an item in the correct direction
    switch(dir){
        case UP:
        item = grab_item_reference(x, y-1);
        break;
        
        case DOWN:
        item = grab_item_reference(x, y+1);
        break;
        
        case LEFT:
        item = grab_item_reference(x-1, y);
        break;
        
        case RIGHT:
        item = grab_item_reference(x+1, y);
        break;
        
        default:
        item = NULL;
    }
    
    //return the pointer, no need to check value first
    return item;
}


int remove_item_from_current_room(gamepiece * item){
    int x = get_piece_x(item);
    int y = get_piece_y(item);
    
    for(int i = 0; i < 5; i++){
        gamepiece * temp = get_current_room()->bounty[i];
        if( temp != NULL){
            int it_x = get_piece_x(temp);
            int it_y = get_piece_y(temp);
            
            if( (it_x == x) && (it_y == y) ){
                get_current_room()->bounty[i] = NULL;
                return 0;
            }
        }
    }
    return 1;
}

gamepiece * monster_iter(){
    if( (_monster_iter_index < 0) || (_monster_iter_index >= 10)){
        return NULL;
    }else{
        gamepiece * monster = get_current_room()->monsters[_monster_iter_index];
        _monster_iter_index++;
        return monster;
    }
}

int random_x(){
    int max = get_max_x();
    int random =  ( rand() % (max - 2) ) + 1;
    return random;
}

int random_y(){
    int max = get_max_y();
    int random =  ( rand() % (max - 2) ) + 1;
    return random;
}


/**************SETTERS************************/

int set_current_room(room * croom){
    //interface to set room
    _current_room = croom;
    return 1;
}

int add_item_to_current_room(gamepiece * item){
    //look for empty slot to add item to room struct
    for(int i = 0; i < 10; i++){
        if(get_current_room()->bounty[i] == NULL){
            get_current_room()->bounty[i] = item;   //look for empty slot to add item
            return 0;
        }
    }
    return 1;
}

int add_monster_to_current_room(gamepiece * monster){
    //look for empty slot to add monster to room    
    for(int i = 1; i < 10; i++){
        if(get_current_room()->monsters[i] == NULL){
            get_current_room()->monsters[i] = monster;   //look for empty slot to add item
            return 0;
        }
    }
    return 1;
}

int add_player_to_current_room(gamepiece * player){
    //add player to front of monster aray
    get_current_room()->monsters[0] = player;   //look for empty slot to add item
    return 1;
}

int add_wall_to_current_room(gamepiece * wall){
    //look for empty slot to add monster to room    
    for(int i = 0; i < 300; i++){
        if(get_current_room()->walls[i] == NULL){
            get_current_room()->walls[i] = wall;   //look for empty slot to add item
            return 0;
        }
    }
    return 1;
}

