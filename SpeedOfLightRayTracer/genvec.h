#pragma once

#include <algorithm>
#include <iostream>
#include <cassert>
#include <functional>


namespace genvec {
	template<typename T, size_t LEN>
	class vec
	{
	private:

		T arr[LEN];

		template<size_t idx>
		inline auto& get() const {
			static_assert(idx < LEN && idx >= 0, "Invalid index");
			return arr[idx];
		}

		template<size_t idx>
		inline auto& get() {
			static_assert(idx < LEN && idx >= 0, "Invalid index");
			return arr[idx];
		}

		inline auto& get(size_t idx) const {
			assert(idx >= 0 && idx < LEN);
			return arr[idx];
		}

		inline auto& get(size_t idx) {
			assert(idx >= 0 && idx < LEN);
			return arr[idx];
		}

	public:
		template<typename... Ts>
		vec(Ts... t)
			: arr{ static_cast<T>(t)... }
		{
			static_assert(LEN > 0, "Vector must have positive number of elements");
			static_assert(LEN >= sizeof...(Ts), "Cannot assign to more elements than exist in the vector!");
			static_assert(sizeof...(Ts) == 0 || sizeof...(Ts) == LEN, "Must assign to all or no elements of a vector");
		}

        template<>
        vec(std::function<T()> f)
        {
            for (int i = 0; i < LEN; i++) {
                arr[i] = f();
            }
        }

		inline auto& operator[](const size_t idx) {
			return arr[idx];
		}

		inline const auto& operator[](const size_t idx) const {
			return arr[idx];
		}

		inline auto lensq() const {
			return dot(*this, *this);
		}

		inline auto len() const {
			return (T)std::sqrt((double)lensq());
		}

		inline auto normalized() const {
			return (*this) / len();
		}

		template<typename TT>
		inline auto operator*(const vec<TT, LEN>& v) const {
			vec<decltype(T()*TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] * v[i];
			}

			return res;
		}

		template<typename TT>
		inline auto operator+(const vec<TT, LEN>& v) const {
			vec<decltype(T() + TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] + v[i];
			}

