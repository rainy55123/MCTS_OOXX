#include<cstdlib>
#include<iostream>
#include<math.h>
#include<ctime>
using namespace std;
int Win=999;
int ComType;
class Tree
{
      public:
             Tree()
             {
                   root=NULL;
             }
             ~Tree()
             {
                   for(int i=0;i<9;i++)
                       delete Child[i];
                   delete Parent; 
             }
             void MCTS(int map[9],int times,int Type)
             {
                  srand((unsigned)time(NULL));
                  root=NULL;
                  ComType=Type;
                  int i,T=0,Choose;
                  double MaxValue=0;
                  if(root==NULL)                    //�Ы�root 
                  {
                                
                        root=new Tree;
                        root->Parent=NULL;
                        root->win=0;
                        root->isLeaf=true;
                        root->visit=0;
                        for(i=0;i<9;i++)
                        {
                            root->map[i]=map[i];
                            root->Child[i]=NULL;
                        }
                        root->value=1;
                  }
                  Tree *Now=root;
                  while(T!=times)                 //����N�� 
                  {
                        Now=root;
                        Select(Now);    
                        T++;
                  }
                  i=0;
                  while(root->Child[i]!=NULL&&i<9)
                  {
                        cout<<"Child "<< i <<" Value = "<<root->Child[i]->value<<endl;                
                        if(root->Child[i]->value>MaxValue)
                        {
                              MaxValue=root->Child[i]->value;
                              Choose=i;                         
                        }
                        i++;
                  }
                  
                  Now = root->Child[Choose];
                  for(i=0;i<9;i++)                    //�^�ǳ̦n���@�� 
                  {
                        if(map[i]!=Now->map[i])
                        {
                              map[i]=Now->map[i];                 
                        }          
                  }
             }
             
             void Select(Tree *Now)
             {
                  int i,Choose;
                  double MaxValue;
                  while(Now->isLeaf!=true)              //�j�M�l���I 
                  {
                       i=0;
                       MaxValue=-1;
                       while(Now->Child[i]!=NULL&&i<9)
                       {
                            if(Now->Child[i]->value>MaxValue)   
                            {
                                  MaxValue=Now->Child[i]->value;
                                  Choose=i;                      
                            }                 
                            i++;                     
                       } 
                       Now=Now->Child[Choose];
                  }
                  Expansion(Now);
             }
             
             void Expansion(Tree *Selected)
             {
                  Tree *Now;
                  int i,j,num=0,Sum,CircleNum;
                  if(!isEnd(Selected->map))           //�T�{��e�L���O�_�w���� , �w�����N���ݭn���U�X�i 
                  {
                      for(i=0;i<9;i++)
                      {
                          if(Selected->map[i]==0)       //�N�Ҧ��i�ౡ�p�X�i , (�إ߾𸭨ð��s��)
                          {
                               Sum=0;
                               CircleNum=0;
                               Now=Selected->Child[num];
                               Now=new Tree;  
                               Now->visit=0;
                               Now->value=100;
                               Now->win=0;
                               for(j=0;j<9;j++)
                               {
                                   Now->Child[j]=NULL;
                                   Now->map[j]=Selected->map[j];
                                   if(Selected->map[j]==1)
                                       CircleNum++;
                                   if(Selected->map[j]!=0)
                                       Sum++;
                               }
                               if(Sum/2 == CircleNum)
                                   Now->map[i]=1;
                               else
                                   Now->map[i]=2;
                               Now->Parent=Selected;
                               Now->isLeaf=true;
                               Selected->Child[num]=Now;
                               Selected->isLeaf=false;   
                               num++;                 
                          }            
                      }
                  }
                  Simulation(Selected);
             }
             
