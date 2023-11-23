#include<stdio.h>
int main(){
    int i, j, h;
    printf("Give me the height: ");
    scanf("%d", &h);
    if (h >= 3 && h%2 == 1){
        for( i = 1; i <= h;  i++ ){
            for ( j = 1; j <= h; j++){
                if ((i == j) || (( i+j ) == (h+1))){
                    printf("*");
                }
else {
                    printf(" ");
                }


            }
            printf("\n");
        }
    }
    else {
         printf("invalid height.\n");
        return -1;
    }
return 0;
}

