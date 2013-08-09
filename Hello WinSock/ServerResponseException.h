class ServerResponseException
{
public:
	ServerResponseException() : description("Uknown"){}
	ServerResponseException(const char *newDescription) :
	description(newDescription){}
	const char* what() { return description; }
private:
	const char *description;
};