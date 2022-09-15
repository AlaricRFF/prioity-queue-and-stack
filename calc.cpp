#include "dlist.h"
#include <iostream>
#include <string>
using namespace std;
typedef void (*funcPtr_t)(Dlist<int>*);
enum error_T{BAD_INPUT, DEFICIENT_OP, DIV_0};

void hasTwoOp(Dlist<int>* stack){
    // EFFECTS: if the stack has >= 2 operands, do nothing; if <2 op, throw DEFICIENT_OP
    if (stack->isEmpty())
        throw DEFICIENT_OP;
    int* temp = stack->removeFront();
    if (stack->isEmpty()){
        stack->insertFront(temp); // reset the stack to original state
        throw DEFICIENT_OP;
    }
    else{ // has enough elements. Therefore, re-push the elem to the stack
        stack->insertFront(temp);
    }
}

void insertNum(const int& num, Dlist<int>* stack){ /// Question: whether the pointer need to be dynamic? Answer: definitely!
    // MODIFIES: *stack
    // EFFECTS: push a legal number at the front of the stack
    int* ptr = new int(num);
    stack->insertFront(ptr);
}

void calcPlus(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: if less than two operands, throw error; else, do as the manual says, free the top two
    // dynamic objects
    hasTwoOp(stack);
    int *numPtr_1 = stack->removeFront();
    int *numPtr_2 = stack->removeFront();
    int *sum = new int(*numPtr_1 + *numPtr_2);
    stack->insertFront(sum);
    delete numPtr_1;
    delete numPtr_2; // free dynamic objects
}

void calcMinus(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: if less than two operands, throw error; else, do as the manual says, free the top two
    // dynamic objects
    hasTwoOp(stack);
    int *numPtr_1 = stack->removeFront();
    int *numPtr_2 = stack->removeFront();
    int *division = new int(*numPtr_2 - *numPtr_1);
    stack->insertFront(division);
    delete numPtr_1;
    delete numPtr_2; // free dynamic objects
}

void calcMultiply(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: if less than two operands, throw error; else, do as the manual says, free the top two
    // dynamic objects
    hasTwoOp(stack);
    int *numPtr_1 = stack->removeFront();
    int *numPtr_2 = stack->removeFront();
    int *result = new int((*numPtr_2) * (*numPtr_1));
    stack->insertFront(result);
    delete numPtr_1;
    delete numPtr_2; // free dynamic objects
}

void calcDivide(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: if less than two operands, throw error; else, do as the manual says, free the top two
    // dynamic objects.
    // if denominator is zero, throw error (later than DEFICIENT_OP
    hasTwoOp(stack);
    int *numPtr_1 = stack->removeFront();
    /// ERROR CHECKING -- DIVIDE BY ZERO
    if (*numPtr_1 == 0){
        stack->insertFront(numPtr_1); // remain the stack unchanged
        throw DIV_0;
    }
    int *numPtr_2 = stack->removeFront();
    int *result = new int((*numPtr_2) / (*numPtr_1));
    stack->insertFront(result);
    delete numPtr_1;
    delete numPtr_2; // free dynamic objects
}

void calcNegate(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: if less than one operands, throw error; else, do as the manual says
    if (stack->isEmpty())
        throw DEFICIENT_OP;
    int *elem = stack->removeFront();
    *elem *= -1;
    stack->insertFront(elem);
}

void calcDuplicate(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS:  do as the manual says
    if (stack->isEmpty())
        throw DEFICIENT_OP;
    int *elem = stack->removeFront();
    int *elt = new int(*elem); // initialize elt with the value of int stored in *elem
    stack->insertFront(elt);
    stack->insertFront(elem);
}

void calcReverse(Dlist<int>* stack){
    // MODIFIES: *stack
    // EFFECTS: do as the manual says
    hasTwoOp(stack);
    int *numPtr_1 = stack->removeFront();
    int *numPtr_2 = stack->removeFront();
    stack->insertFront(numPtr_1); // note the sequence of insertion, first out, first in
    stack->insertFront(numPtr_2);
}

void calcPrint(Dlist<int>* stack){
    // MODIFIES: cout
    // EFFECTS:  do as the manual says

    if (stack->isEmpty())
        throw DEFICIENT_OP;
    int *elem = stack->removeFront();
    stack->insertFront(elem);
    cout << *elem << endl;
}

