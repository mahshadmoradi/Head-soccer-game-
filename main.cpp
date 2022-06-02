#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_gfx.h>
#include<SDL2/SDL_mixer.h>
#include<string>
#include<time.h>
#include<fstream>
#include<map>
using namespace std;
#define WIDTH 1280
#define HEIGHT 800
#define g 20
#define t 0.1
#define zarib 0.83
#define vshoot 100
#define Time 90
#define LP 80
#define D 170
void text(SDL_Renderer *m_renderer,int xp,int yp,int w,int h,string addressOfFont,int fontsize , int R , int G , int B , int A , string S);
SDL_Window *window;
SDL_Renderer *renderer;
    void initialize();
    void handleevents();
    void render();
    void goalcelebrate();
    void playersplaying();
    void ballplaying();
    void game();
    void update_scores();
    void sortt(string names[],int tedad,int lists[][8]);
    SDL_Rect ball,backg;
    SDL_Texture *backimg[2],*ballimg[4],*stars[10] ,*portal[18];
    int backi,ballstate,timep,ballnum,backnum;
    bool isRunning;
    double vx,vy;
    Uint32 start;
class Player{
public:
    SDL_Texture *pimg;
    double theta;
    SDL_Rect pos,pos1,pos2,shoes,prect;
    double pvy,pvx;
    int goalnum;
    int playernum;
    SDL_Point shoescenter;
    string name;
    double powline;
    int timepower;
    bool shoot,pow[7],booltimer,sshoot,freez;
};
Player player1,player2;
SDL_Texture *shoesimg,*punchimg;
struct menu
{
    bool initmenu,music,sound;
    SDL_Texture *clawmenu,*backmenu,*setting;
    SDL_Rect menuclaw,menuback,settingrect;
    void menuinit(bool &music);
    void playerselection();
    void otherselection();
    string nameinput(string pname,int x,int y,int &k);
    void names();
    void scoreboard();
    void scoreboardp();
};
menu Menu;
struct sounds{
    void ballbouncesound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("ballbouncing.wav");
            Mix_PlayChannel(1,music2,0);
        }
    }
    void ballkicksound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("ballkick.wav");
            Mix_PlayChannel(2,music2,0);
        }
    }
    void whistlesound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("whistle.mp3");
            Mix_PlayChannel(3,music2,0);
        }
    }
    void dingsound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("ding.mp3");
            Mix_PlayChannel(3,music2,0);
        }
    }
    void ding2sound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("ding2.mp3");
            Mix_PlayChannel(3,music2,0);
        }
    }
    void ding3sound()
    {
        if(Menu.sound)
        {
            Mix_Chunk* music2=Mix_LoadWAV("ding3.mp3");
            Mix_PlayChannel(3,music2,0);
        }
    }
};
sounds sounds;
void menu::menuinit(bool &music)
{
    initmenu=true;
    TTF_Init();
    SDL_SetWindowTitle(window,"Head Football");
    menuback={0,0,WIDTH,HEIGHT};
    settingrect={1030,700,250,100};
    SDL_Surface* tempts=IMG_Load("claw.png");
    clawmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    tempts=IMG_Load("field.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    tempts=IMG_Load("setting.png");
    setting=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    if(Menu.music)
    {
        Mix_Music* music= Mix_LoadMUS("theme.mp3");
        Mix_PlayMusic(music,20);
    }
    menuclaw={-5,900,400,400};
    int rmenu=0;
    SDL_Event event;
    while(initmenu)
    {
        if(menuclaw.y>0) menuclaw.y-=20;
        SDL_PollEvent(&event);
        if(event.type==SDL_QUIT)
            SDL_Quit();
        if(event.type==SDL_KEYDOWN)
        switch(event.key.keysym.sym)
        {
        case(SDLK_RIGHT):
            rmenu++;
            if(rmenu<=5)
            {
                menuclaw.x+=185;
                sounds.dingsound();
            }
            if(rmenu>5) rmenu=5;
            break;
        case(SDLK_LEFT):
            rmenu--;
            if(rmenu>=0)
            {
                menuclaw.x-=185;
                sounds.dingsound();
            }
            if(rmenu<0) rmenu=0;
            break;
        case(SDLK_SPACE):
            if(rmenu==0 || (rmenu==1 && timep!=0) || rmenu==2)
                initmenu=false;
            if(rmenu==4)
            {
                music=!music;
                if(!music) Mix_HaltMusic();
                else
                {
                Mix_Music* music= Mix_LoadMUS("theme.mp3");
                Mix_PlayMusic(music,20);
                }
            }

            if(rmenu==5) sound=!sound;
                break;
        }
        event.key.keysym.sym=NULL;
        SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
        thickLineRGBA(renderer,menuclaw.x+200,menuclaw.y+10,menuclaw.x+200,0,40,0,0,0,255);
        SDL_RenderCopy(renderer, clawmenu, NULL, &menuclaw);
        boxRGBA(renderer,50,700,WIDTH-30,HEIGHT-20,255,255,255,190);
        if(rmenu>=3)
        {
            SDL_RenderCopy(renderer, setting, NULL, &settingrect);
            if(rmenu==4) filledEllipseRGBA(renderer,1108,745,35,35,(!music)?255:0,(music)?255:0,0,100);
            if(rmenu==5) filledEllipseRGBA(renderer,1200,745,35,35,(!sound)?255:0,(sound)?255:0,0,100);
        }
        text(renderer,100,690,210,100,"FreeSans.ttf",90,(rmenu==0)?255:0,0,0,255,"New game");
        text(renderer,350,690,300,100,"FreeSans.ttf",90,0,(rmenu==1)?255:0,0,255,"resume game");
        text(renderer,700,690,120,100,"FreeSans.ttf",90,0,0,(rmenu==2)?255:0,255,"table");
        text(renderer,850,690,200,100,"FreeSans.ttf",90,(rmenu==3)?255:0,(rmenu==3)?255:0,0,255,"settings");
        SDL_RenderPresent(renderer);
        SDL_Delay(40);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
    sounds.ding2sound();
    if(rmenu==0) playerselection();
    else if(rmenu==1)
    {
        if(Menu.music)
        {
        Mix_Music* music= Mix_LoadMUS("crowdsound.wav");
        Mix_PlayMusic(music,20);
        }
        return;
    }
    else if(rmenu==2)
        scoreboard();
}
void menu::scoreboard()
{
    SDL_Surface* tempts=IMG_Load("score table.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    bool st=true;
    SDL_Event e;
    string s ;
    int tedad=0;
    ifstream read("jadval.txt");
    while(getline(read,s)){
    tedad++; }
    read.close();
    ifstream fin("jadval.txt");
    string names[tedad] ;
    int lists[tedad][8];
    int i=0;
    while(fin>>names[i++])
    {
        for(int j=0 ; j<8 ; j++)
                fin>>lists[i-1][j];
    }
    sortt(names , tedad , lists );
    while(st)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            SDL_Quit();
        if(e.type==SDL_KEYDOWN)
            if(e.key.keysym.sym==SDLK_SPACE)
                st=false;
    SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
    boxRGBA(renderer,10,695,100,790,0,0,0,175);
    int q=12;
    if(tedad<12) q=tedad;
    for(int v=0 ; v<q ; v++){
        text(renderer , 450, 215+35*v , names[v].length()*13 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , names[v] ) ;
        for(int u=0 ; u<8 ; u++){
            if(lists[v][u]<10)
                text(renderer , 750+55*u , 215+35*v , 18 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , to_string(lists[v][u]) ) ;
            else
                text(renderer , 750+55*u , 215+35*v , 36 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , to_string(lists[v][u]) ) ;
        }
    }
    SDL_RenderPresent(renderer); //show table image
    SDL_Delay(50);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    }
    fin.close();
    sounds.ding2sound();
    Menu.menuinit(Menu.music);
}
void menu::scoreboardp()
{
    SDL_Surface* tempts=IMG_Load("score table.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    text(renderer , 380 , 10 , 40 , 30 ,"FreeSans.ttf" ,40 ,255 , 255 , 200 , 255 ,"Lets GO!")  ;
    boxRGBA(renderer,380,10,420,40,0,0,0,175);
    bool st=true;
    SDL_Event e;
    string s ;
    int tedad=0;
    ifstream read("jadval.txt");
    while(getline(read,s)){
    tedad++; }
    read.close();
    ifstream fin("jadval.txt");
    string name[tedad] ;
    int lists[tedad][8];
    int i=0;
    while(fin>>name[i++])
    {
        for(int j=0 ; j<8 ; j++)
                fin>>lists[i-1][j];
    }
    sortt(name , tedad , lists );
    while(st)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            SDL_Quit();
        if(e.type==SDL_KEYDOWN)
            if(e.key.keysym.sym==SDLK_SPACE)
                st=false;
    if(e.type==SDL_MOUSEBUTTONDOWN)
        if(e.button.x>=450 && e.button.x<=790 && e.button.y>=30 && e.button.y<=130) {
         game();
        }
    SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
    boxRGBA(renderer,10,695,100,790,0,0,0,175);
    text(renderer , 450 , 30 , 340, 100 ,"FreeSans.ttf" ,100 ,0 , 250 , 80 , 255 ,"Lets GO!")  ;
    boxRGBA(renderer,450,30,790,130,255,255,255,80);
    int t1=-1 , t2=-1 ;
    int q=12;
    if(tedad<12) q=tedad;
    for(int v=0 ; v<q ; v++){
        if(name[v]==player1.name ){
            text(renderer , 450, 215+35*v , name[v].length()*13 , 30 ,"FreeSans.ttf" , 30 , 255 , 25 , 25 , 255 , name[v] ) ;
            t1=v; }
        else if (name[v]==player2.name){
            text(renderer , 450, 215+35*v , name[v].length()*13 , 30 ,"FreeSans.ttf" , 30 , 255 , 25 , 25 , 255 , name[v] ) ;
            t2=v; }

        else
            text(renderer , 450, 215+35*v , name[v].length()*13 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , name[v] ) ;
        for(int u=0 ; u<8 ; u++){
            if(v==t1 || v==t2){
                if(lists[v][u]<10)
                    text(renderer , 750+55*u , 215+35*v , 18 , 30 ,"FreeSans.ttf" , 30 , 255 , 25 , 25 , 255 , to_string(lists[v][u]) ) ;
                else
                    text(renderer , 750+55*u , 215+35*v , 36 , 30 ,"FreeSans.ttf" , 30 , 255 , 25 , 25 , 255 , to_string(lists[v][u]) ) ;
                }
        else{
            if(lists[v][u]<10)
                text(renderer , 750+55*u , 215+35*v , 18 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , to_string(lists[v][u]) ) ;
            else
                text(renderer , 750+55*u , 215+35*v , 36 , 30 ,"FreeSans.ttf" , 30 , 255 , 255 , 255 , 255 , to_string(lists[v][u]) ) ;
            }
        }
    }
    SDL_RenderPresent(renderer); //show table image
    SDL_Delay(50);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    }
    fin.close();
    sounds.ding2sound();
    names();
}
void menu::playerselection()
{
   bool playerselect=true;
    SDL_Texture *pimg[8];
    SDL_Surface* tempts=IMG_Load("mback.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    for(int i=0;i<8;i++)
    {
        tempts=IMG_Load(("p"+to_string(i)+".png").c_str());
        pimg[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    tempts=IMG_Load("shoes.png");
    shoesimg=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    player1.pos={350,320,230,280};
    player2.pos={700,320,230,280};
    player1.shoes={player1.pos.x+player1.pos.w/3,player1.pos.y+player1.pos.h,3*player1.pos.w/4,player1.pos.h/3};
    player2.shoes={player2.pos.x-player2.pos.w/15,player2.pos.y+player2.pos.h,3*player2.pos.w/4,player2.pos.h/3};
    player1.playernum=7; player2.playernum=0;
    int k=0;
    SDL_Event e;
    while(playerselect)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            SDL_Quit();
        if(e.type==SDL_KEYDOWN) switch(e.key.keysym.sym)
        {
            case(SDLK_RIGHT):
            k++;
            if(k<=3) sounds.dingsound();
            if(k>3) k=3;
            break;
            case(SDLK_LEFT):
            k--;
            if(k>=0) sounds.dingsound();
            if(k<0) k=0;
            break;
            case(SDLK_DOWN):
            if(k==1)
            {
                player1.playernum++;
                if(player1.playernum<=7) sounds.ding3sound();
                if(player1.playernum>7) player1.playernum=7;
            }
            else if(k==2)
            {
                player2.playernum++;
                if(player2.playernum<=7) sounds.ding3sound();
                if(player2.playernum>7) player2.playernum=7;
            }
            break;
            case(SDLK_UP):
            if(k==1)
            {
                player1.playernum--;
                if(player1.playernum>=0) sounds.ding3sound();
                if(player1.playernum<0) player1.playernum=0;
            }
            else if(k==2)
            {
                player2.playernum--;
                if(player2.playernum>=0) sounds.ding3sound();
                if(player2.playernum<0) player2.playernum=0;
            }
            break;
            case(SDLK_SPACE):
                sounds.ding2sound();
                playerselect=false;
                break;
        }
        e.key.keysym.sym=NULL;
        player1.pimg=pimg[player1.playernum]; player2.pimg=pimg[player2.playernum];
        SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
        boxRGBA(renderer,0,0,WIDTH,HEIGHT,0,0,0,150);
        switch(k)
        {
        case(0):
            boxRGBA(renderer,10,695,100,790,255,255,255,105);
            break;
        case(1):
            boxRGBA(renderer,300,300,595,700,255,255,255,105);
            break;
        case(2):
            boxRGBA(renderer,640,300,935,700,255,255,255,105);
            break;
        case(3):
            boxRGBA(renderer,1175,695,1265,790,255,255,255,105);
            break;
        }
        SDL_RenderCopy(renderer, player1.pimg, NULL, &player1.pos);
        SDL_RenderCopyEx(renderer, player2.pimg, NULL, &player2.pos,0,0,SDL_FLIP_HORIZONTAL);
//        SDL_SetRenderDrawColor(renderer,0,0,0,255);
//        SDL_RenderDrawRect(renderer,&player1.shoes);
        SDL_RenderCopy(renderer, shoesimg, NULL, &player1.shoes);
        SDL_RenderCopyEx(renderer, shoesimg, NULL, &player2.shoes,0,0,SDL_FLIP_HORIZONTAL);
        map<int, string>mymap;
        mymap[0]="   No Power   ";
        mymap[1]=" Triple Clone ";
        mymap[2]="Kick Fire Ball";
        mymap[3]="Invisible Ball";
        mymap[4]="     Punch    ";
        mymap[5]="     Thief    ";
        mymap[6]="   No Power   ";
        mymap[7]="     Thief    ";
        text(renderer,370,230,200,85,"FreeSans.ttf",50,255,0,0,255,mymap[player1.playernum]);
        text(renderer,690,230,200,85,"FreeSans.ttf",50,255,0,0,255,mymap[player2.playernum]);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
    if(k==0)
        Menu.menuinit(Menu.music);
    else if(k==3)
        otherselection();
}
void menu::otherselection()
{
    bool select=true;
    SDL_Texture *ballsimg[4],*backsimg[3];
    SDL_Surface* tempts=IMG_Load("mback2.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    for(int i=0;i<3;i++)
    {
        tempts=IMG_Load(("background"+to_string(i+1)+"0.png").c_str());
        backsimg[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    for(int i=0;i<4;i++)
    {
        tempts=IMG_Load(("ball"+to_string(i+1)+"0.png").c_str());
        ballsimg[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    SDL_FreeSurface(tempts);
    player1.pos={370,400,200,200};
    player2.pos={50,50,WIDTH-100,HEIGHT-100};

    ballnum=3;
    int k=0;
    SDL_Event e;
    while(select)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            SDL_Quit();
        if(e.type==SDL_KEYDOWN) switch(e.key.keysym.sym)
        {
            case(SDLK_RIGHT):
            k++;
            if(k<=3) sounds.dingsound();
            if(k>3) k=3;
            break;
            case(SDLK_LEFT):
            k--;
            if(k>=0) sounds.dingsound();
            if(k<0) k=0;
            break;
            case(SDLK_DOWN):
            if(k==1)
            {
                ballnum++;
                if(ballnum<=3) sounds.ding3sound();
                if(ballnum>3) ballnum=3;
            }
            else if(k==2)
            {
                backnum++;
                if(backnum<=2) sounds.ding3sound();
                if(backnum>2) backnum=2;
            }
            break;
            case(SDLK_UP):
            if(k==1)
            {
                ballnum--;
                if(ballnum>=0) sounds.ding3sound();
                if(ballnum<0) ballnum=0;
            }
            else if(k==2)
            {
                backnum--;
                if(backnum>=0) sounds.ding3sound();
                if(backnum<0) backnum=0;
            }
            break;
            case(SDLK_SPACE):
                sounds.ding2sound();
                select=false;
                break;
        }
        e.key.keysym.sym=NULL;
        SDL_RenderCopy(renderer, backsimg[backnum], NULL, &player2.pos);
        SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
        switch(k)
        {
        case(0):
            boxRGBA(renderer,10,695,100,790,0,0,0,175);
            break;
        case(1):
            boxRGBA(renderer,320,400,615,620,0,0,0,175);
            break;
        case(2):
            boxRGBA(renderer,680,400,960,620,0,0,0,175);
            break;
        case(3):
            boxRGBA(renderer,1175,695,1265,790,0,0,0,175);
            break;
        }
        SDL_RenderCopy(renderer, ballsimg[ballnum], NULL, &player1.pos);
        text(renderer,720,460,200,85,"FreeSans.ttf",50,255,255,255,255,"soccer field");
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
    if(k==0)
        playerselection();
    else if(k==3)
        names();
}
void menu::names()
{
    SDL_Surface* tempts=IMG_Load("mback1.png");
    backmenu=SDL_CreateTextureFromSurface(renderer,tempts);
    player1.name="";
    player2.name="";
    bool namein=true;
    int k=0;
    SDL_Event e;
    while(namein)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            SDL_Quit();
        if(e.type==SDL_KEYDOWN) switch(e.key.keysym.sym)
        {
            case(SDLK_RIGHT):
            k++;
            if(k<=3) sounds.dingsound();
            if(k>3) k=3;
            break;
            case(SDLK_LEFT):
            k--;
            if(k>=0) sounds.dingsound();
            if(k<0) k=0;
            break;
            case(SDLK_SPACE):
            if(k==0 || k==3)
                namein=false;
            break;
        }
        e.key.keysym.sym=NULL;
        SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
        boxRGBA(renderer,30,580,380,680,0,0,0,200);
        boxRGBA(renderer,900,580,1250,680,0,0,0,200);
        text(renderer,30,480,260,90,"FreeSans.ttf",50,255,255,255,255,"Player1 name:");
        text(renderer,900,480,260,90,"FreeSans.ttf",50,255,255,255,255,"Player2 name:");
        text(renderer,30,580,player1.name.length()*25,80,"FreeSans.ttf",50,255,255,255,255,player1.name);
        text(renderer,900,580,player2.name.length()*25,80,"FreeSans.ttf",50,255,255,255,255,player2.name);
       switch(k)
        {
        case(0):
            boxRGBA(renderer,10,695,100,790,0,0,0,175);
            break;
        case(1):
            player1.name=nameinput(player1.name,30,530,k);
            break;
        case(2):
            player2.name=nameinput(player2.name,900,530,k);
            break;
        case(3):
            boxRGBA(renderer,1175,695,1265,790,0,0,0,175);
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
    sounds.ding2sound();
    if(k==0)
        otherselection();
    else if(k==3)
        scoreboardp();
}
string menu::nameinput(string pname,int x,int y,int &k)
{
    TTF_Init();
    SDL_StartTextInput();
    SDL_Event e;
    bool namein=true;
    int a=0;
    while(namein)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_TEXTINPUT){
            pname+=e.text.text;}
        else if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_BACKSPACE && pname.length()>0 )
            pname.resize(pname.length()-1);

        SDL_RenderCopy(renderer, backmenu, NULL, &menuback);
        boxRGBA(renderer,30,580,380,680,0,0,0,200);
        boxRGBA(renderer,900,580,1250,680,0,0,0,200);
        text(renderer,x,y+50,pname.length()*25,80,"FreeSans.ttf",50,255,255,255,255,pname);
        a++; if(a>19) a=0;
        text(renderer,x+pname.length()*25,y+45,30,90,"FreeSans.ttf",50,255,255,255,(a/10)?1:255,"|");
        text(renderer,30,480,260,90,"FreeSans.ttf",50,255,255,255,255,"Player1 name:");
        text(renderer,900,480,260,90,"FreeSans.ttf",50,255,255,255,255,"Player2 name:");
        if(k==2)text(renderer,30,580,player1.name.length()*25,80,"FreeSans.ttf",50,255,255,255,255,player1.name);
        if(k==1)text(renderer,900,580,player2.name.length()*25,80,"FreeSans.ttf",50,255,255,255,255,player2.name);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
         if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_RIGHT) { k++; namein=false;}
         else if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_LEFT) { k--; namein=false;}
        e.type=NULL;
    }
    return pname;
    SDL_StopTextInput();
}
int main( int argc, char * argv[] )
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {cout << "Failed at SDL_Init()" << endl; isRunning=false;}
    if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        {cout << "Failed at SDL_CreateWindowAndRenderer()" << endl;  isRunning=false;}
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    timep=0;
    Menu.sound=true; Menu.music=true;
    if(Menu.music)
    {
        Mix_Music* music= Mix_LoadMUS("theme.mp3");
        Mix_PlayMusic(music,20);
    }
    Menu.menuinit(Menu.music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
void machend()
{
    sounds.whistlesound();
    player1.pos={270,320,230,280};
    player2.pos={770,320,230,280};
    player1.shoes={player1.pos.x+player1.pos.w/3,player1.pos.y+player1.pos.h,3*player1.pos.w/4,player1.pos.h/3};
    player2.shoes={player2.pos.x-player2.pos.w/15,player2.pos.y+player2.pos.h,3*player2.pos.w/4,player2.pos.h/3};
    SDL_RenderCopy(renderer, player1.pimg, NULL, &player1.pos);
    SDL_RenderCopyEx(renderer, player2.pimg, NULL, &player2.pos,0,0,SDL_FLIP_HORIZONTAL);
    SDL_RenderCopy(renderer, shoesimg, NULL, &player1.shoes);
    SDL_RenderCopyEx(renderer, shoesimg, NULL, &player2.shoes,0,0,SDL_FLIP_HORIZONTAL);
    SDL_Texture* resultimg;
    SDL_Surface* tempts=IMG_Load("result.png");
    resultimg=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    SDL_Rect resultrect={290,100,700,600};
    string s;
    if(player1.goalnum>player2.goalnum) s=player1.name+" won! ";
    else if(player2.goalnum>player1.goalnum) s=player2.name+" won! ";
    else s="Equal! ";
    update_scores();
    int num=0;
    bool machend=true;
    SDL_Event e;
    while(machend)
    {
        SDL_PollEvent(&e);
        if(e.type==SDL_KEYDOWN)
            switch(e.key.keysym.sym)
        {
        case(SDLK_LEFT):
            if(num>0)
            {
                num--;
                sounds.dingsound();
            }
            break;
        case(SDLK_RIGHT):
            if(num<1)
            {
                num++;
                sounds.dingsound();
            }
            break;
        case(SDLK_SPACE):
            sounds.ding2sound();
            machend=false;
            break;
        }
        e.key.keysym.sym=NULL;
        render();
        boxRGBA(renderer,0,0,WIDTH,HEIGHT,0,0,0,155);
        SDL_RenderCopy(renderer, resultimg, NULL, &resultrect);
        SDL_RenderCopy(renderer, player1.pimg, NULL, &player1.pos);
        SDL_RenderCopyEx(renderer, player2.pimg, NULL, &player2.pos,0,0,SDL_FLIP_HORIZONTAL);
        SDL_RenderCopy(renderer, shoesimg, NULL, &player1.shoes);
        SDL_RenderCopyEx(renderer, shoesimg, NULL, &player2.shoes,0,0,SDL_FLIP_HORIZONTAL);
        text(renderer,540,390,200,70,"FreeSans.ttf",70,255,255,255,255,to_string(player1.goalnum)+" - "+to_string(player2.goalnum));
        text(renderer,540,450,200,60,"FreeSans.ttf",70,255,255,255,255,s.c_str());
        switch(num)
        {
        case(0):
            boxRGBA(renderer,590,525,630,600,0,0,0,155);
            break;
        case(1):
            boxRGBA(renderer,650,525,692,600,0,0,0,155);
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
    if(num==0) game();
    else Menu.menuinit(Menu.music);
}
void game()
{
    initialize();
    while(isRunning)
    {
        timep=(SDL_GetTicks()-start)/1000;
        player1.powline+=0.1; player2.powline+=0.1;
        if(player1.powline>=LP) player1.powline=LP;
        if(player2.powline>=LP) player2.powline=LP;
        if(timep>=Time)
        {
            machend();
            isRunning=false;
        }
        handleevents();
        playersplaying();
        ballplaying();
        render();
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
    }
}
void initialize()
{
    isRunning=true;
    SDL_Surface* tempts=IMG_Load(("background"+to_string(backnum+1)+"0.png").c_str());
    backimg[0]=SDL_CreateTextureFromSurface(renderer,tempts);
    tempts=IMG_Load(("background"+to_string(backnum+1)+"1.png").c_str());
    backimg[1]=SDL_CreateTextureFromSurface(renderer,tempts);
    for(int i=0;i<4;i++)
    {
        tempts=IMG_Load(("ball"+to_string(ballnum+1)+to_string(i)+".png").c_str());
        ballimg[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    tempts=IMG_Load("punch.png");
    punchimg=SDL_CreateTextureFromSurface(renderer,tempts);
    for(int i=0;i<10;i++)
    {
        tempts=IMG_Load(("spr_seestars_"+to_string(i)+".png").c_str());
        stars[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    for(int i=0;i<18;i++)
    {
        tempts=IMG_Load(("portal"+to_string(i)+".gif").c_str());
        portal[i]=SDL_CreateTextureFromSurface(renderer,tempts);
    }
    SDL_FreeSurface(tempts);
    backg={0,40,WIDTH,HEIGHT-80};
    //////////////////////////////////////////////////////////////////
    player1.pos={110,-180,150,170};
    player2.pos={1020,-180,150,170};
    SDL_Rect portal1,portal2;
    portal1={0,-10,350,600};
    portal2={930,-10,350,600};
    int counter=0;
    bool pluss=true;
    if(Menu.music)
    {
        Mix_Music* music= Mix_LoadMUS("crowdsound.wav");
        Mix_PlayMusic(music,20);
    }
    while(player1.pos.y<430-2)
    {
        SDL_RenderCopy(renderer, backimg[0], NULL, &backg);
        player1.shoes={player1.pos.x+player1.pos.w/3,player1.pos.y+player1.pos.h,3*player1.pos.w/4,player1.pos.h/3};
        player2.shoes={player2.pos.x-player1.pos.w/15,player2.pos.y+player2.pos.h,3*player2.pos.w/4,player2.pos.h/3};
        SDL_RenderCopy(renderer,shoesimg,NULL,&player1.shoes);
        SDL_RenderCopy(renderer, player1.pimg, NULL,&player1.pos);
        SDL_RenderCopyEx(renderer,shoesimg,NULL,&player2.shoes,NULL,NULL,SDL_FLIP_HORIZONTAL);
        SDL_RenderCopyEx(renderer, player2.pimg, NULL, &player2.pos,NULL,NULL,SDL_FLIP_HORIZONTAL);
        boxRGBA(renderer,0,0,WIDTH,40,0,0,0,255);
        SDL_RenderCopy(renderer,portal[int(counter/2)],NULL,&portal1);
        SDL_RenderCopyEx(renderer,portal[int(counter/2)],NULL,&portal2,NULL,NULL,SDL_FLIP_HORIZONTAL);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        player1.pos.y+=player1.pvy*t; player2.pos.y=player1.pos.y;
        player1.pvy+=g*t;
        (pluss)? counter++:counter--;
        if(counter==35) pluss=false;
    }
    player1.pos={110,430-2,150,170};
    player2.pos={1020,430-2,150,170};
    /////////////////////////////////
    backi=0;
    ball={WIDTH/2-30,450-30,60,60};
    ballstate=0;
    vx=0;
    vy=0;
    player1.goalnum=0;
    player2.goalnum=0;
    player1.pvy=0;
    player1.theta=0;
    player2.pvy=0;
    player2.theta=0;
    player1.shoot=false;
    player2.shoot=false;
    player1.sshoot=false;
    player2.sshoot=false;
    player1.freez=false;
    player2.freez=false;
    player1.pvx=0;
    player2.pvx=0;
    for(int i=0;i<6;i++)
    {
        player1.pow[i]=false;
        player2.pow[i]=false;
    }
    player1.powline=0;
    player2.powline=0;
    player1.booltimer=false;
    player2.booltimer=false;
    start=SDL_GetTicks();
}
void pause()
{
    SDL_Texture* pauseimg;
    SDL_Surface* tempts=IMG_Load("pause.png");
    pauseimg=SDL_CreateTextureFromSurface(renderer,tempts);
    SDL_FreeSurface(tempts);
    SDL_Rect pausrect={450,250,400,500};
    bool ispaused=true;
    int num=0;
    SDL_Event e;
    while(ispaused)
    {
        render();
        boxRGBA(renderer,0,0,WIDTH,HEIGHT,0,0,0,155);
        SDL_PollEvent(&e);
        SDL_RenderCopy(renderer, pauseimg, NULL, &pausrect);
        if(e.type==SDL_KEYDOWN)
            switch(e.key.keysym.sym)
        {
        case(SDLK_UP):
            if(num>0)
            {
                num--;
                sounds.dingsound();
            }
            break;
        case(SDLK_DOWN):
            if(num<2)
            {
                num++;
                sounds.dingsound();
            }
            break;
        case(SDLK_SPACE):
            ispaused=false;
            break;
        }
        e.key.keysym.sym=NULL;
    boxRGBA(renderer,500,370+num*85,800,442+num*85,0,0,0,100);
    SDL_RenderPresent(renderer);
    SDL_Delay(5);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    }
    sounds.ding2sound();
    start+=SDL_GetTicks()-start-timep*1000;
    if(num==1)
    {
        isRunning=false;
        game();
    }
    else if(num==2)
        Menu.menuinit(Menu.music);
}
void handleevents()
{
    player1.pvx=0; player2.pvx=0;
    player1.shoot=false; player2.shoot=false;
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type==SDL_QUIT)
        isRunning=false;
    //rect(renderer,10,60,75,90,255,2,2,255,0);
    if(event.type==SDL_MOUSEBUTTONDOWN)
        if(event.button.x>=10 && event.button.x<=85 && event.button.y>=60 && event.button.y<=150)
            pause();
    const uint8_t *kstate=SDL_GetKeyboardState(NULL);
    if(!player1.freez)
    {
       if(kstate[SDL_SCANCODE_D] && player1.pos.x<WIDTH-115 && !(player2.pos.x- player1.pos.x<100 && player2.pos.x>player1.pos.x && player1.pos.y>245))
            player1.pvx=30;
        if(kstate[SDL_SCANCODE_A] && player1.pos.x>-20 && !(player1.pos.x- player2.pos.x<100 && player1.pos.x>player2.pos.x && player1.pos.y>245))
            player1.pvx=-30;
        if(kstate[SDL_SCANCODE_W] && player1.pos.y>=430)
        {
            player1.pvy=-90;
            player1.theta=20;
        }
        if(kstate[SDL_SCANCODE_S])
        {
            player1.theta=-20;
            player1.shoot=true;
        }
        if(kstate[SDL_SCANCODE_C])
        {
            if(player1.powline==LP)
            {
                player1.pow[player1.playernum]=true;
                if(player1.playernum==5 || player1.playernum==7) player1.pow[rand()%5]=true;
                player1.booltimer=true;
                player1.powline=0;
            }
        }
    }
    if(!player2.freez)
    {
        if(kstate[SDL_SCANCODE_L] && player2.pos.x<WIDTH-115 && !(player1.pos.x- player2.pos.x<100 && player1.pos.x>player2.pos.x && player2.pos.y>245))
            player2.pvx=30;
        if(kstate[SDL_SCANCODE_J] && player2.pos.x>-3 && !(player2.pos.x- player1.pos.x<100 && player2.pos.x>player1.pos.x && player2.pos.y>245))
            player2.pvx=-30;
        if(kstate[SDL_SCANCODE_I] && player2.pos.y>=430)
        {
            player2.pvy=-90;
            player2.theta=-20;
        }
        if(kstate[SDL_SCANCODE_K])
        {
            player2.theta=20;
            player2.shoot=true;
        }
        if(kstate[SDL_SCANCODE_N])
        {
            if(player2.powline==LP)
            {
                player2.pow[player2.playernum]=true;
                if(player1.playernum==5 || player1.playernum==7) player2.pow[rand()%5]=true;
                player2.booltimer=true;
                player2.powline=0;
            }
        }
    }
}
void playersplaying()
{
    player1.shoes={player1.pos.x+player1.pos.w/3,player1.pos.y+player1.pos.h,3*player1.pos.w/4,player1.pos.h/3};
    player2.shoes={player2.pos.x-player1.pos.w/15,player2.pos.y+player2.pos.h,3*player2.pos.w/4,player2.pos.h/3};
    player1.pos.y+=player1.pvy*t;
    player1.pos.x+=player1.pvx*t;
    player2.pos.x+=player2.pvx*t;
    player1.prect={player1.pos.x-12,player1.pos.y,180,170};
    player2.prect={player2.pos.x-20,player2.pos.y,180,170};
    if(player1.pos.y<430-2)
    {
        player1.pvy+=g*t;
    }
    else
    {
        player1.pos.y=430;
        player1.pvy=0;
        if(!player1.shoot) player1.theta=0;
    }
    player2.pos.y+=player2.pvy*t;
    if(player2.pos.y<430-2)
    {
        player2.pvy+=g*t;
    }
    else
    {
        player2.pos.y=430;
        player2.pvy=0;
        if(!player2.shoot) player2.theta=0;
    }
    if(player2.pow[4]) player1.freez=true;
    if(player1.pow[4]) player2.freez=true;
}
void ballplaying()
{
    double x=ball.x+30;
    double y=ball.y+30;
    x+=vx*t;
    y+=vy*t;
    if(vx>90) vx=90; if(vx<-90) vx=-90;
    if(vy>90) vy=90; if(vy<-90) vy=-90;
    if(x<=110 && y<=350) vx*=-1;
    while(x<=110 && y<350)
    {
        x+=vx*0.1;
        y-=vy*0.1;
    }
    if(x>=1175 && y<350) vx*=-1;
    while(x>=1175 && y<350)
    {
        x+=vx*0.1;
        y-=vy*0.1;
    }
    if(y-vy*t<625 && !(player1.sshoot || player2.sshoot)) vy+=g*t;
    if(y>=625)
    {
        sounds.ballbouncesound();
        vy=-zarib*sqrt(vy*vy-2*g*(y-625));
        y=625;
    }
    //ball hitting the shoes
    if(x<=player1.pos.x+160 && x>=player1.pos.x+135 && y>=player1.pos.y+155 && y<=player1.pos.y+235)
    {
        x=player1.pos.x+165;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
        if(player1.pow[2])
        {
            vx=vshoot; vy=0;
            player1.sshoot=true;
            player1.pow[2]=false;
        }
    }
    if(x>=player1.pos.x+65 && x<=player1.pos.x+135 && y>=player1.pos.y+148 && y<=player1.pos.y+198)
    {
        y=player1.pos.y+145;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
        if(player1.pow[2])
        {
            vx=vshoot; vy=0;
            player1.sshoot=true;
            player1.pow[2]=false;
        }
    }
    if((x>=player1.pos.x+65 && x<=player1.pos.x+135 && y<=player1.pos.y+244 && y>=player1.pos.y+197) || (x>=player1.pos.x+45 && x<player1.pos.x+65 && y<=player1.pos.y+220 && y>=player1.pos.y+200))
    {
        y=player1.pos.y+246;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
        if(player1.pow[2])
        {
            vx=vshoot; vy=0;
            player1.sshoot=true;
            player1.pow[2]=false;
        }
    }
    //cout<<endl<<player1.pow[2]<<endl<<player1.sshoot<<endl; player2.sshoot
    if(player1.sshoot && x>=player2.pos.x-20 && y>=player2.pos.y+145 && y<=player2.pos.y+210)
    {
        if(vx<0) vx*=-1;
        player2.pvx=vx;
        player2.pos.x+=vx*t;
        vx-=28*t;
        if(vx<=9)
        {
            player2.freez=true;
            player1.booltimer=true;
            vx=-10;
            player1.sshoot=false;
        }
    }
    if(player1.booltimer)
    {
        player1.timepower=(SDL_GetTicks()-start)/1000;
        player1.booltimer=false;
    }
    if(timep-player1.timepower>=5)
        player2.freez=false;
    //ball hitting the body
    if(x<=player1.pos.x+134 && x>=player1.pos.x+44 && y>=player1.pos.y+122 && y<=player1.pos.y+180)
    {
//        vx*=-1;
//        while(vx>0 && x<player1.pos.x+135)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=player1.pos.x+135;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    double x1=player1.pos.x+75,y1=player1.pos.y+75,dist,c,s;
    dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));
    c=(x1-x)/dist; s=(y1-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player1.pvx)*(s*s-c*c)-2*(vy-player1.pvy)*s*c;
        vy=(vy-player1.pvy)*(c*c-s*s)-2*(temp-player1.pvx)*s*c;
        player2.sshoot=false;
        }
    if(player1.pow[1])
    {
    double x1=player1.pos.x+D;
    //ball hitting the shoes
    if(x<=x1+160 && x>=x1+135 && y>=player1.pos.y+155 && y<=player1.pos.y+235)
    {
        x=x1+165;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
    }
    if(x>=x1+65 && x<=x1+135 && y>=player1.pos.y+148 && y<=player1.pos.y+196)
    {
        y=player1.pos.y+145;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
    }
    if((x>=x1+65 && x<=x1+135 && y<=player1.pos.y+244 && y>=player1.pos.y+197) || (x>=x1+45 && x<x1+65 && y<=player1.pos.y+220 && y>=player1.pos.y+200) )
    {
        y=player1.pos.y+246;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
    }
    //ball hitting the body
    if(x<=x1+134 && x>=x1+44 && y>=player1.pos.y+122 && y<=player1.pos.y+180)
    {
//        vx*=-1;
//        while(vx>0 && x<player1.pos.x+135)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=x1+135;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    x1=x1+75;
    double y1=player1.pos.y+75,dist,c,s;
    dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));
    c=(x1-x)/dist; s=(y1-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player1.pvx)*(s*s-c*c)-2*(vy-player1.pvy)*s*c;
        vy=(vy-player1.pvy)*(c*c-s*s)-2*(temp-player1.pvx)*s*c;
        }
    x1=player1.pos.x-D;
    //ball hitting the shoes
    if(x<=x1+160 && x>=x1+135 && y>=player1.pos.y+155 && y<=player1.pos.y+235)
    {
        x=x1+165;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
    }
    if(x>=x1+65 && x<=x1+135 && y>=player1.pos.y+148 && y<=player1.pos.y+196)
    {
        y=player1.pos.y+145;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
    }
    if((x>=x1+65 && x<=x1+135 && y<=player1.pos.y+244 && y>=player1.pos.y+197) || (x>=player1.pos.x+45 && x<player1.pos.x+65 && y<=player1.pos.y+220 && y>=player1.pos.y+200) )
    {
        y=player1.pos.y+246;
        vy=2*player1.pvy-vy;
        sounds.ballkicksound();
        if(player1.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
    }
    //ball hitting the body
    if(x<=x1+134 && x>=x1+44 && y>=player1.pos.y+122 && y<=player1.pos.y+180)
    {
//        vx*=-1;
//        while(vx>0 && x<player1.pos.x+135)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=x1+135;
        vx=2*player1.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    x1=x1+75;
    y1=player1.pos.y+75;
    dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));
    c=(x1-x)/dist; s=(y1-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player1.pvx)*(s*s-c*c)-2*(vy-player1.pvy)*s*c;
        vy=(vy-player1.pvy)*(c*c-s*s)-2*(temp-player1.pvx)*s*c;
        }
    } // end of triple clone

    //ball hitting the shoes player 2
   if(x<=player2.pos.x+15 && x>=player2.pos.x-10 && y>=player2.pos.y+155 && y<=player2.pos.y+235)
    {
        x=player2.pos.x-15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=-vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
        if(player2.pow[2])
        {
            vx=-vshoot; vy=0;
            player2.sshoot=true;
            player2.pow[2]=false;
        }
    }
    if(x>=player2.pos.x+15 && x<=player2.pos.x+85 && y>=player2.pos.y+148 && y<=player2.pos.y+196)
    {
        y=player2.pos.y+145;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=-vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
        if(player2.pow[2])
        {
            vx=-vshoot; vy=0;
            player2.sshoot=true;
            player2.pow[2]=false;
        }
    }
    if((x>=player2.pos.x+15 && x<=player2.pos.x+85 && y<=player2.pos.y+244 && y>=player2.pos.y+197) || (x>player2.pos.x+85 && x<player2.pos.x+105 && y<=player2.pos.y+220 && y>=player2.pos.y+200))

    {
        y=player2.pos.y+246;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=-vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
        if(player2.pow[2])
        {
            vx=-vshoot; vy=0;
            player2.sshoot=true;
            player2.pow[2]=false;
        }
    }
     //cout<<player2.sshoot;
    if(player2.sshoot && x<=player1.pos.x+170 && y>=player1.pos.y+145 && y<=player1.pos.y+210)
    {
        if(vx>0) vx*=-1;
        //cout<<vx<<endl;
        player1.pvx=vx;
        player1.pos.x+=vx*t;
        vx+=28*t;

        if(vx>-7)
        {
            player1.freez=true;
            player2.booltimer=true;
            vx=10;
            player2.sshoot=false;
        }
    }
    if(player2.booltimer)
    {
        player2.timepower=(SDL_GetTicks()-start)/1000;
        player2.booltimer=false;
    }
    if(timep-player2.timepower>=5)
        player1.freez=false;
    //ball hitting the body
       if(x<=player2.pos.x+106 && x>=player2.pos.x+16 && y>=player2.pos.y+122 && y<=player2.pos.y+180)
    {
//        vx*=-1;
//        while(vx<0 && x>player2.pos.x+25)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=player2.pos.x+15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    double x2=player2.pos.x+70,y2=player2.pos.y+75;
    dist=sqrt((y2-y)*(y2-y)+(x2-x)*(x2-x));
    c=(x2-x)/dist; s=(y2-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y2-y)*(y2-y)+(x2-x)*(x2-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player2.pvx)*(s*s-c*c)-2*(vy-player2.pvy)*s*c;
        vy=(vy-player2.pvy)*(c*c-s*s)-2*(temp-player2.pvx)*s*c;
        player1.sshoot=false;

        }
//    if(player2.sshoot && x<=player1.pos.x+165 && y>=player1.pos.y+25 && y<=player1.pos.x+210)
//    {
//        player1.pvx=vx;
//        player1.pos.x+=vx*t;
//        vx+=10*t;
//        if(vx>0) player2.sshoot=false;
//    }
    if(player2.pow[1])
    {
    double x1=player2.pos.x+D;
    //ball hitting the shoes
    if(x<=x1+15 && x>=x1-10 && y>=player2.pos.y+155 && y<=player2.pos.y+235)
    {
        x=x1-15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
    }
    if(x>=x1+15 && x<=x1+85 && y>=player2.pos.y+148 && y<=player2.pos.y+196)
    {
        y=player2.pos.y+145;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
    }
    if((x>=x1+15 && x<=x1+85 && y<=player2.pos.y+244 && y>=player2.pos.y+197) || (x>x1+85 && x<x1+105 && y<=player2.pos.y+220 && y>=player2.pos.y+200))
    {
        y=player2.pos.y+246;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
    }
    //ball hitting the body
    if(x<=x1+106 && x>=x1+16 && y>=player2.pos.y+122 && y<=player2.pos.y+180)
    {
//        vx*=-1;
//        while(vx>0 && x<player1.pos.x+135)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=x1+15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    x1=x1+75; y1=player2.pos.y+75;
    dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));
    c=(x1-x)/dist; s=(y1-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player2.pvx)*(s*s-c*c)-2*(vy-player2.pvy)*s*c;
        vy=(vy-player2.pvy)*(c*c-s*s)-2*(temp-player2.pvx)*s*c;
        }
    x1=player2.pos.x-D;
    //ball hitting the shoes
     if(x<=x1+15 && x>=x1-10 && y>=player2.pos.y+155 && y<=player2.pos.y+235)
    {
        x=x1-15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/12);
            vy=-vshoot*sin(M_PI/12);
        }
    }
    if(x>=x1+15 && x<=x1+85 && y>=player2.pos.y+148 && y<=player2.pos.y+196)
    {
        y=player2.pos.y+145;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=-vshoot*sin(M_PI/4);
        }
    }
    if((x>=x1+15 && x<=x1+85 && y<=player2.pos.y+244 && y>=player2.pos.y+197) || (x>x1+85 && x<x1+105 && y<=player2.pos.y+220 && y>=player2.pos.y+200))
    {
        y=player2.pos.y+246;
        vy=2*player2.pvy-vy;
        sounds.ballkicksound();
        if(player2.shoot)
        {
            vx=vshoot*cos(M_PI/4);
            vy=vshoot*sin(M_PI/4);
        }
    }
    //ball hitting the body
    if(x<=x1+106 && x>=x1+16 && y>=player2.pos.y+122 && y<=player2.pos.y+180)
    {
//        vx*=-1;
//        while(vx>0 && x<player1.pos.x+135)
//        {
//            x+=vx*0.01;
//            y+=-vy*0.01;
//            vy+=-g*0.01;
//        }
//        vx*=-1;
        x=x1+15;
        vx=2*player2.pvx-vx;
        sounds.ballkicksound();
    }
    //ball hitting the head
    x1=x1+75; y1=player2.pos.y+75;
    dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));
    c=(x1-x)/dist; s=(y1-y)/dist;
    if(dist<=50+25)
        {
        sounds.ballkicksound();
        //the ball gets inside
        vx*=-1; vy*=-1;
        while(dist<50+25){
            x+=vx/50*t;
            y+=vy/50*t;
            dist=sqrt((y1-y)*(y1-y)+(x1-x)*(x1-x));  }
        vx*=-1; vy*=-1;
        //the ball is out
        double temp=vx;
        vx=(vx-player2.pvx)*(s*s-c*c)-2*(vy-player2.pvy)*s*c;
        vy=(vy-player2.pvy)*(c*c-s*s)-2*(temp-player2.pvx)*s*c;
        }
    }

    ball.x=x-30;
    ball.y=y-30;
    if(x<=55 && y>=350){
        player2.goalnum++;
        if(player2.powline<=LP-0.5) player2.powline+=0.5;
        player2.sshoot=false;
        goalcelebrate();}
    if(x>=1220 && y>=350){
        player1.goalnum++;
        if(player1.powline<=LP-0.5) player1.powline+=0.5;
        player1.sshoot=false;
        goalcelebrate();}
}
void render()
{
    backi++; if(backi>99) backi=0;
    //player1.pos.h=170-2*sin(time*10000); player1.pos.y=400+2*sin(time*10000);
    SDL_RenderCopy(renderer, backimg[backi/50], NULL, &backg);
    //filledEllipseRGBA(renderer,ball.x+30,ball.y+30,25,25,245,245,255,255);
    (player1.pvx==0)? 0: player1.theta=5*sin((SDL_GetTicks()-start)/100);
    (player2.pvx==0)? 0: player2.theta=5*sin((SDL_GetTicks()-start)/100);
    player1.shoescenter={45,15};
    SDL_RenderCopyEx(renderer,shoesimg,NULL,&player1.shoes,player1.theta,&player1.shoescenter,SDL_FLIP_NONE);
    SDL_RenderCopy(renderer, player1.pimg, NULL,&player1.pos);
    player2.shoescenter={65,15};
    SDL_RenderCopyEx(renderer,shoesimg,NULL,&player2.shoes,player2.theta,&player2.shoescenter,SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, player2.pimg, NULL, &player2.pos,NULL,NULL,SDL_FLIP_HORIZONTAL);
    //tashkhis

//    ellipseRGBA(renderer,player1.pos.x+75,player1.pos.y+75,50,50,255,255,255,255);
//    rect(renderer,player1.pos.x+60,player1.pos.y+120,40,50,255,255,255,255,0);
//    rect(renderer,player1.pos.x+65,player1.pos.y+180,70,30,255,255,255,255,0);
//
//    ellipseRGBA(renderer,player2.pos.x+70,player2.pos.y+75,50,50,255,255,255,255);
//    rect(renderer,player2.pos.x+50,player2.pos.y+120,40,50,255,255,255,255,0);
//    rect(renderer,player2.pos.x+15,player2.pos.y+180,70,30,255,255,255,255,0);
    if(player1.pow[1])
    {
        SDL_Rect pc1={player1.pos.x+D,player1.pos.y,player1.pos.w,player1.pos.h};
        SDL_Rect pc2={player1.pos.x-D,player1.pos.y,player1.pos.w,player1.pos.h};
        SDL_Rect psc1={player1.shoes.x+D,player1.shoes.y,player1.shoes.w,player1.shoes.h};
        SDL_Rect psc2={player1.shoes.x-D,player1.shoes.y,player1.shoes.w,player1.shoes.h};
        SDL_RenderCopyEx(renderer,shoesimg,NULL,&psc1,player1.theta,&player1.shoescenter,SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, player1.pimg, NULL,&pc1);
        SDL_RenderCopyEx(renderer,shoesimg,NULL,&psc2,player1.theta,&player1.shoescenter,SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, player1.pimg, NULL,&pc2);
    }
    if(player2.pow[1])
    {
        SDL_Rect pc1={player2.pos.x+D,player2.pos.y,player2.pos.w,player2.pos.h};
        SDL_Rect pc2={player2.pos.x-D,player2.pos.y,player2.pos.w,player2.pos.h};
        SDL_Rect psc1={player2.shoes.x+D,player2.shoes.y,player2.shoes.w,player2.shoes.h};
        SDL_Rect psc2={player2.shoes.x-D,player2.shoes.y,player2.shoes.w,player2.shoes.h};
        SDL_RenderCopyEx(renderer,shoesimg,NULL,&psc1,player2.theta,&player2.shoescenter,SDL_FLIP_HORIZONTAL);
        SDL_RenderCopyEx(renderer, player2.pimg, NULL, &pc1,NULL,NULL,SDL_FLIP_HORIZONTAL);
        SDL_RenderCopyEx(renderer,shoesimg,NULL,&psc2,player2.theta,&player2.shoescenter,SDL_FLIP_HORIZONTAL);
        SDL_RenderCopyEx(renderer, player2.pimg, NULL, &pc2,NULL,NULL,SDL_FLIP_HORIZONTAL);
    }
    static int a=0,b=0;
    SDL_Rect starrect[2];
    if(player1.freez)
    {
        starrect[0]={player1.pos.x-20,player1.pos.y-30,200,80};
        SDL_RenderCopy(renderer,stars[a],NULL,&starrect[0]);
        a++;
        if(a>9) a=0;
    }
    if(player2.freez)
    {
        starrect[1]={player2.pos.x-20,player2.pos.y-30,200,80};
        SDL_RenderCopy(renderer,stars[b],NULL,&starrect[1]);
        b++;
        if(b>9) b=0;
    }
    //sharta
//    rect(renderer,0,0,110,350,255,255,255,255,1);
//    rect(renderer,1175,0,WIDTH-1150,350,255,255,255,255,1);
    //rect(renderer,0,600,300,20,255,255,255,1);
    if(vx>0)
    {
        ballstate++;
        if(ballstate>19) ballstate=0;
    }
    else if(vx<0)
    {
        ballstate--;
        if(ballstate<0) ballstate=19;
    }
    if(player1.playernum!=0 && player1.playernum!=6)
        thickLineRGBA(renderer,482,175,482,175-player1.powline,15,255,0,0,255);
    if(player2.playernum!=0 && player2.playernum!=6)
        thickLineRGBA(renderer,798,175,798,175-player2.powline,15,255,0,0,255);
    if(player1.pow[4])
        SDL_RenderCopy(renderer, punchimg, NULL, &player1.prect);
    if(player2.pow[4])
        SDL_RenderCopyEx(renderer, punchimg, NULL, &player2.prect,NULL,NULL,SDL_FLIP_HORIZONTAL);
    if(player1.pow[player1.playernum])
    {
        if(player1.booltimer)
        {
            player1.timepower=(SDL_GetTicks()-start)/1000;
            player1.booltimer=false;
        }
        if(timep-player1.timepower>=5)
            for(int i=0;i<8;i++) player1.pow[i]=false;
    }
    if(player2.pow[player2.playernum])
    {
        if(player2.booltimer)
        {
            player2.timepower=(SDL_GetTicks()-start)/1000;
            player2.booltimer=false;
        }
        if(timep-player2.timepower>=5)
            for(int i=0;i<8;i++) player2.pow[i]=false;
    }
    map<int, string>mymap;
    mymap[0]="   No Power   ";
    mymap[1]=" Triple Clone ";
    mymap[2]="Kick Fire Ball";
    mymap[3]="Invisible Ball";
    mymap[4]="     Punch    ";
    mymap[5]="     Thief    ";
    for(int i=0;i<5;i++)
    {
        if(player1.pow[i] && player1.playernum) text(renderer,200,750,200,50,"FreeSans.ttf",50,255,0,0,255,mymap[i]);
        if(player2.pow[i] && player2.playernum) text(renderer,900,750,200,50,"FreeSans.ttf",50,255,0,0,255,mymap[i]);
    }
    text(renderer,690,95,70,100,"FreeSans.ttf",70,255,255,255,255,to_string(player2.goalnum));
    text(renderer,520,95,70,100,"FreeSans.ttf",70,255,255,255,255,to_string(player1.goalnum));
    text(renderer,620,110,40,65,"FreeSans.ttf",70,255,255,255,255,to_string(timep));
    text(renderer,510,45,110,35,"FreeSans.ttf",60,255,255,255,255,player1.name.c_str());
    text(renderer,660,45,110,35,"FreeSans.ttf",60,255,255,255,255,player2.name.c_str());
    if(!player1.pow[3] && !player2.pow[3])
        SDL_RenderCopy(renderer, ballimg[ballstate/5], NULL, &ball);
}
void update_scores()
{
    int won1=0,won2=0,equall=0,winner=0;
    if(player1.goalnum>player2.goalnum){
        winner=1;
        won1=1;}
    else if( player1.goalnum<player2.goalnum){
        winner=2;
        won2=1;}
    else equall=1 ;
    int tedad=0;
    string s ;
    ifstream read("jadval.txt");
    while(getline(read,s))
        tedad++;
    read.close();
    ifstream fin("jadval.txt") ;
    string names[tedad+2] ; // if 2 new player
    int lists[tedad+2][8];
    int r=0;
    while(fin>>names[r++]){
        for(int j=0;j<8;j++)
            fin>>lists[r-1][j];}
    fin.close();
    int t1=-1 , t2=-1 ;
    for(int i=0;i<tedad;i++){
        if(player1.name==names[i]) t1=i;
        if(player2.name==names[i]) t2=i;}
    string all[9]={"name","played","won","equal","lost","for","against","goaldifference","points"};
    char space=' '; // name /played/ won/ equal /lost / for(zade)/ against /goaldifference(zade-khorde) / points
    if(t1==-1){
        names[tedad]=player1.name;
        lists[tedad][0]=1;
        lists[tedad][1]=won1;
        lists[tedad][2]=equall;
        lists[tedad][3]=won2;
        lists[tedad][4]=player1.goalnum;
        lists[tedad][5]=player2.goalnum;
        lists[tedad][6]=player1.goalnum-player2.goalnum;
        lists[tedad][7]=won1*3+equall;
        tedad+=1;
        }
    else{
        lists[t1][0]+=1;
        lists[t1][1]+=won1;
        lists[t1][2]+=equall;
        lists[t1][3]+=won2;
        lists[t1][4]+=player1.goalnum;
        lists[t1][5]+=player2.goalnum;
        lists[t1][6]+=player1.goalnum-player2.goalnum;
        lists[t1][7]+=won1*3+equall;
        }
    if(t2==-1){
        names[tedad]=player2.name;
        lists[tedad][0]=1;
        lists[tedad][1]=won2;
        lists[tedad][2]=equall;
        lists[tedad][3]=won1;
        lists[tedad][4]=player2.goalnum;
        lists[tedad][5]=player1.goalnum;
        lists[tedad][6]=player2.goalnum-player1.goalnum;
        lists[tedad][7]=won2*3+equall;
        tedad+=1;
        }
    else{
        lists[t2][0]+=1;
        lists[t2][1]+=won2;
        lists[t2][2]+=equall;
        lists[t2][3]+=won1;
        lists[t2][4]+=player2.goalnum;
        lists[t2][5]+=player1.goalnum;
        lists[t2][6]+=player2.goalnum-player1.goalnum;
        lists[t2][7]+=won2*3+equall;
        }
    ofstream rewrite( "jadval.txt") ;
    for(int a=0 ; a<tedad ; a++){
        rewrite<<names[a]<<space;
        for(int b=0 ; b<8 ; b++)
            rewrite<<lists[a][b]<<space;
    rewrite<<endl;
    }
rewrite.close();
}
void sortt(string names[],int tedad,int lists[][8])
{
    for(int i=0 ;i<tedad-1 ; i++)
    for(int j=0 ; j<tedad-1-i ; j++)
    if(lists[j][7]<lists[j+1][7])
    {
        //switch names
        string temp="";
        temp=names[j];
        names[j]=names[j+1] ;
        names[j+1]=temp ;
        //switch all data
        for(int u=0 ; u<8 ; u++){
        int temp=0;
        temp=lists[j][u] ;
        lists[j][u]= lists[j+1][u];
        lists[j+1][u]=temp; }
   }
}
void texture(SDL_Renderer *m_renderer,int xp,int yp,string addressOfImage,int width,int height)
{
        int n = addressOfImage.length();
        char char_array[n+1];
        strcpy(char_array, addressOfImage.c_str());
        SDL_Texture *myTexture;
        myTexture = IMG_LoadTexture( m_renderer, char_array);
        int w1, h1;
        SDL_QueryTexture(myTexture, NULL, NULL, &w1, &h1);
        SDL_Rect texr1;
        texr1={xp,yp,width,height};
        SDL_RenderCopy( m_renderer, myTexture, NULL, &texr1);
        SDL_DestroyTexture(myTexture);
}
void goalcelebrate()
{
    char c='a';
    for(int i=0;i<25;i++)
    {
        string goal="g";
        goal+=c;
        render();
        boxRGBA(renderer,0,0,WIDTH,HEIGHT,0,0,0,150);
        texture(renderer,200,20,goal+".gif",WIDTH-400,HEIGHT-100);
        SDL_RenderPresent(renderer );
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_Delay(40);
        c++;
    }
    ball.x=WIDTH/2-30; ball.y=450-30;
    vx=0; vy=0;
    player1.pos.x=110; player1.pos.y=428;
    player2.pos.x=1020; player2.pos.y=428;
    player1.sshoot=false;
    player2.sshoot=false;
    player1.freez=false;
    player2.freez=false;
    for(int i=0;i<6;i++) player1.pow[i]=false;
    for(int i=0;i<6;i++) player2.pow[i]=false;
}
void text(SDL_Renderer *m_renderer,int xp,int yp,int w,int h,string addressOfFont,int fontsize , int R , int G , int B , int A , string S)
{
    TTF_Font* Sans = TTF_OpenFont(addressOfFont.c_str(), fontsize);
    SDL_Color textcolor = {R, G, B , A};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, S.c_str(),textcolor);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = xp;
    Message_rect.y = yp;
    Message_rect.w = w;
    Message_rect.h = h;
    SDL_RenderCopy(m_renderer , Message ,NULL , &Message_rect);
    SDL_DestroyTexture(Message);
    TTF_CloseFont(Sans);
}
