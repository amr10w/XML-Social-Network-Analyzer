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
            while(i < (int)xmlContent.size() && xmlContent[i]!='>')
            {
                token.text+=xmlContent[i];
                i++;
            }
            tokens.push_back(token);
        }
        else if(xmlContent[i]=='\n'||xmlContent[i]==' '&&xmlContent[i] == '\t' )
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


std::vector<Token> tokenizeLine(const std::string& line) {
    std::vector<Token> tokens;
    int i = 0;
    while (i < (int)line.size()) {
        if (line[i] == '<') {
            
            Token token;
            token.type = "tag";
            i++; 
            while (i < (int)line.size() && line[i] != '>') {
                token.text += line[i];
                i++;
            }
            tokens.push_back(token);
            if (i < (int)line.size() && line[i] == '>') i++; // skip '>'
        } 
        else if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
            i++; 
        } 
        else {
            
            Token token;
            token.type = "value";
            while (i < (int)line.size() && line[i] != '<') {
                token.text += line[i];
                i++;
            }
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
    Tree<std::string> *tree=new Tree<std::string>(root);
    return tree;

}

std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open the file: " << filename << std::endl;
        return "";  // return empty string on error
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}


int main()
{
    std::string xmlContent = readFileToString("sample.xml");
 
    std::vector<Token> tokens= tokenizeXML(xmlContent);
    for(auto &e:tokens)
    {
        std::cout<<e.type<<": "<<e.text<<"\n";
    }
    Tree<std::string> *tree=buildTree(tokens);
    tree->print_preorder();
    

    
}