#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

  
Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
	Node *curr= head;
	
    if(curr==NULL){
        Node *temp=(Node *) malloc(sizeof(Node));
        temp->pid= new_pid;
        temp->path= new_path;
        temp->next=NULL;
        head=temp;

        return head;
    }

    while(curr->next!=NULL)
    curr=curr->next;


    Node *temp=(Node *) malloc(sizeof(Node));
    temp->pid= new_pid;
    temp->path= new_path;
    temp->next=NULL;
    curr->next=temp;
	return head; // Change this, DO NOT return NULL !
}


Node * deleteNode(Node* head, pid_t pid){
    Node * curr=head;
    Node * currPrev=NULL;

    if(curr->next==NULL && curr->pid==pid){
        head=NULL;
        free(curr);
        return head;
    } else if(curr->pid==pid && curr->next!=NULL){
        head=head->next;
        free(curr);
        return head;
    }

    while(curr!=NULL){
        if(curr->pid==pid){
            currPrev->next= curr->next;
            free(curr);
            return head;
        }
        currPrev=curr;
        curr=curr->next;
    }
    
    return head;
}

void printList(Node *node){
	Node *curr= node;
    
	while(curr!=NULL){
        printf("WHY ARE YOU WORKING\n");
		printf("%d, %s\n", curr->pid, curr->path);
		curr= curr->next;
	}
    
}


int PifExist(Node *node, pid_t pid){
  	Node *curr= node;
	while(curr!=NULL){
		if(curr->pid==pid){
			return 1;
		}
		curr=curr->next;
	}
	return 0;
}
