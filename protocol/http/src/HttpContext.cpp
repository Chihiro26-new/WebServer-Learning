#include "HttpContext.h"

HttpContext::HttpContext(){}

bool HttpContext::gotAll()const
{
    return state_==Finish;
}

HttpRequest& HttpContext::request()
{
    return request_;
}

bool HttpContext::parseRequest(Buffer& buffer)
{
    while(true)
    {
        ParseResult result;
        switch(state_)
        {
        case RequestLine:
        {
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
                return false;
            }


            if(result == ParseResult::Error)
            {
                return false;
            }

            break;
        }
        case Headers:
        {
            result =
                parser_.parseHeader(
                    buffer,
                    request_);


            if(result == ParseResult::Done)
            {
                state_ = Body;
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
