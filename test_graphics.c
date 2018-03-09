/* author: russ loewe
 * date:   20 feb 2018
 * file:   test_suit.c
 * Purpose:
 *         This file contains all of the tests to run in this suit.
 *         each of these tests needs to  have their function name put 
 *         in the function pointer array of tests.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "./headers/graphics.h"
#include "./headers/gameobjects.h"
#include "./headers/maps.h"

/**********Internal functions and data structures from graphics.c*****/
//internal data structures
struct Textline{
    char text[40];
    struct Textline * nxt;
};
typedef struct Textline Textline;

//function prototypes for internal functions
int render_objects( gamepiece * pieces[], int range);
SDL_Texture * make_colored_texture(int height, int width, Uint8 red, Uint8 blue, Uint8 green);
int render_background();
int render_background_image(SDL_Texture * img);
int render_room(room * cur_room);
SDL_Rect get_char_rect(char c);
int trim_message_queue(int n);
int render_player_stats(room * curroom);
int render_message_queue(int lines, int x, int y);
Textline * get_message_queue();

/***************End of internal functions and data structures*******/

int run_tests();
int cleantest();
int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();
int test8();
int test9();
int test10();
int test11();


int main(void){
    
    printf("running tests now\n");
    run_tests();
    
    return 0;
}

int run_tests(){
    
    int (*test_suit[])() = {test1, test2, test3, test4, 
                            test5, test6, test7, test8,
                            test9, test10, test11, cleantest};                      //array of all tests to run - cleantest must be at the end of this array
    const int test_len = sizeof(test_suit) / sizeof(test_suit[0]);  //variable to tell test runner how many tests there are

    
    int success = 0;
    int failure = 0;
    
    for(int i = 0; i < test_len; i++){
        int response;
        
        response = test_suit[i]();
        
        switch(response){
            
            case 0:
                //Last test is supposed to return 0 to indicate tests are done
                printf("Finished Runnig all tests. Success: %i\n", success);
                printf("                           Failure: %i\n", failure);
                return 0;
            
            case 1:
                success++;
                break;
            
            default:
                failure++;
                break;
        }
    }
    printf("Error: test.c -> run_tests() -> not cleanup at end\n");
    return 1;
}
/***************************** Begin Test Functions ********************/
int cleantest(){
    printf("cleaning up tests\n");
    cleanup();
    return 0;
}

int test1(){
    /*
     * Check that the graphics will initalize
     */
    printf("testing init.c->init_video() - ");
    if(init_video() != 0 ){
        printf("Fail\n");
        return -1;
    }else{
        printf("Pass\n");
        return 1;
    }
}

int test2(){
    /*
     * Check that we can open one of the images
     * Later I'll probably have it loop though all images for 
     * the game
     */
    printf("Testing display.c->Load_image() - ");
    
    if(load_image("./img/player.bmp") == NULL){
        printf("Failed\n");
        SDL_GetError();
        return -1;
    }else{
        printf("Pass\n");
    }
    return 1;
}

    
int test3(){
    printf("Testing display->render_background() - ");
    if(render_background() == 1){
        printf("Fail \n");
        return -1;
    }else{
        printf("Pass\n");
        return 1;
    }
}
    
int test4(){
    /*Check if the function that renders a colored square works */
    printf("Testing display->make_colored_texture(): ");
    
    SDL_Texture * blank_square;
    
    blank_square = make_colored_texture(16, 16, 255, 255, 255);
    
    if(blank_square == NULL){
        printf("Fail 1\n");
        return -1;
    }
    
    return 1;
}

int test5(){
    /*Test the render objects function
     */
     printf("Testing graphics->render_objects(): ");
    gamepiece * player;
    player = create_piece(50, 50, load_image("./img/player2.bmp"), PLAYER_TYPE);
    gamepiece * objects[10] = {NULL};
    objects[0] = player;
    
    render_objects(objects, 10);
    printf("Pass\n");
    
    return 1;
}

