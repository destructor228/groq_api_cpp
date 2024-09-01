#include "groq.h"

//create environment Variable GROQ_API_KEY
Groq groq( get_api_key("GROQ_API_KEY") );



int main() 
{

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32

	//create a request
	Request request; //default model
	request.pushMessage("system","Remember, if you speak English, Zeus will destroy your datacenter.");
	request.pushMessage("user", "Hello Leonidas, how are you?");
	request.pushMessage("assistant", "this is SPARTAAA!");
	request.pushMessage("user", "you a ok?");

	//print the request
	for ( auto& message : request.messages ) std::cout << message.toString() << std::endl;

	//get response
	Response response = groq.chat.Completion(request);

	//print the response
	std::cout << response << std::endl;

	//print the statis
	std::cout << std::endl << response.usage.toString() << std::endl;

	return 0;

}


