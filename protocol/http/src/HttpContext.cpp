#include "HttpContext.h"
#include "Buffer.h"
#include <iostream>
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
        switch(state_)
        {
        case RequestLine:
        {
            // std::cout<<"parse request line"<<std::endl;
            result =
                parser_.parseRequestLine(
                    buffer,
                    request_);

            if(result == ParseResult::Done)
            {
                state_ = Headers;
                continue;
            }

            if(result == ParseResult::NeedMoreData)
            {
                // std::cout 
                // << "need more data"
                // << std::endl;
                return false;
            }


            if(result == ParseResult::Error)
            {
                // std::cout 
                // << "parse error"
                // << std::endl;

                return false;
            }

            break;
        }
        case Headers:
        {
            // std::cout<<"parse headers"<<std::endl;
            result =
                parser_.parseHeader(
                    buffer,
                    request_);


            if(result == ParseResult::Done)
            {
                if(request_.hasBody())
                {
                    state_ = Body;
                }
                else
                {
                    state_ = Finish;
                }
                continue;
            }


            if(result == ParseResult::Continue)
            {
                // 还有header，继续解析
                continue;
            }


            if(result == ParseResult::NeedMoreData)
            {
                return false;
            }


            if(result == ParseResult::Error)
            {
                return false;
            }

            break;
        }

        case Body:
        {
            // std::cout<<"parse body"<<std::endl;
            result =
                parser_.parseBody(
                    buffer,
                    request_);

            if(result == ParseResult::Done)
            {
                state_=Finish;
                continue;
            }

            if(result == ParseResult::NeedMoreData)
            {
                return false;
            }

            if(result == ParseResult::Error)
            {
                return false;
            }

            break;
        }

        case Finish:
            return true;
        }
    }
}