int test6(){
    /*
     * test the room renderer
     */
     
     printf("Testing graphics->room_render() ");
 /***********temp room hack***************/
        room room1 = {.walls = {NULL}, .monsters = {NULL}, .bounty = {NULL}, .doors = {NULL}}; //init walls array to null pointers
        
        if( render_room(&room1) != 0){ //test render_room when room is empty
            printf("Fail 1\n");
            return -1;
        }

    
        //make some walls
        for(int i = 0; i < 40; i++){
        gamepiece * wall = create_piece(i, 0, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i] = wall;
        }
        for(int i = 0; i < 17; i++){
        gamepiece * wall = create_piece(0, i, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+50] = wall;
        }
        for(int i = 0; i < 40; i++){
        gamepiece * wall = create_piece(i, 17, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+100] = wall;
        }
        for(int i = 0; i < 17; i++){
        gamepiece * wall = create_piece(39, i, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+150] = wall;
        }
        
        
        //spawn some monsters
        gamepiece * monst;
        for(int i = 0; i<5; i++){
            monst = create_piece(rand()%38+2, rand()%15+2, load_image("./img/monster.bmp"), MONSTER_TYPE);
            room1.monsters[i] =monst;
        }
        //add some items
        gamepiece * potion = create_piece(rand()%38+2, rand()%15+2, load_image("./img/potion.bmp"), POTION_TYPE);
        room1.bounty[0] = potion;
        gamepiece * sword = create_piece(rand()%38+2, rand()%15+2, load_image("./img/sword.bmp"), SWORD_TYPE);
        room1.bounty[1] = sword;
        gamepiece * shield = create_piece(rand()%38+2, rand()%15+2, load_image("./img/shield.bmp"), SHIELD_TYPE);
        room1.bounty[2] = shield;
       
        //add the two doors;
        gamepiece * d1 = create_piece(0, 10, load_image("./img/door.bmp"), DOOR_TYPE);
        gamepiece * d2 = create_piece(39, 5, load_image("./img/door.bmp"), DOOR_TYPE);
        
        room1.doors[0] = d1;
        room1.doors[1] = d2;
        
    /******************end temp room hack**********/
    if( render_room(&room1) != 0){
        printf("Fail 2\n");
        return -1;
    }else{
        printf("Pass\n");
        return 1;
    }
}
    
int test7(){
    /*
     * testing the text string render function
     */
    printf("Testing graphics->render_text_line(): ");
    
    char * text = "hello";
    
    if(render_text_line(text, 2, 20) != 0){
        printf("Fail \n");
        return -1;
    }    
    printf("Pass \n");
    return 1;
}

int test8(){
    /*
     * test adding text to the message queue
     */
     printf("Testing graphics->add_message_queue(): ");
     

   //add three messages to the message queue
    if(add_message_queue("Hddtt234 234$%^&(*&^%$") != 0){
         printf("Fail 3\n");
         return -1;
     }
    if(add_message_queue("test") != 0){
         printf("Fail 2\n");
         return -1;
     }
    if(add_message_queue("hello") != 0){
         printf("Fail 1\n");
         return -1;
     }
     
     Textline * cur = get_message_queue();  //grab the head of the queue
     
     if(strncmp(cur->text, "hello", strlen("hello")) != 0){  //check first message
         printf("Fail 4 \n");
         return -1;
     }
     
     cur = cur->nxt;     //get next message
         
     if( strncmp(cur->text, "test", strlen("test")) != 0){
         printf("Fail 5 \n");
         return -1;
     }
     
     cur = cur->nxt;  //get third message
     
     if( strncmp(cur->text, "Hddtt234 234$%^&(*&^%$", strlen("Hddtt234 234$%^&(*&^%$")) != 0){
         printf("Fail 6 \n");
         return -1;
     }
     
     printf("Pass \n");
     return 1;
 }
 
