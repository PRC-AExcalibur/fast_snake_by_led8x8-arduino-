#include <LedControl.h>

//digital pin
const int din = 7;
const int cs  = 6;
const int clk = 5;
const int sw  = 2;
//analog pin
const int rdm = 14;
const int vrx = 16;
const int vry = 15;
//global var
const int row = 8;
const int col = 8;
const int maxl = row * col;
int FPS = 2;
struct point { int x, y, list_index; point* prior; };   //head<-tail, prior is the pionter
struct snake { point* head; point* tail; int len,vx,vy; };
point place[row][col];
//point_list like this
//  apple | tail | mid...... | head | other space
point* point_list[maxl];
snake player;
LedControl lc = LedControl(din, clk, cs, 1);

void swap_point_list(point* node1, point* node2, point* plist[]) {
    int int_tmp = node1->list_index;
    plist[node1->list_index] = node2;
    plist[node2->list_index] = node1;
    node1->list_index = node2->list_index;
    node2->list_index = int_tmp;
}

void win(){
  for(int i=0;i<row;i++)lc.setRow(0, i, 255);
  FPS=-1;
}

void fail(){
  lc.setRow(0, 0, 255);
  lc.setRow(0, 7, 255);
  FPS=-1;
}

void map_setup(point node[row][col], point* plist[]) {
    int tmp = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            node[i][j].x = i;
            node[i][j].y = j;
            node[i][j].list_index = tmp;
            plist[tmp] = &node[i][j];
            tmp++;
        }
    }
}

snake new_snake(point* plist[]) {
    swap_point_list(plist[maxl / 2 - 4], plist[1], plist);
    swap_point_list(plist[maxl / 2 - 5], plist[2], plist);
    plist[2]->prior = plist[1];
    lc.setLed(0, plist[1]->x, plist[1]->y, true);
    lc.setLed(0, plist[2]->x, plist[2]->y, true);
    return snake{plist[1],plist[2],2,0,1};
}

void new_apple(snake snk,point* plist[]) {
    randomSeed(analogRead(rdm));
    int rdm = random(maxl - snk.len - 1)+ snk.len + 1;
    swap_point_list(plist[rdm], plist[0], plist);
    lc.setLed(0, plist[0]->x, plist[0]->y, true);
}

snake snake_change(point map0[row][col], point* plist[], snake snk) {
    if (snk.len == maxl)win();  //  full of map,win
    int new_x = snk.head->x + snk.vx;
    int new_y = snk.head->y + snk.vy ;     //next point
    if (new_x >= row || new_x < 0 || new_y >= col || new_y < 0) fail(); //out map,fail
    else {
        (snk.head)->prior = &(map0[new_x][new_y]);
        snk.head = &(map0[new_x][new_y]);   //new head
        lc.setLed(0, (snk.head)->x, (snk.head)->y, true);
        if (map0[new_x][new_y].list_index == 0) {
            snk.len++;  //eat apple
            swap_point_list((snk.head), plist[snk.len], plist);
            new_apple(snk, plist);
        }
        else if (map0[new_x][new_y].list_index <= snk.len) fail();   //hit self,fail
        else {
            lc.setLed(0, (snk.tail)->x, (snk.tail)->y, false);
            swap_point_list((snk.head), (snk.tail), plist);   //use new head to replace tail
            snk.tail = (snk.tail)->prior;
        }
    }
    return snk;
}

snake set_dir(snake snk) {
  if (analogRead(vrx)>768 and snk.vx != -1) snk.vx = 1, snk.vy = 0;
  else if (analogRead(vrx)<256 && snk.vx != 1) snk.vx = -1, snk.vy = 0;
  else if (analogRead(vry)>768 && snk.vy != -1) snk.vx = 0, snk.vy = 1;
  else if (analogRead(vry)<256 && snk.vy != 1) snk.vx = 0, snk.vy = -1;
  return snk;
}

void setup() {
  pinMode(sw, INPUT_PULLUP);
  lc.shutdown(0, false);      //启动时，MAX72XX处于省电模式
  lc.setIntensity(0, 1);      //将亮度设置为最大值
  lc.clearDisplay(0);         //清除显示
  map_setup(place, point_list);
  player = new_snake(point_list);
  new_apple(player, point_list);
}
void loop() {
  if(FPS!=-1){
    player = set_dir(player);
    delay(500 / FPS);
    player = set_dir(player);
    player = snake_change(place, point_list, player);
    delay(500 / FPS);
  }
  else{delay (1000);}
}
