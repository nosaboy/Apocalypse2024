#include <Arduino.h>

#include <Vector.h>
#define MAX_SIZE 1000

template <class T>
class Queue {
  private:
    int front, rear;
    T arr[MAX_SIZE];
    
  public:
    Queue() {
      front = -1;
      rear = -1;
    }
    
    bool isEmpty() {
      return (front == -1 && rear == -1);
    }
    
    bool isFull() {
      return ((rear + 1) % MAX_SIZE == front);
    }
    
    void enqueue(T value) {
      if (isFull()) {
        Serial.println("Queue is full");
        return;
      }
      else if (isEmpty()) {
        front = rear = 0;
      }
      else {
        rear = (rear + 1) % MAX_SIZE;
      }
      arr[rear] = value;
    }
    
    void dequeue() {
      if (isEmpty()) {
        Serial.println("Queue is empty");
        return;
      }
      else if (front == rear) {
        front = rear = -1;
      }
      else {
        front = (front + 1) % MAX_SIZE;
      }
    }
    
    T peek() {
      if (isEmpty()) {
        Serial.println("Queue is empty");
        return T();
      }
      return arr[front];
    }
};
struct pair {
    int first;
    int second;
};
struct tuple {
    int first;
    int second;
    int third;
    // Add more members as needed
};

//initializing and declaring led rows
int column[16]={13,12,11,10,9,8,7,6,5,4,3,2,1,0,A5,A4};
//initializing and declaring led layers
int layer[4]={A0,A1,A2,A3};
pair mp[4][4][4];
tuple path[10]; 
int pathnum = 1;
void setup() {
  // put your setup code here, to run once:
  // tells arduino that pins 13,12,11 are outputs
  Serial.begin(9600);
  for(int i = 0;i<16;i++){
    pinMode(column[i], OUTPUT);
  }
  for(int i = 0;i<4;i++){
    pinMode(layer[i], OUTPUT);
  }
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      for(int l = 0;l<4;l++){
        mp[i][j][l]={layer[i], column[j*4+l]};
      }
    }
  }


  // get input
  int startx = 0; int starty = 0; int startz = 0;
  int endx = 3; int endy = 3; int endz = 3;
  bool erased[4][4][4];
  memset(erased,0,sizeof(erased));




  // create graph
  Vector<tuple> aj[4][4][4];
      
  for(int i = 0;i<4;i++){
      for(int j = 0;j<4;j++){
          for(int l = 0;l<4;l++){
              if(erased[i][j][l] == 0){
                  // i
                  if(i - 1 >= 0){
                      aj[i][j][l].push_back({i-1,j,l});
                  }
                  if(i+1<4){
                      aj[i][j][l].push_back({i+1,j,l});
                  }
                  // j
                  if(j - 1 >= 0){
                      aj[i][j][l].push_back({i,j-1,l});
                  }
                  if(j+1<4){
                      aj[i][j][l].push_back({i,j+1,l});
                  }
                  // l
                  if(l - 1 >= 0){
                      aj[i][j][l].push_back({i,j,l-1});
                  }
                  if(l+1<4){
                      aj[i][j][l].push_back({i,j,l+1});
                  }
              }
              
          }
      }   
  }

  Queue <tuple> q;
  int dist[4][4][4];
  tuple par[4][4][4];
  for(int i = 0;i<4;i++){
      for(int j = 0;j<4;j++){
          for(int l = 0;l<4;l++){
              dist[i][j][l] = 100;
              par[i][j][l] = {-1,-1,-1};
          }
      }
  }

  dist[startx][starty][startz] = 0;
  q.enqueue({startx, starty, startz});
  while(!q.isEmpty()){
      tuple node = q.peek();
      int nodex = node.first;
      int nodey = node.second;
      int nodez = node.third;
      // cout<<nodex<<" "<<nodey<<" "<<nodez<<" "<<int(aj[nodex][nodey][nodez].size())<<endl;
      q.dequeue(); // pop
      for(int i = 0;i<int(aj[nodex][nodey][nodez].size());i++){
          int x = aj[nodex][nodey][nodez][i].first;
          int y = aj[nodex][nodey][nodez][i].second;
          int z = aj[nodex][nodey][nodez][i].third;
          if(dist[x][y][z] == 100){ // not vis
              dist[x][y][z] = dist[nodex][nodey][nodez] + 1;
              par[x][y][z] = node;
              q.enqueue({x,y,z}); // push
          }
      }
  }
  if(dist[endx][endy][endz] == 100){
      // cout<<"IMPOSSIBLE\n";
  }
  else{
      // cout<<dist[endx][endy][endz]<<endl;
  }

  int nodex = endx;
  int nodey = endy;
  int nodez = endz;
  Serial.print(nodex);
  path[pathnum] = {nodex,nodey,nodez};
  pathnum++;
  while(!(nodex == startx && nodey == starty && nodez == startz)){
    int newnodex = par[nodex][nodey][nodez].first;
    int newnodey = par[nodex][nodey][nodez].second;
    int newnodez = par[nodex][nodey][nodez].third;
    nodex = newnodex;
    nodey = newnodey;
    nodez = newnodez;
    path[pathnum] = {nodex,nodey,nodez};
    pathnum++;
  }
  
  
}

void cool(){
  for(int i = 0; i<4; i++){
    digitalWrite(layer[i], 1);
    for(int j = 0; j<16; j++){
      digitalWrite(column[j], 1);
      delay(i+j+50);
     
    }
    digitalWrite(layer[i], 0);
  }

}
void loop() {
  cool();
  // put your main code here, to run repeatedly:
  for(int i = pathnum-1;i>=0;i--){
    // cout<<get<0>(path[i])<<" "<<get<1>(path[i])<<" "<<get<2>(path[i])<<endl;
    pair led = mp[path[i].first][path[i].second][path[i].third];
    digitalWrite(led.first, 1); // turn on leds
    delay(500);
    digitalWrite(led.first, 0); // turn off leds
    delay(500);
    digitalWrite(led.second, 1);
    delay(500);
    digitalWrite(led.second, 0);
    delay(500);

  }
}


  
