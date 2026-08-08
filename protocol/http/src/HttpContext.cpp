#include "HttpContext.h"
#include "Buffer.h"
#include "HttpParser.h"
HttpContext::HttpContext():state_(RequestLine){}
HttpContext::~HttpContext(){}
bool HttpContext::gotAll()const
{
    return state_==Finish;
}
void HttpContext::reset()
{
    state_ = RequestLine;
    request_.reset();
}

const HttpRequest& HttpContext::request()const
{
    return request_;
}

bool HttpContext::parseRequest(Buffer& buffer)
{
    while(true)
    {
        //  std::cout 
        //     << "state = "
        //     << state_
        //     << " readable="
        //     << buffer.readableBytes()
        //     << std::endl;
        ParseResult result;
        switch (state_)
        {
            case RequestLine:
            {
                result=parser_.parseRequestLine(buffer
                    ,request_);
                //      std::cout
                //     << "method: "
                //     << request_.method()
                //     << std::endl;

                // std::cout
                //     << "path: "
                //     << request_.path()
                //     << std::endl;

                // std::cout
                //     << "version: "
                //     << request_.version()
                //     << std::endl;
                if(result==ParseResult::Done)
                {
                    state_=Headers;
                    continue;
                }
                if(result==ParseResult::Continue)
                    continue;
                if(result==ParseResult::NeedMoreData)
                    return false;
                if(result == ParseResult::Error)
                    return false;
                break;
            }

            case Headers:
            {
                result=parser_.parseHeader(buffer,request_);
                if(result==ParseResult::Done)
                {
                    if(request_.hasBody())
                        state_ = Body;
                    else
                        state_ = Finish;
                    continue;
                }
                if(result==ParseResult::Continue)
                    continue;
                if(result==ParseResult::NeedMoreData)
                    return false;
                if(result==ParseResult::Error)
                    return false;
                break;
            }
            case Body:
            {
                result=parser_.parseBody(buffer,request_);
                if(result==ParseResult::Done)
                {
                    state_=Finish;
                    continue;
                }
                if(result == ParseResult::NeedMoreData)
                    return false;
                if(result == ParseResult::Error)
                    return false;
                break;
            }
            case Finish:
                return true;
       }
    }
}
