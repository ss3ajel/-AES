#include <stdio.h>
unsigned char input_box[4][4]={{0x1,0x1A,0x9B,0x1C},
                                {0x8,0x7E,0x3,0x09},
                                {0x90, 0xA0, 0xB0, 0xC0},
                                {0x50, 0x60, 0x70, 0x80}

                            
};
highest_bit_position(d){
    
}

void gf256_inverse(unsigned short d){
    unsigned short s=0x11B;
    while (d>=s){
        int bitPos=highest_bit_position(d);

        int shift =bitPos-8;
        d^=(s <<shift);
    }
    return (unsigned char )d;
    


}
void affine_transform(int n){

}



void round_key(unsigned char input_box[4][4],unsigned char look){
   
    for (int i=0;i<4;i++){
         for (int j=0;j<4;j++){
        unsigned char high_nibble =(input_box[i][j] >> 4)& 0x0F  ;
        unsigned char low_nibble =input_box[i][j]& 0x0F;
        unsigned char index = high_nibble*0x10+low_nibble;
        input_box[i][j]=s_box[index];

    }
}
}

    




void main(){
unsigned char  s_box [256];                          
for (int i =0 ;i<256;i++){
    if(i==0){
        s_box[i]=0x63;
    }else {
        unsigned char inv=gf256_inverse(i);
        s_box[i]=affine_transform(inv);
    }

}
}