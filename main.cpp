#include <iostream>
#include "raylib.h"

using namespace std;

Color green=Color{129,204,184,80};
Color yellow=Color{243,213,91,255};
Color darkgreen=Color{20,160,133,255};


int player_score=0;
int cpu_score=0;

bool pause=false;

class Ball{
     public:
     float x,y;
     int speed_x,speed_y;
     int radius;
     void draw(){
         DrawCircle(x,y,radius,yellow);
     }
     void update(){
      if(y + radius >= GetScreenHeight() || y-radius<=0){
        speed_y*=-1;
      }
      if(x + radius >=GetScreenWidth())
      {
        cpu_score++;
        Resetball();
      }
      
      if( x-radius<=0){
        player_score++;
        Resetball();
      }
      x+=speed_x;
      y+=speed_y;
     }

     void Resetball(){
      x=GetScreenWidth()/2;
      y=GetScreenWidth()/2;
      int speed_choice[2]={1,-1};
      speed_x*=speed_choice[GetRandomValue(0,1)];
      speed_y*=speed_choice[GetRandomValue(0,1)];
      
     }
};

class Paddle{
  protected:
  void Limitmovement(){
     if(y+height>=GetScreenHeight()){
            y=GetScreenHeight()-height;

           }
           if(y<=0){
            y=0;
           }
  }
public:
float x,y,height,width;
int speed_y;
      void draw(){
           DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE);
      }
      void update(){
           if(IsKeyDown(KEY_UP)){
            y-=speed_y;
           }
           if (IsKeyDown(KEY_DOWN))
           {
              y+=speed_y;
           }
           Limitmovement();
          
           
      }

};

class Cpu_paddle: public Paddle{
  public:
        void update(int ball_y){
             if(y+height/2>ball_y){
            y-=speed_y;
           }
           if (y+height/2<=ball_y)
           {
              y+=speed_y;
           }
              Limitmovement();
        }
       
};

Ball ball;
Paddle paddle;
Cpu_paddle cpu_paddle;

int main () {
      
      
      const int window_height=800;
      const int window_width=1280;

      //ball values
      ball.x=window_width/2;
      ball.y=window_height/2;
      ball.speed_x=7;
      ball.speed_y=9;
      ball.radius=15;

      //payer paddle value
      paddle.x=window_width-35;
      paddle.y=window_height/2-60;
      paddle.height=120;
      paddle.width=25;
      paddle.speed_y=7;

      //cpu paddle valeu
      cpu_paddle.width=25;
      cpu_paddle.height=120;
      cpu_paddle.x=10;
      cpu_paddle.y=window_height/2 - cpu_paddle.height/2;
      cpu_paddle.speed_y=7;

      

      //main logic
      InitWindow(window_width,window_height,"pong game!!");
      SetTargetFPS(60);
      while (!WindowShouldClose())
      {
        BeginDrawing();
        if(IsKeyPressed(KEY_SPACE))
      pause=!pause;
        //update
        if(!pause){
        ball.update();
        paddle.update();
        cpu_paddle.update(ball.y);
        }
        //collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius, Rectangle{paddle.x,paddle.y,paddle.width,paddle.height})){
          if(ball.speed_x>0){
              ball.speed_x*=-1.1f;
              ball.speed_y=(ball.y-paddle.y)/(paddle.height/2)*ball.speed_x;
              
          }
          
        }

        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius, Rectangle{cpu_paddle.x,cpu_paddle.y,cpu_paddle.width,cpu_paddle.height})){
           if(ball.speed_x<0){
              ball.speed_x*=-1.1f;
               ball.speed_y=(ball.y-cpu_paddle.y)/(cpu_paddle.height/2)* - ball.speed_x;
          }
        }

        //draw
        ClearBackground(darkgreen);
        DrawLine(window_width/2,0,window_width/2,window_height,WHITE);
        cpu_paddle.draw();
        paddle.draw();
        ball.draw();
        DrawCircle(window_width/2,window_height/2,150,green);
        DrawText(TextFormat("%i",cpu_score),window_width/4-20,20,80,WHITE);
        DrawText(TextFormat("%i",player_score),3*window_width/4-20,20,80,WHITE);
        if(pause)
        DrawText("Paused",window_width/2.5,window_height/2,80,WHITE);
        EndDrawing();
      }
      
      



    CloseWindow();
    return 0;
}