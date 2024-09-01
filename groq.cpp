#include "groq.h"

int Groq::init_counter = 0;
std::mutex Groq::curl_mutex;





bool Response::Choice::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("finish_reason") && root["finish_reason"].isString()) finish_reason = root["finish_reason"].asString();
	if (root.isMember("index") && root["index"].isInt()) index = root["index"].asInt();
	if (root.isMember("logprobs") && root["logprobs"].isNull()) logprobs = false;
	if (root.isMember("message") && root["message"].isObject())
	{
		message.fromJson(root["message"]);
	};


	return true;
}

Json::Value Response::Choice::toJson() const
{
	Json::Value root;
	root["finish_reason"] = finish_reason;
	root["index"] = index;
	root["logprobs"] = Json::nullValue;
	root["message"] = message.toJson();
	return root;
}

bool Response::Usage::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("completion_time") && root["completion_time"].isDouble()) completion_time = root["completion_time"].asDouble();
	if (root.isMember("completion_tokens") && root["completion_tokens"].isInt()) completion_tokens = root["completion_tokens"].asInt();
	if (root.isMember("prompt_time") && root["prompt_time"].isDouble()) prompt_time = root["prompt_time"].asDouble();
	if (root.isMember("prompt_tokens") && root["prompt_tokens"].isInt()) prompt_tokens = root["prompt_tokens"].asInt();
	if (root.isMember("queue_time") && root["queue_time"].isDouble()) queue_time = root["queue_time"].asDouble();
	if (root.isMember("total_time") && root["total_time"].isDouble()) total_time = root["total_time"].asDouble();
	if (root.isMember("total_tokens") && root["total_tokens"].isInt()) total_tokens = root["total_tokens"].asInt();
	return true;
}

Json::Value Response::Usage::toJson() const
{
	Json::Value root;
	root["completion_time"] = completion_time;
	root["completion_tokens"] = completion_tokens;
	root["prompt_time"] = prompt_time;
	root["prompt_tokens"] = prompt_tokens;
	root["queue_time"] = queue_time;
	root["total_time"] = total_time;
	root["total_tokens"] = total_tokens;
	return root;
}

bool Response::X_groq::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("id") && root["id"].isString()) id = root["id"].asString();
	return true;
}

Json::Value Response::X_groq::toJson() const
{
	Json::Value root;
	root["id"] = id;
	return root;
}

bool Request::fromJson(const Json::Value& root)
{
	if (!root.isObject()) {
		return false;
	}

	if (root.isMember("messages") && root["messages"].isArray()) {
		for (const auto& message : root["messages"]) {
			if (message.isObject()) {
				Message msg;
				msg.fromJson(message);
				messages.push_back(msg);
			}
		}
	}

	if (root.isMember("model") && root["model"].isString()) {
		model = root["model"].asString();
	}
	if (root.isMember("temperature") && root["temperature"].isDouble()) {
		temperature = root["temperature"].asDouble();
	}
	if (root.isMember("max_tokens") && root["max_tokens"].isInt()) {
		max_tokens = root["max_tokens"].asInt();
	}
	//if (root.isMember("top_p") && root["top_p"].isDouble()) {
	//	top_p = root["top_p"].asDouble();
	//}
	if (root.isMember("stream") && root["stream"].isBool()) {
		stream = root["stream"].asBool();
	}
	if (root.isMember("stop")) {
		if (root["stop"].isString()) {
			stop.push_back(root["stop"].asString());
		}
		else if (root["stop"].isArray()) {
			for (const auto& s : root["stop"]) {
				if (s.isString()) {
					stop.push_back(s.asString());
				}
			}
		}
	}

	return true;
}

bool Request::fromString(const std::string& str)
{
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(str, root)) {
		return false;
	}
	return fromJson(root);
}

