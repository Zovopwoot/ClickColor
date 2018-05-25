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

void randomBlock (SDL_Surface *screen, SDL_Rect *blockPosition);
void randomColor (SDL_Surface *screen, Uint32  *blockColor);
void moveBlock (SDL_Surface *screen, SDL_Rect *blockPosition, int *score, bool *blockCatch, bool * done, SDL_Event *event,Uint32 blockColor,int intervalTime);
void Counter (int score,SDL_Surface *screen);
void globalDisplay (SDL_Surface *screen, SDL_Rect *blockPosition, Uint32 blockColor,int score);
void testDirection (SDL_Surface *screen, SDL_Rect*blockPosition, bool *Right, bool *Left, bool * Up, bool *Down);
void wait (SDL_Surface *screen, SDL_Rect *blockPosition, int *score,bool *done,bool *blockCatch,SDL_Event *event);

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
        SDL_Rect blockPosition ;

        Uint32 blockColor = 0 ;
        int score = 0;
        int intervalTime = 50;
        bool done = false, blockCatch = false ;
        SDL_Event event;


        srand(time(NULL));
        do{
        randomBlock(screen,&blockPosition);
        randomColor(screen,&blockColor);
        globalDisplay(screen,&blockPosition,blockColor,score);
        blockCatch = false;
        wait(screen,&blockPosition,&score,&done,&blockCatch,&event);
        moveBlock(screen,&blockPosition,&score,&blockCatch,&done,&event,blockColor,intervalTime);
        score++;
        intervalTime --;

        }while (done == false);

        TTF_Quit();
        SDL_FreeSurface(screen);
        SDL_Quit();

        return EXIT_SUCCESS;
    }

    void randomBlock (SDL_Surface *screen, SDL_Rect *blockPosition){
        blockPosition->h = 50;
        blockPosition->w = 50;
        blockPosition->x = rand()%(0-(screen->w - blockPosition->w))+1;
        blockPosition->y = rand()%(0-(screen->h - blockPosition->h))+1;
    }

    void randomColor (SDL_Surface *screen,uint32_t *blockColor){

    int randomColor =   rand ()%(0 - 4)+1;
    switch (randomColor){

    case 1:
        //red
        *blockColor = SDL_MapRGB(screen->format,255,0,0);
        break;
    case 2:
    //green
        *blockColor = SDL_MapRGB(screen->format,0,255,0);
        break;
    case 3 :
    //blue
    *blockColor =SDL_MapRGB(screen->format,0,0,255);
    break;
    case 4 :
    //yellow
    *blockColor = SDL_MapRGB(screen->format,255,255,0);
    break;
}
}

    void globalDisplay (SDL_Surface *screen, SDL_Rect *blockPosition, Uint32 blockColor, int score){

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
        SDL_FillRect(screen, blockPosition, blockColor);
        Counter(score,screen);
        SDL_Flip(screen);
    }

    void wait (SDL_Surface *screen,SDL_Rect *blockPosition, int *score,bool *done, bool *blockCatch,SDL_Event *event){

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
                    if (((event->button.x >= blockPosition->x) & (event->button.x <= blockPosition->x + blockPosition->w)) & ((event->button.y>=blockPosition->y) & (event->button.y <= blockPosition->y + blockPosition->h)))
                    {
                        *blockCatch = true ;
                        break;
                    }
                    else
                break;
                }
        }

    void testDirection (SDL_Surface *screen, SDL_Rect*blockPosition,bool *Right, bool *Left, bool * Up, bool *Down){
    //move to Left Condition
        if (blockPosition->x >0){
            *Left = true;
            }
        else if(blockPosition->x ==0){
            *Left = false;
        }

        //Move to right Condition
        if (blockPosition->x < (screen->w - blockPosition->w)){
            *Right =true;
        }
        else if(blockPosition->x >=(screen->w - blockPosition->w)){
            *Right =false ;
        }

        //Move Up condition
        if (blockPosition->y > 0){
            *Up =true;
        }
        else if (blockPosition->y == 0 ){
            *Up = false ;
        }

        // Move Down Condition
        if (blockPosition->y <(screen->h - blockPosition->h)){
            *Down =true;
        }
        else if (blockPosition->y >=(screen->h - blockPosition->h)){
            *Down = false ;
        }
    }

    void moveBlock (SDL_Surface *screen, SDL_Rect *blockPosition, int *score, bool *blockCatch, bool * done, SDL_Event *event, Uint32 blockColor, int intervalTime){

    int currentTime = 0;
    int pastTime = 0;
    int randomMove = rand()%(0-5)+1;
    bool Right= true ,Left=true,Up=true,Down =true;
    bool invertDiagonale = false;

        do{
            testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
            switch(randomMove){

                case 1 : //move to the Right
                while (Right==true && *blockCatch == false && *done == false ){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->x ++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                    wait(screen,blockPosition,score,done,blockCatch,event);
                    testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }   //then, move to the Left
                while (Left==true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->x --;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                    wait(screen,blockPosition,score,done,blockCatch,event);
                    testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                            }
                }
                break;

                case 2 ://move down
                while (Down==true && *blockCatch==false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);

                    }
                }       //then, move up
                while (Up==true && *blockCatch==false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                            }
                }
                break;

                case 3 : // move to the Left
                while (Left==true && *blockCatch == false && *done== false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime)
                    {
                     blockPosition->x --;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                while (Right==true && *blockCatch == false && *done== false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime)
                    {
                     blockPosition->x ++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                break;

                case 4 ://move Up
                while (Up == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                while (Down == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                     testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                break;

                case 5: //Diagonal
                if (invertDiagonale ==false){
                while (Down == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     blockPosition->x++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                     testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                while (Up == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     blockPosition->x++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                    testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                while (Up == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     blockPosition->x--;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                     testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                while (Down == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     blockPosition->x--;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                invertDiagonale=true;
               }
               else if (invertDiagonale ==true){
                    while(Up == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     blockPosition->x--;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                    testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                        }
                    }
                    while (Up == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y --;
                     blockPosition->x++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                     testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                    while (Down == true && Right == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     blockPosition->x++;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                     testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                    while (Down == true && Left == true && *blockCatch == false && *done == false){
                    currentTime =SDL_GetTicks();
                    if (currentTime-pastTime>intervalTime){
                     blockPosition->y ++;
                     blockPosition->x--;
                     globalDisplay(screen,blockPosition,blockColor,*score);
                     pastTime= currentTime;
                     wait(screen,blockPosition,score,done,blockCatch,event);
                      testDirection(screen,blockPosition,&Right,&Left,&Up,&Down);
                    }
                }
                    invertDiagonale =false;
                }
                break;
                } //end switch
            } while (*blockCatch == false);//end "do"
}

    void Counter (int score,SDL_Surface *screen) {

        SDL_Surface *text = NULL ;
        SDL_Surface *timer =NULL;
        SDL_Rect  textPosition ;

        SDL_Rect timerPosition;


        TTF_Font *police = TTF_OpenFont("shakapow.ttf", 30);
        SDL_Color Black = {0,0,0};
        char  displayScore[20] =  "";
        char displayTimer[20] = "";
        int time  = 0;
        time = (SDL_GetTicks()/1000);
        sprintf (displayScore,"Score : %d",score);
        sprintf (displayTimer,"Temps (s): %d",time);
        text = TTF_RenderText_Solid (police,displayScore,Black);
        timer = TTF_RenderText_Solid(police,displayTimer,Black);

        textPosition.x = ((screen->w - text->w)/2);
        textPosition.y = 0;

        timerPosition.x = 0;
        timerPosition.y = screen->h - timer ->h;

        SDL_BlitSurface(text,NULL,screen,&textPosition);
        SDL_BlitSurface(timer,NULL,screen,&timerPosition);
        SDL_Flip(screen);
        TTF_CloseFont(police);
}



