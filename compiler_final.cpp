#include<iostream>
#include<fstream>
#include<cctype>
#include<vector>
#include<stack>
#include<string>
#include<stdlib.h>

#define K_DIGIT       3
#define K_CHAR        4
#define K_STRING      5
#define K_TYPE        6
#define K_KEYWORDS    7
#define K_OPERATOR    8
#define K_IDENTIFIER  9
#define K_BRACKET     10

using namespace std;

typedef struct IDwords
{
    int       id;
    string    word;
}IDwords;

typedef struct Variable
{
    string   var;
    string   value;
}Variable;

typedef struct Target
{
    string    dsc;
    string    mark;
    string    step;
    string    dsf;
    string    op;
    string    dst;

}Target;

vector<Variable>var_table;
vector<Target>target_code;

char   lab='A';
char   vab='A';

void print_the_windows();
void lexical_analysis(string source,vector<IDwords> &AnalysisResults);
int is_operator(char c);
void add_target_code(string dsf,string op,string dst,string dsc,string mark,string step);
int word_token(string s);
string  create_asmfile_name(string source);
int priority(string s);
string char_to_str(char c);
int is_bracket(char c);
int is_blank(char c);
void add_keywords(vector<IDwords> &v,int id,string word);
void add_var_table(vector<IDwords>::iterator &it);
void create_asm(string file);
void if_asm(ofstream &out,string dst,string dsc,string mark,string step);
void print_asm(ofstream &out,string dsf,string mark);
void addsub_asm(ofstream &out,string dsf,string op,string dst,string dsc);
void div_asm(ofstream &out,string dsf,string dst,string dsc);
void mul_asm(ofstream &out,string dsf,string dst,string dsc);
void sign_asm(ofstream &out,string dsf,string dst);
void if_analysis(vector<IDwords>::iterator &it);
void expression(vector<IDwords>::iterator &it);
void print_lexical(vector<IDwords> &v);
void mul_asm(ofstream &out,string dsf,string dst,string dsc);
void printf_analysis_res(vector<IDwords>::iterator &it);
void syntax_analysis(vector<IDwords> &AnalysisResults);
void print_syntax_res();



void print_the_windows()
{
    printf("EASY----GUI------AS----FOLLOW \n");
}
int priority(string s)
{
    if(s=="#")
        return 1;
    else if(s=="=")
        return 2;
    else if(s=="+" || s=="-")
        return 3;
    else if(s=="*" || s=="/")
        return 4;
    else
        return -1;
}



void add_target_code(string dsf,string op,string dst,string dsc,string mark,string step)
{
    Target  tmp;
    tmp.dsf = dsf;
    tmp.op = op;
    tmp.dst = dst;
    tmp.dsc = dsc;
    tmp.mark = mark;
    tmp.step = step;
    target_code.push_back(tmp);
}


string char_to_str(char c)
{
    char s[2] = " ";
    s[0] = c;
    return string(s);
}


int is_operator(char c)
{
    if(c == '+' || c=='-'||c=='*'||c=='/'||c==','||c=='=' ||c=='>' || c=='<')
        return 1;
    else
        return 0;
}


int is_bracket(char c)
{
    if(c=='{' || c=='}' || c=='(' || c==')' ||c==';')
        return 1;
    else
        return 0;
}


int is_blank(char c)
{
    if(c=='\n' || c=='\t' || c==' ' || c=='\r')
        return 1;
    else
        return 0;
}


int word_token(string s)
{
    int    size = s.size();

    if(s[0]=='\'')
    {
        if(s[size-1] == '\'')
            return K_CHAR;
        else
        {
            cout<<"ERROR: Incorrect String-Data："<<s<<endl;
            exit(-1);
        }
    }
    else if(s[0]=='\"')
    {
        if(s[size-1]=='\"')
            return K_STRING;
        else
        {
            cout<<"ERROR: Incorrect String-Data："<<s<<endl;
            exit(-1);
        }
    }
    else if(isdigit(s[0]))
    {
        for(int i=1;i<size;i++)
        {
            if(!isdigit(s[i]))
            {
                cout<<"ERROR: Invalid Identifier："<<s<<endl;
                exit(-1);
            }
        }
        return K_DIGIT;
    }
    else
    {
        for(int i=0;i<size;i++)
        {
            if(!isalnum(s[i]) && s[i]!='_')
            {
                cout<<"ERROR: Invalid Identifier："<<s<<endl;
                exit(-1);
            }
        }
        if(s=="int" || s=="char")
            return K_TYPE;
        else if(s=="if" || s=="else" || s=="printf" || s=="main")
            return K_KEYWORDS;
        else
            return K_IDENTIFIER;
    }
}

