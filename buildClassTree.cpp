//Author:   hzgmaxwell@hotmail.com
//verison:
//0.01      May-13-2008, Created
//0.02      May-16-2008, Multi-Inheritance supported
#include <string>
#include <map>
#include <fstream>
#include <iostream>
//#include <unistd.h>
using namespace std;

class TreeNode
{
public:
    string name;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    bool adopted;
    TreeNode(const string& nm) : name(nm), left(NULL), right(NULL), adopted(false){}
};

TreeNode* buildTree(const char* fileName, map<string, TreeNode*>& cl)
{
    TreeNode* root = new TreeNode("root");
    ifstream ifs(fileName);
    TreeNode* pLast = root;
    //sleep(1);
    char buf[1024] = {0};
    while (ifs.good())
    {
        if(ifs.getline(buf, sizeof(buf)))
        {
            char * childName = buf;
            while(*++childName != ',');
            *childName = '\0';
            ++childName;
            TreeNode* pParent = NULL;
            TreeNode* pChild = NULL;
            if(cl.count(buf))
                pParent = cl[buf];
            else
            {
                pParent = new TreeNode(buf);
                cl[buf] = pParent;
                //whenever a new parent was created, add it as the far right descendant of root
                if(pLast == root)
                    pLast->left = pParent;
                else
                    pLast->right = pParent;
                pParent->parent = pLast;
                pLast = pParent;
            }
            if(cl.count(childName))
            {
                pChild = cl[childName];
                if(!(pChild->adopted))
                {
                    //since I'm going to have the right parent, so remove me from the original position
                    TreeNode* pTmpParent = pChild->parent;
                    if(pTmpParent->right == pChild)
                        pTmpParent->right = pChild->right;
                    else
                        pTmpParent->left = pChild->right;
                    if(pChild->right)
                    {
                        pChild->right->parent = pTmpParent;
                        pChild->right = NULL;
                    }
                    else
                    {
                        //if I was the far right descendant of root, then set pLast to my previous parent.
                        pLast = pChild->parent;
                    }
                }
                else
                {
                    //what's the matter, I have another parent, yes, it's multi-inheritance.
                    pChild = new TreeNode(string(childName)+"*");
                }
            }
            else
            {
                pChild = new TreeNode(childName);
                cl[childName] = pChild;
            }

            if(pParent->left == NULL)
            {
                //if I'm the first child of pParent, then I'm the left.
                pParent->left = pChild;
                pChild->parent = pParent;
            }
            else
            {
                TreeNode* pTmp = pParent->left;
                while(pTmp && pTmp->right) pTmp = pTmp->right;
                pTmp->right = pChild;
                pChild->parent = pTmp;
            }
            pChild->adopted = true;
        }
    } 
    return root;
}
void print(TreeNode* tree,int level)
{
    if(tree == NULL)
    {
        return;
    }
    for(int i=0;i<level;i++)
        cout << "    ";
    cout << tree->name <<endl;
    print(tree->left,level+1);
    print(tree->right,level);
}
void verify(const char* fileName, const map<string, TreeNode*>& cl)
{
    ifstream ifs(fileName);
    char buf[1024] = {0};
    while (ifs.good())
    {
        if(ifs.getline(buf, sizeof(buf)))
        {
            char * childName = buf;
            while(*++childName != ',');
            *childName = '\0';
            ++childName;
            TreeNode* pParent = cl.find(buf)->second;
            TreeNode* pChild = cl.find(childName)->second;
            if(pParent->left->name.find(pChild->name) == 0)
                continue;
            else
            {
                pParent = pParent->left;
                while(pParent->right && pParent->right->name.find(pChild->name) != 0)
                    pParent = pParent->right;
                if(pParent->right == NULL)
                    printf("error: %s->%s\n",buf,childName);
            }
        }
    }
}
static char* helpMsg = "Usage:\n"
    "\t\tclasstree <in_file>\n"
    "\t\tThe file content shall be as follows:\n"
    "\t\tclassA,classB\n"
    "\t\tclassB,classC\n"
    "\t\tclassB,classD\n"
    "\t\tclassA,classE\n"
    "Example:\n"
    "\t\t$ ./classtree2.exe kjs.txt\n"
    "\t\troot\n"
    "\t\t    classA\n"
    "\t\t        classB\n"
    "\t\t            classC\n"
    "\t\t            classD\n"
    "\t\t        classE\n";
int main(int argc,char **argv)
{
    if(argc < 2)
    {
        printf(helpMsg);
        exit(0);
    }

    map<string, TreeNode*> classLb;
    TreeNode* pTree = buildTree(argv[1],classLb);
    print(pTree,0);
    verify(argv[1], classLb);
    /*char buf[512] = {0};
    while(buf[0] != ']')
    {
        scanf("%s",buf);
        pTree = classLb[buf];
        printf("\n%s->%s\n",pTree->parent->name.c_str(),pTree->name.c_str());
        if(pTree->left)
            printf("->(%s\n",pTree->left->name.c_str());
        if(pTree->right)
            printf(",%s,...\n",pTree->right->name.c_str());
    }*/
    return 0;
}
