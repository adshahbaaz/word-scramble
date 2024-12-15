#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define NLINES 5000
#define NWORDS 5
#define NCHAR 5
#define COLOUR_GREEN "\x1b[32m"
#define COLOUR_RESET "\x1b[0m"

#ifdef _WIN32
#define clear() system("cls")
#else
#define clear() system("clear")
#endif

int  get_word(char *words[],int n,int w,FILE *);
char *scramble(char *);
void prepare_words(char *w[],char *jw[],FILE *fp);

FILE *fp;

int main(){

	char *words[NWORDS];
	char *jumbled_words[NWORDS];
	
	srand(time(NULL)); //seed
	prepare_words(words,jumbled_words,fp);	
	
	int n = NWORDS;

	clear();
	while(n-- > 0)
	    printf("%s\n",jumbled_words[n]);
		
	char user_command[50];
	
	for (;;){ 
		scanf("%s",user_command);
	    if (strcmp(user_command,"r") == 0 || strcmp(user_command,"refresh") == 0) {
			prepare_words(words,jumbled_words,fp);	
			n = NWORDS;
			clear();
			while(n-- > 0)
				printf("%s\n",jumbled_words[n]);
		} else if (strcmp(user_command,"a") == 0 || strcmp(user_command,"answer") == 0){
			clear();
			for(n = NWORDS; n-- > 0 ; printf("%s" COLOUR_GREEN "%10s\n" COLOUR_RESET,jumbled_words[n],words[n]));
		} else if (strcmp(user_command,"s") == 0 || strcmp(user_command,"shuffle") == 0){
			clear();
			for(n = NWORDS; n-- > 0 ;){
				jumbled_words[n] = scramble(words[n]);
				printf("%s\n",jumbled_words[n]);
			}
		} else if (strcmp(user_command,"q") == 0 || strcmp(user_command,"quit") == 0){
			clear();
			return 0;
		}
    }
	return 0;
}


void prepare_words(char *w[],char *jw[],FILE *fp) { /* populate array of jumbled words with scrambled words*/
 
	for (int n = 0; n < NWORDS ; n++){
	    get_word(w,rand() % NLINES + 1,n,fp);
		if(strlen(w[n]) - 1 < NCHAR || strlen(w[n]) - 1 > 7)  
			free(w[n--]); //try again and free memory
		else  
			jw[n] = scramble(w[n]);      
	}
}

int  get_word(char *words[],int line_number,int word_number,FILE *iop){  /* Populates array of words with suitable words from dictionary */
	
	if ((iop = fopen("MIT-english.txt","r")) == NULL)  // better list at https://raw.githubusercontent.com/MichaelWehar/Public-Domain-Word-Lists/master/5000-more-common.txt posisbly
		printf("%s","cant open file");
	
	char line[100];
	char *p;
	for (int count = 0; fgets(line,100,iop) != NULL;count++){
		if (count == line_number){
			line[strcspn(line,"\n")] = 0;// remove newline added by fgets
			if((p = malloc(strlen(line) + 1)) == NULL) 
				return -1;
			strcpy(p,line);
			words[word_number] = p;
		    }
	}
	fclose(iop);
	return 0;
}

char  *scramble(char *w){ /* Scrambles a word */

	char *p;
	if((p  = malloc(strlen(w) + 1)) == NULL) return NULL;	
	strcpy(p,w);
	int n = 0, temp = 0;	
	
	while (n < strlen(w)){ 
		int r = rand() % (n + 1); // r will be between 0 - n
		temp = p[n]; 
		p[n] = p[r];
		p[r] = temp;
		n++;
	}
	
	p[n] = '\0';
	
	return p;	
}


	