#ifndef M3D_ANY_H_
#define M3D_ANY_H_
#include <algorithm>
#include <m3d/base/Holder.h>

namespace M3D
{
	class  Any
	{
	public:
		Any() :holder_(nullptr) {}

		template<typename ValueType>
		Any(const ValueType& val)
			: holder_(new Holder<ValueType>(val)) {

		}

		Any(const Any& other)
			: holder_(other.holder_ ? other.holder_->clone() : nullptr) {

		}

		~Any()
		{
			if (holder_) {
				delete holder_;
				holder_ = nullptr;
			}
		}

	public:
		bool	empty() {
			return !holder_;
		}

		const std::type_info& type() const {
			return holder_ ? holder_->type() : typeid(void);
		}

		Any& operator=(Any rhs) {
			return swap(rhs);
		}

		template<typename ValueType>
		Any& operator=(const ValueType& val) {
			return Any(val).swap(*this);
		}

		IHolder* getHolder() {
			return holder_;
		}
	private:
		Any& swap(Any& rhs) {
			std::swap(holder_, rhs.holder_);
			return *this;
		}


	private:
		IHolder* holder_;
	};


	template<typename ValueType>
	ValueType* anyCast(Any* val) {
		return (val && val->type() == typeid(ValueType))
			? &static_cast<Holder<ValueType>*>(val->getHolder())->value_ : 0;
	}
	class badAnyCast : public std::bad_cast
	{
	public:
		virtual const char * what() const throw()
		{
			return "M3D::badAnyCast: "
				"failed conversion using M3D::badAnyCast";
		}
	};

	template<typename ValueType>
	ValueType anyCast(Any& val) {
		ValueType* rtn = anyCast<ValueType>(&val);
		//if (!rtn)std::throw_exception(badAnyCast());
		return *rtn;
	}

}
#endif 

