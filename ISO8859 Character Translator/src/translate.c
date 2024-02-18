#include <stdio.h>


#define GREEK_CAPITAL_LETTER_M 204
#define GREEK_SMALL_LETTER_M  236
#define GREEK_CAPITAL_LETTER_N 205
#define GREEK_SMALL_LETTER_N 237
#define GREEK_CAPITAL_LETTER_T 212
#define GREEK_SMALL_LETTER_T 244
#define GREEK_CAPITAL_LETTER_P 208
#define GREEK_SMALL_LETTER_P 240

#define LATIN_CAPITAL_LETTER_M 78
#define LATIN_SMALL_LETTER_M 109
#define LATIN_CAPITAL_LETTER_N 79
#define LATIN_SMALL_LETTER_N 110
#define LATIN_CAPITAL_LETTER_B 66
#define LATIN_SMALL_LETTER_B 98
#define LATIN_CAPITAL_LETTER_D 68
#define LATIN_SMALL_LETTER_D 100

#define GREEK_TONOS 39
#define GREEK_DIALYTIKA 34

enum State_Machine {SIMPLE, M_UPPER, M_SMALL, N_UPPER, N_SMALL} state; 

struct {
    char letters[3];
} char_map[255];


void Initialize_CharMap(){
    int i;
    for(i = 0; i<255; i++){
        char_map[i].letters[0] = -1;
        char_map[i].letters[1] = -1;
        char_map[i].letters[2] = -1;
    }
    char_map[182].letters[0] = GREEK_TONOS;
    char_map[182].letters[1] = 65;
    char_map[184].letters[0] = GREEK_TONOS;
    char_map[184].letters[1] = 69;
    char_map[185].letters[0] = GREEK_TONOS;
    char_map[185].letters[1] = 72;
    char_map[186].letters[0] = GREEK_TONOS;
    char_map[186].letters[1] = 73;
    char_map[188].letters[0] = GREEK_TONOS;
    char_map[188].letters[1] = LATIN_CAPITAL_LETTER_N;
    char_map[190].letters[0] = GREEK_TONOS;
    char_map[190].letters[1] = 89;
    char_map[191].letters[0] = GREEK_TONOS;
    char_map[191].letters[1] = 87;
    char_map[192].letters[0] = 105;
    char_map[192].letters[1] = GREEK_TONOS;
    char_map[192].letters[2] = GREEK_DIALYTIKA;
    char_map[193].letters[0] = 65;
    char_map[194].letters[0] = 86;
    char_map[195].letters[0] = 71;
    char_map[196].letters[0] = LATIN_CAPITAL_LETTER_D;
    char_map[197].letters[0] = 69;
    char_map[198].letters[0] = 90;
    char_map[199].letters[0] = 72;
    char_map[200].letters[0] = 56;
    char_map[201].letters[0] = 73;
    char_map[202].letters[0] = 75;
    char_map[203].letters[0] = 76;
    char_map[204].letters[0] = 77;
    char_map[205].letters[0] = LATIN_CAPITAL_LETTER_M;
    char_map[206].letters[0] = 75;
    char_map[206].letters[1] = 83;
    char_map[207].letters[0] = 79;
    char_map[208].letters[0] = 80;
    char_map[209].letters[0] = 82;
    char_map[211].letters[0] = 83;
    char_map[212].letters[0] = 84;
    char_map[213].letters[0] = 89;
    char_map[214].letters[0] = 70;
    char_map[215].letters[0] = 88;
    char_map[216].letters[0] = 80;
    char_map[216].letters[1] = 83;
    char_map[217].letters[0] = 87;
    char_map[218].letters[0] = GREEK_DIALYTIKA;
    char_map[218].letters[1] = 73;
    char_map[219].letters[0] = GREEK_DIALYTIKA;
    char_map[219].letters[1] = 89;
    char_map[220].letters[0] = 97;
    char_map[220].letters[1] = GREEK_TONOS;
    char_map[221].letters[0] = 101;
    char_map[221].letters[1] = GREEK_TONOS;
    char_map[222].letters[0] = 104;
    char_map[222].letters[1] = GREEK_TONOS;
    char_map[223].letters[0] = 105;
    char_map[223].letters[1] = GREEK_TONOS;
    char_map[224].letters[0] = 121;
    char_map[224].letters[1] = GREEK_TONOS;
    char_map[224].letters[2] = GREEK_DIALYTIKA;
    char_map[225].letters[0] = 97;
    char_map[226].letters[0] = 118;
    char_map[227].letters[0] = 103;
    char_map[228].letters[0] = LATIN_SMALL_LETTER_D;
    char_map[229].letters[0] = 101;
    char_map[230].letters[0] = 122;
    char_map[231].letters[0] = 104;
    char_map[232].letters[0] = 56;
    char_map[233].letters[0] = 105;
    char_map[234].letters[0] = 107;
    char_map[235].letters[0] = 108;
    char_map[236].letters[0] = LATIN_SMALL_LETTER_M;
    char_map[237].letters[0] = LATIN_SMALL_LETTER_N;
    char_map[238].letters[0] = 107;
    char_map[238].letters[1] = 115;
    char_map[239].letters[0] = 111;
    char_map[240].letters[0] = 112;
    char_map[241].letters[0] = 114;
    char_map[242].letters[0] = 115;
    char_map[243].letters[0] = 115;
    char_map[244].letters[0] = 116;
    char_map[245].letters[0] = 121;
    char_map[246].letters[0] = 102;
    char_map[247].letters[0] = 120;
    char_map[248].letters[0] = 112;
    char_map[248].letters[1] = 115;
    char_map[249].letters[0] = 119;
    char_map[250].letters[0] = 105;
    char_map[250].letters[1] = GREEK_DIALYTIKA;
    char_map[251].letters[0] = 121;
    char_map[251].letters[1] = GREEK_DIALYTIKA;
    char_map[252].letters[0] = 111;
    char_map[252].letters[1] = GREEK_TONOS;
    char_map[253].letters[0] = 121;
    char_map[253].letters[1] = GREEK_TONOS;
    char_map[254].letters[0] = 119;
    char_map[254].letters[1] = GREEK_TONOS;
}

