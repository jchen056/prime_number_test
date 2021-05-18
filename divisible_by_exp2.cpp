#include<iostream>
using namespace std;
#include<vector>
using std::vector;

int exp2(int x){
  int c=0;
  while(x%2==0){
    c++;
    x/=2;}
  return c;}

int main(){
  int x;
  int maxDeg=0;
  vector<int> V;//vector used to store integers that is divisible by the largest power of 2
  while(cin>>x){
    int c=exp2(x);
    if(c>maxDeg){
      maxDeg=c;
      V.clear();//make V empty
      V={x};
      }
    else if(c==maxDeg){
      V.push_back(x);}}
  //print elements in vector V
  for(size_t i=0;i<V.size();i++){
    cout<<V[i]<<" ";}
  cout<<"\n";
  return 0;}