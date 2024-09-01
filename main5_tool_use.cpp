#include "consolestyles.h"
#include "groq.h"

//1) create environment Variable GROQ_API_KEY
Groq groq( get_api_key("GROQ_API_KEY") );

//2) create tool
std::string tool_command(ToolCall * t)
{
	std::string style = t->function.get_string("style");
	std::string text =  t->function.get_string("text");

	std::cout << ConsoleStyles::BRIGHT_BLACK << "toolprint: " << ConsoleStyles::RESET;
	std::cout << ConsoleStyles::apply(text, style) << std::endl;
	return "true";
}
//2) create tool
#include <random>
std::string tool_price(ToolCall* t)
{
	std::string item = t->function.get_string("item");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(10, 100);
	std::string price = std::to_string(dis(gen));

	std::cout << ConsoleStyles::BRIGHT_BLACK  << "toolprice: " << item << " = " << price  << ConsoleStyles::RESET << std::endl;

	return price + " gold";
}
//2) create tool
std::string tool_telephone(ToolCall* t)
{
	std::string item = t->function.get_string("user");
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 9);
	std::string phone_number = "";
	for (int i = 0; i < 8; ++i) {
		phone_number += std::to_string(dis(gen));
	}
	std::cout << ConsoleStyles::BRIGHT_BLACK << "toolphone: " << item << " = " << phone_number << ConsoleStyles::RESET << std::endl;
	return phone_number;
}
//2) create tool
#ifdef _WIN32
#include <sapi.h>
std::string tool_speech(ToolCall* t)
{

	std::string text = t->function.get_string("text");
	if(text == "") return "parametr text is empty";

	//utf8_to_LPCWSTR
	WCHAR wtext[1024];
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wtext, 1024);

	std::cout << ConsoleStyles::BRIGHT_BLACK << "toolspeech: " << text << ConsoleStyles::RESET << std::endl;
	HRESULT hr = ::CoInitialize(nullptr);
	if (FAILED(hr)) {
		std::cout << "CoInitialize error: " << hr << std::endl;
		return "CoInitialize error\n";
	}

	ISpVoice* pVoice = nullptr;
	hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (FAILED(hr)) {
		std::cout << "CoCreateInstance error: " << hr << std::endl;
		::CoUninitialize();
		return "CoCreateInstance error\n";
	}

	if (pVoice) {
		hr = pVoice->Speak(wtext, 0, nullptr);
		pVoice->Release();
		if (FAILED(hr)) {
			std::cout << "Speak error: " << hr << std::endl;
			return "Speak error\n";
		}
	}

	::CoUninitialize();

	return "true";
}
#else
std::string tool_speech(ToolCall* t)
{
	std::string text = t->function.get_string("text");
	std::cout << ConsoleStyles::BRIGHT_BLACK << "toolspeech not supported: " << text << ConsoleStyles::RESET << std::endl;
	return "true";
}
#endif
//2) create tool
bool breakflag = false;
std::string tool_exit(ToolCall* t)
{
	int item = t->function.get_int("code");
	breakflag = true;

	std::cout << ConsoleStyles::BRIGHT_BLACK << "toolexit " << item << ConsoleStyles::RESET << std::endl;
	return "1";
}

int main() {

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32

	//3)create tool discription
	Function func2("cost", "Function return cost of item");
	func2.pushParament("item", "string", "item name");

	Function func3("exit", "use it to close dialog");
	func3.pushParament("code", "int", "exit code");

	Function func4("telephone", "Function return telephone number");
	func4.pushParament("user", "string", "user name");

	Function func5("speech", "function speak text");
	func4.pushParament("text", "string", "this text will be spoken");

	Function func1("command",
		"Function out system commnad to computer\n \
		\ NOT USE for user and assistant message \n \
		\ The style much reflects the mood\n"	
	);
	func1.pushParament("text", "string", "command for system");
	func1.pushParament("style", "string", "standar ascii console codes:\n RED = \033[31m, YELLOW = \033[33m, GREEN = \033[32m");


	Request request(kDefaultToolModel);
	request.temperature = 0.5;
	
	//4)add tools
	request.pushFunction(tool_command, func1);
	request.pushFunction(tool_price, func2);
	request.pushFunction(tool_exit, func3);
	request.pushFunction(tool_telephone, func4);
	request.pushFunction(tool_speech, func5);


	//5)add message	
	request.pushMessage("system", 
		" not use command for user ansver\n\
		\ not use speech  for user ansver\n\
		\ chek your ansver on nonsense \n\
		\ if USER wants close dialog, call function exit\n"
	);
	//for less nonsense
	request.pushMessage("user", "hello");
	request.pushMessage("assistant", "hello");
	

	//6) go chat
	Message * toolmessage = nullptr;
	while (true)
	{
		using namespace ConsoleStyles;
		std::string input = "";

		//If a function call is present in the message, respond with the function, otherwise respond with the user.
		if(toolmessage != nullptr)
		{
			//tools ansver
			for (auto& tool : toolmessage->tool_calls)
			{
				Message tm = request.UseTool(tool);
				request.pushMessage(tm);
			}
			toolmessage = nullptr;
		}
		else
		{
			//user ansver
			std::cout << apply("user:", BRIGHT_BLUE) << std::endl;
			std::getline(std::cin, input);
			cp866_to_utf8(input);

			request.pushMessage("user", input);
		}
		
		//exit if ai use tool_exit
		if (breakflag) break;

		Response response = groq.chat.Completion(request);

		//
		if (response.hasError()) {

			std::cout << apply(response.getError(), BRIGHT_RED) << std::endl;
		}
		else
		{
			//add ansver to request
			Message* msg = response.getMessage();
			request.pushMessage(msg);

			if (msg->isToolCalls())
			{
				//call tool
				toolmessage = &request.messages.back();
			}
			else
			{
				//print asistant ansver
				std::cout << msg->toString(GREEN) << std::endl;
			}
		}
	}

	return 0;

}


