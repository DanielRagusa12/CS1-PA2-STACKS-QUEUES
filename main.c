#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"
#define MAXSIZE 10
#define EMPTY -1

/* COP 3502C Assignment 2 
This program is written by: Daniel Ragusa */ 

typedef struct monster 
{ 
  int sequence_num;
  struct monster* next; 
}monster;

typedef struct queue 
{ 
  struct monster* front; 
  struct monster* back;
  int node_count;
  int k;
  int th;
  int ground_num;
  char name[100];
}queue;

monster* createMonster(int sequence);

void init(queue* q)
{
  //setting queues front & back to NULL, and node count to EMPTY
  q->front = NULL;
  q->back = NULL;
  q->node_count=EMPTY;
}

int enqueue(queue *q, int val)
{
  monster* temp=createMonster(val); // temp monster to hold newly created monster that will be returned from createMonster. The val is the sequence number of the monster

  if(temp!=NULL) //ensuring monster was created
  {
    if(q->back!=NULL) //if the back of the queue already has something
    {
      q->back->next=temp; //we want to update the back of the queue by pointing to temp
    }

    q->back=temp; //if this is the first node of the list we want to update back to point there, as back is NULL before this line

    if(q->front == NULL) // if front is NULL it means that temp will be our first node in the list
    {
      q->front=temp; //so we set the front to point to temp

      // if a list is not empty, then we do not want to update front 
      
    }
    
    return 1; 
    
  }
  else
    return 0; //will return 0 if space is not allocated for new node
  
}

int dequeue(queue* q)
{
  if(q->front==NULL) //if the queue is empty return so
    return EMPTY;

  monster *temp=q->front; //temp node set to front of queue 

  if(q->node_count==1) //if node count is 1, then we want to set front & back to NULL as there will be no nodes left to point to 
  {
    q->back=NULL;
    q->front=NULL;
    q->node_count=EMPTY;
    free(temp); // we free the node
    return 1;
  }

  q->front=temp->next; // if the queue contains more than 1 node, then we have to update front
  q->node_count--; //updating node count

  free(temp); //freeing node

  return 1; //return 1 for sucessful operation
  
}

int peek(queue* q)
{
  if(q->front!=NULL)
  return q->front->sequence_num; //returns the sequence number of the front of the passed queue
  else
    return EMPTY;
  
}

int isEmpty(queue* q) //returns 1 if queue is empty, 0 if not
{
  if(q->front==NULL) //if front is NULL the queue is empty so we return 1
    return 1;
  else
    return 0;
  
}

monster* createMonster(int sequence) //dynamically allocates a monster, sets its sequence number, & returns it
{
  monster* m1 = malloc(sizeof(monster));

  m1->next = NULL;
  m1->sequence_num = sequence;

  return m1;
  
}

void createReverseCircle(queue *q) //before this the queue is linear, this function will link the last node to point to head, and create the circular queue.
{
  
  for(int i=q->node_count;i>0;i--) //i is set to node count, and decremented until 1 to create the sequence numbers
    {
      enqueue(q,i);  //enqueue is called for each sequence number to be added to the queue
    }
  
  q->back->next=q->front; //setting the back to point to front, making the queue circular.

}

void rearrangeCircle(queue* q) // this function will reverse the now circular queue
{
  monster* mainlist=q->front->next; //will act as our walking variable 
  monster* reverselist=q->front; //reverse list set to point to front
  //reverselist->next=NULL;

  do 
    {
      monster* temp=mainlist; //temp is set equal to main list
      mainlist=mainlist->next; //mainlist is moved forward
      temp->next=reverselist; //temps next will now point to the previous node
      reverselist=temp; //reverse list is moved forward
      
    }
  while(mainlist!=q->front); //since mainlist is our most forward pointer we will use it to determine when we have reached the end of the list.
  
  //since in the loop we did not handle the queues pointers we must do so now.
  q->front=q->back;  //front will now point to back
  q->back=mainlist; //since we know mainlist will pointing to the front we do not need a temp variable for front
  //the above 2 lines just swap front & back
  
  q->back->next=q->front; //to make the queue circular again we must point the backs->next to front.
  
}

