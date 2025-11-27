#include<XMLValidator.h>
#include<iostream> 
#include<stack> 

bool verify(const std::string& tokens)
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

// helper function 1

bool is_valid_char(const char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    
    if (c >= '0' && c <= '9')
        return true;

    if (c == '-' || c == '_' || c == '.' || c == ':')
        return true;

    return false;
}

// helper function 2
bool isValidFirstChar(const char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    return false;
}


