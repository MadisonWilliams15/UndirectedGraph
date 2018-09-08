//Madison Williams HW3
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include "input_error.h"


struct node{
    int vertex;
    int distance;
    struct node* next;
    };
struct q{
    int key;
    struct q* nextq;
};
struct Graph{
    int numVerticies;
    struct node** adjLists;
};
struct node* createNode(int);
struct Graph* createGraph(int verticies);
void addEdge(struct Graph* graph, int v1, int v2);
void printGraph(struct Graph* graph, FILE* fp2);
void BFS(struct Graph* graph);
struct q* dequeue(struct q* head);
struct q* enqueue(struct q* head, struct node* add);
int deq(struct q* head);

void parse_file(FILE* fp, FILE* fp2);

int main(int argc, char** argv) {
    
    
    if (argc != 3)
         exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
    
    FILE* fpin = fopen(argv[1], "r");
    if (!fpin)
        exit(INPUT_FILE_FAILED_TO_OPEN);
  
  
    FILE* fpout = fopen(argv[2], "w");
    if (!fpout)
        exit(OUTPUT_FILE_FAILED_TO_OPEN);
    
    parse_file(fpin, fpout);
    
    if (fclose(fpin) == EOF)
        exit(INPUT_FILE_FAILED_TO_CLOSE);
    if (fclose(fpout) == EOF)
        exit(OUTPUT_FILE_FAILED_TO_CLOSE);

    
    return 0;
    
}


void parse_file(FILE* fp, FILE* fp2)
{    

    
    if (feof(fp))
    {exit(PARSING_ERROR_EMPTY_INPUT_FILE);}
    
    //get number of total verticies
    int total;
    fscanf(fp,"%d\n", &total);
    //printf("Number of verticies: %d \n", total);

    
    int v1, v2;
    struct Graph* graph = createGraph(total);
	while (fscanf(fp, "(%d,%d)\n", &v1, &v2) == 2)//scan in verticies and add their edge
    {    
         if(v1<0 ||v1>total || v2<0 ||v1>total)
         {
             exit(INTEGER_IS_NOT_A_VERTEX);
         }

        // printf("%d %d ", v1, v2);

        addEdge(graph, v1, v2);

    }
    
    
     if (!feof(fp))
    {exit(PARSING_ERROR_INVALID_FORMAT);}
    
    printGraph(graph, fp2);
 }   
    struct node* createNode(int v)
    {
        struct node* newNode= malloc(sizeof(struct node));
        newNode->vertex=v;
        newNode->distance=-1;
        newNode->next=NULL;
        return newNode;
    }

    struct Graph* createGraph(int verticies){
        struct Graph* graph = malloc(sizeof(struct Graph));
        graph->numVerticies= verticies;
        graph->adjLists = malloc(verticies* (sizeof(struct node)));

        int i;
        for(i=0; i<verticies;i++)
        {
        
         graph->adjLists[i]=malloc(sizeof(struct node));
         graph->adjLists[i]->vertex=i+1;
         graph->adjLists[i]->distance=-1;
         graph->adjLists[i]->next=NULL;
        }
        return graph;
    }

    void addEdge(struct Graph* graph, int v1, int v2){
        
        struct node* temp= graph->adjLists[v1-1];
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=createNode(v2);

        temp= graph->adjLists[v2-1];
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=createNode(v1);

    }

    void printGraph(struct Graph* graph, FILE* fpout){
        int v;
        /*for(v=0; v<graph->numVerticies; v++)
        {
            struct node* temp = graph->adjLists[v];
            printf("\n Adjacency list for vertex %d\n",graph->adjLists[v]->vertex); 
            temp=temp->next;
            while(temp)
            {
                printf("%d -> ", temp->vertex);
                temp=temp->next;

            }
            printf("\n");

        }
        */

        BFS(graph);
        
        for(v=0; v<graph->numVerticies; v++)
        {   
            fprintf(fpout, "%d\n", graph->adjLists[v]->distance);
            //printf("Distance of %d from vertex 1 is: %d\n",graph->adjLists[v]->vertex, graph->adjLists[v]->distance);
        }
     
       //free memory

       int i;
       for(i=0; i<graph->numVerticies; i++)
       {
           struct node* temp=graph->adjLists[i];
           while(temp!=NULL)
           {
                struct node* hold=temp;
                temp = temp->next;
                free(hold);
           }

       }
       free(graph->adjLists);
       free(graph);

    
    }

    

void BFS(struct Graph* graph)
{
    graph->adjLists[0]->distance=0;
    //initialize queue
    struct q* Q = NULL;
    Q=enqueue(Q,graph->adjLists[0]);
    while(Q!=NULL)//Queue is not empty 
    {
       //printf("dequeue %d \n",Q->key);
        int u= deq(Q);
        Q=dequeue(Q);//Q=dequeue before have some function to get key
        if(u==-1)
        {
            break;
        }
        struct node* temp = graph->adjLists[u-1];
        while(temp!=NULL)
        {
            temp->distance=graph->adjLists[temp->vertex-1]->distance;
            if(temp->distance==-1)
            {   
                temp->distance=(graph->adjLists[u-1]->distance)+1;
                graph->adjLists[(temp->vertex)-1]->distance= temp->distance;
            
                //printf("Changing distance of %d to %d\n",temp->vertex , temp->distance);

                //printf("Enqueue %d \n", temp->vertex);
                Q=enqueue(Q,temp);
            }
            temp=temp->next;
        }
    }




}


struct q* dequeue(struct q* head){//is this supposed to return the head of the Queue or the item that was removed or integer?
    if(head==NULL)
    {   
        return NULL;
    }
    if(head->nextq == NULL)
    {
    
        free(head);
        return NULL;
    }
    struct q* temp = head->nextq;
    free(head);
    return temp;
}


int deq(struct q* head)
{
    if(head==NULL)
    {
        return -1;
    }
    return head->key;
}

struct q* enqueue(struct q* head, struct node* add)
{
    struct q* currentNode= head;
    if(currentNode==NULL)//if this is first node
    {
        currentNode= malloc(sizeof(struct q));
        currentNode->key=add->vertex;
        currentNode->nextq=NULL;

        return currentNode;

    }
    while(currentNode->nextq != NULL)//find the end of linked list
    {
        currentNode= currentNode->nextq;
    }

    struct q* new = malloc(sizeof(struct q));//add q to Q
    new->key= add->vertex;
    new->nextq=NULL;
    currentNode->nextq=new;
    return head;

}
    
    




