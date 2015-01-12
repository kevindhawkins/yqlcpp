/*
    yqlcpp is a lightweight C++ API for performing Yahoo Query Language queries. 
    At this time it is intended to be a simple interface with minimal error 
    checking and no concern for thread safety. Built on top of and requires libcurl.

    Author: Kevin Hawkins <kevindhawkins@gmail.com>
    Copyright (c) 2014 Kevin Hawkins. All rights reserved.
    https://github.com/kevindhawkins/yqlcpp

    Licensed under the MIT license.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
 
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
 
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#include "yqlcpp.hpp"

namespace yqlcpp
{
    ///////////////////////////////////////////////////////////////////////

    //!< Execute the YQL query.
    bool yqlquery::execute()
    {		
        if (m_curl)
        {
            m_response.clear();
            std::string strRequest = "q=" + m_command + envCmd + "&format=" + formatToStr(m_format);

            curl_easy_setopt(m_curl, CURLOPT_URL, baseUrl.c_str());
            curl_easy_setopt(m_curl, CURLOPT_POST, 1);
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, strRequest.c_str());
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, responseWriter);
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_response);

            CURLcode result = curl_easy_perform(m_curl);
            return (result == CURLE_OK);
        }

        return false;
    }

    ///////////////////////////////////////////////////////////////////////

    size_t yqlquery::responseWriter(char* contents, size_t size, size_t nmemb, std::string* data)
    {
        if (data)
        {
            size_t realSize = size * nmemb;
            data->append(contents, realSize);

            return realSize;
        }

        return 0;
    }

    ///////////////////////////////////////////////////////////////////////

    std::string yqlquery::formatToStr(const yqlformat& format)
    {
        switch(format)
        {
        case yqlformat::JSON:
            return "json";
        case yqlformat::XML:
            return "xml";
        default:
            return "json";
        }
    }
}