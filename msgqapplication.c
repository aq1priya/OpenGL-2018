#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

#define MAXMTEXT 1024

struct mymsg
{
        long mtype;
        char mtext[MAXMTEXT];
};

int main(void)
{
        int len, id, receiving, opt;
        key_t key;
        struct mymsg msg;
        char message[] = "end";
    char ChatPersonName[20],ChatAliasName[20];
        //strcpy(msg.mtext, message);
        msg.mtype = 1;
        key = ftok("./MyMessageQueue.txt", 'X');
        if (key == -1)
        {
                perror("ftok");
                exit(1);
        }

        id = msgget(key, 0666 | IPC_CREAT );
        if (id == -1)
        {
                perror("msgget");
                exit(1);
        }
	printf("\nWelcome to Chat Application.\n\n");
    printf("\tEnter your chat alias name: ");
    scanf("%s",&ChatAliasName);
    while(1)
    {
        switch(opt)
        {
            case 1:
                printf("Enter chat person name:");
                scanf("%s",&ChatPersonName);
                printf("%s:Type your messages\n",ChatAliasName);
                while (fgets(msg.mtext, sizeof(msg.mtext), stdin) != NULL)
                {
                    len = strlen(msg.mtext);
                    /* remove newline at end, if it exists */
                    if (msg.mtext[len - 1] == '\n')
                    {
                        msg.mtext[len - 1] = '\0';
                    }
                    if (msgsnd(id, &msg, len + 1, 0) == -1) /* +1 for '\0' */
                    {
                        perror("msgsnd");
                        exit(1);
                    }
                }
                strcpy(msg.mtext, message);
                len = strlen(msg.mtext);
                if(msgsnd(id, &msg, len+1, 0) == -1)
                {
                    perror("msgsnd");
                    exit(1);
                }
                    printf("message sent.\n");
                break;
			case 2:
                printf("Receiving messages...\n");
                receiving = 1;
                while(receiving)
                {
                    if(msgrcv(id,&msg, sizeof(msg.mtext), 0, 0) == -1)
                    {
                        perror("msgrcv");
                        exit(1);
                    }
                    if(strcmp(msg.mtext, "end") == 0)
                    {
                       // printf("%s\n", msg.mtext);
                        break;
                    }
                            else
                            {
                        printf("%s says: ",ChatPersonName);
                        printf("msg: %s\n", msg.mtext);
                    }
                }
                printf("Msg received\n");
                break;

            case 3:
                printf("Exit application\n");
                return(0);
                break;
        }
        printf("\t\tEnter your Choice\n");
        printf("\t\t1. Send Messages\n");
        printf("\t\t2. receive Messages\n");
        printf("\t\t3. Exit from application\n");
        scanf("%d", &opt);
    }
}