void Print_State_Machine(int letter){
	if (char_map[letter].letters[0] == -1){
		putchar(letter);
	}else{
        if(char_map[letter].letters[0] != -1){
            putchar(char_map[letter].letters[0]);
        }
        if(char_map[letter].letters[1] != -1){
            putchar(char_map[letter].letters[1]);
        }
        if(char_map[letter].letters[2] != -1){
            putchar(char_map[letter].letters[2]);
        }
    }
}

enum State_Machine SIMPLE_F(int letter){
    if (letter == GREEK_CAPITAL_LETTER_M){
		return M_UPPER;
    }else if (letter == GREEK_SMALL_LETTER_M){	
		return M_SMALL;
	}else if (letter == GREEK_CAPITAL_LETTER_N){
		return N_UPPER;
	}else if (letter == GREEK_SMALL_LETTER_N)	{
		return N_SMALL;
	}else{
		Print_State_Machine(letter);
		return SIMPLE;
	}   
}

enum State_Machine M_UPPER_F(int letter){
    if (letter == GREEK_CAPITAL_LETTER_P){
		putchar(LATIN_CAPITAL_LETTER_B); 
		return SIMPLE;
	}
	else if (letter == GREEK_SMALL_LETTER_P){
		putchar(LATIN_CAPITAL_LETTER_B);
		return SIMPLE;
	}
	else if (letter == GREEK_CAPITAL_LETTER_M){
		putchar(LATIN_CAPITAL_LETTER_M);
		return M_UPPER;
	}
	else if (letter == LATIN_SMALL_LETTER_M){
		putchar(GREEK_CAPITAL_LETTER_M); 	
		return M_SMALL;
	}
	else if (letter == GREEK_CAPITAL_LETTER_N){
		putchar(LATIN_CAPITAL_LETTER_M); 
        return N_UPPER;
	}
	else if (letter == GREEK_SMALL_LETTER_N){
		putchar(LATIN_CAPITAL_LETTER_M); 	
		return N_SMALL;
	}
	else{
		putchar(LATIN_CAPITAL_LETTER_M); 
		Print_State_Machine(letter);
		return SIMPLE;
	}
}
enum State_Machine M_SMALL_F(int letter){
    if(letter == GREEK_CAPITAL_LETTER_P){
        putchar(LATIN_SMALL_LETTER_B);
        return SIMPLE;
    }else if(letter == GREEK_SMALL_LETTER_P){
        putchar(LATIN_SMALL_LETTER_B);
        return SIMPLE;
    }else if(letter == GREEK_CAPITAL_LETTER_M){
        putchar(LATIN_SMALL_LETTER_M);
        return M_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_M){
        putchar(LATIN_SMALL_LETTER_M);
        return M_SMALL;
    }else if(letter == GREEK_CAPITAL_LETTER_N){
        putchar(LATIN_SMALL_LETTER_M);
        return N_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_N){
        putchar(LATIN_SMALL_LETTER_M);
        return N_SMALL;
    }else{
        putchar(LATIN_SMALL_LETTER_M);
        Print_State_Machine(letter);
        return SIMPLE;
    }
}

