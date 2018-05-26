    #ifdef __cplusplus
        #include <cstdlib>
    #else
        #include <stdlib.h>
    #endif
    #include <time.h>
    #include <SDL/SDL.h>
    #include <SDL_image.h>
    #include<SDL_ttf.h>

    //PROTOTYPES
void displayscreenBorder (SDL_Surface *screen, int borderWidth, Uint32 borderColor);
void blockPop(SDL_Surface* screen, SDL_Rect* block, Uint32* blockColor);
void randomBlock (SDL_Surface *screen, SDL_Rect *block);
Uint32 randomColor (SDL_Surface *screen);
void moveBlock (SDL_Surface *screen, SDL_Rect *block, int *score, bool *blockCatch, bool * done, SDL_Event *event, Uint32 blockColor, Uint32 borderColor, int borderWidth, int intervalTime);
void Counter (int score,SDL_Surface *screen,int borderWidth);
void globalDisplay (SDL_Surface *screen, SDL_Rect *block, Uint32 blockColor, Uint32 borderColor, int borderWidth, int score);
void testDirection (SDL_Surface *screen, SDL_Rect*block, bool *Right, bool *Left, bool * Up, bool *Down);
void wait (SDL_Surface *screen, SDL_Rect *block, int *score, bool *done, bool *blockCatch,SDL_Event *event);

    int main ( int argc, char** argv ){
        // initialize and test SDL video
        if ( SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ){
            fprintf(stderr,"Unable to init SDL: %s\n", SDL_GetError() );
            return 1;
        }
        //Initialize and test TTF lib
        if (TTF_Init()<0){
            fprintf(stderr,"Erreur SDL_TTF : %s \n", TTF_GetError());
            return 1;
        }

        SDL_Surface* screen = SDL_SetVideoMode(640,480, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);

        SDL_Rect block ;
        int borderWidth = 5 ;

        Uint32 blockColor = 0 ;
        Uint32 borderColor =0;
        int score = 0;
        int intervalTime = 50;
        bool done = false, blockCatch = false ;
        SDL_Event event;


        srand(time(NULL));

        do{
        borderColor = randomColor(screen);
        //displayscreenBorder(screen,borderWidth,&borderColor);
        blockPop(screen,&block,&blockColor);
        globalDisplay (screen, &block, blockColor,borderColor,borderWidth, score);
        blockCatch = false;
        wait (screen, &block, &score, &done, &blockCatch, &event);
        moveBlock (screen, &block, &score, &blockCatch, &done, &event, blockColor,borderColor,borderWidth, intervalTime);
        score ++;
        intervalTime --;

        }while (done == false);

        TTF_Quit();
        SDL_FreeSurface(screen);
        SDL_Quit();

        return EXIT_SUCCESS;
    }

    void blockPop(SDL_Surface *screen,SDL_Rect *block,Uint32 *blockColor){
        randomBlock(screen,block);
        *blockColor = randomColor(screen);

    }

    void randomBlock (SDL_Surface *screen, SDL_Rect *block){
        block->h = 50;
        block->w = 50;
        block->x = rand()%(0-(screen->w - block->w))+1;
        block->y = rand()%(0-(screen->h - block->h))+1;
    }

    Uint32 randomColor (SDL_Surface *screen){

    int randomColor =   rand ()%(0 - 4)+1;
    Uint32 resultColor =0 ;
    switch (randomColor){

    case 1:
        //red
        resultColor = SDL_MapRGB(screen->format,255,0,0);
        break;
    case 2:
    //green
        resultColor = SDL_MapRGB(screen->format,0,255,0);
        break;
    case 3 :
    //blue
    resultColor =SDL_MapRGB(screen->format,0,0,255);
    break;
    case 4 :
    //yellow
    resultColor = SDL_MapRGB(screen->format,255,255,0);
    break;
}
return resultColor;
}

    void displayscreenBorder (SDL_Surface *screen, int borderWidth,Uint32 borderColor){

    SDL_Rect screenBorder [4];
    int i =0;

    //LeftBorder
    screenBorder[0].x =0 ;
    screenBorder[0].y =0 ;
    screenBorder[0].h =screen->h;
    screenBorder[0].w =borderWidth;

    //HightBorder
    screenBorder[1].x = borderWidth;
    screenBorder[1].y = 0;
    screenBorder[1].h = borderWidth;
    screenBorder[1] .w = (screen->w - (borderWidth*2));

    //RightBorder
    screenBorder[2].x = (screen->w - borderWidth) ;
    screenBorder[2].y = 0;
    screenBorder[2].h = screen->h;
    screenBorder[2].w = borderWidth;

    //DownBorder
    screenBorder[3].x = 0;
    screenBorder[3].y = (screen->h - borderWidth);
    screenBorder[3].h = borderWidth;
    screenBorder[3].w = (screen ->w - borderWidth);

    for (i =0; i<4;i++){
        SDL_FillRect(screen,screenBorder+i, borderColor);
    }
}

    void globalDisplay (SDL_Surface *screen, SDL_Rect *block, Uint32 blockColor,Uint32 borderColor,int borderWidth, int score){

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 192));
        displayscreenBorder(screen,borderWidth,borderColor);
        SDL_FillRect(screen, block, blockColor);
        Counter(score,screen,borderWidth);
        SDL_Flip(screen);
    }

    void wait (SDL_Surface *screen, SDL_Rect *block, int *score, bool *done, bool *blockCatch, SDL_Event *event){

                SDL_PollEvent(event);
                 switch (event->type){
                //close window
                case SDL_QUIT:
                    *done = true;
                break;

                //press Escape
                case SDL_KEYDOWN:
                    if (event->key.keysym.sym == SDLK_ESCAPE)
                    *done = true;
                break;

                //catch a block
                case SDL_MOUSEBUTTONDOWN:
                    if (((event->button.x >= block->x) & (event->button.x <= block->x + block->w)) & ((event->button.y>=block->y) & (event->button.y <= block->y + block->h)))
                    {
                        *blockCatch = true ;
                        break;
                    }
                    else
                break;
                }
        }

    void testDirection (SDL_Surface *screen, SDL_Rect*block, bool *Right, bool *Left, bool * Up, bool *Down){
    //move to Left Condition
        if (block->x >0){
            *Left = true;
            }
        else if(block->x ==0){
            *Left = false;
        }

        //Move to right Condition
        if (block->x < (screen->w - block->w)){
            *Right =true;
        }
        else if(block->x >=(screen->w - block->w)){
            *Right =false ;
        }

        //Move Up condition
        if (block->y > 0){
            *Up =true;
        }
        else if (block->y == 0 ){
            *Up = false ;
        }

        // Move Down Condition
        if (block->y <(screen->h - block->h)){
            *Down =true;
        }
        else if (block->y >=(screen->h - block->h)){
            *Down = false ;
        }
    }

    void moveBlock (SDL_Surface *screen, SDL_Rect *block, int *score, bool *blockCatch, bool * done, SDL_Event *event, Uint32 blockColor, Uint32 borderColor, int borderWidth, int intervalTime){

    int currentTime = 0;
    int pastTime = 0;
    int randomMove = rand()%(0-5)+1;
    bool Right= true ,Left=true,Up=true,Down =true;
    bool invertDiagonale = false;

        do{
            testDirection(screen,block,&Right,&Left,&Up,&Down);
            switch(randomMove){

                case 1 : //move to the Right
                while (Right==true && *blockCatch == false && *done == false ){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->x ++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                    wait(screen,block,score,done,blockCatch,event);
                    testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }   //then, move to the Left
                while (Left==true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->x --;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                    wait(screen,block,score,done,blockCatch,event);
                    testDirection(screen,block,&Right,&Left,&Up,&Down);
                            }
                }
                break;

                case 2 ://move down
                while (Down==true && *blockCatch==false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);

                    }
                }       //then, move up
                while (Up==true && *blockCatch==false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                            }
                }
                break;

                case 3 : // move to the Left
                while (Left==true && *blockCatch == false && *done== false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime)
                    {
                     block->x --;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                while (Right==true && *blockCatch == false && *done== false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime)
                    {
                     block->x ++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                break;

                case 4 ://move Up
                while (Up == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                while (Down == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                     testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                break;

                case 5: //Diagonal
                if (invertDiagonale ==false){
                while (Down == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     block->x++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                     testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                while (Up == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     block->x++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                    testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                while (Up == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     block->x--;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                     testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                while (Down == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     block->x--;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                invertDiagonale=true;
               }
               else if (invertDiagonale ==true){
                    while(Up == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     block->x--;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                    testDirection(screen,block,&Right,&Left,&Up,&Down);
                        }
                    }
                    while (Up == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y --;
                     block->x++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                     testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                    while (Down == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     block->x++;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                     testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                    while (Down == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     block->y ++;
                     block->x--;
                     globalDisplay (screen, block, blockColor,borderColor,borderWidth, *score);
                     pastTime= currentTime;
                     wait(screen,block,score,done,blockCatch,event);
                      testDirection(screen,block,&Right,&Left,&Up,&Down);
                    }
                }
                    invertDiagonale =false;
                }
                break;
                } //end switch
            } while (*blockCatch == false);//end "do"
}

    void Counter (int score, SDL_Surface *screen,int borderWidth) {

        SDL_Surface *text = NULL ;
        SDL_Surface *timer =NULL;
        SDL_Rect  textPosition ;

        SDL_Rect timerPosition;


        TTF_Font *police = TTF_OpenFont("shakapow.ttf", 30);
        SDL_Color Orange = {255,128,0};
        char  displayScore[20] =  "";
        char displayTimer[20] = "";
        int time  = 0;
        time = (SDL_GetTicks()/1000);
        sprintf (displayScore,"Score : %d",score);
        sprintf (displayTimer,"Temps (s): %d",time);
        text = TTF_RenderText_Solid (police,displayScore,Orange);
        timer = TTF_RenderText_Solid(police,displayTimer,Orange);

        textPosition.x = ((screen->w - text->w)/2);
        textPosition.y = borderWidth;

        timerPosition.x = borderWidth;
        timerPosition.y = (screen->h - borderWidth) - timer ->h;

        SDL_BlitSurface(text,NULL,screen,&textPosition);
        SDL_BlitSurface(timer,NULL,screen,&timerPosition);
        SDL_Flip(screen);
        TTF_CloseFont(police);
}



