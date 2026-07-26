#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<stdint.h>
#include<stdbool.h>
//node header
typedef struct  free_list{                   //                       p  n s s
    __uint32_t size;                  ///b1     b2        b3           b4         b5       a4swap with b4 for the split
    bool state;                                     //                 a4
    struct free_list* nextf;
    struct free_list* prevf;
} free_list;

void print_chunk(free_list *list){
    free_list *r=list;
    while(r!=NULL){
        printf("%d",r->size);
        r=r->nextf;
    }
}
free_list *split(free_list *f,__uint32_t allocated){
   free_list *h = (free_list*)((char*)f + sizeof(free_list) + allocated);
    h->size=(f->size)-allocated-sizeof(free_list);
    h->nextf=f->nextf;
    h->prevf=f->prevf;
    if(f->prevf!=NULL){  f->prevf->nextf=h;}
    if(f->nextf!=NULL){  f->nextf->prevf=h;}
    f->nextf=NULL;           
    f->prevf=NULL;
    f->size=allocated;
    return f;
}
free_list *first_fit(free_list *h,__uint32_t allocated,__uint32_t minimum){
    free_list *b=h;
      free_list *last =NULL;
   
    
  while(b!=NULL){
    if(b->size>allocated&&b->size>minimum){
         b=split(b,allocated);
          printf("\n%d",b->size);
         
         return b;


    }else{
        last=b;
        b=b->nextf;
        
    }

   
}
free_list *nb=(free_list*)sbrk(1024+sizeof(free_list));
   last->nextf=nb;
    nb->size=1024;
    nb->nextf=NULL;
    nb->prevf=last;
    nb->state=0;
    nb=split(nb,allocated);
     printf("\n%d",nb->size);
  
       
}
void coalece(free_list *l){
    free_list *t=l;
    while(t->nextf!=NULL&&t->prevf!=NULL){
        free_list * cot=(free_list*)(char *)t+sizeof(free_list)+t->size;
        if(cot->state==0&&cot==t->nextf){
        t->size+=sizeof(free_list)+cot->size;
        t->nextf=cot->nextf;
        if(cot->nextf!=NULL){
            cot->nextf->prevf=t;
        }
    }
    if(t->prevf != NULL && t->prevf->state == 0&&(char*)t->prevf + sizeof(free_list) + t->prevf->size == (char*)t){   
    free_list *cot2 = t->prevf;             
    cot2->size += sizeof(free_list) + t->size;   
    cot2->nextf = t->nextf;                     
    if (t->nextf != NULL) {
        t->nextf->prevf = cot2;                  
    }
    t = cot2;   
}
        

    }
        

        
            
        
        
}

int main(){
    ///run example  just random values
     size_t node_size = 1024;
    printf(" %zu bytes\n", node_size);
    free_list *chunk =(free_list *)sbrk(node_size+sizeof(free_list));
    chunk->size=node_size;
    chunk->state=0;
    uint32_t min =50;
    first_fit(chunk,200,min);
    first_fit(chunk,300,min);
    first_fit(chunk,200,min);
    first_fit(chunk,300,min);
    first_fit(chunk,100,min);
    first_fit(chunk,800,min);
    coalece(chunk);
    print_chunk(chunk);
  
    return 0;
}
