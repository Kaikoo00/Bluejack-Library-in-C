#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1007
struct book{
    char book_id[70];
    char book_name[30];
    char book_author[30];
    char book_ISBN[13];
    int page_num;
    struct book *next;
};

struct book *hashTable[SIZE]={NULL};
int book_count=0;

void clear(){
    system("pause");
    system("cls");
}

void book_details(struct book *book){
    printf("Book ID: %s\n", book->book_id);
    printf("Book Title: %s\n", book->book_name);
    printf("Book Author: %s\n", book->book_author);
    printf("Book ISBN: %s\n", book->book_ISBN);
    printf("Page Number: %d\n", book->page_num);
}

int hash(char *book_id){
    int key=0;
    for(int i=0; book_id[i]!='\0'; i++){
        key += book_id[i];
    }
    return key%1007;
}

bool exist(char *book_id, char *check){
    if(strcmp(book_id, check)==0){
        return true;
    }
    return false;
}

void print_menu(){
    printf("Blue Jack Library\n");
    printf("=================\n");
    printf("1. View Book\n");
    printf("2. Insert Book\n");
    printf("3. Remove Book\n");
    printf("4. Exit\n");
    printf(">> ");
    return;
}

void view_book(){
    struct book *curr;
    if(book_count==0){
        printf("There's no book(s) !\n");
        return;
    }
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| Book ID                  | Book Title                                | Book Author           | ISBN          | Page Number |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
    int print_count = 0;
    for(int i=0; i<SIZE; i++){
        if(print_count==book_count){
            printf("\n");
            return;
        }
        for(curr=hashTable[i]; curr!=NULL; curr=curr->next){
            printf("| %-24s | %-41s | %-21s | %-13s | %-11d |\n", curr->book_id, curr->book_name, curr->book_author, curr->book_ISBN, curr->page_num);
            printf("------------------------------------------------------------------------------------------------------------------------------\n");
            print_count++;
        }
    }
    printf("\n");
}

void insert_book(){
    struct book *newbook = malloc(sizeof(struct book));
    
    while(true){ //book name input
        printf("Input book title[5-50][unique]: ");
         scanf("%[^\n]", newbook->book_name);getchar();
        if(strlen(newbook->book_name)>50 || strlen(newbook->book_name)<5){
            continue;
        }
        else{
            break;
        }
    }

    bool gender;
    while(true){ //book author input
        printf("Input author's name[3 - 25][Mr. |Mrs. ]: ");
        scanf("%[^\n]", newbook->book_author);getchar();
        if(strlen(newbook->book_author)>25 || strlen(newbook->book_author)<4){
            continue;
        }
        if(strncmp(newbook->book_author, "Mr. ", 4) == 0){
            gender = true;
            break;
        }else if(strncmp(newbook->book_author, "Mrs. ", 5) == 0){
            gender = false;
            break;
        }else{
            continue;
        }
    }

    bool check_isbn=false;
    while(!check_isbn){ //isbn input
        printf("Input ISBN[10-13][numeric]: ");
         scanf("%[^\n]", newbook->book_ISBN);getchar();
        if(strlen(newbook->book_ISBN)<10 || strlen(newbook->book_ISBN)>13){
            continue;
        }
        for(int i=0; newbook->book_ISBN[i]!='\0'; i++){
            if(isdigit(newbook->book_ISBN[i])){
                check_isbn = true;
                continue;
            }else{
                check_isbn = false;
                break;
            }
        }
    }
    
    while(true){ //page number input
        printf("Input page number[>= 16]: ");
        scanf("%d", &newbook->page_num);getchar();
        if(newbook->page_num>=16){
            break;
        }else{
            continue;
        }
    }
    
    char book_id_1[8], book_id_2[10]; //book id generate
    itoa(++book_count, book_id_1, 10);
    if(book_count>0 && book_count<10){
		strcpy(book_id_2, "0000");
		strcat(book_id_2, book_id_1);
	}else if(book_count>9 && book_count<100){
		strcpy(book_id_2, "000");
		strcat(book_id_2, book_id_1);
	}else if(book_count>99 && book_count<1000){
		strcpy(book_id_2, "00");
		strcat(book_id_2, book_id_1);
	}else if(book_count>999 && book_count<10000){
		strcpy(book_id_2, "0");
		strcat(book_id_2, book_id_1);
	}

    char inpChar;
    char book_initial = toupper(newbook->book_name[0]);
    if(gender){
        inpChar = toupper(newbook->book_author[4]);
    }else{
        inpChar = toupper(newbook->book_author[5]);
    }
    sprintf(newbook->book_id, "B%s-%s-%c%c", book_id_2, newbook->book_ISBN, inpChar, book_initial);

    int key = hash(newbook->book_id); //key generate
    newbook->next = NULL;
    struct book *curr;
    if(hashTable[key]==NULL){
        hashTable[key]=newbook;
        printf("\nInsert Success !\n");
        return;
    }
    for(curr = hashTable[key]; curr->next != NULL; curr = curr->next){ //input to array linkedlist loop
        if(exist(newbook->book_id, curr->book_id)){
            printf("book already exist\n");
            return;
        }
    }
    curr->next = newbook;
    printf("\nInsert Success !\n");
    return;
}

