#ifndef GROQ_API_H
#define GROQ_API_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>

#include <curl/curl.h>
#include <json/json.h>


const std::string kBaseUrl = "https://api.groq.com/openai/v1";
const std::string kChatCompletionsEndpoint = "/chat/completions";
const std::string kTranscriptionsEndpoint = "/audio/transcriptions";
const std::string kTranslationsEndpoint = "/audio/translations";
const std::string kModelsEndpoint = "/models";

const double kDefaultTemperature = 0.6;
const int kDefaultMaxTokens = 1024;
const std::string kDefaultModel = "llama-3.1-70b-versatile";
const std::string kDefaultToolModel = "llama3-groq-70b-8192-tool-use-preview";


class ToolCall
{
private:
	static std::string valueTypeToString(Json::ValueType type);
public:
	std::string id;
	std::string type;
	class FunctionCall
	{
	public:
		std::string name;
		std::string arguments;
		

		std::vector <std::string>     debug_argum;
		std::vector <Json::ValueType> debug_types;
		std::vector <std::string>     debug_names;

		//std::cout << root << std::endl;
		//nullValue = 0, ///< 'null' value
		//intValue,   1   ///< signed integer value
		//uintValue,   2  ///< unsigned integer value
		//realValue,    3 ///< double value
		//stringValue,  4 ///< UTF-8 string value
		//booleanValue, 5 ///< bool value
		//arrayValue,   6 ///< array value (ordered list)
		//objectValue   7 ///< object value (collection of name/value pairs).
		Json::Value arguments_root;

		int get_int(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isInt())
			{
				return arguments_root[name].asInt();
			}
			return 0;
		}
		unsigned int get_uint(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isUInt())
			{
				return arguments_root[name].asUInt();
			}
			return 0;
		}
		double get_double(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isDouble())
			{
				return arguments_root[name].asDouble();
			}
			return 0.0;
		}
		std::string get_string(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isString())
			{
				return arguments_root[name].asString();
			}
			return "";
		}
		bool get_bool(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isBool())
			{
				return arguments_root[name].asBool();
			}
			return false;
		}
		Json::Value get_array(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isArray())
			{
				return arguments_root[name];
			}
			return Json::Value::null;
		}
		Json::Value get_object(const std::string& name)
		{
			if (arguments_root.isMember(name) && arguments_root[name].isObject())
			{
				return arguments_root[name];
			}
			return Json::Value::null;
		}

		bool parse_arguments()
		{	
			Json::Reader reader;
			if (reader.parse(arguments, arguments_root))
			{
				if (!arguments_root.isObject()) return false;

				auto names = arguments_root.getMemberNames();
				for (const auto& n : names)
				{
					//std::cout << n << std::endl;
					debug_names.push_back(n);
					auto t = arguments_root[n].type();
					debug_types.push_back(t);

					if (t == Json::objectValue)
					{
						debug_argum.push_back(arguments_root[n].toStyledString());
					}
					else
					{
						debug_argum.push_back(arguments_root[n].asString());
					}

				};
			}
			else
			{
				//std::cout << "parse error" << std::endl;
				return false;
			}
			return true;
		}

	} function;

	bool fromJson(const Json::Value& root);;

	Json::Value toJson() const;;

	std::string toString()
	{
		std::string text = "call: " + function.name + "(";

		for (int i = 0; i < function.debug_names.size(); i++)
		{
			std::string param = function.debug_names[i];
			std::string type = valueTypeToString(function.debug_types[i]);
			std::string value = function.debug_argum[i];

			if(value[0] == '\033')
			{
				value = "\\033" + value.substr(1);
			}
			

			text += type + " " + param + "=" + value;
			if (i < function.debug_names.size() - 1)
			{
				text += ", ";
			}

		}
		text += ")";


		return text;
	}

private:

};

class Message
{
public:
	enum
	{
		None = 0,
		System,
		User,
		Assistant,
		Tool
	} type = None;

	//all
	std::string _role = "";
	std::string content = "";
	std::string username = "";

	//tool
	std::string tool_call_id = "";
	Json::Value result;


	std::vector <ToolCall> tool_calls;
	bool isToolCalls() const
	{
		return tool_calls.size() > 0;
	}

	Message() {};
	Message( const std::string& r, const std::string& c)
	{
		set_role( r );
		content = c;
	}

