#include "groq.h"

//create environment Variable GROQ_API_KEY
Groq groq(get_api_key("GROQ_API_KEY"));


int main() {

	//get models
	groq.models.List();

	//print models
	for (auto model : groq.models.data)
		std::cout << model.toString() << std::endl;

	return 0;

}