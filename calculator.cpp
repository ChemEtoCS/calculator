#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
using namespace std; 

int DebugMode = false;
// Enumarated Type specifying all of the Tokens
enum TokenType{
  ERROR, OPERATOR, VALUE, EOLN, QUIT, HELP, EOFILE
};
 
void printCommands()
{
 printf ("The commands for this program are:\n\n");
 printf ("q - to quit the program\n");
 printf ("? - to list the accepted commands\n");
 printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
}
 
 
 
// Class to hold the Token information
class Token
{
  private:
    TokenType type;
    char      op;       // using '$' as undefined/error
    int       val;      // using -999 as undefined/error
 
  public:
 
  // Default to initialize to the ERROR TokenType
  Token()
  {
    type = ERROR;
    op = '$';
    val = -999;
  }
 
  // Initialize to a specific TokenType
  Token (TokenType t)
  {
    type = t;
    op = '$';
    val = -999;
  }
 
  // Set to a specific TokenType
  void setToType(TokenType t)
  {
    type = t;
    op = '$';
    val = -999;
  }
 
  // Set to a OPERATOR TokenType with specific operator value
  void setToOperator(char c)
  {
    type = OPERATOR;
    op = c;
    val = -999;
  }
 
  // Set to a VALUE TokenType with a specific numeric value
  void setToValue(int v)
  {
    type = VALUE;
    op = '$';
    val = v;
  }
 
  // return true if the Current Token is of the given TokenType
  bool equalsType(TokenType t)
  {
    if (type == t)
      return true;
    else
      return false;
  }
 
  // return true if the Current Token is of the OPERATOR TokenType
  //     and contains the given operator character
  bool equalsOperator(char c)
  {
    if (type == OPERATOR && op == c)
      return true;
    else
      return false;
  }
 
  // Return the Operator for the current Token
  //   verify the current Token is of the OPERATOR TokenType
  char getOperator ()
  {
    if (type == OPERATOR)
      return op;
    else
      return '$';   // using $ to indicate an error value
  }
 
  // Return the Value for the current Token
  //   verify the current token is of the value TokenType
  int getValue()
  {
    if (type == VALUE)
      return val;
    else
      return -999;  // using -999 to indicate an error value
  }
};
 
 
class TokenReader
{
  private:
     char inputline[300];  // this assumes that all input lines are 300 characters or less in length
     bool needline;
     int pos;
 
 
  public:
 
  // initialize the TokenReader class to read from Standard Input
  TokenReader()
  {
    // set to read from Standard Input
    inputline[0] = '\0';
    pos = 0;
    needline = true;
  }
 
  // Force the next getNextToken to read in a line of input
  void clearToEoln()
  {
    needline = true;
  }
 
  // Return the next Token from the input line
  Token getNextToken()
  {
    char* endCheck;
 
    //printf ("getToken %d, %d, %s\n", pos, needline, inputline);
 
    // get a new line of input from user
    if (needline)
    {
      endCheck = fgets ( inputline, 300, stdin);
 
      if (endCheck == NULL )
      {
        printf ("Error in reading");
        Token t(EOFILE);
        return t;
      }
 
      for (int i = 0 ; i < strlen(inputline) ; i++)
          if ('\n' == inputline[i])
              inputline[i] = ' ';
      strcat (inputline , " ");    // add a space at end to help deal with digit calculation
      needline = false;
      pos = 0;
    }
 
    // skip over any white space characters in the beginning of the input
    while ( pos < strlen(inputline) && isspace(inputline[pos]) )
    {
      pos++;
    }
 
    // check for the end of the current line of input
    if (pos >= strlen(inputline))
    { // at end of line
      needline = true;
      Token t(EOLN);
      return t;
    }
 
    // Get the next character from the input line
    char ch = inputline[pos]; pos++;
 
    // check if 'q' or 'Q' was entered ==> QUIT Token
    if ( 'q' == ch || 'Q' == ch )
    {
      return Token (QUIT);
    }
 
    // check if "?" was entered ==> HELP Token
    if ( '?' == ch )
    {
      return Token (HELP);
    }
 
    // check for Operator values of: + - * / ( )  ==> OPERATOR Token
    if ( ('+' == ch) || ('-' == ch) || ('*' == ch) ||
      ('/' == ch) || ('(' == ch) || (')' == ch) )
    {
      Token t;
      t.setToOperator( ch );
      return t;
    }
 
    // check for a number  ==> VALUE Token
    if (isdigit(ch))
    {
      int number = int (ch) - int('0');  // subtract ascii value of ch from ascii value of '0'
      ch = inputline[pos]; pos++;
      while (isdigit(ch))
      {
        number = number * 10 + int (ch) - int('0');
        ch = inputline[pos]; pos++;
      }
      pos--; // since number calcuation check one character after the last digit
      Token t;
      t.setToValue( number );
      return t;
    }
 
    // Input in not valid if code get to this part ==> ERROR Token
    return Token (ERROR);
  }
 
};
 
