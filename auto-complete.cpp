#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
using namespace std;
struct Node
{
    char data;
    int flag;
    struct Node *left, *center, *right;
};

void split_data(char* file, int *total);
void read_data(struct Node** root, char* file, int *total);
void fileparse(char *file, struct Node** root);
int searchTST(struct Node *root, char *word, char* match, char* input);
void dfs(struct Node* root, char* buf, int depth, char* input);
void freeTST(struct Node* root);

struct Node* newNode(char data)
{
    struct Node* temp = (struct Node*) malloc(sizeof( struct Node ));
    if (!temp) {
      cout<<"Unable to allocate memory properly... Exiting!\n";
      exit(EXIT_FAILURE);
    }
    temp->data = data;
    temp->flag = 0;
    temp->left = NULL;
    temp->center = NULL;
    temp->right = NULL;
    return temp;
}

struct Node** insert(struct Node** root, char *word)
{
    if (!(*root))
        *root = newNode(*word);
    if ((*word) < (*root)->data)
        root=insert(&( (*root)->left ), word);
    else if ((*word) > (*root)->data)
        root=insert(&( (*root)->right ), word);
    else
    {
        if (*(word+1))
            root=insert(&( (*root)->center ), word+1);

        // the last character of the word
        else
            (*root)->flag = 1;
    }
    return root;
}
 static int c=1;                                                                              //c=n //for printing not more than n times;
void dfs(struct Node* root, char* buf, int depth, char* input)
{

     if (root)
    {
        dfs(root->left, buf, depth, input);
        buf[depth] = root->data;
        if (root->flag)
        {
            if(c<=4)                                                                       // comment this for only for results
            {
                char outstr[50]="";
                buf[depth+1]='\0';
                strncat(outstr, input, strlen(input));
                strncat(outstr, buf, strlen(buf));
                cout<<outstr<<"\n";
                c++;
            }
        }
        dfs(root->center, buf, depth+1, input);
        dfs(root->right, buf, depth, input);
        ;
    }
}

int searchTST(struct Node *root, char *word, char* match, char* input)
{

if (!root)
        return 0;

    if (*word < (root)->data) {
          if (root->left==NULL) {
            cout<<("No match found\n");
            return 0;
          }
        return searchTST(root->left, word, match, input);
    }

    else if (*word > (root)->data) {
         if (root->right==NULL) {
            cout<<("No match found\n");
            return 0;
          }
        return searchTST(root->right, word, match, input);
    }

    else
    {
        if (strlen(word)==1) {
          if (root->center) {
            char stringCreate[20]="";
            strcat(stringCreate, match);
            strcat(stringCreate,&(root->data));
            strcat(stringCreate,&(root->center->data));
            dfs(root->center, stringCreate,0, input);
          }
            return 1;
        }
        char stringCreate[20]="";
        strcat(stringCreate, match);
        strcat(stringCreate,word);
        return searchTST(root->center, word+1, stringCreate, input);
    }
 }

void freeTST(struct Node* root)
{

        if (root->center ==  NULL && root->left == NULL && root->right == NULL)
			free(root);

        if (root->center != NULL) {
            freeTST(root->center);
        }

        if (root->right != NULL){
            freeTST(root->right);
        }

      if (root->left != NULL){
            freeTST(root->left);
        }

}

void split_data(char* file, int *total)
{
    FILE *fp, *fp1;
    char line[20];

     if ((fp = fopen(file,"r")) == NULL)
      {
      cout<<("Exiting to system.");
      exit(EXIT_FAILURE);
      }

  int fid=1, linecount=0;
  char newfile[20]="";
  strncpy(newfile,file,strlen(file));
  newfile[strlen(newfile)-5]='\0';
  char str[15];
  sprintf(str, "%d", fid);                   //adding 1,2,3 to str
  strcat(newfile,str);
  strcat(newfile,".txt");                    //creating file name abcd1.txt
  fp1=fopen(newfile,"w");
  while (fscanf(fp, "%s", line) != EOF)
    {
     linecount++;
     fprintf(fp1,"%s\n",line);
     if (!(linecount%(1000)) && (linecount>1))
     {
        fclose(fp1);
        cout<<"File created: "<<newfile<<"\n";
        fid+=1;

        strncpy(newfile,file,strlen(file));
        newfile[strlen(newfile)-5]='\0';
	//	cout<<("naam chck...."<<newfile<<"\n"; //√√
        sprintf(str, "%d", fid);
        strcat(newfile,str);
        strcat(newfile,".txt");
        fp1=fopen(newfile,"w");
     }
  }
  fclose(fp1);
  fclose(fp);
  *total=fid;
}

void read_data(struct Node** root, char* file, int *total)
{
  int fid=1;
  char newfile[20]="";

  while (fid<=*total)
    {
       strncpy(newfile,file,strlen(file));
       newfile[strlen(newfile)-5]='\0';                               //.txt removal from file name
       char str[15];
       sprintf(str, "%d", fid);
       strcat(newfile,str);
       strcat(newfile,".txt");
       fileparse(newfile,root);
       fid+=1;
  }
}

void fileparse(char *file, struct Node** root)
{

    FILE *fp;
    char line[20];
     cout<<"Reading file: "<< file<<"\n";
     if ((fp = fopen(file,"r")) == NULL)
      {
      cout<<("Exiting to system.");
      exit(EXIT_FAILURE);
      }

    while (fscanf(fp, "%s", line) != EOF) {
             if (strlen(line)>20) {
               cout<<("Current phrase longer than 20 characters, moving to next word..\n");
               continue;
             }
             strtok(line,"\r\n");
             line[strlen(line)+1]='\0';
             insert(root, line);
    }
    fclose(fp);
}

int main()
{
    char file[20], input[20];
    int total=0;
    struct Node *root = NULL;

    cout<<("Welcome to the autocomplete program.\nPlease enter the full path to the database file containing the list of reference phrases, and press ENTER:\n");
    fgets(file, 20, stdin);
    cout<<"Loading database file "<<file<<" containing list of phrases into memory ...\n";
    strtok(file,"\r\n");
    split_data(file, &total);
    read_data(&root,file, &total);

    cout<<("Finished loading database file.\n");

    cout<<("Please type the prefix of a word (more than one character) and press ENTER:\n");
    cin>>input;
    if (strlen(input)> 20) {
      cout<<("Prefix size is limited to 20 characters only. Please reduce accordingly. Exiting ..!\n");
      exit(EXIT_FAILURE);
    }
    while(strcmp(input,"")>0)
    {
     if (strcmp(input,"quit\n")!=0)
        {
        //cout<<("You entered %s\n", input);
        strtok(input,"\r\n");
        char buf[20]="";
        strncpy(buf, input, strlen(input));
        cout<<("\nSuggestions:\n");
        searchTST(root,input,"", buf);
        cout<<("\nPlease type the prefix of a word (more than one character and no more than 20 characters) :\n");
        fgets(input, 20, stdin);
        c=1;                                                                                                        // reinitialization of number of prints
      }  else {
       cout<<("Exiting the program\n");
       break;
      }
    }

    cout<<("Freeing memory ...\n");
    freeTST(root);
    cout<<("Freeing memory done\n");

    return 0;
}

/*
****pseudocode****
spell checker
input
if no match found
did You mean
loop check kaha tk same h...dfs and sabko lexographic check
nearest ko print kro
*/