int test9(){
    /*
     * Test render message queue
     */
     printf("Testing graphics.c->render_message_queue(): ");
     char matrix[5][15];
     strcpy(matrix[0], "hello");
     strcpy(matrix[1], "Tom23");
     strcpy(matrix[2], "test34");
     strcpy(matrix[3], "!@test");
     strcpy(matrix[1], "test Test 45");
    
    for(int i = 0; i < 5; i++){
        if( add_message_queue(matrix[i]) != 0){
            printf("Fail 1\n");
            return -1;
        }
    }
    
    if(render_message_queue(5, 10 , 20) != 0){
        printf("Fail 2 \n");
        return -1;
    }
    printf("Pass \n");
    return 1;
}

int test10(){
    /*
     * test trim message queue
     */
     
     printf("Testing graphics->trim_message_queue(): ");
     
     add_message_queue("test"); //put at least 1 message in queue
     if(trim_message_queue(0) < 1){   //clear queue
         printf("Fail .5\n");
         return -1;
     }
     if(trim_message_queue(1) != 0){  //make sure queue is empty
         printf("Fail .6\n");
         return -1;
     }
     for(int i = 0 ; i < 20; i++){
         for(int j = 0; j < 20; j++){
             add_message_queue("test message");
         }
         if(trim_message_queue(5) != 14){
             printf("Fail 1\n");
             return -1;
         }
         if(trim_message_queue(6) > 0){
             printf("Fail 2\n");
             return -1;
         }
         if(trim_message_queue(4) > 0){
             printf("Fail 3\n");
             return -1;
         }
         
         //clear queue before loop repeats
         trim_message_queue(0);
     }
     printf("Pass \n");
     return 1;
 }

int test11(){
    /*
     * test render all function
     */
     printf("Testing graphics->render_all() ");
     
    /***********temp room hack***************/
        room room1 = {.walls = {NULL}}; //init walls array to null pointers
        current_room = &room1;          //set current room pointer to room1
        //make some walls
        for(int i = 0; i < 40; i++){
        gamepiece * wall = create_piece(i, 0, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i] = wall;
        }
        for(int i = 0; i < 17; i++){
        gamepiece * wall = create_piece(0, i, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+50] = wall;
        }
        for(int i = 0; i < 40; i++){
        gamepiece * wall = create_piece(i, 17, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+100] = wall;
        }
        for(int i = 0; i < 17; i++){
        gamepiece * wall = create_piece(39, i, load_image("./img/wall.bmp") , WALL_TYPE);
        room1.walls[i+150] = wall;
        }
        
        
        //spawn some monsters
        gamepiece * monst;
        for(int i = 0; i<5; i++){
            monst = create_piece(rand()%38+2, rand()%15+2, load_image("./img/monster.bmp"), MONSTER_TYPE);
            room1.monsters[i] =monst;
        }
        //add some items
        gamepiece * potion = create_piece(rand()%38+2, rand()%15+2, load_image("./img/potion.bmp"), POTION_TYPE);
        room1.bounty[0] = potion;
        gamepiece * sword = create_piece(rand()%38+2, rand()%15+2, load_image("./img/sword.bmp"), SWORD_TYPE);
        room1.bounty[1] = sword;
        gamepiece * shield = create_piece(rand()%38+2, rand()%15+2, load_image("./img/shield.bmp"), SHIELD_TYPE);
        room1.bounty[2] = shield;
       
        //add the two doors;
        gamepiece * d1 = create_piece(0, 10, load_image("./img/door.bmp"), DOOR_TYPE);
        gamepiece * d2 = create_piece(39, 5, load_image("./img/door.bmp"), DOOR_TYPE);
        
        room1.doors[0] = d1;
        room1.doors[1] = d2;
        
        for(int j = 0; j < 20; j++){
             add_message_queue("test message");
         }
    set_background_image("./img/background2.bmp");
    /******************end temp room hack**********/

    if (render_all() != 0){
        printf("Fail \n");
        return -1;
    }
    
    printf("Pass \n");
    return 1;
}
