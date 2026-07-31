#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include <stddef.h>
#include <unistd.h>


//executable 

#define MAX_PAGE 16550
typedef  struct allocator{
   void* start_ptr;    
    int start_page;     
    int num_pages;      //
}allocator;
#define PAGE_SIZE 4096


 static unsigned char bitmap[MAX_PAGE] = {0};//mapping the allotion

 

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
    n=(size+PAGE_SIZE-1)/PAGE_SIZE ;/// number of page required
  
   //for swapping the free to allocated // fixed
   int i=0;
   int j=0;
   while(i<n&&j<MAX_PAGE){
    if(bitmap[j]==0){
       i++; 
       j++; 

    }

    else{
       i=0;
       j++;
    
       continue;
    }    
}
    for(int k=j-n;k<j+n-1;k++){
        bitmap[k]=1;
        }
        
        
      
    char* debut_ptr= base_ptr+ (PAGE_SIZE  * (j-n));
    
    array[alloc_counter].start_ptr=debut_ptr;
    array[alloc_counter].start_page=j-n;
    array[alloc_counter].num_pages=n;
    alloc_counter++;
    return debut_ptr;




}
void free_chungus(void* ptr){
            for(int i=0;i<MAX_PAGE;i++){
                if(array[i].start_ptr==ptr){
                    for(int k = array[i].start_page; k < array[i].start_page + array[i].num_pages-1; k++){
                      bitmap[k] = 0;
                      
    }
                  array[i].num_pages=0;
                      array[i].start_page=0;
                      array[i].start_ptr=NULL;

                }
            }
        }

int main(){
   size_t total=500000;
   fill_big_chungus(total);
   size_t m=8192;
   size_t l=total/PAGE_SIZE;
  
   if (l> MAX_PAGE){
    printf("too much");
   }
   void* ptr1 =bit_allocator(5000);
 
   
   void* ptr2=bit_allocator(2000);
  
   void* ptr3=bit_allocator(6000);
   printf("before free/////////////////////////////////////////////////////////////////////\n");
   for(int i=0;i<l;i++){
    printf("%3d",bitmap[i]);
   }
   printf("____________________________________________________________________________________________________");
   for(int i=0;i<l;i++){
    printf("%3d %3d %3p",array[i].start_page,array[i].num_pages,array[i].start_ptr);
   }
  
   free_chungus(ptr1);
   free_chungus(ptr2);
   free_chungus(ptr3);
   printf("after free////////////////////////////////////////////////////////////////////////\n");
   for(int i=0;i<l;i++){
    printf("%3d",bitmap[i]);
   }
    printf("____________________________________________________________________________________________________");
   for(int i=0;i<l;i++){
    printf("%3d %3d %3p",array[i].start_page,array[i].num_pages,array[i].start_ptr);
   }
   
  
   
   
    
  
   

     
    
   

   

           
   }
    