void display(queue q,FILE *outfile)
{
  //printing the queues ground number & name
  printf("G%d %s ",q.ground_num,q.name);
  fprintf(outfile,"G%d %s ",q.ground_num,q.name);

  
  monster*t=q.front; //assigning a walker variable at the front of the queue 

  // A do while loop is used here because the link is circular. This will us to stop when t is equal to the front again, meaning we have traversed the entire list 
  do
    {
      printf("%d ",t->sequence_num);
      fprintf(outfile,"%d ",t->sequence_num);
      t=t->next; 
    }
    while(t!=q.front);

  printf("\n");
  fprintf(outfile,"\n");
  
}

void phase1 (queue *q, FILE *outfile) //dequeue not required. So we will be deleting nodes from the linked list.
{
  //phase 1 takes 1 queue at a time 

  printf("G%d %s\n",q->ground_num,q->name);
  fprintf(outfile,"G%d %s\n",q->ground_num,q->name);
  
  monster* t = q->back; //I set t to the back so that I can stop 1 before the node we are deleting
  
  while(q->node_count!=q->th) //controlling main loop with th value from queue
    {
      for(int i=0;i<q->k-1;i++) //k-1 represents how many nodes to skip 
        {
          t=t->next;
        }
      //t now points to the node before the one we want to delete
      if(t->next==q->front) //we need to check if the node we want to delete is the front so we can handle it 
      {
        monster* temp=q->front; //setting a temp node equal to the front
        
        q->front=t->next->next; //since we are deleting the front we must update front to the next node. we use next->next because we are stopping 1 node before the one we are deleting
        t->next=q->front; //since we are deleting the front of the queue we must update the last node to point to the new front
        printf("Soldier# %d executed\n",temp->sequence_num);
        fprintf(outfile,"Soldier# %d executed\n",temp->sequence_num);
        free(temp);
        q->node_count--;
  
      }
      else if(t->next==q->back) //we also need to handle how to delete the back
      {
        monster* temp=q->back; //setting temp to back
        t->next=q->front; //since we are deleting the back we must update the new back of the list to point to the front. Since we stopped 1 node before the back we know that t is pointing to what will be the new back of the list
        q->back=t; //we update the back
        printf("Soldier# %d executed\n",temp->sequence_num);
        fprintf(outfile,"Soldier# %d executed\n",temp->sequence_num);
        free(temp);
        q->node_count--;
        
      }
      else if(t->next!=q->front && t->next!=q->back) //if the node we want to delete is not the front or back
      {
        monster* temp=t->next; 
        t->next=temp->next; //since t is the node before the one we are deleting we must introduce it to the next node 
        printf("Soldier# %d executed\n",temp->sequence_num);
        fprintf(outfile,"Soldier# %d executed\n",temp->sequence_num);
        free(temp);
        q->node_count--;
        
      }
      
      
    }
  
  printf("\n");
  fprintf(outfile,"\n");
  
  
}

void phase2 (queue *q, FILE *outfile) //dequeue is required here so we will be utlizing front & back pointers of queue to effectively hide data in the list.
{

  //phase 2 handles all queues in one call
  
  int total_pop=0; //represents total number of monsters across all queues

  queue *max=NULL; //a queue pointer that will point to the queue we need to send to dequeue function


  //calculating total population once, and then decrementing each time dequeue is called. This will allow us to keep it outside of the while loop
  for(int i=0;i<MAXSIZE;i++)
      {
        if(isEmpty(&q[i])==0)
          total_pop+=q[i].node_count;
      }

  while(1)
    {
      

      if(total_pop==1) //the loop will break when 1 monster remains across all queues
        break;

      for(int i=0;i<MAXSIZE;i++) //iterating through the queues with the help of isEmpty to establish the max. For now the max will be the first non empty queues front monster
      {
        if(isEmpty(&q[i])==0)
        {
          max=&q[i];
          break; //once we find this monster we need to break the loop
        
        }
      
      }

      for(int i=0;i<MAXSIZE;i++)
      {
        if(isEmpty(&q[i])==0) //ensuring we only evaluate non empty queues
        {
          if(q[i].front->sequence_num>max->front->sequence_num) //if the current queues front is bigger than max
          {
            max=&q[i]; //we update max and break
            break;
          }
          else if(q[i].front->sequence_num==max->front->sequence_num) //if there is a tie 
          {
            if(q[i].ground_num<max->ground_num) //we compare ground numbers
            {
              max=&q[i]; //the lowest ground number queue becomes max
              break; 
            }
          }
        
        }  
      }
      total_pop--; //decrement total pop

      printf("Executed Soldier %d from G%d\n",max->front->sequence_num,max->ground_num);
      fprintf(outfile,"Executed Soldier %d from G%d\n",max->front->sequence_num,max->ground_num);
      
      dequeue(max); // send the queue to dequeue

      
      
    } 
  
  
}



