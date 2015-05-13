#ifndef KArrayDimension_H_ 
#define KArrayDimension_H_ 
class KArrayDimension
{
public:
	KArrayDimension();
	KArrayDimension(int start_index, int end_index);
	~KArrayDimension();
	int starts_at();
	int ends_at();
	int get_size();

private:
	int start_index;
	int end_index;
};

inline KArrayDimension::KArrayDimension()
{
	start_index = 0;
	end_index = 0;
}

inline KArrayDimension::KArrayDimension(int _start_index, int _end_index)
{
	start_index = _start_index;
	end_index = _end_index;
}

inline KArrayDimension::~KArrayDimension()
{
}

inline int KArrayDimension::starts_at()
{
	return start_index;
}

inline int KArrayDimension::ends_at()
{
	return end_index;
}

inline int KArrayDimension::get_size()
{
	return end_index - start_index + 1;
}
#endif // KArrayDimension_H_