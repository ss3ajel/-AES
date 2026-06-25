#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//the logic of the entire algo 
/*data
key for encryption
key expansion depend on bits of the original key
loop{


add round key to all bits of data
subByt subtite the bytes by other values
shift row 
mix columns


}until all expansion keys are used;*/







const unsigned char SBox[16][16] = {
    {0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76},
    {0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0},
    {0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15},
    {0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75},
    {0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84},
    {0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF},
    {0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8},
    {0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2},
    {0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73},
    {0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB},
    {0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79},
    {0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08},
    {0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A},
    {0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E},
    {0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF},
    {0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16}
};
const unsigned char RCi[10] = {0x01, 0x02, 0x04, 0x08, 0x10,0x20, 0x40, 0x80, 0x1B, 0x36};
const unsigned char epank[4][4]={
    {0x01, 0x02, 0x03, 0x04},
        {0x05, 0x06, 0x07, 0x08},
        {0x09, 0x0A, 0x0B, 0x0C},
        {0x0D, 0x0E, 0x0F, 0x10}

};





typedef struct Node{
    unsigned char data[4];
    struct Node* next;
}Node;
typedef struct node {
    unsigned char array [4][4];
    struct  node* next;
}node;


unsigned char mix_constant[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};



unsigned char GF(unsigned char a,unsigned char b){
    unsigned char result=0;
    unsigned char x =0x02;
   while(b){
    
    if (b&0x01){
        result ^= a;        
                       
    }
     b >>= 1;
     unsigned char irr = a & 0x80;
    a <<= 1; 
    
    if (irr){
        a ^= 0x1B;
    }
      
   } 
   return result;
   

    
    
    


}

void sub(const unsigned char SBox[16][16],Node* head,const unsigned char RCi[10],int round){
    Node* parcour=head;
    
    if (round>=10) return;
 
    while(parcour!=NULL ){
        for (int j=0;j<4;j++){
        int r=(parcour->data[j]>>4) & 0x0F;
        int c=(parcour->data[j])& 0x0F;
        parcour->data[j]=SBox[r][c];  
        }





         if(parcour==head){
            parcour->data[0]^=RCi[round];
           
        }
        
      
}
parcour=parcour->next;
}
void permutation(Node* head) {
    unsigned char temp = head->data[0];
    for (int i = 0; i < 3; i++) {
        head->data[i] = head->data[i+1];
    }
    head->data[3] = temp;
}
    




Node* create(unsigned char arr[4][4]){
    Node* head=NULL;
    Node* tail=NULL;
    for (int i=0 ;i<4;i++){
        Node* newNode=(Node*)malloc(sizeof(Node));
        if (newNode==NULL){return NULL;}
        for (int j=0;j<4;j++){
        newNode->data[j]=arr[i][j];   
        }
        newNode->next=NULL;
        if (head==NULL){
            head=newNode;
            tail=newNode;
         }else {
            tail->next=newNode;
            tail=newNode;
         }


        


    }
    return head;
}