enum State_Machine N_UPPER_F(int letter){
    if(letter == GREEK_CAPITAL_LETTER_T){
        putchar(LATIN_CAPITAL_LETTER_D);
        return SIMPLE;
    }else if(letter == GREEK_SMALL_LETTER_T){
        putchar(LATIN_CAPITAL_LETTER_D);
        return SIMPLE;
    }else if(letter == GREEK_CAPITAL_LETTER_M){
        putchar(LATIN_CAPITAL_LETTER_N);
        return M_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_M){
        putchar(LATIN_CAPITAL_LETTER_N);
        return M_SMALL;
    }else if(letter == GREEK_CAPITAL_LETTER_N){
        putchar(LATIN_CAPITAL_LETTER_N);
        return N_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_N){
        putchar(LATIN_CAPITAL_LETTER_N);
        return N_SMALL;
    }else{
        putchar(LATIN_CAPITAL_LETTER_N);
        Print_State_Machine(letter);
        return SIMPLE;
    }
}

enum State_Machine N_SMALL_F(int letter){
    if(letter == GREEK_CAPITAL_LETTER_T){
        putchar(LATIN_SMALL_LETTER_D);
        return SIMPLE;
    }else if(letter == GREEK_SMALL_LETTER_T){
        putchar(LATIN_SMALL_LETTER_D);
        return SIMPLE;
    }else if(letter == GREEK_CAPITAL_LETTER_M){
        putchar(LATIN_SMALL_LETTER_N);
        return M_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_M){
        putchar(LATIN_SMALL_LETTER_N);
        return M_SMALL;
    }else if(letter == GREEK_CAPITAL_LETTER_N){
        putchar(LATIN_SMALL_LETTER_N);
        return N_UPPER;
    }else if(letter == GREEK_SMALL_LETTER_N){
        putchar(LATIN_SMALL_LETTER_N);
        return N_SMALL;
    }else{
        putchar(LATIN_SMALL_LETTER_N);
        Print_State_Machine(letter);
        return SIMPLE;
    }
}

enum State_Machine ((*action[])(int letter)) =  {SIMPLE_F, M_UPPER_F, M_SMALL_F, N_UPPER_F, N_SMALL_F};

int main(){
    int letter;
    Initialize_CharMap();

    state = SIMPLE;
    while( (letter = getchar() ) != EOF){
		state = (action[state])(letter);
	}
    return 0;
}