Json::Value Request::toJson()
{
	Json::Value root;
	root["messages"] = Json::Value(Json::arrayValue);
	for (const auto& message : messages) {
		Json::Value msg = message.toJson();
		root["messages"].append(msg);
	}
	root["model"] = model;
	root["temperature"] = temperature;
	root["max_tokens"] = max_tokens;
	//root["top_p"] = top_p;
	root["stream"] = stream;

	if (stop.empty()) {
		root["stop"] = Json::nullValue;
	}
	else if (stop.size() == 1) {
		root["stop"] = stop[0];
	}
	else {
		root["stop"] = Json::Value(Json::arrayValue);
		for (const auto& s : stop) {
			root["stop"].append(s);
		}
	}

	//
	if(!tool_data.empty())
	{
		root["tool_choice"] = tool_choice;

		root["tools"] = Json::Value(Json::arrayValue);
		for (const auto& tool : tool_data) {
			Json::Value t = tool.toJson();
			root["tools"].append(t);
		}
	}

	return root;
}

std::string Request::toString()
{
	return toJson().toStyledString();
}

bool Response::fromJson(const Json::Value& root)
{

	if (!root.isObject()) return false;

	if (root.isMember("choices") && root["choices"].isArray()) {
		for (const auto& choice : root["choices"]) {
			if (choice.isObject()) {
				Choice ch;
				ch.fromJson(choice);
				choices.push_back(ch);
			}
		}
	}

	if (root.isMember("created") && root["created"].isInt()) {
		created = root["created"].asInt();
	}

	if (root.isMember("id") && root["id"].isString()) {
		id = root["id"].asString();
	}

	if (root.isMember("model") && root["model"].isString()) {
		model = root["model"].asString();
	}

	if (root.isMember("object") && root["object"].isString()) {
		object = root["object"].asString();
	}

	if (root.isMember("system_fingerprint") && root["system_fingerprint"].isString()) {
		system_fingerprint = root["system_fingerprint"].asString();
	}

	if (root.isMember("usage") && root["usage"].isObject()) {
		usage.fromJson(root["usage"]);
	}

	if (root.isMember("x_groq") && root["x_groq"].isObject()) {
		x_groq.fromJson(root["x_groq"]);
	}

	if (root.isMember("error") && root["error"].isObject()) {
		error.fromJson(root["error"]);
		error_flag = true;
	}
	
	return true;
}

bool Response::fromString(const std::string& str)
{
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(str, root)) 
	{
		error_flag = true;
		error.type = "parse error";
		error.message = str;
		return false;
	}

	error_flag = false;
	return fromJson(root);
}

Json::Value Response::toJson()
{
	Json::Value root;

	Json::Value choicesArray(Json::arrayValue);
	for (const auto& choice : choices) {
		choicesArray.append(choice.toJson());
	}
	root["choices"] = choicesArray;

	root["created"] = created;
	root["id"] = id;
	root["model"] = model;
	root["object"] = object;
	root["system_fingerprint"] = system_fingerprint;
	root["usage"] = usage.toJson();
	root["x_groq"] = x_groq.toJson();

	if (error.message.empty()) {
		root["error"] = Json::nullValue;
	}
	else {
		root["error"] = error.toJson();
	}

	return root;
}

std::string Response::toString()
{
	return toJson().toStyledString();
}

bool Response::Error::fromJson(const Json::Value& root)
{
	message = root["message"].asString();
	type = root["type"].asString();
	failed_generation = root["failed_generation"].asString();
	return true;
}

Json::Value Response::Error::toJson()
{
	Json::Value root;
	root["message"] = message;
	root["type"] = type;
	return root;
}

std::ostream& operator << (std::ostream& os, const Response & response)
{
	if (response.error_flag)
	{
		return os << response.error.toString();
	};

	if (response.choices.size() == 0)
	{
		return os << "Empty response";
	}

	if (response.choices.size() > 1)
	{
		return os << "Multiple choices";
	}

	if (response.choices[0].message.tool_calls.empty())
	{
		return os << response.choices[0].message.toString();
	}

	if (response.choices[0].message.tool_calls.size() >= 1)
	{		
		return os << response.choices[0].message.toString();
	}




	return os << "unknown error";
}

