# Introduction #

Information about use Google API, testing platforms, Qt versions.

# Details #

Project **qt-google-plus** uses Google API for Plus.

### Google API ###

Project **qt-google-plus** uses Google API for Plus.

How it works:
Work with Google-API perfomed by send https-request with using QNetworkAccessManager (see files _plus\_data\_manager.h_ and _plus\_data\_manager.cpp_)

API features used in this project:
|Search all public profiles.|
|:--------------------------|
|Get a person's profile.|
|List all of the people in the specified collection for a particular activity.|
|List all of the activities in the specified collection for a particular user.|
|Get an activity.|
|Search public activities.|
|List all of the comments for an activity.|
|Get a comment.|


File **[HowToRegisterYourAppIicationInGoogle](http://code.google.com/p/qt-google-plus/wiki/HowToRegisterYourApplicationInGoogle)** describes how register your own application on Google.

### Tested platforms ###
Project was tested on:
| **OS** | **Qt version** |
|:-------|:---------------|
|Linux 64bit|Qt 4.7.4, 4.8.1|
|Windows 7 64bit|Qt 4.8.0|
|Windows XP SP3 32bit|Qt 4.8.0|

# Various comments #

This application can be compiled on Linux, MacOS, Windows. For Windows you can use either MingW compiler or MSVC compiler.