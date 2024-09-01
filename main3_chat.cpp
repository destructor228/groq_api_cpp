#include "groq.h"

//create environment Variable GROQ_API_KEY
Groq groq( get_api_key("GROQ_API_KEY") );


int main() {


	//create a request
	Request request;
	//optional system instruction
	request.pushMessage("system", "evil answers");

	std::string input = "";

	while (input != "exit")
	{
		//get user input
		std::cout << "user:" << std::endl;
		std::getline(std::cin, input);

		//add the user input to the prompt
		request.pushMessage("user", input);

		//get response
		Response response = groq.chat.Completion(request);

		//print the response
		std::cout << response << std::endl;

		//add the response to the prompt
		request.pushMessage(response.getMessage());
	}



	return 0;

}