//recreated stack class for practice
class myStack{
  private:
    int size;
    int inUse;
    int* arr;
    int* grow(){
      int* new_array = new int[size+2];
      size = size +2;
      for(int i =0; i <size;i++){
        new_array[i]=arr[i];
      }
      return new_array;
    }
  public:

  void init(){
    size = 2;
    inUse = 0;
    arr = new int[size];
    if(DebugMode == true){
      cout<<"my initial values are size:"<<size<<" inUse:"<< inUse<<endl;
    }
  }
  bool isEmpty(){
    if(this->inUse == 0)
      return true;
    else
      return false;
  }
  void push(int data){

    if(inUse < this->size){
      if(DebugMode== true){
        printf("data:%d is pushed onto inUse:%d with size:%d \n",data,inUse,size);
      }
      arr[inUse++]=data;
    }
    else{
      if(DebugMode== true){
        printf("data:%d is pushed on by growing the stack\n",data);
      }
      arr = this->grow();
      arr[inUse++]=data;
    }
  }
  void pop(){
    if(isEmpty()){
      cout<<"It's empty!\n";
      return;
    }
    arr[inUse--]=0;
  }
  void reset(){
    delete arr;
    inUse = 0;
    size =0;
  }
  int top(){return arr[inUse-1];}

  void printStack(int charOrint){//0 for char, 1 for int
    if(charOrint == 1){
      for(int i =0;i<inUse;i++){
        printf("%d. Value:%d Position: %d Size: %d\n",i,arr[i],inUse, size);
      }
    }
    else{
      for(int i =0;i<inUse;i++){
        printf("%d. Value:%c Position: %d Size: %d\n",i,arr[i],inUse, size);
      }
    }
  }
};

void processExpression (Token inputToken, TokenReader *tr);

bool isNotParantheses(char op){
  if(DebugMode== true){
    cout<<"Testing if not parantheses "<< op;
  }
  switch(op){
    case '+': return true;
    case '-': return true;
    case '/': return true;
    case '*': return true;

    default: return false;
  }

}
bool isMultorDiv(char op){
  switch(op){
    case '/': return true;
    case '*': return true;

    default: return false;
  }
}
int eval(int v1, char op, int v2){
  int v3;
  switch(op){
    case '+':
      v3 = v1 + v2;
      return v3;
    case '-':
      v3 = v1 - v2;
      return v3;
    case '/':
      v3 = v1/v2;
      return v3;
    case '*':
      v3 = v1*v2;
      return v3;
  }
  if(DebugMode == true){
    cout<<"error occured, not valid op\n";
  }
  return -999;
}
void popAndEval(myStack &operatorStack, myStack &valueStack){
  char op = static_cast<char>(operatorStack.top());
  operatorStack.pop();
  int v2 = valueStack.top();
  valueStack.pop();
  int v1 = valueStack.top();
  valueStack.pop();
  int v3 = eval(v1,op,v2);
  if(DebugMode== true){
    cout<<v1<<" "<<op<<" "<< v2<<"="<<v3<<endl;
  }
  valueStack.push(v3);
}

