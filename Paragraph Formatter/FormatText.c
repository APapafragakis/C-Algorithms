#include <stdio.h>
#include <stdlib.h>
int LengthCounter(char* string){
    int count = 0;
    while(*string != '\0'){
        count++;
        string++;
    }
    return count;
}

char* CutP(int count, int acceptedParagraphLength, char* string){
    int i = 0;
    int j = 0;
    int flag = 0;
    char *runner = (char *)malloc(sizeof(char)*acceptedParagraphLength+1);
    char *newString;
    for(i = 0; i < acceptedParagraphLength; i++){
        *(runner+i) = *(string+i);
    }
    *(runner+i) = '\0';
    if(*(runner+i-1)  != ' ' && *(string+i) != ' '){
        for(j = 0; j < acceptedParagraphLength; j++){
            if(flag == 1)
                break;
            if(*(runner+i-1-j) == ' ')
                flag = 1;
        }
        newString = (char *)malloc(sizeof(char)*(acceptedParagraphLength+1-j));
        for(i = 0; i < (acceptedParagraphLength-j); i++){
            *(newString+i) = *(runner+i);
        }
        *(newString+i)= '\0';
        printf("%*s\n",acceptedParagraphLength,newString);
        string += (acceptedParagraphLength-j);
        return string;
    }else if(*(runner+i-1)  != ' ' && *(string+i) == ' '){
        printf("%*s\n",acceptedParagraphLength,runner);
        string += (acceptedParagraphLength);
        return string;
    }
    return string;
}


int main(int argc, char *argv[]){
    char *paragraphs = argv[2];
    int acceptedParagraphLength = atoi(argv[1]);
    int length = 0, count=0, i=0;
    char *ptr = paragraphs, *str;
    length = LengthCounter(paragraphs);

    while(length > 0){
        while(*ptr != '\0'){
            if(*ptr == '.'){
                count++;
                break;
            }
            count++;
            ptr++;
        }
        if(count > acceptedParagraphLength){
            ptr = paragraphs;
            str = (char *)malloc(sizeof(char)*count+1);
            for(i=0; i<count; i++){
                *(str+i) = *(ptr+i);
            }
            *(str+i) = '\0';
            while(*str != '\0'){
                if(count <= acceptedParagraphLength){
                    printf("%*s\n", acceptedParagraphLength ,str);
                    break;
                }
                str = CutP(count,acceptedParagraphLength,str);
                count = LengthCounter(str);
                length = length - (count);
                paragraphs = paragraphs + count;
            }
            free(str);
        }else if(count <= acceptedParagraphLength){
            ptr = paragraphs;
            str = (char *)malloc(sizeof(char)*count + 1);
            for(i = 0; i < count; i++){
                *(str+i) = *(ptr+i);
            }
            *(str+i) = '\0';
            free(str);
            length = length - (count);
            paragraphs = paragraphs + count;
        }
    }
    return 0;
}