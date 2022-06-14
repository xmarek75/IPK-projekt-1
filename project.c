#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *get_cpu_name()
{   
    char cpu_name[1000]="";
   
    FILE *cpufile = popen("cat /proc/cpuinfo | grep 'model name' | uniq","r");
    char ch;
    int i = 0;
    while ((ch = fgetc(cpufile)) != EOF)
    {   
        cpu_name[i]=ch;
        i++;
    }
    char * token = strtok(cpu_name, ":");
    token = strtok(NULL, ":");  //odstraneni "model name:"
    token = token+1;            //posunuti o jedno misto
    return token;

}

int main(int argc, char *argv[]){

    int server_sock;

    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //vytvoreni socketu
    if(server_sock<=0){perror("ERROR in socket"); exit(EXIT_FAILURE);} //test socketu
    int port = atoi(argv[1]);
        //nastaveni struktury adresy socketu
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    socklen_t server_address_len = sizeof(server_address);
        //bind socketu
    if(bind(server_sock, (struct sockaddr*) &server_address, sizeof(server_address))<0)
    {
        perror("ERROR:bind");
        exit(EXIT_FAILURE);
    }
        //listen
    if( (listen(server_sock, 1)) < 0 )
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }
    char buff[1024];
    char beg_response[1024]="HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
    char response[1024+1024];
    char hostname[1024];
    hostname[1023] = '\0';
    bzero(response,1024+1024);
    int res = 0;
    while(1){
        int comm_socket = accept(server_sock, (struct sockaddr*) &server_address, &server_address_len );
        if(comm_socket > 0)
        {
            for(;;)
            {   
                res = recv(comm_socket, buff, 1024,0);
                if(res <= 0)
                {
                    break;
                }
                    //zacatek bufferu: "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"
                if(strncmp("GET /hostname",buff,13)==0)
                {   
                    strcat(response,beg_response);
                    gethostname(hostname, 1023);
                    strcat(response, hostname);
                    strcat(response, "\n");
                }
                else if(strncmp("GET /cpu-name",buff,13)==0)
                {   
                    char *name = get_cpu_name();
                    strcat(response,beg_response);
                    strcat(response, name);
                }
                else if(strncmp("GET /load",buff,9)==0)
                {   
                    char cpu_usage[1000]="";
                    FILE *cpu_usagefile = popen(" grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage }'","r");
                    char chu;
                    int i = 0;
                    while ((chu = fgetc(cpu_usagefile)) != EOF)
                    {   
                        cpu_usage[i]=chu;
                        i++;
                    }
                    cpu_usage[i-1]='%';

                    strcat(response,beg_response);
                    strcat(response, cpu_usage);
                    strcat(response, "\n");
                }
                else
                {
                    strcat(response,beg_response);
                    strcat(response, "ERROR: wrong input");
                    strcat(response, "\n");
                }

                send(comm_socket, response, strlen(response),0);
                close(comm_socket);
                bzero(response,1024+1024);
            }
        }
    }
    return 0;
}
