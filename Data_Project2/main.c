// Qusay Bider                      1220649                       se:1
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
struct String {
    char String_name[30];
    struct String *next_String;
    int position;
};
struct queue{
    char string[30];
    int position;
    struct queue *next;
};
struct queue *front=NULL;
struct queue *rear=NULL;
struct UndoStack{
    int operation_type ;      //if operation_type =0 its insert \\ if its =1 its remove
    char *word;
    int position;
    struct UndoStack *next;


};
struct RedoStack{
    int operation_type ;//if operation_type =0 its insert \\ if its =1 its remove
    char *word;
    int position;
    struct RedoStack *next;


};
struct UndoStack *Top_Undo=NULL;
struct RedoStack *Top_Redo=NULL;

void Menu();
void loadData( struct String * String_head);              // this function use for load and print the data from file
void addString(struct String *,struct queue*);           // This function use to add new district to linked list.
void printLinkedList(struct String *);                    // this function use for print the linked list .
void InsertString(struct String *String_head);            //this function use to insert the new node from user to linked list
void enqueue(char [],int);                                // this function use to add the inserted data to queue
struct queue *dequeue();                                  //this function use to delete and return the node from queue
void Undo_push(struct String*,int operation_type) ;
struct UndoStack *Undo_pop();
void Redo_push(struct RedoStack *Redo_node);
struct RedoStack* Redo_pop();
void Remove_String(struct String *String_head,struct String * Removed_node);       // this function use to delete the data from linked list
struct String*Search(struct String* String_head,char InsertWord [],int InsertIndex);    // this function use to search to a specific node.
void Refresh (struct String *Refresh_Node,int);       // this function use to refresh the index of nodes after removing or inserting.
void print_The_stacks();