void add_keywords(vector<IDwords> &v,int id,string word)
{
    IDwords    temp;
    temp.id = id;
    temp.word = word;
    v.push_back(temp);
}

void lexical_analysis(string source,vector<IDwords> &AnalysisResults)
{
    char       ch;
    ifstream   rfile(source.c_str());
    if(!rfile.is_open())
    {
        cout<<"ERROR: Unable To Open Source-File! "<<endl;
        exit(-1);
    }

    rfile>>noskipws;
    while(rfile>>ch)
    {
        int         state=0;
        string      temp("");
        char        try_ch;

        switch(state)
        {
        case 0:
            if(ch=='/')
            {
                rfile>>try_ch;
                if(try_ch=='/')
                {
                    while(rfile>>try_ch)
                    {
                        if(try_ch=='\n')
                            break;
                    }
                    break;
                }
                else if(try_ch=='*')
                {
                    while(rfile>>try_ch)
                    {
                        if(try_ch=='*')
                        {
                            rfile>>try_ch;
                            if(try_ch=='/')
                                break;
                        }
                    }
                    break;
                }
                else
                {
                    add_keywords(AnalysisResults,K_OPERATOR,char_to_str(ch));
                    ch = try_ch;
                }
            }
        case 1:
            if(is_operator(ch))
            {
                add_keywords(AnalysisResults,K_OPERATOR,char_to_str(ch));
                break;
            }
        case 2:
            if(is_bracket(ch))
            {
                add_keywords(AnalysisResults,K_BRACKET,char_to_str(ch));
                break;
            }
        case 3:
            if(is_blank(ch))
                break;
        case 4:
            if(ch=='#')
            {
                while(rfile>>ch)
                {
                    if(is_blank(ch))
                    {
                        break;
                    }
                }
                break;
            }
        default:
            temp = temp + char_to_str(ch);
            while(rfile>>try_ch)
            {
                if(try_ch == '\"')
                {
                    temp = temp + char_to_str(try_ch);
                    if(ch == '\"')
                    {
                        add_keywords(AnalysisResults,word_token(temp),temp);
                        break;
                    }
                    else
                    {
                        cout<<"ERROR: Invalid Identifier："+temp<<endl;
                        exit(-1);
                    }
                }
                else if(is_blank(try_ch) )
                {
                    if(ch != '\'' && ch != '\"')
                    {
                        add_keywords(AnalysisResults,word_token(temp),temp);
                        break;
                    }
                    else
                        temp = temp + char_to_str(try_ch);
                }
                else if(is_operator(try_ch) )
                {
                    if(ch !='\'' && ch != '\"' )
                    {
                        add_keywords(AnalysisResults,word_token(temp),temp);
                        add_keywords(AnalysisResults,K_OPERATOR,char_to_str(try_ch));
                        break;
                    }
                    else
                        temp = temp + char_to_str(try_ch);
                }
                else if(is_bracket(try_ch))
                {
                    add_keywords(AnalysisResults,word_token(temp),temp);
                    add_keywords(AnalysisResults,K_BRACKET,char_to_str(try_ch));
                    break;
                }
                else
                    temp = temp + char_to_str(try_ch);
            }
        }
    }
    rfile.close();
}

void print_lexical(vector<IDwords> &v)
{
    vector<IDwords>::iterator  it;
    for(it = v.begin();it != v.end();it++)
        cout<<it->id<<" "<<it->word<<endl;
}

void add_var_table(vector<IDwords>::iterator &it)
{
    while(it->id == K_TYPE)
    {
        it++;
        while(it->word != ";")
        {

            if(it->id == K_IDENTIFIER)
            {
                Variable     tmp;
                tmp.var = it->word;
                string   tmp_var = it->word;
                if((it+1)->word=="=")
                {
                    it = it+2;
                    tmp.value = it->word;
                    add_target_code(tmp_var,"=",tmp.value," "," "," ");
                }
                var_table.push_back(tmp);
            }
            it++;
        }
        it++;
    }
}



