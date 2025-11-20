#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<stack>
#include "Tree.h"

struct Token
{
    std::string type{}; // tag or value
    std::string text{};
};

std::vector<Token> tokenizeXML(const std::string& xmlContent)
{
    std::vector<Token> tokens;

    for(int i=0;i<xmlContent.length();i++)
    {
        if(xmlContent[i]=='<')
        {
            Token token;
            token.type = "tag";
            i++;
            while(xmlContent[i]!='>')
            {
                token.text+=xmlContent[i];
                i++;
            }
            tokens.push_back(token);
        }
        else if(xmlContent[i]=='\n'||xmlContent[i]==' ')
        {
            continue;
        }
        else
        {
            Token token;
            token.type= "value";
            while(xmlContent[i]!='<')
            {
                
                token.text+=xmlContent[i];
                i++;
            }
            i--;
            tokens.push_back(token);
            
        }
    }

    return tokens;

}

Tree<std::string>* buildTree(const std::vector<Token>& tokens)
{
    
    Node<std::string>* root = nullptr;
    std::stack<Node<std::string>*> st;
    

    for(auto &e:tokens)
    {
        if(e.type == "tag")
        {
            if(e.text[0]!= '/' && !e.text.empty()){
                Node<std::string>* node=new Node<std::string>(e.text);
                if(!st.empty())
                {
                    Node<std::string>* par=st.top();
                    par->addChild(node);
                }
                else
                {
                    root=node;
                }
                st.push(node);
            }
            else if(!e.text.empty())
            {
                st.pop();
            }
        }
        else
        {
            Node<std::string>* node=new Node<std::string>(e.text);
            Node<std::string>* par=st.top();
            par->addChild(node);
        }
    }
    Tree<std::string> *tree=new Tree(root);
    return tree;

}


int main()
{
    std::ifstream file("sample.xml");
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open the file!" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string xmlContent = buffer.str();
 
    std::vector<Token> tokens= tokenizeXML(xmlContent);
    for(auto &e:tokens)
    {
        std::cout<<e.type<<": "<<e.text<<"\n";
    }
    Tree<std::string> *tree=buildTree(tokens);
    tree->print_preorder();
    

    file.close();
}