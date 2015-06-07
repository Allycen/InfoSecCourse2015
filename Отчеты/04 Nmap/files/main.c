#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char**argv)
{
   int listenfd;
   int connfd;
   int msgsize;

   struct    sockaddr_in servaddr;
   struct    sockaddr_in cliaddr;

   socklen_t clilen;
   pid_t     childpid;
   char      mesg[1000];

   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   bzero(&servaddr, sizeof(servaddr));

   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);            /* ADDR: ANY! */
   servaddr.sin_port = htons(2404);                         /* PORT: 2404 */
   bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   listen(listenfd, 1024);

   for(;;)
   {
      clilen = sizeof(cliaddr);
      connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

      if ((childpid = fork()) == 0)
      {
         close (listenfd);

         for(;;)
         {
            msgsize = recvfrom(connfd, mesg, 1000, 0, (struct sockaddr *)&cliaddr, &clilen);
            if (!strncmp(mesg,"version", 7))
            {
                strcpy(mesg, "1.0.0\n");
                msgsize = strlen(mesg);
            }
            sendto(connfd, mesg, msgsize, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
            
         }
         
      }
      close(connfd);
   }
}