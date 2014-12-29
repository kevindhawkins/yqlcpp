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


#ifndef __YQLCPP_H
#define __YQLCPP_H

#include <string>
#include <fstream>
#include <curl/curl.h>

namespace yqlcpp
{
    const std::string baseUrl = "http://query.yahooapis.com/v1/public/yql";
    const std::string envCmd = "&env=store://datatables.org/alltableswithkeys";

    class yqlquery
    {
    public:
        
        enum class yqlformat { JSON, XML };

        yqlquery(const std::string& cmd, const yqlformat& format = yqlformat::JSON) : m_command(cmd)
        {
            m_curl = curl_easy_init(); 
            m_format = formatToStr(format);
        }

        virtual ~yqlquery() { curl_easy_cleanup(m_curl); }

        //!< Execute the YQL query.
        bool execute()
        {		
            if (m_curl)
            {
                std::string strRequest = "q=" + m_command + envCmd + "&format=" + m_format;

                curl_easy_setopt(m_curl, CURLOPT_URL, baseUrl.c_str());
                curl_easy_setopt(m_curl, CURLOPT_POST, 1);
                curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, strRequest.c_str());
                curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, responseWriter);

                CURLcode result = curl_easy_perform(m_curl);
                return (result == CURLE_OK);
            }

            return false;
        }

        //!< Get the YQL query response as a string.
        std::string getResponse() { return (*m_response); }

        //!< Write the YQL response to a file.
        void toFile(const std::string& filename)
        {
            if (m_response)
            {
                std::ofstream out(filename);
                out << (*m_response);
                out.close();
            }
        }

    private:
        yqlquery() {}

        CURL* m_curl;				//!< Our curl handle.
        std::string m_command;		//!< Query command sent to YQL.
        std::string m_format;		//!< Format of the response data.

        static std::string* m_response;		//!< Response data, provided and managed by curl.

        ///////////////////////////////////////////////////////////////////////

        //!<  Writes curl response data to the response string.
        static size_t responseWriter(char* contents, size_t size, size_t nmemb, std::string* data)
        {
            if (data)
            {
                size_t realSize = size * nmemb;
            
                data->append(contents, realSize);
                m_response = data;

                return realSize;
            }

            return 0;
        }

        ///////////////////////////////////////////////////////////////////////

        std::string formatToStr(const yqlformat& format)
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
    };

    std::string* yqlquery::m_response = NULL;

}

#endif
