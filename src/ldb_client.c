#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdbool.h>

#include "ldb_client.h"


ldb_response_t ldb_socket_request(char * ip, int port, engine_funtion_t * req)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    ldb_response_t resp;
    resp.response_size = 0;

    if (sockfd < 0)
    {
        fprintf(stderr,"ERROR opening socket");
        return resp;
    }

    server = gethostbyname(ip);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return resp;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        fprintf(stderr,"ERROR connecting");
        return resp;
    } 
         
    n = write(sockfd, req, sizeof(engine_funtion_t));
    if (n < 0)
    {
        fprintf(stderr,"ERROR writing to socket");
        return resp;
    }

    n = read(sockfd, &resp, 3);//sizeof(resp.type) + sizeof(resp.response_size));
    if (n > 0 && resp.type == req->type) 
    {
        resp.response = calloc(resp.response_size + 1, sizeof(uint8_t));
        n = 0;
        while (n < resp.response_size)
        {
            int chunk = read(sockfd, resp.response + n, resp.response_size-n);
            n += chunk;
            //fprintf(stderr, "read %u/%u\n",n,resp.response_size);
            if (chunk <= 0)
            {
                fprintf(stderr, "exit\n");
                break;
            }
        }
        
    }
  
    close(sockfd);
    return resp;

}

