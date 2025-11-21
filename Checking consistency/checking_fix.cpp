#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<stack>


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

bool is_valid_char(const char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    
    if (c >= '0' && c <= '9')
        return true;

    if (c == '-' || c == '_' || c == '.' || c == ':')
        return true;

    return false;
}

bool isValidFirstChar(const char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    return false;
}

bool verify(const std::string& tokens)
{
    std::stack<std::string> st;
    for(int i=0;i<(int)tokens.size();i++)
    {
        if(tokens[i]=='<')
        {
            bool closed=false;
            std::string tag="";
            i++;
            if(tokens[i]=='/'&&st.empty())
                return false;
            else if(tokens[i]=='/')
            {    
                closed =true;
                i++;
            }

            // check for first char (can't be digit)
            if(!isValidFirstChar(tokens[i]))
                return false;
            
            while(i < tokens.size() && tokens[i] != '>')
            {
                if(!is_valid_char(tokens[i]))
                    return false;
                tag+=tokens[i];
                i++;
            }

            if(closed&&tag==st.top())
                st.pop();
            else if(closed)
                return false;
            else
                st.push(tag);

        }
        else if(tokens[i]=='\n'||tokens[i]==' ')
            continue;
        else if(tokens[i]=='>')
            return false;
            
    }
    
    return st.empty() ? true:false;
}

bool verify_improved(const std::string& tokens)
{
    std::stack<std::string> st;
    int i = 0;
    while (i < (int)tokens.size())
    {
        char c = tokens[i];
        if (c == '<')
        {
            i++; 
            if (i >= tokens.size()) return false;
            bool is_closing = false;
            if (tokens[i] == '/')
            {
                is_closing = true;
                i++; 
                if (st.empty()) return false; 
            }
            if (i >= tokens.size() || !isValidFirstChar(tokens[i]))
                return false;
            std::string tag = "";
            while (i < tokens.size() && tokens[i] != '>' && tokens[i] != '/' && tokens[i] != ' ' && tokens[i] != '\n')
            {
                if (!is_valid_char(tokens[i]))
                    return false; // Invalid char in tag name
                tag += tokens[i];
                i++;
            }

            // 2. Skip Attributes and Find End of Tag ('>' or '/>')
            bool is_self_closing = false;

            // CASE 1: Closing tag -> attributes NOT allowed
            if (is_closing)
            {
                // Only whitespace is allowed before '>'
                while (i < tokens.size() && (tokens[i] == ' ' || tokens[i] == '\n'))
                    i++;

                // If anything appears before '>', this is invalid
                if (i >= tokens.size() || tokens[i] != '>')
                    return false;
            }
            else
            {
                // CASE 2: Opening tag -> skip attributes, detect '/>'
                while (i < tokens.size() && tokens[i] != '>')
                {
                    if (tokens[i] == '/' && i + 1 < tokens.size() && tokens[i + 1] == '>')
                    {
                        is_self_closing = true;
                        i++; // move to '>'
                        break;
                    }
                    i++;
                }
            }
            
            if (i >= tokens.size() || tokens[i] != '>')
                return false; // Missing closing '>'

            // last step
            if (is_closing)
            {
                if (st.top() == tag)
                    st.pop();
                else
                    return false; // Mismatched closing tag
            }
            else if (is_self_closing)
            {
                // Self-closing tag (e.g., <br /> or <img />). Do nothing to the stack.
                // Note: If you want to support <tag/> instead of <tag />, you would modify the skip loop.
            }
            else
            {
                st.push(tag); // Opening tag
            }

            i++; // Move past '>'
        }
        else if (c == '\n' || c == ' ')
        {
            i++; // Skip whitespace
        }
        else if (c == '>')
        {
            return false; // Stray '>' character outside of a tag sequence
        }
        else
        {
            // CONTENT BLOCK (Text, entities, etc.)
            // We can safely skip all content until we find the next '<'
            while (i < tokens.size() && tokens[i] != '<')
            {
                i++;
            }
            // The loop will continue, and the next char will be handled in the next iteration.
        }
    }
    return st.empty();
}
void runTest(const std::string& name, const std::string& input, bool expected) {
    bool actual = verify(input);
    std::cout << "  - " << name << ": ";
    if (actual == expected) {
        std::cout << "[PASS]";
    } else {
        std::cout << "[FAIL] (Expected: " << (expected ? "true" : "false") 
                  << ", Got: " << (actual ? "true" : "false") << ")";
    }
    std::cout << " -> Input: \"" << input << "\"\n";
}


void testValidCases() {
    std::cout << "\n--- 1. Valid Cases (Expected: true) ---\n";

    runTest("Simple Nesting", "<root><child></child></root>", true);
    runTest("Empty Content", "<tag></tag>", true);
    runTest("Whitespace/Newline", "\n<t>\n <a></a> </t> ", true);
    
    // Test for attribute handling (should be skipped)
    runTest("Tag with Attributes", "<doc id=\"1\"><item name='a'></item></doc>", true);
    runTest("Tag with Complex Attributes", "<a:root data-id=\"123\" attr='test-val'>\n</a:root>", true);

    // Test for self-closing tags
    runTest("Self-Closing Tag", "<root><br/></root>", true);
    runTest("Self-Closing with space", "<root><br /></root>", true);
    runTest("Mixed Closing", "<doc><p><br/></p></doc>", true);
    
    // Test for content handling (should be skipped)
    runTest("Content Handling", "<book>Chapter 1: Hello &amp; World 123!</book>", true);
    
    // Test for valid tag name characters
    runTest("Valid Tag Name Chars", "<m-y_t:ag.123></m-y_t:ag.123>", true);
}

void testInvalidNesting() {
    std::cout << "\n--- 2. Invalid Nesting Cases (Expected: false) ---\n";

    runTest("Mismatched Order", "<A><B></A></B>", false);
    runTest("Unclosed Tag", "<root><child>", false);
    runTest("Unopened Close Tag", "</root><tag></tag>", false);
    runTest("Dangling Close Tag", "<root></root></extra>", false);
    runTest("Mismatched Tag Name", "<root></ROOT>", false);
}


void testInvalidStructure() {
    std::cout << "\n--- 3. Invalid Structure Cases (Expected: false) ---\n";

    // Structural errors
    runTest("Stray '>' at start", ">>><root></root>", false);
    runTest("Stray '>' in middle", "<root> > </root>", false);
    runTest("Stray '<' in middle", "<root><<child></child></root>", false);

    // Tag name validation errors
    runTest("Empty Tag Name", "<></>", false);
    runTest("Bad First Char (Digit)", "<1tag></1tag>", false);
    runTest("Invalid Char in Name (!)", "<tag!name></tag!name>", false);

    // Malformed tag endings
    runTest("Missing End Bracket", "<tag", false);
    runTest("Closing Tag w/ Attributes", "<root></root attr=\"val\">", false);
}


int main()
{
    std::string xmlContent = readFileToString("sample.xml");
    std::cout<<xmlContent<<"\n";
    std::cout<<verify_improved(xmlContent)<<"\n";


    std::cout << "Starting XML/HTML Tag Verifier Test Suite\n";

    testValidCases();
    testInvalidNesting();
    testInvalidStructure();

    std::cout << "\n--- Test Suite Complete ---\n";
    

}