int main(void) 
{
  atexit(report_mem_leak);
  
  queue queues[MAXSIZE]; //array of structure queues with size 10
  
  for(int i=0;i<MAXSIZE;i++)
    {
      init(&queues[i]); //sending all queues to get intialized
    }

  int G;

  FILE *infile;

  FILE *outfile;

  infile=fopen("in.txt","r");

  outfile=fopen("out.txt","w");

  fscanf(infile,"%d",&G); //scanning in the first integer of the file to G


  for(int i=0;i<G;i++) //G will determine how many grounds are in the input file
    {

      //gathering all of a queues information & assigning 
      int tempGroundNum;
      char temp_name[100];
      fscanf(infile,"%d",&tempGroundNum); 
      tempGroundNum--; // the reason I decrement ground number here is so that we can use ground number as an index for the current queue. For example queue 5 will be stored at queues[4]
      
      fscanf(infile,"%s",temp_name);
      strcpy(queues[tempGroundNum].name,temp_name);
      queues[tempGroundNum].ground_num=(1+tempGroundNum); //we add 1 because we decremented ground number above

      fscanf(infile,"%d",&queues[tempGroundNum].node_count);

      fscanf(infile,"%d",&queues[tempGroundNum].k);

      fscanf(infile,"%d",&queues[tempGroundNum].th);

      
    }

  
  for(int i=0;i<MAXSIZE;i++) 
    {
      if(queues[i].node_count!=EMPTY) //the reason we check node count instead of calling isEmpty is because all of the queues still have front as NULL, for now we can use node count to find non empty until the queues front & back pointers have nodes to point to
      createReverseCircle(&queues[i]); 
    }

  

  printf("\n\nInitial nonempty lists status \n");
  fprintf(outfile,"\n\nInitial nonempty lists status \n");

  for(int i=0;i<MAXSIZE;i++)
    {
      if(isEmpty(&queues[i])==0)
      display(queues[i],outfile); //displaying queues before we reverse 
    }

  for(int i=0;i<MAXSIZE;i++)
    {
      if(isEmpty(&queues[i])==0)
      rearrangeCircle(&queues[i]);
    }
  
  printf("\n");
  printf("After ordering nonempty lists status \n");
  fprintf(outfile,"\n");
  fprintf(outfile,"After ordering nonempty lists status \n");

  
  for(int i=0;i<MAXSIZE;i++)
    {
      if(isEmpty(&queues[i])==0)
      display(queues[i],outfile); //displaying queues after reversal
    }

  printf("\nPhase1 execution\n\n");
  fprintf(outfile,"\nPhase1 execution\n\n");

  for(int i=0;i<MAXSIZE;i++)
    {
      if(isEmpty(&queues[i])==0)
      phase1(&queues[i],outfile); //calling phase 1 with each queue
    }

  printf("Phase2 execution\n");
  fprintf(outfile,"Phase2 execution\n");

  phase2(queues,outfile); //calling phase 2 with all queues

  
  

  for(int i=0;i<MAXSIZE;i++)
    {
      if(isEmpty(&queues[i])==0)
      {
        printf("\nSoldier %d from group %d will survive",peek(&queues[i]),queues[i].ground_num); //using peek function to print details of surviving monster
        fprintf(outfile,"\nSoldier %d from group %d will survive",peek(&queues[i]),queues[i].ground_num);
        free(queues[i].front);
        
      }
      
    }

  
  return 0;
}