void printList(Node* head){
    Node* current=head;
    int count=0;
    while (current!=NULL){
        for(int i=0;i<4;i++){
            printf("%02X ",current->data[i]);
        }
        count++;
        printf("%d",count); 
        current=current->next;
    }
   
}
Node* append(Node* tail, unsigned char data[4]) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    for (int i = 0; i < 4; i++) {
        newNode->data[i] = data[i];
    }
    newNode->next = NULL;
    tail->next = newNode;
    return newNode; 
}
node* createList(char password[],char list[][16],int node_number){
    node* n=(node*)malloc(sizeof(node));
    node* head =NULL;
    node* tail=NULL;
   
    int l =strlen(password);
    int m=0;
    if (head==NULL){
        head=n;
        tail=n;
        
        for(int k=0;k<16;k++){
        for (int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                n->array[i][j]=list[node_number][k];

            }
            
        }
        
    }
}




    else{
        tail->next=n;
        tail=n;
        n->next=NULL;
        for(int k=0;k<16;k++){
        for (int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                n->array[i][j]=list[node_number][k];

            }
            
        }
        
    }
        
        
}
}
char**  wordsegment(char password[]){
    int n = strlen(password);
    int m;
    if(n%16>0){
        m=(n/16)+1;
    }else {
        m=n/16;
    }
    char **list = calloc(m, sizeof(char*));
    if (list == NULL) return NULL;
        for (int i = 0; i < m; i++) {
        list[i] = calloc(16, sizeof(char));
                if (list[i] == NULL) {        
            for (int k = 0; k < i; k++) free(list[k]);
            free(list);
            return NULL;
        }
    }
;  
    int currentS=0;
    int currentP=0;
    
    for (int i = 0; i < n; i++) {
    
        list[currentS][currentP] = password[i];
        currentP++;
        
        
        if (currentP == 16) {
            currentS++;
            currentP = 0;
    }
}
return list;
}
void addRoundKEy(node* n,Node* r){
    int node_number=0;
    while(n!=NULL){
        while(node_number<4){
            for (int i=0;i<4;i++){
                n->array[node_number][i]^=r->data[i];
            }
            r=r->next;
            node_number++;
        }
        n=n->next;
        node_number=0;       
    }
}
void shiftRows(node* state) {
    unsigned char temp;
    
   
    temp = state->array[1][0];
    state->array[1][0] = state->array[1][1];
    state->array[1][1] = state->array[1][2];
    state->array[1][2] = state->array[1][3];
    state->array[1][3] = temp;
    
   
    temp = state->array[2][0];
    state->array[2][0] = state->array[2][2];
    state->array[2][2] = temp;
    
    temp = state->array[2][1];
    state->array[2][1] = state->array[2][3];
    state->array[2][3] = temp;
    
   
    temp = state->array[3][3];
    state->array[3][3] = state->array[3][2];
    state->array[3][2] = state->array[3][1];
    state->array[3][1] = state->array[3][0];
    state->array[3][0] = temp;
}
void mixC(node* temp,unsigned char mix_constant[4][4]){
    
    for(int i =0;i<4;i++){
          unsigned char out[4] = {0, 0, 0, 0};
        for (int j=0;j<4;j++){
            unsigned char res=0;
             for (int k=0;k<4;k++){
                res^=GF(temp->array[k][i],mix_constant[j][k]);
                    
                }
                       for (int r= 0; r < 4; r++) {
                      temp->array[r][i] = out[r];
        }

            
               
        }
    }


}
void subByte(node* state)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int r = (state->array[i][j] >> 4) & 0x0F;
            int c =  state->array[i][j]       & 0x0F;
            state->array[i][j] = SBox[r][c];
        }
    }
}

Node* expansion_key(unsigned char expanK[4][4])
{
    Node* head = create(expanK);
    if (head == NULL) return NULL;
 
    printf("_____List created_________\n");
    printList(head);
 
    
    Node* tail = head;
    while (tail->next != NULL) tail = tail->next;
 
    
    Node* round_start = head;
 
    for (int round = 0; round < 10; round++) {
        Node* prev4 = round_start;   
 
        for (int w = 0; w < 4; w++) {
            unsigned char newdata[4];
            for (int i = 0; i < 4; i++) newdata[i] = tail->data[i];
 
            if (w == 0) {
               
                Node temp;
                for (int i = 0; i < 4; i++) temp.data[i] = newdata[i];
                temp.next = NULL;
                permutation(&temp);
                sub(SBox, &temp, RCi, round);
                for (int i = 0; i < 4; i++) newdata[i] = temp.data[i];
            }
 
            for (int i = 0; i < 4; i++) newdata[i] ^= prev4->data[i];
 
           
            Node* appended = append(tail, newdata);
            if (appended == NULL) return head;   
            tail = appended;
            prev4 = prev4->next;
        }
 
        for (int k = 0; k < 4; k++) round_start = round_start->next;
    }
 
   
    return head;
}
 

int main(){
    int node_number=0;
    char password="du bist gut geneug";
    char r[]=wordsegment(password);
    size_t l = sizeof(r) / sizeof(r[0]);
    Node* head =(Node*)malloc(sizeof(Node));
    head= expansion_key(epank);
    node* hihi=(node*)malloc(sizeof(node));
    while(node_number<l){
    hihi=createList(password,r,node_number);
    hihi=hihi->next;
    }
  
    addRoundKEy(hihi,head);
    subByte(hihi);
    shiftRows(hihi);
    mixC(hihi, mix_constant);
    printList(hihi);

}
