#ifndef TYPES_H_
#define TYPES_H_

#include <boost/multi_array.hpp>

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

struct helper{
	bool use_ab;
	int depth;
	int N;
	int cols;
	int rows;
	bool ai_first;
};

typedef boost::multi_array<int,2> MatrixType;

#endif /* TYPES_H_ */
