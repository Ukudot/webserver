#ifndef OPTIONAL_HPP
# define OPTIONAL_HPP

class	NoValueException: public std::exception {
	virtual const char	*what() const throw() {
		return ("no value found");
	}
};

template<typename T>
class	Optional {
	private:
		bool	set;
		T		m_value;

	public:
		Optional(void) {
			this->set = false;
		}

		Optional(T value) {
			this->set = true;
			this->m_value = value;
		}

		Optional	&operator=(Optional const &opt) {
			if (this == &opt)
				return (*this);
			this->set = opt.hasValue();
			if (this->set)
				this->m_value = opt.value();
			return (*this);
		}

		~Optional(void) {}

		bool	hasValue(void) const {
			return (this->set);
		}

		T		value(void) const {
			if (this->set)
				return (this->m_value);
			throw (NoValueException());
		}
};

#endif