	Message( const std::string& role, const std::string& id, const std::string& result)
	{
		set_role( role );
		this->tool_call_id = id;
		this->content = result;
	}

	void set_role(std::string role)
	{
		this->_role = role;
		// Convert role to type
		if (role == "system")
		{
			type = System;
		}
		else if (role == "user")
		{
			type = User;
		}
		else if (role == "assistant")
		{
			type = Assistant;
		}
		else if (role == "tool")
		{
			type = Tool;
		}
		else
		{
			type = None;
		}

	};
	std::string get_role() const
	{
		if (_role.empty())
		{
			std::cout << "Error: role is empty" << std::endl;
			throw std::runtime_error("role is empty");
		}

		return _role;
	}

	bool fromJson(const Json::Value& root);
	Json::Value toJson() const;

	std::string system_toString(const std::string& style = "") const
	{
		std::string text = style + get_role() + ":\n" + "\033[0m";
		text += content;
		return text;
	}
	std::string user_toString(const std::string& style = "") const
	{
		std::string text = style + get_role() + ":\n" + "\033[0m";
		text += content;
		return text;
	}
	std::string assistant_toString(const std::string& style = "") const
	{
		std::string text = style + get_role() + ":\n" + "\033[0m";
		if (isToolCalls())
		{
			for (auto tool_call : tool_calls)
			{
				text += tool_call.toString();
				text += "\n";
			}
			text.pop_back();
		}
		else
		{
			text += content;
		};
		return text;
	}
	std::string tool_toString(const std::string& style = "") const
	{
		std::string text = style + get_role() + ": " + "\033[0m";
		text += "tool_call_id = " + tool_call_id + " ";
		text += "result = " + content;
		
		return text;
	}


	std::string toString(const std::string& style = "") const
	{
		switch (type)
		{
			case None: break;
			case System: return system_toString(style);
			case User: return user_toString(style);
			case Assistant: return assistant_toString(style);
			case Tool: return tool_toString(style);
			default: break;
		}

		return "unknown";
	};

};

//
using ToolPtr = std::string(*)(ToolCall*);


class Tool  //for any tools in future
{
public:
	std::string tool_type;
};

class Function: public Tool
{
public:
	class Parametr {
	public:
		std::string name;
		std::string type;
		std::string description;

		bool fromJson(const std::string&  name,  const Json::Value& root )
		{
			this->name = name;
			if(root.isMember("type") && root["type"].isString()) type = root["type"].asString();
			if(root.isMember("description") && root["description"].isString()) description = root["description"].asString();

			return true;
		};
		Json::Value toJson() const
		{
			Json::Value root;
			root["type"] = type;
			root["description"] = description;
			return root;
		}

	private:
	};

	std::string name;
	std::string description;
	std::map<std::string, Parametr > parameters;
	std::vector<std::string> required;

	void pushParament(const std::string& name, const std::string& type, const std::string& description)
	{
		parameters[name].name = name;
		parameters[name].type = type;
		parameters[name].description = description;

		required.push_back(name);
	}

	bool fromJson(const Json::Value& root);
	bool fromString(const std::string& str)
	{
		Json::CharReaderBuilder readerBuilder;
		Json::CharReader* reader1 = readerBuilder.newCharReader();
		Json::Value jsonData;
		std::string errors;

		// Парсинг JSON строки
		bool parsingSuccessful = reader1->parse(str.c_str(),
			str.c_str() + str.size(),
			&jsonData, &errors);
		if (parsingSuccessful)
		{
			fromJson(jsonData);
		}
		else
		{
			std::cout << "parse error" << std::endl;
		}

		delete reader1;
		return parsingSuccessful;
	}
	
	void testload()
	{
		std::string json =	"\
			{\
				\"type\": \"function\",\
				\"function\" : \
					{\
						\"name\": \"function_name\",\
						\"description\" : \"details about the function\",\
			 			\"parameters\":	\
			 			  {\
							\"type\": \"object\",\
							\"properties\" :\
							{\
								\"param1\":\
								{\
									\"type\": \"type_name1\",\
									\"description\" : \"type description 1.\",\
								},\
								\"param2\":\
								{\
									\"type\": \"type_name2\",\
									\"description\" : \"type description 2.\",\
								},\
							},\
							\"required\": [\"param1\", \"param2\"],\
						},\
				},\
			}";
		
		fromString(json);
	}
	Json::Value toJson() const;;
	std::string toString()
	{
		return toJson().toStyledString();
	}

