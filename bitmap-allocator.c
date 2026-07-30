#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include <stddef.h>

//under construction just learning about bitmap allocator the code is over the place
//2 just added a free function 
//still nnot executable 

#define MAX_PAGE 16550
typedef  struct allocator{
   void* start_ptr;    
    int start_page;     
    int num_pages;      //
}allocator;


 static unsigned char bitmap[MAX_PAGE][4096] = {0};//mapping the allotion
 static unsigned char bitmap_prc[MAX_PAGE][4096]={0};
 

allocator array[MAX_PAGE];
int alloc_counter = 0;
void* big_chungus=NULL;
void fill_big_chungus(size_t bytes){
    big_chungus=(void*)sbrk(bytes);
    if(big_chungus==(void*)-1){
        perror("big_chungus still hungry");
    }
}


void * bit_allocator(size_t size){
    char* base_ptr=big_chungus;
   
    int n;
   
   if (size%4096==0){
    n=size/4046;
   }else{
    n=(size/4046)+1;
   }
   //for swapping the free to allocated
   int i=0;
   int j=0;
   int d;
   while(i<=n&&j<n){
    if(bitmap[j]==0){
       i++; 
       j++; 

    }else {
       i=0;
       j++;
       d=j;
       continue;
    }
    
    }
    bitmap_prc[d][4096]=2;
 
    
    for(int k=d;k<d+n-1;k++){
        bitmap[k][4096]=1;
        for(int e=k+1;e<d+n-1;e++){
            bitmap_prc[e][4096];
        }
        
        
      }
    char* debut_ptr= base_ptr+ (4096 * d);
    
    array[alloc_counter].start_ptr=debut_ptr;
    array[alloc_counter].start_page=d;
    array[alloc_counter].num_pages=n;
    alloc_counter++;
    return debut_ptr;



}
void free_chungus(void** ptr){
            for(int i=0;i<MAX_PAGE;i++){
                if(array[bitmap_prc[i][4096]].start_ptr==ptr&&bitmap_prc[i]==2){
                    free(ptr);
                }
            }
            
                
                    
                }

        



void print_map(){}

int main(){
   size_t total=500000;
   fill_big_chungus(total);
   size_t m=8192;
   size_t l = m /4096;
  
   if (l> MAX_PAGE){
    printf("too much");
   }
   void* ptr1 =bit_allocator(5000);
 
   
   void* ptr2=bit_allocator(2000);
  
   void* ptr3=bit_allocator(6000);
  
   free_chungus(ptr1);
   free_chungus(ptr2);
   free_chungus(ptr3);

  
   
   
    
  
   

     
    
   

   

           
   }
    

