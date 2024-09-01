# Groq API SDK

This project provides a C++ wrapper for accessing the [Groq](https://groq.com/) API.

## Description

The Groq API Wrapper encapsulates the main functions for interacting with the Groq API, such as:

- Retrieving a list of available models
- Executing a chat completion request
- Executing tool calls from a model
- Audio in progress

## Installation

Install `libcurl` and `jsoncpp` libraries.

Add `groq.h` and `groq.cpp` files to your project.
Optionally, you can also add `consolestyles.h` and `consolestyles.cpp`.

## Usage

0. Initialize variables:
    ```cpp
    #include "groq.h"
    Groq groq("key");
    Request request;
    ```
1. Fill a request:
    ```cpp
    request.pushMessage("user", "how do I get antimatter in the kitchen?");
    ```
2. Get the response:
    ```cpp
    Response response = groq.chat.Completion(request);
    ```
3. Print the response:
    ```cpp
    cout << response << endl;
    ```
4. Goto step1;

## Samples

The example files inside the project are sorted by increasing complexity:

- `main1_model_list.cpp`
- `main2_completion.cpp`
- `main3_chat.cpp`
- `main4_chat_color.cpp`
- `main5_tool_use.cpp`

If you have any further questions or need additional assistance, feel free to ask!