void expression(vector<IDwords>::iterator &it)
{
    string dsf,op,dst,dsc;
    stack<string>      word_stack;
    stack<string>      oper_stack;
    oper_stack.push("#");
    while(it->word != ";")
    {

        if(it->word == "(")
            oper_stack.push(it->word);
        else if(it->word == ")")
        {

            while(oper_stack.top() != "(")
            {
                op = oper_stack.top();

                oper_stack.pop();
                dsc = word_stack.top();
                word_stack.pop();
                dst = word_stack.top();
                word_stack.pop();
                vab = vab+1;
                if(vab == 91)
                    vab = '0';
                dsf = "tmp" + char_to_str(vab);

                Variable     tmp;
                tmp.var = dsf;
                var_table.push_back(tmp);

                word_stack.push(dsf);
                add_target_code(dsf,op,dst,dsc," "," ");
            }
            oper_stack.pop();

        }
        else if(it->id != K_OPERATOR)
            word_stack.push(it->word);
        else if(oper_stack.top() == "(")
        {
            oper_stack.push(it->word);
        }
        else if(priority(it->word) < priority(oper_stack.top()))
        {
            op = oper_stack.top();
            oper_stack.pop();
            oper_stack.push(it->word);
            dsc = word_stack.top();
            word_stack.pop();
            dst = word_stack.top();
            word_stack.pop();
            vab = vab+1;
            if(vab == 91)
                vab = '0';
            dsf = "tmp" + char_to_str(vab);

            Variable     tmp;
            tmp.var = dsf;
            var_table.push_back(tmp);

            word_stack.push(dsf);
            add_target_code(dsf,op,dst,dsc," "," ");
        }
        else
            oper_stack.push(it->word);
        it++;
    }
    while(oper_stack.top() != "#")
    {
        op = oper_stack.top();
        oper_stack.pop();
        dsc = word_stack.top();
        word_stack.pop();
        dst = word_stack.top();
        word_stack.pop();

        if(op=="=")
        {
            add_target_code(dst,op,dsc," "," "," ");
        }
        else
        {
            vab = vab+1;
            if(vab == 91)
                vab = '0';
            dsf = "tmp" + char_to_str(vab);

            Variable     tmp;
            tmp.var = dsf;
            var_table.push_back(tmp);

            word_stack.push(dsf);
            add_target_code(dsf,op,dst,dsc," "," ");
        }
    }
}

void printf_analysis_res(vector<IDwords>::iterator &it)
{
    int j,i=1;
    it = it+2;
    string str = it->word;
    string strvar;

    Variable       tmp;
    for(j=1;j<str.size()-1;)
    {
        if(str[j]=='%')
        {
            if(i != j)
            {
                vab = vab + 1;
                if(vab == 91)
                    vab = '0';
                add_target_code("\'"+str.substr(i,j-i)+"$\'","p"," "," ","tmp"+char_to_str(vab)," ");
                tmp.var = "tmp"+char_to_str(vab);
                tmp.value = "\'"+str.substr(i,j-i)+"$\'";
                var_table.push_back(tmp);
            }

            i = j+2;
            it = it+2;
            strvar = it->word;
            add_target_code(strvar,"p"," "," ",str.substr(j,2)," ");
            j = i;
            continue;
        }
        j++;
    }
    if(i!=j)
    {
        vab = vab+1;
        if(vab == 91)
            vab = '0';
        add_target_code("\'"+str.substr(i,j-i)+"$\'","p"," "," ","tmp"+char_to_str(vab)," ");
        tmp.var = "tmp"+char_to_str(vab);
        tmp.value = "\'"+str.substr(i,j-i)+"$\'";
        var_table.push_back(tmp);
    }
    it = it+2; //略过“)}”
}

//分析if语句
void if_analysis(vector<IDwords>::iterator &it)
{
    string  op,mark,dst,dsc;
    it++;
    if(it->word != "(")
    {
        cout<<"ERROR: Incorrect if Statement: Missing '(' ."<<endl;
        exit(-1);
    }
    it++;
    dst = it->word;
    it++;
    mark = it->word;
    it++;
    dsc = it->word;
    op = "if";

    add_target_code(" ",op,dst,dsc,mark,"step"+char_to_str(lab+1));
    it++;
    if(it->word != ")")
    {
        cout<<"ERROR: Incorrect if Statement: Missing ')' ."<<endl;
        exit(-1);
    }
    it++;
    it++;

    vector<IDwords>::iterator   it2 = it;
    while(it2->word != "}")
    {
        it2++;
    }
    it2++;
    if(it2->word == "else")
    {
        it2++;
        while(it2->word != "}")
        {
            expression(it2);
            it2++;
        }
    }
    else
        it2--;
    lab = lab + 2;
    add_target_code(" ","jmp"," "," "," ","step"+char_to_str(lab));

    add_target_code(" ","ret"," "," "," ","step"+char_to_str(lab-1));

    while(it->word != "}")
    {
        expression(it);
        it++;
    }
    add_target_code(" ","jmp"," "," "," ","step"+char_to_str(lab));
    add_target_code(" ","ret"," "," "," ","step"+char_to_str(lab));

    it = it2;
}