int main(int argc, char *argv[])
{
    
    int i;
    for ( i = 0 ; i < argc ; i++ ){
  //tries to find -d flag
    if ( strcmp (argv[i], "-d") == 0 )
        {DebugMode = true;}
    }
 
 
    Token inputToken;
    TokenReader tr;
 
    printf ("Starting Expression Evaluation Program\n\n");
    printf ("Enter Expression: ");
 
 
    inputToken = tr.getNextToken ();
 
    while (inputToken.equalsType(QUIT) == false)
    {
      /* check first Token on Line of input */
      if(inputToken.equalsType(HELP))
      {
       printCommands();
       tr.clearToEoln();
      }
      else if(inputToken.equalsType(ERROR))
      {
       printf ("Invalid Input - For a list of valid commands, type ?\n");
       tr.clearToEoln();
      }
      else if(inputToken.equalsType(EOLN))
      {
       printf ("Blank Line - Do Nothing\n");
       /* blank line - do nothing */
      }
      else
      {
       processExpression(inputToken, &tr);
      }
 
      printf ("\nEnter Expression: ");
      inputToken = tr.getNextToken ();
    }
 
  printf ("Quitting Program\n");
  return 1;
}
 
void processExpression (Token inputToken, TokenReader *tr)
{
 /**********************************************/
 /* Declare both stack head pointers here      */
 myStack operatorStack;
 myStack valStack;
 operatorStack.init();
 valStack.init();
 /* Loop until the expression reaches its End */
 while (inputToken.equalsType(EOLN) == false)
   {
    /* The expression contain a VALUE */
    if (inputToken.equalsType(VALUE))
      {
       /* make this a debugMode statement */
       if(DebugMode== true){
        printf ("Val: %d, ", inputToken.getValue() );
       }
       // add code to perform this operation here
       if(DebugMode== true){
            cout<<"value stack push"<<endl;
       }
       valStack.push(inputToken.getValue());
      }
 
    /* The expression contains an OPERATOR */
    else if (inputToken.equalsType(OPERATOR))
      {
        /* make this a debugMode statement */
        if(DebugMode== true){
            printf ("OP: %c, ", inputToken.getOperator() );
        }
        if(inputToken.getOperator() ==  '('){
         if(DebugMode== true){
           
            cout<<"operator stack push ("<<endl;
         }
          operatorStack.push(inputToken.getOperator());
        }

        if(inputToken.getOperator() ==  '+' ||inputToken.getOperator() ==  '-'){
        while(!operatorStack.isEmpty() && isNotParantheses(operatorStack.top())){
            //pop and eval
            popAndEval(operatorStack,valStack);
        }
        if(DebugMode == true){
          cout<<"operator stack push + or -"<<endl;
        }
        operatorStack.push(inputToken.getOperator());
        }
        if(inputToken.getOperator() ==  '*' || inputToken.getOperator() ==  '/'){
        while(!operatorStack.isEmpty() && isMultorDiv(operatorStack.top())){
            //pop and eval
            popAndEval(operatorStack,valStack);
        }
        if(DebugMode== true){
            cout<<"operator stack push * or /"<<endl;
        }
        operatorStack.push(inputToken.getOperator());
        }

        if(inputToken.getOperator() ==  ')'){
            while(operatorStack.isEmpty() != true && operatorStack.top() != 40){
            //pop and eval
            popAndEval(operatorStack,valStack);
            }
            if(operatorStack.isEmpty()){
                printf("Error! Stack is Empty\n");
            }
            else{
                operatorStack.pop();
            }
        }

       // add code to perform this operation here
       //operatorStack.push(inputToken.getOperator());
      }
 
    /* get next token from input */
    inputToken = tr->getNextToken ();
   }
 
 /* The expression has reached its end */
 
 // add code to perform this operation here
 while(!operatorStack.isEmpty()){
    //pop and eval
    popAndEval(operatorStack,valStack);
 }
  printf("Value is %d\n", valStack.top());
  printf ("\n");
  if(!operatorStack.isEmpty()){
    cout<<"Stack should be empty\n";
  }
  operatorStack.reset();
  valStack.reset();
}