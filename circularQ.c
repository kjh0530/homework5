#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //ť�� �ִ� ũ�� ���ڴ� (�ִ� ũ�� - 1)�� ��ŭ ����. 

typedef char element; //char���� element�� ������ Ÿ������ ����
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //char�� �迭 ���� ũ��� ť�� �ִ� ũ��
	int front, rear; //ť�� front, rear ���� ������ int�� ���� ����
}QueueType; //����ü �̸�


QueueType *createQueue(); //���� �迭(ũ�Ⱑ ť�� �ִ�ũ��) ���� �� �ּ� ���� �ִ� �Լ�
int freeQueue(QueueType *cQ); //�����Ϳ� ����� �ּҿ� �ش��ϴ� �޸𸮸� free��Ű�� �Լ�
int isEmpty(QueueType *cQ); //ť�� ����ִ��� Ȯ���ϴ� �Լ�, ��������� 1���� �ƴϸ� 0����
int isFull(QueueType *cQ); //ť�� ���� á���� Ȯ���ϴ� �Լ�, ���� �������� 1���� �ƴϸ� 0���� 
void enQueue(QueueType *cQ, element item); //ť�� ���� �Է��ϴ� �Լ�
void deQueue(QueueType *cQ, element* item); //ť�� ���� �����ϴ� �Լ�(���� �������� ���� ������)
void printQ(QueueType *cQ); //ť�� ������ ����ϴ� �Լ�
void debugQ(QueueType *cQ); //ť�� ����� front, rear ���� ����ϴ� �Լ�
element getElement(); //char�� ���� �Է¹޴� �Լ�


int main(void)
{
	printf("[----- ������ 2020039027 -----]\n\n");//�̸� �й� ���
	QueueType *cQ = createQueue(); //ť�� �ִ�ũ�Ⱑ ũ���� �����迭 ���� ��
                                   //�ּ� ���� ����ü �����Ϳ� ����
	element data; //������ Ÿ���� element�� ���� ����

	char command; //��ɾ �Է¹��� char�� ���� ����
	
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
        //�޴� ���
		printf("Command = ");
		scanf(" %c", &command);
        //��ɾ� �Է�
		switch(command) {
		case 'i': case 'I': //i �Ǵ� I�� �Էµ��� ��
			data = getElement(); //element�� �Է¹޾Ƽ� data�� ����
			enQueue(cQ, data); //enQueue�Լ��� ���� data�� ����� ���� ť�� �Է�
			break;
		case 'd': case 'D': //d �Ǵ� D�� �Էµ��� ��
			deQueue(cQ,&data); //deQueue�Լ��� ���� ť�� �� �ϳ� ����
			break;
		case 'p': case 'P': //p �Ǵ� P�� �Էµ��� ��
			printQ(cQ); //printQ�Լ��� ť�� ���
			break;
		case 'b': case 'B': //b �Ǵ� B�� �Էµ��� ��
			debugQ(cQ); //debugQ�Լ��� ���� ť�� ���� ���(front, rear)
			break;
		case 'q': case 'Q': //q �Ǵ� Q�� �Էµ��� ��
			break; //do while�� Ż��
		default: //���� ���� �ƴ� ��
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); //����� �Է��϶�� �˷���
			break;
		}

	}while(command != 'q' && command != 'Q'); //q�Ǵ� Q�� ���;� ���ѷ��� Ż��


	return 1;
}

QueueType *createQueue() //ť�� �����ϴ� �Լ�
{
	QueueType *cQ; //����ü ������ ����
	cQ = (QueueType *)malloc(sizeof(QueueType)); //�����Ϳ� ť�� ũ�⸸ŭ �޸� �Ҵ�
	cQ->front = 0; //front�� 0���� �Ҵ�
	cQ->rear = 0; //rear�� 0���� �Ҵ�
	return cQ; //�޸𸮸� �Ҵ���� ������ ����(��, �ּҸ� ����)
}

int freeQueue(QueueType *cQ) //����ü �����Ϳ� �������� �Ҵ�� �޸𸮸� free��Ű�� �Լ� 
{
    if(cQ == NULL) return 1; //�������� ���� NULL�̸� �Լ� ����
    free(cQ); //NULL�� �ƴϸ� cQ�� free
    return 1;
}

element getElement()
{
	element item; //element�� item ���� ����
	printf("Input element = "); 
	scanf(" %c", &item); //���� �Է�
	return item; //������ �� ����
}


int isEmpty(QueueType *cQ) //ť�� ����ִ��� Ȯ���ϴ� �Լ�
{
	if (cQ->front == cQ->rear){ //front�� rear�� ���� ������ ť�� �������
		printf("Circular Queue is empty!");
		return 1; //������ 1�� ����
	}
	else return 0; //�ƴϸ� 0�� ����
}

int isFull(QueueType *cQ) //ť�� ���� �� �ִ��� Ȯ���ϴ� �Լ�
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { 
        //rear�� 1�� ���� ���� ť�� �ִ� ũ��� ��ⷯ ������ �� ���� 
        //front�� ������ ť�� ���� �� ��
		printf(" Circular Queue is full!");
		return 1; //������ 1�� ����
	}
	else return 0; //Ʋ���� 0�� ����
}

void enQueue(QueueType *cQ, element item) //ť�� ���� �ϳ� �Է��ϴ� �Լ�
{
	if(isFull(cQ)) return; //ť�� ���� �� ������ �Լ� ����
	else { //���� �� ���� ������
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear�� 1 ����,
                                                    //���� ť�̹Ƿ� ť�� �ִ� ũ�⸸ŭ ��ⷯ ����
		cQ->queue[cQ->rear] = item; //rear�� �ش��ϴ� ť�� �� �Է�
	}
}

void deQueue(QueueType *cQ, element *item) //ť�� ���� �ϳ� �����ϴ� �Լ�
{
	if(isEmpty(cQ)) return; //ť�� ��������� �Լ� ����
	else { //������� ������
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; //front�� 1 ����, 
                                                    //���� ť �̹Ƿ� ť�� �ִ� ũ�⸸ŭ ��ⷯ ����
		*item = cQ->queue[cQ->front]; //item�� ����Ű�� �ִ� ���� front�� �ش��ϴ� ť ���� �Ҵ�
									  //��, deQueue�� ť�� ���� �Ҵ�
		return;
	}
}


void printQ(QueueType *cQ) //ť�� ������ ����ϴ� �Լ� 
{
	int i, first, last; //int�� ���� �� �� ����

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first�� front+1�� ��ⷯ ������ ���� ����
                                            //front+1�� ť�� ù ��° ����
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //last�� rear+1�� ��ⷯ ������ ���� ����

	printf("Circular Queue : [");

	i = first;
	while(i != last){ //i�� ���� last�� �������� ����
		printf("%3c", cQ->queue[i]); //ù ��° ���Һ��� ���ʴ�� ���
		i = (i+1)%MAX_QUEUE_SIZE; //1�� ���� �� ��ⷯ ����

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //���� ť�� ���¸� ���
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //ť�� �ִ� ũ�⸸ŭ �ݺ�
	{
		if(i == cQ->front) { //i�� front���� ������
			printf("  [%d] = front\n", i); //i��° ���Ұ� front��� ���
			continue; //���
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //ť�� ���� 0~�ִ�ũ����� ���

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front�� rear �� ���
}