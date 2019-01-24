

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

template <class T>
class Deserializer
{
public:
	virtual ~Deserializer() {}

	virtual T deserialize(class Request*) = 0;
};

class Request
{
public:
	template <typename T>
	T deserializeToType(Deserializer<T>& deserializer)
	{
		return deserializer.deserialize(this);
	}
};

#endif
