#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
/// WORDLE SOLVER - VERSION 1 ///



typedef struct Node
{
    char * word;
    struct Node *left;
    struct Node * right;
} Node;

Node * constructor(char * word)
{
    Node * t = malloc(sizeof(Node));
    t->left = NULL;
    t->right = NULL;
    t->word = malloc(strlen(word)+1);
    strcpy(t->word,word);
    return t;
}

Node *readWords(char * filename); ///reads 5-letter-words from dictionary file to tree
Node* insert(Node * root, char * word); ///inserts a node with passed word into tree
Node *collector(Node * big, Node * n, int mode, char someStr[]); ///collects possibilities

int count(Node *root); ///counts total number of 5-letter-words

void letsPlay(Node * root); ///processing
void loadCorrect(char arr[]); ///loading correctly placed letters
void partialCorrect(char arr[]); ///loading correct but misplaced letters
void exclude(char arr[]); ///loading incorrect letters
void startingPrint(Node *t); ///prints success of loading file and number of words once program starts
void displayer(Node * p); ///displays results


int main()
{
    printf("Welcome to Wordle Solver!\n\n");
    Node * dTree = readWords("EN-US-Dictionary.txt");
    if(dTree != NULL)
        startingPrint(dTree);
    else
        printf("Failed to Load Dictionary!\n");
    letsPlay(dTree);
    return 0;
}

Node* insert(Node * root, char * word)
{
    if (root == NULL)
        return constructor(word);
    if (strcasecmp(word,root->word)<0)
        root->left = insert(root->left,word);
    else if (strcasecmp(word,root->word)>0)
        root->right = insert(root->right,word);
    return root;
}

Node *readWords(char * filename)
{
    FILE *f = fopen(filename, "r");
    Node * t = NULL;
    if(f)
    {
        while(!feof(f))
        {
            char * temp = malloc(1000);
            fscanf(f,"%s\n",temp); ///scans word from file and inserts it to tree
            if (strlen(temp) == 5 && strchr(temp,'\'') == NULL)
                t = insert(t,temp);
        }
        fclose(f);
    }
    else
        printf("FILE NOT FOUND!!\n");
    return t;
}

int count(Node * root)
{
    if(root == NULL) return 0;
    else return 1 + count(root->left) + count(root->right);
}

void startingPrint(Node * t)
{
    printf("......................................\n");
    printf("Dictionary is loaded successfully .. !\n");
    printf("......................................\n");
    printf("Total number of words = %d\n",count(t));
    printf("......................................\n");
}

void loadCorrect(char arr[])
{
    printf("Enter the word with correct guessed letters so far. Type any other symbol/number in places of the incorrect ones:\n");
    gets(arr);
    for (int i = 0; i < 5; i++)
    {
        if(!((arr[i]>='a' && arr[i]<='z') || (arr[i]>='A' && arr[i]<='Z')))
            arr[i] = '*';
    }
}

void partialCorrect(char arr[])
{
    printf("Enter the correct but misplaced letters (adjacently)\nPress 'Enter' if there's nothing to enter:\n");
    gets(arr);
}

void exclude(char arr[])
{
    printf("Enter the incorrect letters (adjacently):\nPress 'Enter' if there's nothing to enter:\n");
    gets(arr);
}

char * checkCorrect(char arr[],char word[])
{
    int flag = 1;
    for(int i = 0; i < 5; i++)
    {
        if(arr[i] == '*')
            continue;
        if(arr[i] != word[i])
        {
            flag = 0; break;
        }
    }
    if (flag)
        return word;
    else
        return "no";
}

char *catchPartialCorrect(char arr[], char word[])
{
        int counter = 0;
        for (int i = 0; i < strlen(arr) ; i++)
        {
            if(strchr(word,arr[i]) != NULL)
                counter++;
        }
        if (counter == strlen(arr))
            return word;
        else
            return "no";
}

char *throwIncorrect(char arr[], char word[])
{
        int flag = 1;
        for (int i = 0; i < strlen(arr) ; i++)
        {
            if(strchr(word,arr[i]) != NULL)
            {
                flag = 0;
                break;
            }
        }
        if (flag)
            return word;
        else
            return "no";
}

void displayer(Node * p)
{
    if(p)
    {
        printf("-%s\n",p->word);
        displayer(p->left);
        displayer(p->right);
    }
}

Node *collector(Node * big, Node * n, int mode, char someStr[])
{
    char w[6]="no";
    if(big)
    {
        if (mode == 1)
            strcpy(w,checkCorrect(someStr,big->word));
        else if (mode ==2)
            strcpy(w,catchPartialCorrect(someStr,big->word));
        else if (mode == 3)
             strcpy(w,throwIncorrect(someStr,big->word));
        n = collector(big->left,n,mode, someStr);
        n = collector(big->right,n,mode, someStr);
    }
    if(strcasecmp(w,"no"))
    {
        Node * temp = insert(n,w);
        return temp;
    }
    else return n;
}

void letsPlay(Node *source)
{
    char correctWord[6];
    char goodLetters[5];
    char badLetters[26];
    int badCount = 0;
    char space; //space catcher

    printf("You must've guessed at least once to determine which letters to use!\n\n");

    for(int i =0; i<5; i++)
    {
        int c = 0;
        if(i!=0)
        {
            while(c==0)
            {
                printf("Choose an action:\n......................\n");
                printf("1-->Skip turn .. Mawasltesh le Haga gdeda!\n2-->Got updates!!! Let's reenter data!\n3-->Ana kasabt!Exit, bye bye!\n");
                printf("..........................\n");
                scanf("%d",&c);
                scanf("%c",&space);
                if (c==1)
                    continue;
                else if (c==2);
                else if (c==3)
                    break;
                else
                    c=0;
            }
            if(c ==3) break;
        }
        printf("Trial number %d:\n.................\n",i+1);
        loadCorrect(correctWord);
        partialCorrect(goodLetters);
        exclude(badLetters);
        Node * candidates1 = NULL;
        Node * candidates2 = NULL;
        Node * candidates3 = NULL;
        candidates1 = collector(source,candidates1,1,correctWord);
        if(!strcasecmp(candidates1->word,NULL))
            candidates1 = source;
            printf("success phase 1\n");

        candidates2 = collector(candidates1,candidates2,2, goodLetters);
        if(!strcasecmp(candidates2->word,NULL))
            candidates2 = candidates1;
            printf("success phase 1\n");

        candidates3 = collector(candidates2,candidates3,3,badLetters);
        if (!strcasecmp(candidates3->word,NULL))
            candidates3 = candidates2;
            printf("success phase 1\n");

        printf("..............................................\n");
        printf("So far, those are the possible solutions:\n");
        printf("..............................................\n");
        displayer(candidates3);
        printf("................................\n");
        printf("Total number of results = %d\n",count(candidates3));
        printf(".................................\n");
    }
}