void calcClear(Dlist<int>* stack){
    // MODIFES: *stack
    // EFFECTS: remove all elements from the stack
    while (!stack->isEmpty()){
        auto val = stack->removeFront();
        delete val;
    }
}

void calcPrintAll(Dlist<int>* stack){
    // EFFECTS: print all the elements in the stack and stack remains unchanged
    if (stack->isEmpty())
        cout << endl;
    else{
        Dlist<int> copy(*stack) ; // make a copy of the original list and print the copy of the list
        while (!copy.isEmpty()){
            auto val = copy.removeFront();
            cout << *val << ' ';
            delete val;
        }
        cout << endl;
    }
}

bool inputProcess(const string& input, funcPtr_t* func, int& num){
    // MODIFIES: *func, a pointer to a (function pointer)
    // EFFECTS: convert "input" to corresponding input direction; if not, throw BAD_INPUT error
    // if input is valid number, return true; if it's other input, return false
            if ((input[0] == '-'&& input.size()>1) || isdigit(input[0])) { // check whether it satisfy the format of number; if not, throw bad input
                string c_number; // store the intermediate digits
                bool hasNonZeroHead = false; // a flag to indicate whether non-zero digits has appeared
                if (input[0] == '-'){
                    c_number += '-';
                }else{
                    c_number += input[0];
                    if (input[0] != '0')
                        hasNonZeroHead = true;
                }
                for (auto i = input.begin()+1; i!=input.end();i++){
                    if ( !isdigit((*i)) ){ // not number
                        throw BAD_INPUT;
                    }
                    else{
                        if ( (*i) != '0'){ // has non-zero highest digit, then begin recording the real input number
                            hasNonZeroHead = true;
                            c_number += *i;
                        }
                        if ( (*i) =='0' && hasNonZeroHead){ // only record the zero after the non-zero highest decimal
                            c_number += *i;
                        }
                    }
                } // end for
                /// process the c_number to int
                num = stoi(c_number);
                return true ;
            }
    if (input.size()>1){
        throw BAD_INPUT;
    }
        /// deal with input size == 1
        if (input[0] == '+'){
            *func = calcPlus;
        }
        else if( input[0] == '-'){
            *func = calcMinus;
        }
        else if (input[0] == '*'){
            *func = calcMultiply;
        }
        else if ( input[0] == '/'){
            *func = calcDivide;
        }
        else if (input[0] == 'n'){
            *func = calcNegate;
        }
        else if (input[0] == 'd'){
            *func = calcDuplicate;
        }
        else if (input[0] == 'r'){
            *func = calcReverse;
        }
        else if (input[0] == 'p'){
            *func = calcPrint;
        }
        else if (input[0] == 'c'){
            *func = calcClear;
        }
        else if (input[0] == 'a'){
            *func = calcPrintAll;
        }
        else{
            throw BAD_INPUT;
        }

    return false;
}

int main(){
    Dlist<int> stack;
    /*   use dlist as stack
     * 1. ONLY use remove from FRONT as POP
     * 2. ONLY use insert from FRONT as push */
    string input;
    funcPtr_t func; /// define a function pointer to simplify
    /// begin simulation
    cin >> input;
    while ( input != "q"){
        try {
            /* process the input*/
            int number; // store number, if the user input any
            bool flag; // to judge whether the input is any valid number
            flag = inputProcess(input,&func,number);
            if (flag){ // it means the user enters a number
                insertNum( number, &stack);
            }
            /// DEBUG: bad input
            /* execute the function (except for the case the user enters a number)*/
            else{func(&stack);}
            /// DEBUG: not enough op or div_0
            cin >> input; // renew input
        }
        catch (error_T error){
            if (error == BAD_INPUT){
                cout << "Bad input\n";
                cin >> input; // renew input
                continue;
            }
            else if (error == DEFICIENT_OP){
                cout << "Not enough operands\n";
                cin >> input; // renew input
                continue;
            }
            else if (error == DIV_0){
                cout << "Divide by zero\n";
                cin >> input; // renew input
                continue;
            }
            else {
                cout << "unknown error !" << endl;
                return -1;
            }
        }
    }

    return 0;
}

