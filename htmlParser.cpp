//
//  htmlParser.cpp
//  isCorrectlyNested
//
//  Created by Vince Mansel on 10/2/11.
//  Copyright 2011 Wave Ocean Networks. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "scanner.h"
#include "stack.h"
#include "simpio.h"

struct tokenLineT {
    string tk;
    int line;
};

string GetHtml();
bool IsCorrectlyNested(string htmlStr);
bool IsBeginToken(string token);
bool IsInlineToken(string token);
bool IsMetaToken(string token);
bool IsCommentToken(string token);
bool IsEndToken(string token);
bool IsMateOnStack(string stackToken, string token);

int main() {
 
    //string test = "<html><b><i>CS106 rules!</i></b></html>";
    
    //cout << test << endl;
    
    string test = GetHtml();
    
    //cout << test << endl;
    
    cout << "Finished reading file. Parsing... " << endl;
    
    if (IsCorrectlyNested(test)) {
        cout << "Success." << endl;
    }
    else {
        cout << "Error: Bad token found" << endl;
        
    }
    
    return 0;
}

string GetHtml() {
    
    string outString;
    ifstream input;
    
    while (true) {
        cout << "Input file: ";
        string filename = GetLine();
        //string filename = "a1.html";
        //string filename = "assignments.html";
        //string filename = "cppref.html";
        input.open(filename.c_str());
        if (input.fail()) {
            cout << "Error: can not open " << filename << ". Try again..." << endl;
            input.clear();
        }
        else {
            break;
        }
    }
    
    while (true) {
        string str;
        getline(input, str);
        if (input.fail()) {
            break;
        }
        outString += str + "\n";
    }
    
    input.close();
    return outString;
}

bool IsCorrectlyNested(string htmlStr) {
    bool result = true;
    
    Scanner scanner;
    Stack<tokenLineT> stack;
    
    scanner.setSpaceOption(Scanner::PreserveSpaces);
    scanner.setBracketOption(Scanner::ScanBracketsAsTag);
    
    scanner.setInput(htmlStr);
    
    int line = 1;
    
    while(scanner.hasMoreTokens()) {
        tokenLineT token;
        token.tk = scanner.nextToken();
        token.line = line;
        //cout << endl << "*> Token: " << token << endl;
        if (IsBeginToken(token.tk)) {
            stack.push(token);
            //cout << "***** Stack Push: " << token << endl;
        }
        else if (IsEndToken(token.tk)) {
        tokenLineT stackToken;
            if (!stack.isEmpty()) {
                stackToken = stack.pop();
                if (!IsMateOnStack(stackToken.tk,token.tk)) {
                    cout << "BAD token: Line " << stackToken.line << ": " << stackToken.tk << " or Line " << token.line << ": " << token.tk << endl;
                    result = false;
                }
            }
            else {
                //cout << " >>>>>>>> **** >>>>>>>>>>  ***** >>>> Awesome ";
            }
            //cout << "Stack Pop: " << stackToken << ":" << token << endl;
        }
        if (token.tk == "\n") {
            line += 1;
        }
    }
    
    if (!stack.isEmpty()) {
        cout << "Stack is not empty!!!" << endl << "Content remaining on stack: " << endl;
        while (!stack.isEmpty()) {
            tokenLineT stackToken = stack.pop();
            cout << "Line " << stackToken.line << " : " << stackToken.tk << endl;
        }
        result = false;
    }
    return result;
}

bool IsBeginToken(string token) {
    bool result = false;
    if (token.find('<',0) != -1 && token.compare(1,1,"/") != 0 && token.find('>',token.size()-1) != 1) {
        if (!IsMetaToken(token) && !IsInlineToken(token) && !IsCommentToken(token)) {
            result = true;
        }
    }
    
    return result;
}

bool IsCommentToken(string token) {
    //Scanner has already verified < > boundary.
    //Just Verify ! at index 1
    if (token.compare(1,1,"!") == 0) {
        //cout << endl << "Comment Token Match Found: " << token << endl;
        return true;
    }
    return false;
}

bool IsMetaToken(string token) {
    //Scanner has already verified < > boundary.
    //Just Verify "meta" at index 1
    if (token.compare(1,4,"meta") == 0) {
        //cout << endl << "Meta Token Match Found: " << token << endl;
        return true;
    }
    return false;
}

bool IsInlineToken(string token) {
    //Scanner has already verified < > boundary.
    //Just Verify the string / starting at index length - 2
    //cout << "IsInlineToken: " << token << ":";
    if (token.compare(token.length()-2,1,"/") == 0) {
        //cout << endl << "Inline Token Match Found: " << token << endl;
        return true;
    }
    return false;
}

bool IsEndToken(string token) {
    bool result = false;
    
    if (token.find('<',0) != -1 && token.compare(1,1,"/") == 0 && token.find('>',token.size()-1) != 1) {
        result = true;
    }
    
    return result;
}

bool IsMateOnStack(string stackToken, string token) {
    bool result = false;
    
    token.replace(1,1,"");
    
    token.replace(0,1,"");
    stackToken.replace(0,1,"");
    
    Scanner scannerStack;
    Scanner scannerToken;
    
    scannerStack.setSpaceOption(Scanner::PreserveSpaces);
    scannerStack.setBracketOption(Scanner::ScanBracketsAsPunctuation);
    scannerToken.setSpaceOption(Scanner::PreserveSpaces);
    scannerToken.setBracketOption(Scanner::ScanBracketsAsPunctuation);
    
    scannerStack.setInput(stackToken);
    scannerToken.setInput(token);
    
    if (scannerStack.hasMoreTokens() && scannerToken.hasMoreTokens()) {
        string t1 = scannerStack.nextToken();
        string t2 = scannerToken.nextToken();
        //cout << "IsMateOnStack: stack=" << t1 << ":" << t2 << endl;
        if (t1 == t2) {
            result = true;
        }
    }
    return result;
}