std::string get_api_key(const char* env)
{
	std::string apikey;
	size_t requiredSize = 0;

	getenv_s(&requiredSize, nullptr, 0, env);
	if (requiredSize == 0)
	{
		std::cerr << "Environment variable " << env << " doesn't exist!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	apikey.resize(requiredSize);
	if (getenv_s(&requiredSize, (char*)apikey.data(), requiredSize, env) != 0)
	{
		std::cerr << "Failed to get environment variable!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	apikey.resize(strlen(apikey.c_str())); // remove null characters
	return apikey;
}

void cp866_to_utf8(std::string& str) {

#ifdef 	_WIN32
	int wchars_num = MultiByteToWideChar(866, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(wchars_num, L'\0');
	MultiByteToWideChar(866, 0, str.c_str(), -1, &wstr[0], wchars_num);
	int utf8_chars_num = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8_str(utf8_chars_num, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], utf8_chars_num, nullptr, nullptr);
	utf8_str.resize(utf8_chars_num - 1);
	str = utf8_str;
#endif // 
}

std::string ToolCall::valueTypeToString(Json::ValueType type) {
	switch (type) {
	case Json::nullValue: return "null";
	case Json::intValue:  return "int";
	case Json::uintValue: return "unsigned int";
	case Json::realValue: return "double";
	case Json::stringValue: return "string";
	case Json::booleanValue: return "bool";
	case Json::arrayValue:  return "Json::Array";
	case Json::objectValue:  return "Json::Object";
	default: return "unknown";
	}
}

bool ToolCall::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("id") && root["id"].isString()) id = root["id"].asString();
	if (root.isMember("type") && root["type"].isString()) type = root["type"].asString();
	if (root.isMember("function") && root["function"].isObject())
	{
		const auto func_root = root["function"];
		if (func_root.isMember("name") && func_root["name"].isString()) function.name = func_root["name"].asString();
		if (func_root.isMember("arguments") && func_root["arguments"].isString())
		{
			function.arguments = func_root["arguments"].asString();
			function.parse_arguments();
		}
		//auto text = get_param_string("text");
		//auto color = get_param_int("color");
	}
	return true;
}

Json::Value ToolCall::toJson() const
{
	Json::Value root;
	root["id"] = id;
	root["type"] = type;

	Json::Value functionRoot;
	functionRoot["name"] = function.name;
	functionRoot["arguments"] = function.arguments;

	root["function"] = functionRoot;

	return root;
}

bool Models::Model::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("id") && root["id"].isString()) id = root["id"].asString();
	if (root.isMember("object") && root["object"].isString()) object = root["object"].asString();
	if (root.isMember("created") && root["created"].isInt()) created = root["created"].asInt();
	if (root.isMember("owned_by") && root["owned_by"].isString()) owned_by = root["owned_by"].asString();
	if (root.isMember("active") && root["active"].isBool()) active = root["active"].asBool();
	if (root.isMember("context_window") && root["context_window"].isInt()) context_window = root["context_window"].asInt();
	if (root.isMember("public_apps") && root["public_apps"].isString()) public_apps = root["public_apps"].asString();
	return true;
}

bool Models::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("object") && root["object"].isString())  object = root["object"].asString();

	if (root.isMember("data") && root["data"].isArray()) {
		for (const auto& model : root["data"]) {
			if (model.isObject()) {
				Model mod;
				mod.fromJson(model);
				data.push_back(mod);
			}
		}
	}

	if (root.isMember("error") && root["error"].isObject())
	{
		auto o = root["error"];
		if (o.isMember("message") && o["message"].isString()) error_message = o["message"].asString();
		if (o.isMember("type") && o["type"].isString()) error_type = o["type"].asString();
		if (o.isMember("code") && o["code"].isString()) error_code = o["code"].asString();
	}

	return true;

}

bool Models::List()
{
	std::string response_string = _completion("");
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(response_string, root))
		return false;

	return fromJson(root);
}

