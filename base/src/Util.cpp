#include "Util.h"
#include <string.h>
#include <signal.h>
const int MAXBUF=4096;



ssize_t readNBytes(int fd,std::string&inBuffer)
{
    ssize_t readSum = 0;
    char buff[MAXBUF];
    while (true) 
    {
        ssize_t nread = read(fd, buff, MAXBUF);
        if (nread > 0) {
            readSum += nread;
            inBuffer.append(buff,nread);
        } else if (nread == 0) {
            return readSum; // 对端关闭
        } else {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN)
                return readSum;
            return -1;
        }
    }
}

ReadResult readn(int fd, std::string& buffer)
{
    ssize_t readSum = 0;
    while (true)
    {
        char buff[MAXBUF];
        ssize_t nread = read(fd, buff, MAXBUF);
        if (nread > 0)
        {
            readSum += nread;
            buffer.append(buff, nread);
        }
        else if (nread == 0)
        {
            // 对端关闭连接
            return {readSum, true, false};
        }
        else
        {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 非阻塞fd暂时没有数据
                return {readSum, false, false};
            }
            return {readSum, false, true};
        }
    }
}

ssize_t readNBytes(int fd,void*buffer,size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char *ptr =(char *)buffer;
    while (nleft>0) 
    {
        if ((nread = read(fd, ptr, nleft))<0) 
        {
            if (errno == EINTR)
                nread = 0;
            else if (errno == EAGAIN) 
                return readSum;
            else 
                return -1;
        } 
        else if (nread == 0)
            break;
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

ssize_t writeNBytes(int fd,const void *buff, size_t n)
{
    ssize_t nleft=n;
    ssize_t writenSum=0;
    const char* ptr = static_cast<const char*>(buff);
    while(nleft>0)
    {
        ssize_t nwriten=write(fd,ptr,nleft);
        if(nwriten<0){
            if(errno == EINTR)
                continue;
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                return writenSum;
            return -1;
        }
        writenSum += nwriten;
        nleft -= nwriten;
        ptr += nwriten;
    }
    return writenSum;
}

ssize_t writeNBytes(int fd,std::string&outBuffer)
{
    ssize_t nleft=outBuffer.size();
    size_t nwriten=0;
    size_t writeSum=0;
    const char*w_ptr=outBuffer.c_str();
    while(nleft>0)
    {
        if((nwriten=write(fd,w_ptr,nleft))<=0)
        {
            if(nwriten<0)
            {
                if(errno==EINTR)
                    continue;
                else if(errno==EAGAIN)
                    break;
                else
                    return -1;
            }
        }
        writeSum+=nwriten;
        nleft-=nwriten;
        w_ptr+=nwriten;
    }
    if(writeSum==static_cast<ssize_t>(outBuffer.size()))
        outBuffer.clear();
    else
        outBuffer = outBuffer.substr(writeSum);
    return writeSum;
}
void handle_for_sigpipe()
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_flags=0;
    sa.sa_handler = SIG_IGN;
    if(sigaction(SIGPIPE,&sa,nullptr)==-1)return;
}