void syntax_analysis(vector<IDwords> &AnalysisResults)
{
    vector<IDwords>::iterator  it=AnalysisResults.begin();
    if(it->word != "main")
    {
        cout<<"ERROR: Missing main ."<<endl;
        exit(-1);
    }
    it = it+3;
    if(it->word != "{")
    {
        cout<<"ERROR: The main Function Is Missing '{' ."<<endl;
        exit(-1);
    }
    it++;

    add_var_table(it);
    while(it != AnalysisResults.end())
    {

        if(it->word == "printf")
        {
            printf_analysis_res(it);
        }
        else if(it->word == "if")
        {
            if_analysis(it);
        }
        else if(it->word == "}")
            break;
        else
        {
            expression(it);
        }
        it++;
    }
}

void print_syntax_res()
{
    vector<Variable>::iterator  it;
    cout<<"Variable Declaration And Initialization"<<endl;
    for(it = var_table.begin();it != var_table.end();it++)
    {
        cout<<it->var<<"   "<<it->value<<endl;
    }

    vector<Target>::iterator it2;
    cout<<"Intermediate Code"<<endl;
    for(it2 = target_code.begin();it2 != target_code.end();it2++)
    {
        cout<<it2->dsf<<"  "<<it2->op<<"  "<<it2->dst<<"  "<<it2->dsc<<"  "<<it2->mark<<"  "<<it2->step<<endl;
    }
}


void addsub_asm(ofstream &out,string dsf,string op,string dst,string dsc)
{
    out<<"    mov BL,"<<dst<<endl;
    if(op == "+")
        out<<"    add BL,"<<dsc<<endl;
    else
        out<<"    sub BL,"<<dsc<<endl;
    out<<"    mov "<<dsf<<",BL"<<endl;
}

void mul_asm(ofstream &out,string dsf,string dst,string dsc)
{
    out<<"    mov AL,"<<dst<<endl;
    out<<"    mov BH,"<<dsc<<endl;
    out<<"    mul BH"<<endl;
    out<<"    mov BL,1"<<endl;
    out<<"    div BL"<<endl;
    out<<"    mov "<<dsf<<",AL"<<endl;
}


void div_asm(ofstream &out,string dsf,string dst,string dsc)
{
    out<<"    mov AL,"<<dst<<endl;
    out<<"    cbw"<<endl;
    out<<"    mov BL,"<<dsc<<endl;
    out<<"    div BL"<<endl;
    out<<"    mov "<<dsf<<",AL"<<endl;
}


void sign_asm(ofstream &out,string dsf,string dst)
{
    out<<"    mov BL,"<<dst<<endl;
    out<<"    mov "<<dsf<<",BL"<<endl;
}


void print_asm(ofstream &out,string dsf,string mark)
{

    if(mark=="%c")
    {
        out<<"    mov DL,"<<dsf<<endl;
        out<<"    mov AH,02H"<<endl;
        out<<"    int 21H"<<endl;
    }

    else if(mark=="%d")
    {
        out<<"    mov AL,"<<dsf<<endl;
        out<<"    cbw"<<endl;
        out<<"    mov BL,10"<<endl;
        out<<"    div BL"<<endl;
        out<<"    mov BH,AH"<<endl;
        out<<"    add BH,30H"<<endl;
        out<<"    add AL,30H"<<endl;
        out<<"    cmp AL,48"<<endl;

        lab = lab + 2;
        string step2 = "step" + char_to_str(lab);
        out<<"    JE "<<step2<<endl;
        string step1 = "step" + char_to_str(lab-1);
        out<<"  "<<step1<<":"<<endl;
        out<<"    mov DL,AL"<<endl;
        out<<"    mov AH,2"<<endl;
        out<<"    int 21H"<<endl;

        out<<"  "<<step2<<":"<<endl;
        out<<"    mov DL,BH"<<endl;
        out<<"    mov AH,2"<<endl;
        out<<"    int 21H"<<endl;
    }
    else
    {
        out<<"    LEA edx,"<<mark<<endl;
        out<<"    mov eax,09"<<endl;
        out<<"    int 21H"<<endl;
    }
}

void if_asm(ofstream &out,string dst,string dsc,string mark,string step)
{
    out<<"    mov AL,"<<dst<<endl;
    out<<"    cmp AL,"<<dsc<<endl;
    if(mark == ">")
        out<<"    jz "<<step<<endl;
    else if(mark == "<")
        out<<"    jne "<<step<<endl;
    else
    {
        cout<<"ERROR: Sorry, Other conditions are not supported!!! "<<endl;
        exit(-1);
    }
}