	Function()
	{
		tool_type = "function";
	}

	Function(const std::string& name, const std::string& description)
	{
		tool_type = "function";
		this->name = name;
		this->description = description;
	}
};



class Request
{
public:
	std::vector<Message> messages;
	std::string model;
	double temperature;
	int max_tokens;
	//double top_p;
	bool stream;
	std::vector<std::string> stop;

	std::vector<Function> tool_data;
	std::map< std::string, ToolPtr> tool_pointer;

	std::string tool_choice = "auto"; // ?? none, auto, required, function code

	void pushMessage(const std::string& role, const std::string& content)
	{
		messages.push_back(Message(role, content));
	}
	void pushMessage(const Message& message)
	{
		messages.push_back(message);
	}
	void pushMessage(const Message * message)
	{
		if (message != nullptr)
		{
			messages.push_back(*message);
		}		
	}

	void pushFunctionJson( ToolPtr pfunk, const std::string& funcJsonText )
	{
		Function f;
		f.fromString(funcJsonText);
		pushFunction(pfunk, f);
	}
	void pushFunctionJson( ToolPtr pfunk, const Json::Value& funcJson )
	{
		Function f;
		f.fromJson(funcJson);
		pushFunction(pfunk, f);
	}
	void pushFunction( ToolPtr pfunk, Function& f )
	{
		tool_data.push_back(f);
		tool_pointer[f.name] = pfunk;
	}

	Message UseTool( ToolCall & toolcall )
	{
		if (tool_pointer.count(toolcall.function.name))
		{
			ToolPtr ptool = tool_pointer[toolcall.function.name];
			std::string toolCallOutput = ptool(&toolcall);
			Message tm = Message("tool", toolcall.id, toolCallOutput);
			return tm;
		};

		return Message("tool", toolcall.id, "function not found, NOT CALL THIS");

	}

	Request()
	{
		stream = false;
		temperature = kDefaultTemperature;
		max_tokens = kDefaultMaxTokens;
		model = kDefaultModel;
		//top_p = 1.0;
	}
	Request(std::string model)
	{
		stream = false;
		temperature = kDefaultTemperature;
		max_tokens = kDefaultMaxTokens;
		this->model = model;
		//top_p = 1.0;
	}

	bool fromJson(const Json::Value& root);
	bool fromString(const std::string& str);
	Json::Value toJson();
	std::string toString();
};

class Response
{
public:
	class Choice
	{
	public:
		std::string finish_reason;
		int index;
		bool logprobs;
		Message message;

		bool fromJson(const Json::Value& root);
		Json::Value toJson() const;
	};

	std::vector<Choice> choices;
	int created;
	std::string id;
	std::string model;
	std::string object;
	std::string system_fingerprint;
	class Usage
	{
	public:
		double completion_time;
		int completion_tokens;
		double prompt_time;
		int prompt_tokens;
		double queue_time;
		double total_time;
		int total_tokens;

		std::string toString() const
		{
			return
				"completion_time: " + std::to_string(completion_time) + "\n" +
				"completion_tokens: " + std::to_string(completion_tokens) + "\n" +
				"prompt_time: " + std::to_string(prompt_time) + "\n" +
				"prompt_tokens: " + std::to_string(prompt_tokens) + "\n" +
				"queue_time: " + std::to_string(queue_time) + "\n" +
				"total_time: " + std::to_string(total_time) + "\n" +
				"total_tokens: " + std::to_string(total_tokens);
		}
		bool fromJson(const Json::Value& root);
		Json::Value toJson() const;
	} usage;	
	class X_groq
	{
	public:
		std::string id;

		bool fromJson(const Json::Value& root);
		Json::Value toJson() const;
	} x_groq;
	class Error
	{
	public:
		std::string message;
		std::string type;
		std::string failed_generation;

		bool fromJson(const Json::Value& root);;
		Json::Value toJson();

		std::string toString() const
		{
			std::string res = type + ":\n" + message;
			if (failed_generation != "")
			{
				res += ":\n" + failed_generation;
			}
			return res;
		};

	} error;
	bool error_flag = true;

	std::string debug_str;

