//Nokia interview, using the least memory to reverse a sentence separated by space.
#include <stdio.h>
#include <string.h>
int getLastSpace(char* sentence)
{
    int i, ret = -1, len = strlen(sentence);
    for(i=len-1;i>=0;i--)
    {
        if(sentence[i] == ' ')
        {
            ret = i;
            break;
        }
    }
    return ret;
}
char* rightShift(char* sentence,int spaceLoc)
{
    int i, j, len = strlen(sentence);
    //put the space at then end of the string
    for(i=spaceLoc;i<len-1;i++)
        sentence[i] = sentence[i+1];
    sentence[len-1] = ' ';
    int step = len-spaceLoc;
    for(j=0;j<step;j++)
    {
        char a = sentence[len-1];
        for(i=len-1;i>0;i--)
            sentence[i] = sentence[i-1];
        sentence[0] = a;
    }
    return sentence+step;
}
void reverse(char* sentence)
{
    int spaceLoc = getLastSpace(sentence);
    char* p = sentence;
    while(spaceLoc != -1)
    {
        p = rightShift(p,spaceLoc);
        spaceLoc = getLastSpace(p);
    }
}
int main(int argc,char **argv)
{
    if(argc>1)
    {
        printf("%s\n",argv[1]);
        reverse(argv[1]);
        printf("%s\n",argv[1]);
    }
    else
    {
        char test[] = "welcome to china"; //"welcome tochina "->"china welcome to"
        printf("%s\n",test);
        reverse(test);
        printf("%s\n",test);
    }
    return 0;
}