int option;
int count;
int choose_Insert=0;
int main() {
    struct String *String_head = (struct String *) malloc(sizeof(struct String));  // heir we are build the head node for district_Linked list and initialize the first value  .
    String_head->next_String = NULL;
    String_head->position=1;
    Menu();
    scanf("%d", &option);
    do {
        switch (option) {
            case 1:loadData(String_head );
                break;
            case 2:loadData( String_head);
                break;
            case 3:{
                InsertString(String_head);
                while (rear!=NULL){                     // this while loop used for insert the new string to queue.
                    struct queue* temp=dequeue();
                    struct String* temp_String_node=(struct String*) malloc(sizeof (struct String));
                    strcpy(temp_String_node->String_name,temp->string);
                    temp_String_node->position=temp->position;
                    Undo_push(temp_String_node,0);
                    addString(String_head,temp);
                }
                printf("The text has been added\n");
            }
                break;
            case 4:{
                if(String_head->next_String==NULL){
                    printf("There's no data\n");
                    break;
                }
                char InsertWord[30];
                struct String*Temp_Removed;
                printLinkedList(String_head);
                printf("Enter the word \n");
                printf("Text :");
                scanf(" %[^\n]s",InsertWord);
                if(strcmp(InsertWord,".")==0){
                    strcpy(InsertWord,".\n");
                }
                Temp_Removed=Search(String_head, InsertWord,0);
                if(Temp_Removed==NULL){
                    printf("The Text is not found \n");
                    break;
                }
                Undo_push(Temp_Removed,1);
                Remove_String(String_head,Temp_Removed);
                printf("'%s' has been removed \n",Temp_Removed->String_name);

            }
                break;
            case 5:{
                struct RedoStack*Temp_undo_node= (struct RedoStack*)Undo_pop();
                if (Temp_undo_node == NULL) {
                    break;
                }
                struct String*Temp_String_node=(struct String*)malloc(sizeof (struct String));
                strcpy(Temp_String_node->String_name,Temp_undo_node->word);
                Temp_String_node->position=Temp_undo_node->position;
                if(Temp_undo_node->operation_type==0){
                    Remove_String(String_head,  Temp_String_node);
                }
                else{
                    struct queue*Temp_queue_node=(struct queue*)malloc(sizeof (struct queue));
                    strcpy(Temp_queue_node->string,Temp_undo_node->word);
                    Temp_queue_node->position=Temp_undo_node->position;
                    struct String*Temp =Search(String_head,Temp_String_node->String_name,Temp_String_node->position);
                    if(Temp!=NULL) {
                        Refresh(Temp, 2);
                    }
                    addString(String_head,Temp_queue_node);
                }
                Redo_push(Temp_undo_node);

            }
                break;
            case 6:{
                struct UndoStack*Temp_undo_node= (struct UndoStack*)Redo_pop();
                if (Temp_undo_node == NULL) {
                    break;
                }
                struct String*Temp_String_node=(struct String*)malloc(sizeof (struct String));
                strcpy(Temp_String_node->String_name,Temp_undo_node->word);
                Temp_String_node->position=Temp_undo_node->position;
                if(Temp_undo_node->operation_type==0){
                    struct queue*Temp_queue_node=(struct queue*)malloc(sizeof (struct queue));
                    strcpy(Temp_queue_node->string,Temp_undo_node->word);
                    Temp_queue_node->position=Temp_undo_node->position;
                    struct String*Temp =Search(String_head,Temp_String_node->String_name,Temp_String_node->position);
                    Refresh(Temp,2);
                    addString(String_head,Temp_queue_node);
                }
                else{
                    Remove_String(String_head,  Temp_String_node);
                }
                Undo_push(Temp_String_node,Temp_undo_node->operation_type);
            }
                break;
            case 7:print_The_stacks();
                break;
            case 8:loadData(String_head );
                break;
            case 9:
                exit(0);
            default:
                printf("The option is not valid.\n");
        }
        Menu();
        scanf("%d", &option);
    } while (option != 9);
    return 0;
}
void loadData(struct String*String_head){
    FILE *in, *out;
    char Temp_InputData[1000000];
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    struct String *Print_String;
    if (in == NULL) {
        printf("Error! can't open the file\n");
        exit(1);
    }
    if (option == 2) {
        while (fgets(Temp_InputData, sizeof(Temp_InputData), in) !=NULL) {
            char *Temp_InputData2=strtok(Temp_InputData, " ");
            while (Temp_InputData2!=NULL) {
                printf("%s ", Temp_InputData2);
                Temp_InputData2 = strtok(NULL, " ");
            }
        }
    }
    if (option == 1) {
        int position;
        struct String*Temp=String_head;
        if(Temp->next_String==NULL){
            position=1;
        }
        else{
            Temp=Temp->next_String->next_String;
            while (Temp->next_String!=NULL){
                position=Temp->next_String->position;
                Temp=Temp->next_String->next_String;
            }
        }
        while (fgets(Temp_InputData, sizeof(Temp_InputData), in) !=NULL) {
            char *Temp_InputData2=strtok(Temp_InputData, " ");
            while (Temp_InputData2!=NULL) {
                struct queue *String_node = (struct queue *) malloc(sizeof(struct queue));
                strcpy(String_node->string, Temp_InputData2);
                String_node->position = position;
                addString(String_head, String_node);
                Temp_InputData2 = strtok(NULL, " ");
                position++;
            }
        }
        printf("The data has been loaded\n");
    }
    if(option == 8){
        if(String_head==NULL){
            printf("There's no data \n");
            return;
        }
        else{
            String_head=String_head->next_String;
            while (String_head!=NULL) {
                fprintf(out, "%s ", String_head->String_name);
                String_head=String_head->next_String;
            }

        }
        fclose(out);
    }
    fclose(in);

}
void Menu(){
    printf("\n1. Load the input file which contains the initial text.\n"
           "2. Print the loaded text.\n"
           "3. Insert strings to the text.\n"
           "4. Remove strings from the text.\n"
           "5. Perform Undo operation.\n"
           "6. Perform Redo operation.\n"
           "7. Print the Undo Stack and the Redo stack.\n"
           "8. Save the updated text to the output file.\n"
           "9.Exit.\n Please inter your option :\n");
}
void addString(struct String *String_head, struct queue *String_node) {
    struct String *newString = (struct String *) malloc(sizeof(struct String));
    strcpy(newString->String_name, String_node->string);
    newString->position = String_node->position;
    newString->next_String = NULL;

    if (newString->position == 1) {
        newString->next_String = String_head->next_String;
        String_head->next_String = newString;
        return;
    }

    struct String *current = String_head;
    while (current->next_String != NULL) {
        if (current->next_String->position > newString->position&&current->position<newString->position) {
            break;
        }
        current = current->next_String;
    }
    newString->next_String = current->next_String;
    current->next_String = newString;
}
void InsertString(struct String *String_head){
    count=0;
    char Inserted_String[30];
    char *Temp_Inserted_String;
    struct String *Temp_Node=String_head;
    int Temp_Index;
    int copy_Index;
    printf("Insert your text \n");
    printf("Text :");
    scanf(" %[^\n]s",Inserted_String);
    do {
        printf("Choose were do you want to add the text \n 1- At first \n 2- At specific index\n 3- At last \n");
        scanf("%d",&choose_Insert);
        switch (choose_Insert) {
            case 1:
                Temp_Index = 1;
                break;
            case 2: {
                printLinkedList(String_head);
                printf("Index: ");
                scanf("%d", &Temp_Index);
            }
                break;
            case 3: {
                while (Temp_Node->next_String != NULL) {
                    count = Temp_Node->position;
                    Temp_Node = Temp_Node->next_String;
                }
                Temp_Index = count+1;
            }
                break;
            default: {
                printf("The option is not valid.\n");
            }
                break;
        }
    }
    while (!(choose_Insert==3||choose_Insert==2||choose_Insert==1));
    copy_Index = Temp_Index ;
    Temp_Inserted_String=strtok(Inserted_String, " ");
    while (Temp_Inserted_String!=NULL){
        enqueue(Temp_Inserted_String,copy_Index);
        Temp_Inserted_String=strtok(NULL, " ");
        copy_Index++;
        count++;
    }
    Temp_Node=String_head;
    if(Temp_Index==1){
        Temp_Node = Temp_Node->next_String;
    }

    else {
        while (Temp_Node->position!= (Temp_Index)) {
            Temp_Node = Temp_Node->next_String;
        }
    }
    Refresh(Temp_Node,0);
}
void printLinkedList(struct String *String_head){
    if(String_head==NULL){
        printf("The data base is empty\n");
        return;
    }
    else{
        String_head=String_head->next_String;
        while (String_head != NULL) {
            if(strcmp(String_head->String_name,".\n")!=0) {
                if (choose_Insert == 2 || option == 4) {
                    printf("%s(%d) ", String_head->String_name, String_head->position);
                }
            }
            else{
                printf("%s ", String_head->String_name);
            }
            String_head = String_head->next_String;
        }
        printf("\n");
    }
}
void enqueue(char Insert_String[],int position){
    struct queue* new_queue = (struct queue*)malloc(sizeof(struct queue));
    strcpy(new_queue->string,Insert_String);
    new_queue->position=position;
    new_queue->next = NULL;
    if (front == NULL && rear == NULL) {
        front = rear = new_queue;
        return;
    }
    rear->next=new_queue;
    rear = rear->next;
}
struct queue *dequeue(){
    if(front==NULL) {
        printf("Queue is empty\n");
        struct queue *Temp_NULL;
        Temp_NULL=NULL;
        return Temp_NULL;
    }
    else{
        struct queue *Temp_queue = (struct queue *) malloc(sizeof(struct queue));
        strcpy(Temp_queue->string,front->string);
        Temp_queue->position=front->position;
        struct queue *Temp=front;
        if(front==rear){
            front=rear=NULL;
        }
        else{
            front=front->next;
        }
        Temp->next=NULL;
        free(Temp);
        return Temp_queue;

    }
}
void Undo_push(struct String *queue_node,int operation_type) {
    if (queue_node != NULL) {
        struct UndoStack *Temp_node = (struct UndoStack *) malloc(sizeof(struct UndoStack));
        Temp_node->position = queue_node->position;
        Temp_node->word= queue_node->String_name;
        Temp_node->next = NULL;
        Temp_node->operation_type = operation_type;

        if (Top_Undo == NULL) {
            Top_Undo = Temp_node;
        }
        else {
            struct UndoStack *Temp = Top_Undo;
            Top_Undo = Temp_node;
            Top_Undo->next = Temp;
        }
    }
    else {
        printf("The queue is empty\n");
        return;
    }
}
struct UndoStack *Undo_pop(){
    if (Top_Undo == NULL) {
        printf("The stack is empty\n");
        return NULL;
    } else {
        struct UndoStack* Remove_node = Top_Undo;
        Top_Undo = Top_Undo->next;
        Remove_node->next = NULL; // Make sure to disconnect the popped node from the stack
        return Remove_node;
    }
}
struct String *Search(struct String* String_head,char InsertWord [],int InsertIndex){
    if (option == 6 || option == 5) {
        struct String* move=String_head->next_String;
        while (move != NULL) {
            if (move->position == InsertIndex) {
                struct String *Returned_Node = move;
                return Returned_Node;
            }
            move = move->next_String;
        }
        return NULL;
    }
    else{
        while (String_head!=NULL) {
            if (strcmp(String_head->String_name, InsertWord)==0) {
                struct String *Returned_Node = (struct String *) malloc(sizeof(struct String));
                strcpy(Returned_Node->String_name, String_head->String_name);
                Returned_Node->position = String_head->position;
                Returned_Node->next_String = NULL;
                return Returned_Node;
            }
            String_head = String_head->next_String;
        }
        return NULL;
    }

}
void Remove_String(struct String *String_head, struct String *Removed_node) {
    if (String_head == NULL || Removed_node == NULL) {
        return;
    }
    struct String *current = String_head;
    struct String *previous_node = NULL;
    //   find the node to be removed
    while (current != NULL && (strcmp(strlwr(current->String_name), strlwr(Removed_node->String_name)) != 0 || current->position != Removed_node->position)) {
        previous_node = current;
        current = current->next_String;
    }
    // If the node is not found
    if (current == NULL) {
        return;
    }
    // Remove the node from the list
    previous_node->next_String= current->next_String;
    Removed_node=current;
    Refresh(Removed_node,1);
    free(current);
}
void Refresh (struct String *Refresh_Node,int type_opration){
    if(type_opration!=0&&type_opration!=2) {
        Refresh_Node = Refresh_Node->next_String;
        while (Refresh_Node != NULL) {                                          // this while use for update the index for the node after the add node.
            Refresh_Node->position--;
            Refresh_Node = Refresh_Node->next_String;
        }
    }
    else {
        if(type_opration==0){
            count=1;
        }
        while (Refresh_Node != NULL) {                     // this while use for update the index for the node after the add node.
            Refresh_Node->position = Refresh_Node->position +count;
            Refresh_Node = Refresh_Node->next_String;
        }
    }
}
void Redo_push(struct RedoStack *Redo_node) {
    if (Redo_node != NULL) {
        struct RedoStack *Temp_node=Redo_node;
        if (Top_Redo == NULL) {
            Top_Redo = Temp_node;
        } else {
            struct RedoStack *Temp = Top_Redo;
            Top_Redo = Temp_node;
            Top_Redo->next = Temp;
        }
    }
    else {
        printf("The queue is empty\n");
        return;
    }
}
struct RedoStack* Redo_pop() {
    if (Top_Redo == NULL) {
        printf("The Redo stack is empty\n");
        return NULL;
    } else {
        struct RedoStack *Remove_node = Top_Redo;
        Top_Redo = Top_Redo->next;
        Remove_node->next = NULL;                           // Make sure to disconnect the popped node from the stack
        return Remove_node;
    }
}
void print_The_stacks(){
    struct UndoStack *Temp_UndoStack=Top_Undo;
    struct RedoStack *Temp_RedoStack=Top_Redo;
    if(Temp_UndoStack==NULL&&Temp_RedoStack==NULL){
        printf("The stacks empty\n");
        return;
    }
    else{
        printf("The undo stack:\n");
        printf("Token\t\tOperation\tIndex\n");
        while (Temp_UndoStack!=NULL){                      //this while use to pass to the nodes in the stack
            printf("%s\t",Temp_UndoStack->word);
            if(Temp_UndoStack->operation_type==0){
                printf("\tInsert\t");
            }
            else{
                printf("\tRemove\t");
            }
            printf("\t%d\n",Temp_UndoStack->position);
            Temp_UndoStack=Temp_UndoStack->next;
        }
        printf("....................................................................\n");
        printf("The Redo stack:\n");
        printf("Token\t\tOperation\tIndex\n");
        while (Temp_RedoStack!=NULL){                          //this while use to pass to the nodes in the stack
            printf("%s\t",Temp_RedoStack->word);
            if(Temp_RedoStack->operation_type==0){
                printf("\tInsert\t");
            }
            else{
                printf("\tRemove\t");
            }
            printf("\t%d\n",Temp_RedoStack->position);
            Temp_RedoStack=Temp_RedoStack->next;
        }
    }
}