	bool fromJson(const Json::Value& root);
	bool fromString(const std::string& str);

	Message* getMessage(int n = 0)
	{
		if (error_flag)
		{
			return nullptr;
		}
		if (choices.size() <= n)
		{
			return nullptr;
		};
		
		return &choices[n].message;		
	}
	std::string getError()
	{
		if (error_flag)
		{
			return error.toString();
		}
		else
		{
			return "";
		}

	}
	bool hasError()
	{
		return error_flag;
	}

	Json::Value toJson();
	std::string toString();
	
};

std::ostream& operator << (std::ostream& os, const Response & response);

class Curlbase
{
public:
	
	std::string _completion(const std::string& request_string)
	{
		std::string response_string;
		if (curl != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

			if (request_string.size() > 0)
			{
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string.c_str());
			}


			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK) {
				std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			}
		}
		return response_string;
	}

private:
	friend class Groq;
	friend class Chat;
	friend class Models;

	bool init(curl_slist* headers)
	{
		curl = curl_easy_init();
		if (curl) {			
			curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		}
		return (curl != NULL);
	}
	void curl_cleanup()
	{
		if (curl != NULL)
		{
			curl_easy_cleanup(curl);
			curl = NULL;
		}
	}
	
	static size_t writeFunction(void* contents, size_t size, size_t nmemb, void* userp)
	{
		std::string * response = static_cast<std::string*>(userp);

		if (response->size() > 0)
		{
			//std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		}

		response->append(static_cast<const char*>(contents), size * nmemb);
		return size * nmemb;
	}

	std::string apiUrl;
	CURL* curl = NULL;

};

class Chat : public Curlbase
{
public:
	Response Completion(Request request)
	{
		std::string request_string = request.toString();
		std::string response_string = Curlbase::_completion(request_string);

		 

		Json::Value val;
		Json::Reader reader;
		if (!reader.parse(response_string, val))
		{
			std::cout << "parse error" << std::endl;
		}
		else
		{
			//std::cout << val << std::endl;
		};


		Response response;
		response.debug_str = response_string;
		if (!response.fromString(response_string))
		{
			response.error_flag = true;
		}
		return response;
	}

private:
	friend class Groq;
	bool initchat(curl_slist* headers)
	{
		apiUrl = kBaseUrl + kChatCompletionsEndpoint;

		if (init(headers))
		{
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curlbase::writeFunction);
			return true;
		};

		return false;
	}
};

class Models : public Curlbase
{
public:
	class Model {
	public:
		std::string id;
		std::string object;
		long created;
		std::string owned_by;
		bool active;
		int context_window;
		std::string public_apps;

		bool fromJson(const Json::Value& root);		
		std::string toString()
		{
			return id + " by " + owned_by + "  context:" + std::to_string(context_window) + " tokens";
		}	
	};

	std::string object;
	std::vector<Model> data;

	std::string error_message;
	std::string error_type;
	std::string error_code;

	bool fromJson(const Json::Value& root);
	bool List();

private:
	friend class Groq;
	bool initmodels(curl_slist* headers)
	{
		apiUrl = kBaseUrl + kModelsEndpoint;
		if (init(headers))
		{
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curlbase::writeFunction);
			return true;
		};

		return false;
	}
};

class Groq
{

public:
	Chat chat;
	Models models;

	Groq(std::string apiKey)
	{	
		if (apiKey.size() > 0)
		{
			curl_init();
			headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
			headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");

			chat.initchat(headers);
			models.initmodels(headers);
		}

	}
	~Groq()
	{
		curl_slist_free_all(headers);

		chat.curl_cleanup();
		models.curl_cleanup();

		curl_free();
	};

private:
	struct curl_slist* headers = NULL;

	static int init_counter;
	static std::mutex curl_mutex;

	static void curl_init()
	{
		std::lock_guard<std::mutex> lock(curl_mutex);
		if (init_counter == 0) {
			curl_global_init(CURL_GLOBAL_DEFAULT);
		}
		init_counter++;
	}

	static void curl_free()
	{
		std::lock_guard<std::mutex> lock(curl_mutex);
		init_counter--;
		if (init_counter == 0) {
			curl_global_cleanup();
		}
	}

};


//utils
std::string get_api_key(const char* env);
void cp866_to_utf8(std::string& str);

#endif // MY_HEADER_FILE_H