			return res;
		}

		template<typename TT>
		inline auto operator-(const vec<TT, LEN>& v) const {
			vec<decltype(T() - TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] - v[i];
			}

			return res;
		}

		template<typename TT>
		inline auto operator/(const vec<TT, LEN>& v) const {
			vec<decltype(T() / TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] / v[i];
			}

			return res;
		}

		template<typename TT>
		inline auto operator*(const TT& c) const {
			vec<decltype(T()*TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] * c;
			}

			return res;
		}


		template<typename TT>
		inline auto operator+(const TT& c) const {
			vec<decltype(T() + TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] + c;
			}

			return res;
		}

		template<typename TT>
		inline auto operator-(const TT& c) const {
			vec<decltype(T() - TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] - c;
			}

			return res;
		}

		template<typename TT>
		inline auto operator/(const TT& c) const {
			vec<decltype(T() / TT()), LEN> res;

			for (auto i = 0; i < LEN; i++) {
				res[i] = arr[i] / c;
			}

			return res;
		}

		template<typename TT>
		inline auto& operator=(const TT& c) {
			for (auto i = 0; i < LEN; i++) {
				arr[i] = c;
			}
			return *this;
		}

		template<typename TT>
		inline auto& operator=(const vec<TT, LEN>& v) {
			for (auto i = 0; i < LEN; i++) {
				arr[i] = v[i];
			}
			return *this;
		}

		template<typename TT>
		inline auto operator==(const TT& c) const {
			for (auto i = 0; i < LEN; i++) {
				if (arr[i] != c)
					return false;
			}

			return true;
		}

		template<typename TT>
		inline auto operator==(const vec<TT, LEN>& v) const {
			for (auto i = 0; i < LEN; i++)
				if (arr[i] != v[i])
					return false;

			return true;
		}

		template<typename TT>
		inline auto operator!=(const vec<TT, LEN>& v) const {
			return !(*this == v);
		}

		template<typename TT>
		inline auto operator!=(const TT& c) const {
			return !(*this == c);
		}

		template<typename TT>
		inline auto& operator+=(const TT& v) {
			(*this) = (*this) + v;
			return *this;
		}

		template<typename TT>
		inline auto& operator-=(const TT& v) {
			(*this) = (*this) - v;
			return *this;
		}

		template<typename TT>
		inline auto& operator/=(const TT& v) {
			(*this) = (*this) / v;
			return *this;
		}

		template<typename TT>
		inline auto& operator*=(const TT& v) {
			(*this) = (*this) * v;
			return *this;
		}

		template<size_t... IDXs>
		inline auto swizzle() {
			static_assert(sizeof...(IDXs) > 0, "swizzle requires more than one index");
			return vec<T, sizeof...(IDXs)> { get<IDXs>()... };
		}

		vec<T, LEN> abs() const {
			vec<T, LEN> ret;
			for (auto i = 0; i < LEN; i++) {
				ret[i] = std::abs(arr[i]);
			}
			return ret;
		}

		auto str() const {
			stringstream s;
			s << '(';
			for (auto i = 0; i < LEN - 1; i++) {
				s << arr[i] << ',' << ' ';
			}
			s << arr[LEN - 1] << ')';
			return s.str();
		}

        auto all_unit() const {
            vec<T, LEN> ret;
            auto max = arr[0];
            for (auto i = 0; i < LEN; i++) {
                max = std::max(max, arr[i]);
            }
            for (auto i = 0; i < LEN; i++) {
                ret = arr[i] / max;
            }
            return ret;
        }

        auto sum() const {
            auto sum = arr[0];
            for (auto i = 1; i < LEN; i++) {
                sum += arr[i];
            }
            return sum;
        }
	}; // end of class

	template<typename lhsT, typename rhsT, size_t rhsLEN >
	inline auto operator+(const lhsT& lhs, const vec<rhsT, rhsLEN>& rhs) {
		return rhs + lhs;
	}

	template<typename lhsT, typename rhsT, size_t rhsLEN>
	inline auto operator-(const lhsT& lhs, const vec<rhsT, rhsLEN>& rhs) {
		return rhs - lhs;
	}

	template<typename lhsT, typename rhsT, size_t rhsLEN>
	inline auto operator*(const lhsT& lhs, const vec<rhsT, rhsLEN>& rhs) {
		return rhs * lhs;
	}

	template<typename lhsT, typename rhsT, size_t rhsLEN>
	inline auto operator/(const lhsT& lhs, const vec<rhsT, rhsLEN>& rhs) {
		vec<rhsT, rhsLEN> ret;
		for (auto i = 0; i < rhsLEN; i++) {
			ret[i] = lhs / rhs[i];
		}
		return ret;
	}

	template<typename T1, typename T2>
	auto cross(const vec<T1, 3>& u, const vec<T2, 3>& v) {
		auto i = u[1] * v[2] - u[2] * v[1];
		auto j = u[2] * v[0] - u[0] * v[2];
		auto k = u[0] * v[1] - u[1] * v[0];

		return vec<decltype(i), 3>(i, j, k);
	}

	template<typename T1, typename T2, size_t LEN>
	auto dot(const vec<T1, LEN>& u, const vec<T2, LEN>& v) {
		auto res = u[0] * v[0];;
		for (auto i = 1; i < LEN; i++) {
			res += u[i] * v[i];
		}
		return res;
	}

	template<typename T, size_t LEN>
	auto mod(const vec<T, LEN>& u, const vec<T, LEN>& v) {
		vec<T, LEN> res = 0;
		for (auto i = 0; i < LEN; i++) {
			res[i] = fmod(u[i], v[i]);
		}
		return res;
	}

	template<typename T, size_t LEN>
	void normalize(vec<T, LEN>& u) {
		u = u.normalized();
	}

	template<size_t LEN>
	using fvec = vec<float, LEN>;
	using fvec3 = vec<float, 3>;
	template<size_t LEN>
	using ivec = vec<int, LEN>;
	using ivec2 = vec<int, 2>;
	template<size_t LEN>
	using dvec = vec<double, LEN>;
	template<size_t LEN>
	using bvec = vec<uint8_t, LEN>;

	using rgb = fvec<3>;
	using rgba = fvec<4>;
	using pixel = bvec<4>; // rgba
	using pos = fvec<3>;


	template<typename... T>
	auto make_fvec(T&&... args) {
	return fvec<sizeof...(args)>(args...);
	}

	template<typename... T>
	auto make_dvec(T&&... args) {
	return dvec<sizeof...(args)>(args...);
	}

	template<typename... T>
	auto make_ivec(T&&... args) {
	return ivec<sizeof...(args)>(args...);
	}

	template<typename... T>
	auto make_bvec(T&&... args) {
	return bvec<sizeof...(args)>(args...);
	}
}

template<typename T, int N>
std::ostream& operator<<(std::ostream& o, const genvec::vec<T, N>& v) {
    o << v.str();
    return o;
}