#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //큐의 최대 크기 문자는 (최대 크기 - 1)개 만큼 들어간다. 

typedef char element; //char형을 element형 데이터 타입으로 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //char형 배열 선언 크기는 큐의 최대 크기
	int front, rear; //큐의 front, rear 값을 저장할 int형 변수 선언
}QueueType; //구조체 이름


QueueType *createQueue(); //동적 배열(크기가 큐의 최대크기) 생성 후 주소 값을 주는 함수
int freeQueue(QueueType *cQ); //포인터에 저장된 주소에 해당하는 메모리를 free시키는 함수
int isEmpty(QueueType *cQ); //큐가 비어있는지 확인하는 함수, 비어있으면 1리턴 아니면 0리턴
int isFull(QueueType *cQ); //큐가 가득 찼는지 확인하는 함수, 가득 차있으면 1리턴 아니면 0리턴 
void enQueue(QueueType *cQ, element item); //큐에 값을 입력하는 함수
void deQueue(QueueType *cQ, element* item); //큐의 값을 삭제하는 함수(가장 먼저들어온 값이 삭제됨)
void printQ(QueueType *cQ); //큐의 값들을 출력하는 함수
void debugQ(QueueType *cQ); //큐의 값들과 front, rear 값을 출력하는 함수
element getElement(); //char형 변수 입력받는 함수


int main(void)
{
	printf("[----- 김주훈 2020039027 -----]\n\n");//이름 학번 출력
	QueueType *cQ = createQueue(); //큐의 최대크기가 크기인 동적배열 생성 후
                                   //주소 값을 구조체 포인터에 저장
	element data; //데이터 타입이 element인 변수 선언

	char command; //명령어를 입력받을 char형 변수 선언
	
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
        //메뉴 출력
		printf("Command = ");
		scanf(" %c", &command);
        //명령어 입력
		switch(command) {
		case 'i': case 'I': //i 또는 I가 입력됐을 때
			data = getElement(); //element를 입력받아서 data에 저장
			enQueue(cQ, data); //enQueue함수를 통해 data에 저장된 값을 큐에 입력
			break;
		case 'd': case 'D': //d 또는 D가 입력됐을 때
			deQueue(cQ,&data); //deQueue함수를 통해 큐의 값 하나 삭제
			break;
		case 'p': case 'P': //p 또는 P가 입력됐을 때
			printQ(cQ); //printQ함수로 큐를 출력
			break;
		case 'b': case 'B': //b 또는 B가 입력됐을 때
			debugQ(cQ); //debugQ함수로 현재 큐의 상태 출력(front, rear)
			break;
		case 'q': case 'Q': //q 또는 Q가 입력됐을 때
			break; //do while문 탈출
		default: //위의 것이 아닐 때
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); //제대로 입력하라고 알려줌
			break;
		}

	}while(command != 'q' && command != 'Q'); //q또는 Q가 나와야 무한루프 탈출


	return 1;
}

QueueType *createQueue() //큐를 생성하는 함수
{
	QueueType *cQ; //구조체 포인터 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); //포인터에 큐의 크기만큼 메모리 할당
	cQ->front = 0; //front값 0으로 할당
	cQ->rear = 0; //rear값 0으로 할당
	return cQ; //메모리를 할당받은 포인터 리턴(즉, 주소를 리턴)
}

int freeQueue(QueueType *cQ) //구조체 포인터에 동적으로 할당된 메모리를 free시키는 함수 
{
    if(cQ == NULL) return 1; //포인터의 값이 NULL이면 함수 종료
    free(cQ); //NULL이 아니면 cQ를 free
    return 1;
}

element getElement()
{
	element item; //element형 item 변수 선언
	printf("Input element = "); 
	scanf(" %c", &item); //변수 입력
	return item; //변수의 값 리턴
}


int isEmpty(QueueType *cQ) //큐가 비어있는지 확인하는 함수
{
	if (cQ->front == cQ->rear){ //front와 rear의 값이 같으면 큐는 비어있음
		printf("Circular Queue is empty!");
		return 1; //맞으면 1을 리턴
	}
	else return 0; //아니면 0을 리턴
}

int isFull(QueueType *cQ) //큐가 가득 차 있는지 확인하는 함수
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { 
        //rear에 1을 더한 값에 큐의 최대 크기로 모듈러 연산을 한 값이 
        //front와 같으면 큐가 가득 찬 것
		printf(" Circular Queue is full!");
		return 1; //맞으면 1을 리턴
	}
	else return 0; //틀리면 0을 리턴
}

void enQueue(QueueType *cQ, element item) //큐의 값을 하나 입력하는 함수
{
	if(isFull(cQ)) return; //큐가 가득 차 있으면 함수 종료
	else { //가득 차 있지 않으면
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear값 1 증가,
                                                    //원형 큐이므로 큐의 최대 크기만큼 모듈러 연산
		cQ->queue[cQ->rear] = item; //rear에 해당하는 큐에 값 입력
	}
}

void deQueue(QueueType *cQ, element *item) //큐의 값을 하나 삭제하는 함수
{
	if(isEmpty(cQ)) return; //큐가 비어있으면 함수 종료
	else { //비어있지 않으면
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; //front값 1 증가, 
                                                    //원형 큐 이므로 큐의 최대 크기만큼 모듈러 연산
		*item = cQ->queue[cQ->front]; //item이 가리키고 있는 값에 front에 해당하는 큐 값을 할당
									  //즉, deQueue된 큐의 값을 할당
		return;
	}
}


void printQ(QueueType *cQ) //큐의 값들을 출력하는 함수 
{
	int i, first, last; //int형 변수 세 개 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first에 front+1을 모듈러 연산한 값을 저장
                                            //front+1은 큐의 첫 번째 원소
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //last에 rear+1을 모듈러 연산한 값을 저장

	printf("Circular Queue : [");

	i = first;
	while(i != last){ //i의 값이 last와 같아지면 종료
		printf("%3c", cQ->queue[i]); //첫 번째 원소부터 차례대로 출력
		i = (i+1)%MAX_QUEUE_SIZE; //1씩 증가 후 모듈러 연산

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //현재 큐의 상태를 출력
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //큐의 최대 크기만큼 반복
	{
		if(i == cQ->front) { //i가 front값과 같으면
			printf("  [%d] = front\n", i); //i번째 원소가 front라고 출력
			continue; //계속
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //큐의 원소 0~최대크기까지 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front와 rear 값 출력
}