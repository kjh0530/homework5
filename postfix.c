#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack ������ �켱������ ��������, lparen = 0 ���� ���� */
typedef enum{
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; //���������� �迭 ����(infix�� ���ڿ��� �޴� �迭)
char postfixExp[MAX_EXPRESSION_SIZE]; //���������� �迭 ����(postfix�� ���ڿ��� �޴� �迭)
char postfixStack[MAX_STACK_SIZE]; //���������� �迭 ����(postfix�� ���� �� ����ϴ� stack �迭)
int evalStack[MAX_STACK_SIZE]; //���������� �迭 ����(��� ���� ����� �� ����ϴ� stack �迭)

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
    printf("[----- ������ 2020039027 -----]\n\n"); //�̸� �й� ���
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //��ɾ� �Է�

		switch(command) {
		case 'i': case 'I': 
			getInfix(); //getInfix�Լ��� infix������ �� �Է�
			break;
		case 'p': case 'P':
			toPostfix(); //toPostfix�Լ��� infix������ ����
                         //postfix������ ������ �ٲ۴�.
			break;
		case 'e': case 'E':
			evaluation(); //evaluation �Լ��� postfix������ ����
                          //����ؼ� ���� ����Ѵ�.
			break;
		case 'd': case 'D':
			debug(); //debug �Լ��� ���� ���¸� ����Ѵ�.
			break;
		case 'r': case 'R':
			reset(); //���� ���� �������� �ʱ�ȭ�Ѵ�.
			break;
		case 'q': case 'Q':
			break; 
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //q�Ǵ� Q�� �� ����

	return 1;


}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
    //top�� +1�ϰ� top�� �ش��ϴ� ���� x�� �����Ѵ�.
}

char postfixPop()
{
	char x;
    if(postfixStackTop == -1) //top�� -1�� ��
        return '\0'; //stack�� �ƹ��͵� ���� ���̹Ƿ� \0 ����
    else { //�� ���� ��
    	x = postfixStack[postfixStackTop--];
        //top�� �ش��ϴ� ���� x�� �����ϰ� top�� -1 �Ѵ�.
    }
    return x; //x�� �����Ѵ�.
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
    //top�� +1�ϰ� top�� �ش��ϴ� ���� x�� �����Ѵ�.
}

int evalPop()
{
    if(evalStackTop == -1) //top�� -1�� ��
        return -1; //stack�� �ƹ��͵� ���� ���̹Ƿ� -1 ����
    else //�� ���� ��
        return evalStack[evalStackTop--];
        //top�� �ش��ϴ� ���� �����ϰ� top�� -1 �Ѵ�.
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */

void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol) /* char�� ���ڸ� �޾Ƽ� �� ���ڿ� �ش��ϴ�
                                    enum�� �켱������ �����Ѵ�. */
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; //�����ڿ� �ش����� ������ �ǿ����ڷ� ����
	}
}