bool Function::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;

	if (root.isMember("type") && root["type"].isString())
	{
		auto type = root["type"].asString();
		if (type == tool_type)
		{
			if (root.isMember("function") && root["function"].isObject())
			{
				const auto func_root = root["function"];

				if (func_root.isMember("name") && func_root["name"].isString()) name = func_root["name"].asString();
				if (func_root.isMember("description") && func_root["description"].isString()) description = func_root["description"].asString();

				if (func_root.isMember("parameters") && func_root["parameters"].isObject())
				{
					const auto param_root = func_root["parameters"];
					if (param_root.isMember("type") && param_root["type"].isString())
					{
						auto type1 = param_root["type"].asString();
						if (type1 == "object")
						{
							if (param_root.isMember("required") && param_root["required"].isArray())
							{
								for (const auto& param : param_root["required"])
								{
									required.push_back(param.asString());
								}

								if (param_root.isMember("properties") && param_root["properties"].isObject())
								{
									const auto prop_root = param_root["properties"];

									for (const auto& paramname : required)
									{
										parameters[paramname].fromJson(paramname, prop_root[paramname]);
									}
								}
							}
						}
						else
						{
							//type param not object
						}
					}
					else
					{
						//notype
					}
				}
				else
				{
					//no parameters
				}
			}
			else
			{
				//object function no found
				return false;
			}
		}
		else
		{
			// unexpected type
			return false;
		}

	}


	return true;
}

Json::Value Function::toJson() const
{
	Json::Value parametrs_object = Json::Value(Json::objectValue);
	if (required.size() > 0)
	{
		Json::Value parameters_rquired = Json::Value(Json::arrayValue);
		for (const auto& req : required)
		{
			parameters_rquired.append(req);
		}

		parametrs_object["type"] = "object";
		Json::Value properties_object = Json::Value(Json::objectValue);
		for (const auto& paramname : required)
		{
			properties_object[paramname] = parameters.at(paramname).toJson(); // исправленная строка 232
		}

		parametrs_object["properties"] = properties_object;
		parametrs_object["required"] = parameters_rquired;
	}

	Json::Value function;
	function["name"] = name;
	function["description"] = description;

	if (required.size() > 0)
	{
		function["parameters"] = parametrs_object;
	}


	Json::Value root;
	root["type"] = "function";
	root["function"] = function;


	return root;
}

bool Message::fromJson(const Json::Value& root)
{
	if (!root.isObject()) return false;
	if (root.isMember("role") && root["role"].isString()) set_role(root["role"].asString());
	if (root.isMember("content") && root["content"].isString()) content = root["content"].asString();
	if (root.isMember("name") && root["name"].isString()) username = root["name"].asString();

	//response only
	if (root.isMember("tool_calls") && root["tool_calls"].isArray())
	{
		for (const auto& tool_call : root["tool_calls"])
		{
			ToolCall tc;
			tc.fromJson(tool_call);
			tool_calls.push_back(tc);
		}
	}
	return true;
}

Json::Value Message::toJson() const
{
	Json::Value root;
	root["role"] = get_role();


	switch (type)
	{
	case Message::None: break;
	case Message::System:
		root["content"] = content; // The contents of the system message.			
		if (username != "") root["name"] = username; //An optional name for the participant.
		break;
	case Message::User:
		root["content"] = content;
		if (username != "") root["name"] = username;
		break;
	case Message::Assistant:
		if (username != "") root["name"] = username;//An optional name for the participant.
		if (isToolCalls())
		{
			root["tool_calls"] = Json::Value(Json::arrayValue);
			for (auto tool_call : tool_calls)
			{
				root["tool_calls"].append(tool_call.toJson());
			}
		}
		else
		{
			root["content"] = content; //The contents of the assistant message.Required unless tool_calls or function_call is specified.
		}
		break;
	case Message::Tool:
		root["content"] = content;
		root["tool_call_id"] = tool_call_id;
		break;
	default:
		break;
	}



	return root;
}