             void Simulation(Tree *Selected)
             {
                  Tree *Now=Selected,*Record;
                  int RandomMap[9],i=0,Choose,Order,Sum=0,CircleNum=0,Winner=999;
                  double MaxValue=-1;
                  /*while(Now->isLeaf!=true)
                  {
                       while(Now->Child[i]!=NULL&&i<9)
                       {
                            if(Now->Child[i]->value>MaxValue)
                            {
                                 MaxValue=Now->Child[i]->value;
                                 Choose=i;                           
                            }             
                            i++;        
                       }              
                       Now=Now->Child[Choose];
                  }*/

                  if(Now->isLeaf!=true)       //�Ҽ{��i��O�����L���ӨS���X�i , �ҥH�p�G���O�� , �h����Ĥ@�Ӥl���I  
                       Now=Now->Child[0];
                  for(i=0;i<9;i++)            //�ƻs��e�L���Ӱ�����, �çP�_�U�@��OO�٬OX�U 
                  {
                       RandomMap[i]=Now->map[i];           
                       if(RandomMap[i]==1)
                            CircleNum++;
                       if(RandomMap[i]!=0)
                            Sum++;
                  }
                  if(Sum/2 == CircleNum)
                       Order=1;
                  else
                       Order=2;
                  while(!isEnd(RandomMap))   //�����쵲������, �ýT�{�ӧQ�� 
                  {
                       int RandNum;
                       do
                       {
                           RandNum = rand()%9;    
                       }while(RandomMap[RandNum]!=0);
                       RandomMap[RandNum]=Order;
                       Order=Order%2;    
                       Order++;
                  }
                  Backpropagation(Now);
             } 
             
             void Backpropagation(Tree *Selected)
             {
                  Tree *Now=Selected;
                  int i;
                  while(Now!=NULL)               //��s��e�𸭤Ψ�Ҧ� Parent �����X���ƤγӧQ���� 
                  {
                       Now->visit+=1;
                       if(Win==2&&ComType==0)
                           Now->win+=1;
                       else if(Win==1&&ComType==1)
                           Now->win+=1;
                       else if(Win==0)
                           Now->win+=0.5;   
                       Now = Now->Parent;     
                  }
                  update(root);               //�qroot�}�l����Ҧ��l�`�I �� value�Ȫ���s 
             }
             
             void update(Tree *node)
             {
                  int i=0;
                  if(node!=root&&node->visit!=0)
                  {
                       node->value=node->win/node->visit + sqrt(2*log(node->Parent->visit)/node->visit);        //UCT����  ExeValue ���]��1 
                  }
                  while(node->Child[i]!=NULL&&i<9)
                  {
                       update(node->Child[i]);
                       i++;                           
                  }
             }
             
             bool isEnd(int map[9])            //�T�{�L���O�_���� , �ç�X�ӧQ�� 
             {
                 if(map[0]==map[1]&map[1]==map[2]&map[0]!=0)
                 {
                       if(map[0]==1)
                           Win=1;
                       else
                           Win=2;                       
                       return true;                           
                 }
                 else if(map[3]==map[4]&map[4]==map[5]&map[3]!=0)
                 {
                       if(map[3]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[6]==map[7]&map[7]==map[8]&map[6]!=0)
                 {
                       if(map[6]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[0]==map[3]&map[3]==map[6]&map[0]!=0)
                 {
                       if(map[0]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[1]==map[4]&map[4]==map[7]&map[1]!=0)
                 {
                       if(map[1]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[2]==map[5]&map[5]==map[8]&map[2]!=0)
                 {
                       if(map[2]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[0]==map[4]&map[4]==map[8]&map[0]!=0)
                 {
                       if(map[0]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[2]==map[4]&map[4]==map[6]&map[2]!=0)
                 {
                       if(map[2]==1)
                           Win=1;
                       else
                           Win=2;             
                       return true;                           
                 }
                 else if(map[0]!=0&&map[1]!=0&&map[2]!=0&&map[3]!=0&&map[4]!=0&&map[5]!=0&&map[6]!=0&&map[7]!=0&&map[8]!=0)
                 {
                       Win=0;            
                       return true;
                 }
                 return false;
             }
             double value,win;
             bool isLeaf;
             int map[9],visit;
             Tree *root,*Parent,*Child[9];
};
