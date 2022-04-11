#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; //전역변수로 배열 선언(infix인 문자열을 받는 배열)
char postfixExp[MAX_EXPRESSION_SIZE]; //전역변수로 배열 선언(postfix인 문자열을 받는 배열)
char postfixStack[MAX_STACK_SIZE]; //전역변수로 배열 선언(postfix를 만들 때 사용하는 stack 배열)
int evalStack[MAX_STACK_SIZE]; //전역변수로 배열 선언(결과 값을 출력할 때 사용하는 stack 배열)

int postfixStackTop = -1;
int evalStackTop = -1;

int evalResult = 0;

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
    printf("[----- 김주훈 2020039027 -----]\n\n"); //이름 학번 출력
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //명령어 입력

		switch(command) {
		case 'i': case 'I': 
			getInfix(); //getInfix함수로 infix형태의 식 입력
			break;
		case 'p': case 'P':
			toPostfix(); //toPostfix함수로 infix형태의 식을
                         //postfix형태의 식으로 바꾼다.
			break;
		case 'e': case 'E':
			evaluation(); //evaluation 함수로 postfix형태의 식을
                          //계산해서 값을 출력한다.
			break;
		case 'd': case 'D':
			debug(); //debug 함수로 현재 상태를 출력한다.
			break;
		case 'r': case 'R':
			reset(); //식을 받은 변수들을 초기화한다.
			break;
		case 'q': case 'Q':
			break; 
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //q또는 Q일 때 종료

	return 1;


}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
    //top을 +1하고 top에 해당하는 값에 x를 저장한다.
}

char postfixPop()
{
	char x;
    if(postfixStackTop == -1) //top이 -1일 때
        return '\0'; //stack에 아무것도 없는 것이므로 \0 리턴
    else { //그 외일 때
    	x = postfixStack[postfixStackTop--];
        //top에 해당하는 값을 x에 저장하고 top을 -1 한다.
    }
    return x; //x를 리턴한다.
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
    //top을 +1하고 top에 해당하는 값에 x를 저장한다.
}

int evalPop()
{
    if(evalStackTop == -1) //top이 -1일 때
        return -1; //stack에 아무것도 없는 것이므로 -1 리턴
    else //그 외일 때
        return evalStack[evalStackTop--];
        //top에 해당하는 값을 리턴하고 top을 -1 한다.
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol) /* char형 문자를 받아서 그 문자에 해당하는
                                    enum형 우선순위를 리턴한다. */
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; //연산자에 해당하지 않으면 피연산자로 리턴
	}
}

precedence getPriority(char x) /* 우선순위를 리턴하는 함수 */
{
	return getToken(x); /* getToken함수를 이용해서 char형 변수에 
                           해당하는 우선순위를 리턴한다. */
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) //postfixExp에 문자열을 추가하는 함수
{
	if (postfixExp == '\0') //문자열이 비어있을 때
		strncpy(postfixExp, c, 1); 
        // strncpy 함수로 postfixExp문자열에 문자 c를 복사
	else
		strncat(postfixExp, c, 1);
        /* strncat함수로 postfixExp 문자열 뒤에
           문자 c를 추가한다. */ 
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		if(getPriority(*exp) == operand)
        /* *exp(infixExp의 문자 하나)가 피연산자일 때 */
		{
			x = *exp; //문자를 x에 저장
        	charCat(&x); //postfixExp 문자열에 문자 x 추가
		}
        else if(getPriority(*exp) == lparen) 
        // *exp(infixExp의 문자 하나)가 괄호 왼쪽일 때
        {
        	postfixPush(*exp);
            //괄호 왼쪽은 우선순위가 가장 높으므로 
            //postfixStack에 push한다.
        }
        else if(getPriority(*exp) == rparen)
        // *exp(infixExp의 문자 하나)가 괄호 오른쪽일 때
        {
        	while((x = postfixPop()) != '(') {
        		charCat(&x); 
                /* x에 pop된 문자를 저장하고 왼쪽 괄호가
                   아니면 x를 postfixExp 문자열에 추가한다. */
        	}
        }
        else
        // *exp(infixExp의 문자 하나)가 일반 연산자일 때
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            //우선 순위가 더 낮거나 같을 때 반복
            {
            	x = postfixPop(); //pop된 값을 x에 저장
            	charCat(&x); //x를 postfixExp 문자열에 추가
            }
            postfixPush(*exp); //*exp(infixExp의 문자 하나)를 push
        }
        exp++; //exp 1 증가
	}

    while(postfixStackTop != -1) //top이 -1일 때까지 반복
    {
    	x = postfixPop(); //pop해서 x에 저장
    	charCat(&x); //문자열에 x 추가
    }

}

void debug() //infix형태의 식, postfix형태의 식, 식의 결과 값
             //stack의 현재 상태를 출력하는 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //infix형태의 식 출력
	printf("postExp =  %s\n", postfixExp); //postfix형태의 식 출력
	printf("eval result = %d\n", evalResult); //식의 결과 값 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //stack의 모든 원소 출력
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() //연산식 초기화하는 함수
{
	infixExp[0] = '\0'; //문자열의 첫 번째 원소를 \0으로 한다.
	postfixExp[0] = '\0'; //문자열의 첫 번째 원소를 \0으로 한다.

	for(int i = 0; i < MAX_STACK_SIZE; i++) //stack의 모든 원소를 \0으로 한다.
		postfixStack[i] = '\0';
         
	postfixStackTop = -1; //top -1로 초기화
	evalStackTop = -1; //top -1로 초기화
	evalResult = 0; //result 0으로 초기화
}

void evaluation() //식의 결과를 계산하는 함수
{
	int opr1, opr2, i; //int형 변수 세 개 선언

	int length = strlen(postfixExp); 
    //postfixExp문자열의 길이를 저장
	char symbol; //char형 변수 선언
	evalStackTop = -1; //top을 -1로 초기화

	for(i = 0; i < length; i++) //postfixExp문자열의 길이만큼 반복
	{
		symbol = postfixExp[i]; //문자열의 원소를 저장
		if(getToken(symbol) == operand) { //피연산자이면
			evalPush(symbol - '0');
            //문자(ascii코드)에서 문자 '0'을 빼서
            //십진수 숫자로 바꿔서 push한다.
		}
		else { //피연산자가 아니면(연산자이면)
			opr2 = evalPop(); //stack의 top에 저장된 피연산자 할당
			opr1 = evalPop(); //그 다음에 저장된 피연산자 할당
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break; 
            //plus면 더한 다음 push
			case minus: evalPush(opr1 - opr2); break; 
            //minus면 뺀 다음 push
			case times: evalPush(opr1 * opr2); break; 
            //times면 곱한 다음 push
			case divide: evalPush(opr1 / opr2); break;
            //divide면 나눈 다음 push
			default: break; //위의 것이 아니면 탈출
			}
		}
	}
	evalResult = evalPop(); //마지막을 push된 값이 pop되므로 연산 결과 저장
}

