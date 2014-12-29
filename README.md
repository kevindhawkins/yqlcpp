yqlcpp
======

yqlcpp is a lightweight C++ API for performing Yahoo Query Language queries. 

At this time it is intended to be a simple interface with minimal error 
checking and no concern for thread safety. Built on top of and requires libcurl.

Licensed under the MIT license.

##Usage

```
// query setup, optional parameter to specify JSON/XML defaulting to JSON
yqlcpp::yqlquery query("select * from yahoo.finance.quotes where symbol='AAPL'");

// execute the query
query.execute();

// get the response string
std::string jsonData = query.getResponse();
	
// optional save response data to file	
query.toFile("quotetest2.xml");
```

[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/kevindhawkins/yqlcpp/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
