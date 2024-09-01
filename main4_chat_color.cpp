#include "consolestyles.h"
#include "groq.h"

//create environment Variable GROQ_API_KEY
Groq groq( get_api_key("GROQ_API_KEY") );


int main() 
{

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32


	Request request("llama-3.1-70b-versatile");
	request.temperature = 0.5;
	request.pushMessage("system", "short answers only");

	std::string input = "";
	while (input != "exit")
	{
		using namespace ConsoleStyles;

		std::cout << apply("user:", BRIGHT_BLUE) << std::endl;
		std::getline(std::cin, input);
		cp866_to_utf8(input);

		request.pushMessage("user", input);

		Response response = groq.chat.Completion(request);

		if (response.hasError()) {

			std::cout << apply(response.getError(), BRIGHT_RED) << std::endl;
		}
		else
		{
			Message* msg = response.getMessage();
			std::cout << msg->toString(GREEN) << std::endl;
			request.pushMessage(msg);
		}
	}

	return 0;

}


