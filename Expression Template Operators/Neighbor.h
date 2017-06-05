#pragma once

#include <vector>

template <typename T>
class Neighbor
{
public:
	typedef typename std::vector<T> Vec;
	typedef typename Vec::iterator iterator;
	typedef typename Vec::const_iterator const_iterator;
	typedef typename std::vector<iterator> IterContainer;

	std::size_t size() { return vec_.size(); }

	iterator begin() { return *(permutation_.begin()); }
	iterator end()   { return *(permutation_.end() - 1); }

	const_iterator cbegin() { return *(permutation_.cbegin()); }
	const_iterator cend()   { return *(permutation_.cend() - 1); }

	// permutation_ should be a container of vec iterators in such a way
	// that it creates a permutation of the original vector
	Neighbor(Vec const &vec, IterContainer const &iterContainer) :
		permutation_(iterContainer),
		vec_(vec)
	{}

	// Constructs concrete vector out of reference container
	operator Vec ()
	{
		auto const size = vec_.size();
		Vec convert(size);

		for (std::size_t x = 0; x < size; x++)
			convert[x] = this->operator[](x);

		// Doesn't work as intended, just copies vec_.
		//std::copy(this->begin(), this->end(), convert.begin());

		return convert;
	}

	// Neighbor acts as a vector of components
	T &operator [] (std::size_t const &index) const
	{
		return *(permutation_[index]);
	}

private:
	// Container of iterators which allows us to treat permutation_
	// as a permutation vector of the component vector given in the constructor
	const IterContainer &permutation_;
	const Vec			&vec_;
};