void remove_book(){
    char inp_ID[50]; 
    if(book_count==0){
        return;
    }

    view_book();
    printf("\nInput book ID to delete: ");
    fflush(stdin);
    scanf("%[^\n]", inp_ID);getchar();
    int key = hash(inp_ID);

    if(hashTable[key]==NULL){
        system("cls");
        printf("\n\nBook not found !\n");clear();
        return;
    }
    
    if(exist(hashTable[key]->book_id,inp_ID)){
        system("cls");
        book_details(hashTable[key]);
        while(true){
            printf("Are you sure [y|n]? ");
            char yn; scanf("%c", &yn); getchar();
            if(yn == 'y'){
                break;
            }else if(yn == 'n'){
                system("cls");
                return;
            }
        }
        hashTable[key]=NULL;
        book_count--;
        return;
    }

    struct book *prev;
    bool book_exist=false;

    system("cls");
    for(struct book *curr = hashTable[key]; curr != NULL; curr = curr->next){
        prev = curr;
        if(exist(curr->book_id, inp_ID) && curr->next != NULL){
            while(true){
                printf("Are you sure [y|n]? ");
                char yn; scanf("%c", &yn); getchar();
                if(yn == 'y'){
                    break;
                }else if(yn == 'n'){
                    system("cls");
                    return;
                }
            }
            prev->next = curr->next;
            free(curr);
            book_exist = true;
            book_count--;
            return;
        }
        if(exist(curr->book_id, inp_ID) && curr->next == NULL){
            while(true){
                printf("Are you sure [y|n]? ");
                char yn; scanf("%c", &yn); getchar();
                if(yn == 'y'){
                    break;
                }else if(yn == 'n'){
                    system("cls");
                    return;
                }
            }
            prev->next = NULL;
            free(curr);
            book_exist = true;
            book_count--;
            return;
        }
        else{
            continue;
        }
    }
    if(book_exist==false){
        system("cls");
        printf("Book doesn't exist\n");
        return;
    }
}

void main_menu(){
    while(true){
        print_menu();
        int ch; scanf("%d", &ch); getchar();
        system("cls");
        if(ch==1){
            view_book();clear();
        }else if(ch==2){
            view_book();
            insert_book();clear();
        }else if(ch==3){
            if(book_count==0){
                view_book();clear();
                continue;
            }
            remove_book();
            view_book();
            clear();
        }else if(ch==4){
            printf("=======================================\n");
            printf("|| Thankyou for using the program :D ||\n");
            printf("=======================================\n");
            exit(0);
        }else{
            system("cls");
        }
    }
}

int main(){
    main_menu();
    return 0;
}