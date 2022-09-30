// cc: https://web.archive.org/web/20120504033548/https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_EPOLL_EVENTS 64

int make_socket_non_blocking(
    int socketfd
) {
    int flags,s;
    
    flags = fcntl(socketfd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }
    
    flags |= O_NONBLOCK;
    s = fcntl(socketfd, F_SETFL, flags);
    if (s < 0) {
        return -1;
    }
    
    return 0;
}

int checkpollerror(int event)
{
    if ((event & EPOLLERR) ||
        (event & EPOLLHUP) ||
        (!(event & EPOLLIN))) {
            return 1;
        } else {
            return 0;
        }
}

void webhookUpdates(int *port)
{
    // vars
    int socket_fd, epoll_fd, ctlres, opt, s;
    
    struct sockaddr_in srvAddr;
    struct epoll_event event, *events;
    
    // end vars
    
    // socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
        printf("socket() fail");
        abort();
    }
    
    memset(&srvAddr, 0, sizeof (struct sockaddr_in));
    
    if ((s = make_socket_non_blocking(socket_fd)) < 0) {
        printf("make_socket_non_blocking() fail");
        abort();
    }
    
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("setsockopt() fail");
        abort();
    }
    
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = INADDR_ANY;
    srvAddr.sin_port = htons(*port);
    
    if (bind(socket_fd, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0) {
        printf("bind() fail");
        abort();
    }
    
    if (listen(socket_fd, 3) < 0) {
        printf("listen() error");
        abort();
    }

    // end socket
    
    // epoll
    if ((epoll_fd = epoll_create(1)) < 0) {
        printf("epoll_create() fail");
        abort();
    }
    
    event.data.fd = socket_fd;
    event.events = EPOLLIN | EPOLLET;
    
    if ((ctlres = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event)) < 0) {
        printf("epoll_ctl() fail");
        abort();
    }
    
    events = calloc(MAX_EPOLL_EVENTS, sizeof (event));
    // end epoll
    
    while(1)
    {
        int n;
        
        n = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (checkpollerror(events[i].events)) {
                printf("epoll err");
                close(events[i].data.fd);
                continue;
            } else if (socket_fd == events[i].data.fd) {
                // incoming connections
                while(1) {
                    int newsock;
                    struct sockaddr income_addr;
                    socklen_t income_addr_len;
                    
                    if ((newsock = accept(
                        socket_fd,
                        &income_addr,
                        &income_addr_len
                    )) < 0) {
                        printf("accept() fail");
                        break;
                    }
                    
                    if ((s = make_socket_non_blocking(newsock)) < 0) {
                        abort();
                    }
                    
                    event.data.fd = newsock;
                    event.events = EPOLLIN | EPOLLET;
                    
                    s = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newsock, &event);
                    if (s < 0) {
                        printf("epoll_ctl() error");
                        abort();
                    }
                }
                continue;
            } else {
                int done = 0;
                while(1)
                {
                    ssize_t cnt;
                    char buf[512];
                    
                    if ((cnt = read(
                        events[i].data.fd,
                        buf,
                        sizeof(buf)
                    )) < 0) {
                        if (errno == EAGAIN) {
                            printf("read() EAGAIN");
                            done = 1;
                        }
                        
                        break;
                    } else if (cnt == 0) {
                        done = 1;
                        break;
                    }
                    
                    s = write(1, buf, cnt);
                    if (s < 0) {
                        printf("write() fail");
                        abort();
                    }
                }
            
                if (done > 0) {
                    close(events[i].data.fd);
                }
            }
        }
    }
    
    free(events);
    close(socket_fd);
}