string  create_asmfile_name(string source)
{
    if(source.size()==0)
    {
        cout<<"Tips: The Source-File Name Cannot Be Empty"<<endl;
        exit(-1);
    }
    string temp="";
    int i,j;
    j = source.size();
    for(i = j-1;i>=0;i--)
    {
        if(source[i] == '.')
        {
            j = i;
            break;
        }
    }
    temp = source.substr(0,j) + ".asm";
    return temp;
}


void create_asm(string file)
{

    ofstream   wfile(file.c_str());
    if(!wfile.is_open())
        cout<<"ERROR: failed to create assembly file. "<<endl;

    vector<Variable>::iterator  it_var;

    wfile<<"SECTION .data "<<endl;
    wfile<<"msg db ' ' , 0h"<<endl;
    wfile<<"global _start"<<endl;
    for(it_var=var_table.begin();it_var!=var_table.end();it_var++)
    {
        wfile<<"    "<<it_var->var<<" db ";
        if(it_var->value != "")
            wfile<<it_var->value<<endl;
        else
            wfile<<"\'?\'"<<endl;
    }
    wfile<<"call quit"<<endl;
    wfile<<"codesg segment"<<endl;
    wfile<<" _start:"<<endl;
    wfile<<"    mov eax,msg"<<endl;
    wfile<<"    mov edx,eax"<<endl;

    vector<Target>::iterator     it;
    Target        tmp;
    for(it = target_code.begin();it != target_code.end();it++)
    {
        if(it->op == "+" || it->op=="-")
            addsub_asm(wfile,it->dsf,it->op,it->dst,it->dsc);
        else if(it->op == "*")
            mul_asm(wfile,it->dsf,it->dst,it->dsc);
        else if(it->op == "/")
            div_asm(wfile,it->dsf,it->dst,it->dsc);
        else if(it->op == "=")
            sign_asm(wfile,it->dsf,it->dst);
        else if(it->op == "p")
            print_asm(wfile,it->dsf,it->mark);
        else if(it->op == "if")
        {
            if_asm(wfile,it->dst,it->dsc,it->mark,it->step);
        }
        else if(it->op == "else")
        {
            cout<<"else not find the matched if"<<endl;
            exit(-1);
        }
        else if(it->op == "jmp")
        {
            wfile<<"    JMP "<<it->step<<endl;
        }
        else if(it->op == "ret")
        {
            wfile<<"  "<<it->step<<":"<<endl;
        }
        else
        {
            cout<<"ERROR: This syntax operation is not supported by the compiler! "<<endl;
            exit(-1);
        }
    }

    wfile<<"quit: " <<endl;
    wfile<<"    mov exb,0"<<endl;
    wfile<<"   mov eax,1"<<endl;
    wfile<<"   int 80h"<<endl;
    wfile.close();
}


int main(int argc,char* argv[])
{
    vector<IDwords>  AnalysisResults;

    string source;

    //缺省时给以提示
    if(argc == 1)
    {
        cout<<"-----------------------------------------------------"<<endl;
        cout<<"|                  F LANGUAGE COMPILER               |"<<endl;
        cout<<"|                                                    |"<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"\nGenerate in the ‘source directory’ .asm Assembly File\n"<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"|HELP: write the address of code in the last line.   |"<<endl;
        cout<<"|                                                    |"<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"|Tips:YOU WILL GET .asm FILE IN THE SOURCE DIRECTORY |"<<endl;
        cout<<"|                                                    |"<<endl;
        cout<<"*****************************************************"<<endl;;
        cout<<"\nPlease Input the Address Of Source File：";


        cin>>source;
        lexical_analysis(source,AnalysisResults);
        syntax_analysis(AnalysisResults);
        create_asm(create_asmfile_name(source));
        cout<<"\nThanks for using F Compiler! "<<endl;
    }
    else if(argc == 2)
    {
        lexical_analysis(argv[1],AnalysisResults);
        syntax_analysis(AnalysisResults);
        create_asm(create_asmfile_name(argv[1]));
    }
    else if(argc == 3)
    {
        lexical_analysis(argv[1],AnalysisResults);
        syntax_analysis(AnalysisResults);
        create_asm(create_asmfile_name(argv[2]));
    }
    else
    {
        cout<<"Using format as follow: " << endl;
        cout<<"compile. Exe [source file] [compile file]"<<endl;

    }
   print_lexical(AnalysisResults);
   print_syntax_res();
    return 0;
}