precedence getPriority(char x) /* �켱������ �����ϴ� �Լ� */
{
	return getToken(x); /* getToken�Լ��� �̿��ؼ� char�� ������ 
                           �ش��ϴ� �켱������ �����Ѵ�. */
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c) //postfixExp�� ���ڿ��� �߰��ϴ� �Լ�
{
	if (postfixExp == '\0') //���ڿ��� ������� ��
		strncpy(postfixExp, c, 1); 
        // strncpy �Լ��� postfixExp���ڿ��� ���� c�� ����
	else
		strncat(postfixExp, c, 1);
        /* strncat�Լ��� postfixExp ���ڿ� �ڿ�
           ���� c�� �߰��Ѵ�. */ 
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		if(getPriority(*exp) == operand)
        /* *exp(infixExp�� ���� �ϳ�)�� �ǿ������� �� */
		{
			x = *exp; //���ڸ� x�� ����
        	charCat(&x); //postfixExp ���ڿ��� ���� x �߰�
		}
        else if(getPriority(*exp) == lparen) 
        // *exp(infixExp�� ���� �ϳ�)�� ��ȣ ������ ��
        {
        	postfixPush(*exp);
            //��ȣ ������ �켱������ ���� �����Ƿ� 
            //postfixStack�� push�Ѵ�.
        }
        else if(getPriority(*exp) == rparen)
        // *exp(infixExp�� ���� �ϳ�)�� ��ȣ �������� ��
        {
        	while((x = postfixPop()) != '(') {
        		charCat(&x); 
                /* x�� pop�� ���ڸ� �����ϰ� ���� ��ȣ��
                   �ƴϸ� x�� postfixExp ���ڿ��� �߰��Ѵ�. */
        	}
        }
        else
        // *exp(infixExp�� ���� �ϳ�)�� �Ϲ� �������� ��
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            //�켱 ������ �� ���ų� ���� �� �ݺ�
            {
            	x = postfixPop(); //pop�� ���� x�� ����
            	charCat(&x); //x�� postfixExp ���ڿ��� �߰�
            }
            postfixPush(*exp); //*exp(infixExp�� ���� �ϳ�)�� push
        }
        exp++; //exp 1 ����
	}

    while(postfixStackTop != -1) //top�� -1�� ������ �ݺ�
    {
    	x = postfixPop(); //pop�ؼ� x�� ����
    	charCat(&x); //���ڿ��� x �߰�
    }

}

void debug() //infix������ ��, postfix������ ��, ���� ��� ��
             //stack�� ���� ���¸� ����ϴ� �Լ�
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //infix������ �� ���
	printf("postExp =  %s\n", postfixExp); //postfix������ �� ���
	printf("eval result = %d\n", evalResult); //���� ��� �� ���

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //stack�� ��� ���� ���
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() //����� �ʱ�ȭ�ϴ� �Լ�
{
	infixExp[0] = '\0'; //���ڿ��� ù ��° ���Ҹ� \0���� �Ѵ�.
	postfixExp[0] = '\0'; //���ڿ��� ù ��° ���Ҹ� \0���� �Ѵ�.

	for(int i = 0; i < MAX_STACK_SIZE; i++) //stack�� ��� ���Ҹ� \0���� �Ѵ�.
		postfixStack[i] = '\0';
         
	postfixStackTop = -1; //top -1�� �ʱ�ȭ
	evalStackTop = -1; //top -1�� �ʱ�ȭ
	evalResult = 0; //result 0���� �ʱ�ȭ
}

void evaluation() //���� ����� ����ϴ� �Լ�
{
	int opr1, opr2, i; //int�� ���� �� �� ����

	int length = strlen(postfixExp); 
    //postfixExp���ڿ��� ���̸� ����
	char symbol; //char�� ���� ����
	evalStackTop = -1; //top�� -1�� �ʱ�ȭ

	for(i = 0; i < length; i++) //postfixExp���ڿ��� ���̸�ŭ �ݺ�
	{
		symbol = postfixExp[i]; //���ڿ��� ���Ҹ� ����
		if(getToken(symbol) == operand) { //�ǿ������̸�
			evalPush(symbol - '0');
            //����(ascii�ڵ�)���� ���� '0'�� ����
            //������ ���ڷ� �ٲ㼭 push�Ѵ�.
		}
		else { //�ǿ����ڰ� �ƴϸ�(�������̸�)
			opr2 = evalPop(); //stack�� top�� ����� �ǿ����� �Ҵ�
			opr1 = evalPop(); //�� ������ ����� �ǿ����� �Ҵ�
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break; 
            //plus�� ���� ���� push
			case minus: evalPush(opr1 - opr2); break; 
            //minus�� �� ���� push
			case times: evalPush(opr1 * opr2); break; 
            //times�� ���� ���� push
			case divide: evalPush(opr1 / opr2); break;
            //divide�� ���� ���� push
			default: break; //���� ���� �ƴϸ� Ż��
			}
		}
	}
	evalResult = evalPop(); //�������� push�� ���� pop�ǹǷ� ���� ��� ����
}

